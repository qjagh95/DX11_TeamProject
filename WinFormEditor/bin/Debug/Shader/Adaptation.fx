#include "Share.fx"

Texture2D<float4> HDRTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);

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


//static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

VS_OUTPUT_TEX FullScreenAdaptQuadVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(varrNullPos[iVertexID].xy, 0.f, 1.f);
    output.vUV = varrNullUV[iVertexID].xy;

    return output;
}

//float3 ToneMapping(float3 vHDRColor)
//{
//    // ���� �ȼ��� ���� �ֵ� ������ ���
//    float fLScale = dot(vHDRColor, LUM_FACTOR);
//    fLScale *= g_fMiddleGrey / AvgLum[0];
//    fLScale = (fLScale + fLScale * fLScale / fLumWhiteSqr) / (1.f + fLScale);

//    // �ֵ� �������� �ȼ� ���� ����
//    return vHDRColor * fLScale;
//}

//float4 FinalPassAdaptPS(VS_OUTPUT_TEX Input) : SV_TARGET
//{
//    // ���� ���� ���
//    float3 vColor = HDRTex.Sample(PointSampler, Input.vUV.xy).xyz;

//    // �� ����
//    vColor = ToneMapping(vColor);

//    // LDR �� ���
//    return float4(vColor, 1.f);
//}

//PS_OUTPUT_SINGLE FinalPassAdaptPS(VS_OUTPUT_TEX Input) : SV_TARGET
//{
//    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

//    // ���� ���� ���
//    float3 vColor = HDRTex.Sample(PointSampler, Input.vUV.xy).xyz;

//    // �� ����
//    vColor = ToneMapping(vColor);

//    output.vTarget0 = float4(vColor, 1.f);

//    // LDR �� ���
//    return output;
//}