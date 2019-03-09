
#include "Share.fx"

struct VS_OUTPUT_SKY
{
    float4 vPos : SV_POSITION;
    float3 vUV : TEXCOORD;
};

TextureCube g_SkyTex : register(t10);
SamplerState g_SkySmp : register(s10);

VS_OUTPUT_SKY SkyVS(VS_INPUT_POS input)
{
    VS_OUTPUT_SKY output = (VS_OUTPUT_SKY) 0;

    output.vPos = mul(float4(input.vPos, 1.f), g_matWVP).xyww;
    output.vUV = input.vPos;

    return output;
}

PS_OUTPUT_SINGLE SkyPS(VS_OUTPUT_SKY input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = g_SkyTex.Sample(g_SkySmp, input.vUV);

    return output;
}