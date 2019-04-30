#include "Share.fx"

cbuffer HitEffectCB : register(b4)
{
    float g_fAlpha;
    float3 g_vEmpty12314;
}

VS_OUTPUT_TEX HitEffectFullScreenVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(vNullPos[iVertexID], 0.f, 1.f);
    output.vUV = vNullUV[iVertexID];

    return output;
}

PS_OUTPUT_SINGLE HitEffectPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
    
    vColor.a = g_fAlpha;

    if(vColor.a > 1.f)
        vColor.a = 1.f;

    if(vColor.a <= 0.f)
        vColor.a = 0.00001f;
    
    output.vTarget0 = vColor;

    return output;
}