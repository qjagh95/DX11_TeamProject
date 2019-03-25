#include "ComputeShare.fx"

Texture2D g_DepthTex : register(t1);

cbuffer Blur : register(b1)
{
    int     g_iNumPixel;
    int     g_iDepthOfField;
    float   g_fCameraFar;
    float   g_fBlurEmpty;
}

groupshared float4 g_vSharedColor[2][2];
groupshared float g_vDepth[2][2];
groupshared float4 g_vBlurColorH[640];
groupshared float4 g_vBlurColorV[360];

[numthreads(640, 1, 1)]
void HorizontalBlur(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;
    int iMaxCount = g_iNumPixel;
    int iMaxOffset = iMaxCount / 2;
    int3 vUV;

    vUV.xy = vDispatchThreadID.xy;
    vUV.z = 0;

    float4 vOriginColor = g_BaseTex.Load(vUV);    

    if (!isfinite(vOriginColor.x) || !isfinite(vOriginColor.y) || !isfinite(vOriginColor.z) || !isfinite(vOriginColor.a))
        vOriginColor = float4(0.0f, 0.0f, 0.0f, 10.0f);
    

    g_vBlurColorH[vGroupThreadID.x] = vOriginColor;
    
    GroupMemoryBarrierWithGroupSync();

    int k = 0;
    
    float fBlur = 1.0f / (float)iMaxCount;

    vColor = vOriginColor * fBlur;

    if (g_iDepthOfField == 1)
    {
        float fDepth = vOriginColor.a / g_fCameraFar;
       if (fDepth > 1.0f || fDepth < 0.000001f)
            fDepth = 0.9f;

        fBlur = fDepth / (float)(iMaxCount - 1);
        vColor = vOriginColor * (1.0f - fDepth);
    }    

    for (int i = -iMaxOffset; i <= iMaxOffset; ++i)
    {
        k = vGroupThreadID.x + i;

        if (k >= 0 && k < 640)
        {
            if (i == 0)
                continue;

            vColor += g_vBlurColorH[k] * fBlur;
        }
    }

    g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}

[numthreads(1, 360, 1)]
void VerticalBlur(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;
    int iMaxCount = g_iNumPixel;
    int iMaxOffset = iMaxCount / 2;
    int3 vUV;
    vUV.xy = vDispatchThreadID.xy;
    vUV.z = 0;

    float4 vOriginColor = g_BaseTex.Load(vUV);

    if (!isfinite(vOriginColor.x) || !isfinite(vOriginColor.y) || !isfinite(vOriginColor.z))
        vOriginColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

    g_vBlurColorV[vGroupThreadID.y] = vOriginColor;

    
    GroupMemoryBarrierWithGroupSync();     

    float fBlur = 1.0f / (float)iMaxCount;

    vColor = vOriginColor * fBlur;

    if (g_iDepthOfField == 1)
    {
        float fDepth = vOriginColor.a / g_fCameraFar;
        if (fDepth > 1.0f || fDepth < 0.000001f)
            fDepth = 0.9f;
        fBlur = fDepth / (float)(iMaxCount - 1);
        vColor = vOriginColor * (1.0f - fDepth);
    }

    int k = 0;
  
    for (int i = -iMaxOffset; i <= iMaxOffset; ++i)
    {
        k = vGroupThreadID.y + i;

        if (k >= 0 && k < 360)
        {
            if (i == 0)
                continue;

            vColor += g_vBlurColorV[k] * fBlur;
        }
    }

    g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}