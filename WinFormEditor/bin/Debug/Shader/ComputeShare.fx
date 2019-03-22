cbuffer Basic : register(b0)
{
    float2 g_fPixelSize;
    float2 g_fBasicEmpty;
}

Texture2D g_BaseTex : register(t0);

RWTexture2D<float4> g_RWOutputTex : register(u0);

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

#define COM_PI 3.145692
#define MAX_SAMPLE 7

cbuffer MotionBlur : register(b1)
{
    int g_iCount;
    float3 g_vBlurEmpty;
}

[numthreads(1, 1, 1)]
void main(int3 vDispatchID : SV_DispatchThreadID)
{
}

float ComputeWeight(float fOffsetX, float fOffsetY, float fRho)
{
    return exp(-(fOffsetX * fOffsetX + fOffsetY, fOffsetY) / 2 * fRho * fRho) / (2 * COM_PI * fRho * fRho);
}