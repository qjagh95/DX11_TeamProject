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


//1280x720따리 텍스쳐를 입력받아서 640x360따리 텍스쳐로 바꾼다.
[numthreads(2, 2, 1)]
void DownScaleTex2x2(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;
    float fDepth = 0.0f;
    int3 vUV = vDispatchThreadID;
    vUV.z = 0;

    g_vSharedColor[vGroupThreadID.x][vGroupThreadID.y] = g_BaseTex.Load(vUV);
   
    float2 vDepth = g_DepthTex.Load(vUV).ba;

    g_vDepth[vGroupThreadID.x][vGroupThreadID.y] = vDepth.g;

    GroupMemoryBarrierWithGroupSync();

    if (vGroupThreadID.x == 0 && vGroupThreadID.y == 0)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                vColor += g_vSharedColor[i][j];
                fDepth += g_vDepth[i][j];
            }
        }
        
        vColor.a = fDepth;
       
        vColor *= 0.25f;

        vUV.xy = vUV.xy / 2;
   
        g_RWOutputTex[vUV.xy] = vColor;
    }
}

groupshared float4 g_vUpScaleColor;

[numthreads(2, 2, 1)]
void UpScaleTex2x2(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;
    int3 vUV = vDispatchThreadID / 2;
    vUV.z = 0;

    if (vGroupThreadID.x == 0 && vGroupThreadID.y == 0)
        g_vUpScaleColor = g_BaseTex.Load(vUV);
    
    GroupMemoryBarrierWithGroupSync();

    vColor = g_vUpScaleColor;
    vColor.a = 1.0f;

    g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}

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

    if (!isfinite(vOriginColor.x) || !isfinite(vOriginColor.y) || !isfinite(vOriginColor.z))
        vOriginColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    g_vBlurColorH[vGroupThreadID.x] = vOriginColor;
    
    GroupMemoryBarrierWithGroupSync();

    int k = 0;
    
    float fBlur = 1.0f / (float)iMaxCount;

    vColor = vOriginColor * fBlur;

    if (g_iDepthOfField == 1)
    {
        float fDepth = vOriginColor.a / g_fCameraFar;
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