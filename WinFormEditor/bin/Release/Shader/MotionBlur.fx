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

    float4 ReturnColor = (float4) 0.0f;

    float4 vColorArr1 = g_MotionBlurTex0.Load(vUV) * g_fMBWeight[0];
    float4 vColorArr2 = g_MotionBlurTex1.Load(vUV) * g_fMBWeight[1];
    float4 vColorArr3 = g_MotionBlurTex2.Load(vUV) * g_fMBWeight[2];
    float4 vColorArr4 = g_MotionBlurTex3.Load(vUV) * g_fMBWeight[3];
    float4 vColorArr5 = g_MotionBlurTex4.Load(vUV) * g_fMBWeight[4];
    float4 vColorArr6 = g_MotionBlurTex5.Load(vUV) * g_fMBWeight[5];
    float4 vColorArr7 = g_MotionBlurTex6.Load(vUV) * g_fMBWeight[6];

    ReturnColor += lerp(vColorArr1, vColorArr2, g_DeltaTime);
    ReturnColor += lerp(vColorArr2, vColorArr3, g_DeltaTime);
    ReturnColor += lerp(vColorArr3, vColorArr4, g_DeltaTime);
    ReturnColor += lerp(vColorArr4, vColorArr5, g_DeltaTime);
    ReturnColor += lerp(vColorArr5, vColorArr6, g_DeltaTime);
    ReturnColor += lerp(vColorArr6, vColorArr7, g_DeltaTime);

	ReturnColor /= 4.1f;

    g_RWOutputTex[vDispatchThreadID.xy] = ReturnColor;
}