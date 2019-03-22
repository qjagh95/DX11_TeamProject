#include "ComputeShare.fx"

Texture2D g_MotionBlurTex[MAX_SAMPLE] : register(t0);


static float g_fMBWeight[MAX_SAMPLE] = { 1.0f, 0.8f, 0.7f, 0.6f, 0.5f, 0.3f, 0.2f };

[numthreads(10, 10, 1)]
void MotionBlur(int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;

    int3 vUV = (int3) 0;
    vUV = vDispatchThreadID;

    for (int i = 0; i < g_iCount; ++i)
    {
        vColor += g_MotionBlurTex[i].Load(vUV) * g_fMBWeight[i];
    }

    g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}