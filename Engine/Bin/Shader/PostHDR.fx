#include "Share.fx"

Texture2D<float4> OriginTex         : register(t0);
Texture2D<float4> DepthTex          : register(t1);
Texture2D<float4> BlurTex           : register(t2); //��Ǻ��� ���ÿ� ������ �ʰ�?
Texture2D<float4> BloomTex          : register(t3);

StructuredBuffer<float> AvgLum    : register(t10);


static const float2 varrNullPos[4] =
{
    float2(-1.f, 1.f),
    float2(1.f, 1.f),
    float2(-1.f, -1.f),
    float2(1.f, -1.f)
};

static const float2 varrNullUV[4] =
{
    float2(0.f, 0.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 1.f)
};

cbuffer BloomConstants : register(b8)
{
    float fBloomScale;
    float3 vEmpty;
}

cbuffer FinalPass : register(b9)
{
    int     g_iHDR;
    int     g_iBlur;
    int     g_iMotionBlur;
    int     g_iDepthFog;
    int     g_iBloom;
    int     g_iAdaptation;
    float   g_fFadeAmount;
    float   g_Empty1234141241;
}

cbuffer DepthFog : register(b13)
{
    float4 g_vFogColor;
    float  g_fStartDepth;
    float  g_fEndDepth;
    float2 g_vDepthFogEmpty;
}

float3 ToneMapping(float3 vHDRColor)
{
    // ���� �ȼ��� ���� �ֵ� ������ ���
    float fLScale = dot(vHDRColor, LUM_FACTOR);
    fLScale *= g_fMiddleGrey / AvgLum[0];
    fLScale = (fLScale + fLScale * fLScale / g_fLumWhiteSqr) / (1.f + fLScale);

    // �ֵ� �������� �ȼ� ���� ����
    return vHDRColor * fLScale;
}

//static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

VS_OUTPUT_TEX FullScreenQuadVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(varrNullPos[iVertexID].xy, 0.f, 1.f);
    output.vUV = varrNullUV[iVertexID].xy;

    return output;
}

PS_OUTPUT_SINGLE FinalPassPS(VS_OUTPUT_TEX Input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    float4 vDepth = DepthTex.Sample(PointSampler, Input.vUV.xy);
    float3 vColor = OriginTex.Sample(PointSampler, Input.vUV.xy).xyz;

    float Focus = vDepth.y;
    float fDepth = vDepth.w;

    if (g_iBlur == 1 || g_iMotionBlur == 1)
    {
        if (Focus < 0.5f)
            vColor = BlurTex.Sample(PointSampler, Input.vUV.xy).xyz;
    }

    if (g_iDepthFog == 1)
    {
        //ī�޶� ���� ������ۿ� ����, ����, �� �� �־ ����.

        float fDensity = saturate((g_fEndDepth - fDepth) / (g_fEndDepth - g_fStartDepth));

        vColor = (vColor * fDensity) + (g_vFogColor.xyz * (1.0f - fDensity));
    }
    
    // Bloom �� ������ ����
    if(g_iBloom == 1)
    {
    // Bloom Contribution �� �߰��Ѵ�
        vColor += fBloomScale * BloomTex.Sample(g_DiffuseSmp, Input.vUV.xy).xyz;
    }
    
    // �� ����(HDR ���� LDR������ ��ȯ)
    if (g_iHDR == 1 || g_iAdaptation == 1)
        vColor = ToneMapping(vColor);

    output.vTarget0 = float4(vColor, 1.f) * g_fFadeAmount;

    // LDR �� ���
    return output;
}
