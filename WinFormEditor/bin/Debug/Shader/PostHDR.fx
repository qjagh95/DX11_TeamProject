#include "Share.fx"

Texture2D<float4> LightBlendTex   : register(t0);
Texture2D<float4> DepthTex        : register(t1);
Texture2D<float4> BlurTex         : register(t2);//��Ǻ��� ���ÿ� ������ �ʰ�?

StructuredBuffer<float> AvgLum  : register(t16);


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

cbuffer ScreenEffect : register(b9)
{
    int     g_iHDR;
    int     g_iBlur;
    float2  g_SEEmpty;
}

float3 ToneMapping(float3 vHDRColor)
{
    // ���� �ȼ��� ���� �ֵ� ������ ���
    float fLScale = dot(vHDRColor, LUM_FACTOR);
    fLScale *= fMiddleGrey / AvgLum[0];
    fLScale = (fLScale + fLScale * fLScale / fLumWhiteSqr) / (1.f + fLScale);

    // �ֵ� �������� �ȼ� ���� ����
    return vHDRColor * fLScale;
}


//cbuffer FinalPassConstants : register(b6)
//{
//    float fMiddleGrey;
//    float fLumWhiteSqr;
//    float2 vEmpty;
//}

//static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

VS_OUTPUT_TEX FullScreenQuadVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(varrNullPos[iVertexID].xy, 0.f, 1.f);
    output.vUV = varrNullUV[iVertexID].xy;

    return output;
}


//float4 FinalPassPS(VS_OUTPUT_TEX Input) : SV_TARGET
//{
//    // ���� ���� ���
//    float3 vColor = HDRTex.Sample(PointSampler, Input.vUV.xy).xyz;

//    // �� ����
//    vColor = ToneMapping(vColor);

//    // LDR �� ���
//    return float4(vColor, 1.f);
//}


PS_OUTPUT_SINGLE FinalPassPS(VS_OUTPUT_TEX Input) : SV_TARGET
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    // ���� ���� ���
    float Focus = DepthTex.Sample(PointSampler, Input.vUV.xy).y;
    float3 vColor = LightBlendTex.Sample(PointSampler, Input.vUV.xy).xyz;    

    if(g_iBlur == 1)
    {
        if(Focus < 0.5f)
            vColor = BlurTex.Sample(PointSampler, Input.vUV.xy).xyz;
    }

    // �� ����
    if (g_iHDR == 1)
        vColor = ToneMapping(vColor);

    output.vTarget0 = float4(vColor, 1.f);

    // LDR �� ���
   // return float4(vColor, 1.f);
    return output;
}

