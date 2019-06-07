#include "ComputeShare.fx"

Texture2D g_DepthTex : register(t1);

groupshared float4 SharedColor4[2][2];
groupshared float4 SharedColor16[4][4];
groupshared float4 SharedColor64[8][8];

RWTexture2D<float4> g_RWOutputTex4 : register(u0);
RWTexture2D<float4> g_RWOutputTex16 : register(u1);
RWTexture2D<float4> g_RWOutputTex64 : register(u2);

[numthreads(8, 8, 1)]
void DownScale(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    int3 vUV = vDispatchThreadID;
    int3 vGroupIdx = vGroupThreadID;
    float4 vOriginColor = g_BaseTex.Load(vUV);
    float fDepth = g_DepthTex.Load(vUV).a;
    vOriginColor.a = fDepth;

    SharedColor64[vGroupIdx.x][vGroupIdx.y] = vOriginColor;

    GroupMemoryBarrierWithGroupSync();

    float4 vColor = (float4) 0;

    int2 iIdxCheck = vGroupIdx.xy % 2;
    int2 iIdx = vGroupIdx.xy * 0.5f;

    if (iIdxCheck.x == 0 && iIdxCheck.y == 0)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                vColor += SharedColor64[vGroupIdx.x + i][vGroupIdx.y + j];
            }
        }
        vColor *= 0.25f;

        vUV = vDispatchThreadID * 0.5f;

        SharedColor16[iIdx.x][iIdx.y] = vColor;
        g_RWOutputTex4[vUV.xy] = vColor;
    }

    GroupMemoryBarrierWithGroupSync();

    iIdxCheck = vGroupIdx.xy % 4;

    vColor = (float4) 0;

    if (iIdxCheck.x == 0 && iIdxCheck.y == 0)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                vColor += SharedColor16[iIdx.x + i][iIdx.y + j];
            }
        }
        vColor *= 0.25f;

        iIdx = vGroupIdx.xy * 0.25f;
        vUV = vDispatchThreadID * 0.25f;

        SharedColor4[iIdx.x][iIdx.y] = vColor;
        g_RWOutputTex16[vUV.xy] = vColor;
    }

    GroupMemoryBarrierWithGroupSync();

    vColor = (float4) 0;

    if (vGroupThreadID.x == 0 && vGroupThreadID.y == 0)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                vColor += SharedColor4[vGroupThreadID.x + i][vGroupThreadID.y + j];
            }
        }
        vColor *= 0.25f;

        vUV = vDispatchThreadID * 0.125f;

        g_RWOutputTex64[vUV.xy] = vColor;
    }
}

[numthreads(1024, 1, 1)]
void ComputeMaxDepth(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    
    
}
