#include "ComputeShare.fx"

//Texture2D g_MotionBlurTex[MAX_SAMPLE] : register(t0);
Texture2D g_MotionBlurTex0 : register(t0);
Texture2D g_MotionBlurTex1 : register(t1);
Texture2D g_MotionBlurTex2 : register(t2);
Texture2D g_MotionBlurTex3 : register(t3);
Texture2D g_MotionBlurTex4 : register(t4);
Texture2D g_MotionBlurTex5 : register(t5);
Texture2D g_MotionBlurTex6 : register(t6);


static float g_fMBWeight[MAX_SAMPLE] = { 1.0f, 0.8f, 0.7f, 0.6f, 0.5f, 0.3f, 0.2f };

[numthreads(10, 10, 1)]
void MotionBlur(int3 vDispatchThreadID : SV_DispatchThreadID)
{
    float4 vColor = (float4) 0;

    int3 vUV = (int3) 0;
    vUV = vDispatchThreadID;

    vColor += g_MotionBlurTex0.Load(vUV) * g_fMBWeight[0];
    vColor += g_MotionBlurTex1.Load(vUV) * g_fMBWeight[1];
    vColor += g_MotionBlurTex2.Load(vUV) * g_fMBWeight[2];
    vColor += g_MotionBlurTex3.Load(vUV) * g_fMBWeight[3];
    vColor += g_MotionBlurTex4.Load(vUV) * g_fMBWeight[4];
    vColor += g_MotionBlurTex5.Load(vUV) * g_fMBWeight[5];
    vColor += g_MotionBlurTex6.Load(vUV) * g_fMBWeight[6];

    vColor /= 4.1f;

    g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}