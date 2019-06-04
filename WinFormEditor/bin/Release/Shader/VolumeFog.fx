#include "Share.fx"

Texture2D g_DepthTex    : register(t9);
Texture2D g_FogDepthTex : register(t10);
Texture2D g_OriginTex   : register(t11);

struct VS_OUTPUT_FOG
{
    float4 vPos     : SV_POSITION;
    float4 vProjPos : POSITION;
    float2 vUV      : TEXCOORD;
};

cbuffer FogCBuffer : register(b9)
{
    float4  g_vFogColor;
    float   g_fFogMaxScale;
    float3  g_vFogEmpty;
}

VS_OUTPUT_FOG VolumeFogDepthVS(VS_INPUT_POS input)
{
    VS_OUTPUT_FOG output = (VS_OUTPUT_FOG) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;
    output.vUV.x = output.vProjPos.x / output.vProjPos.w * 0.5f + 0.5f;
    output.vUV.y = -output.vProjPos.y / output.vProjPos.w * 0.5f + 0.5f;

    return output;
}

PS_OUTPUT_SINGLE VolumeFogDepthFrontPS(VS_OUTPUT_FOG input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float fDepth = input.vProjPos.w;

    output.vTarget0 = float4(fDepth, 0.0f, input.vUV.x, input.vUV.y);

    return output;
}

PS_OUTPUT_SINGLE VolumeFogDepthBackPS(VS_OUTPUT_FOG input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float fDepth = input.vProjPos.w;

    output.vTarget0 = float4(fDepth, 0.0f, input.vUV.x, input.vUV.y);

    return output;
}

PS_OUTPUT_SINGLE VolumeFogColorPS(VS_OUTPUT_FOG input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    float2 vUV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vColor = g_OriginTex.Sample(g_DiffuseSmp, vUV);
    float fFrontDepth = g_DepthTex.Sample(g_DiffuseSmp, vUV).x;
    float fBackDepth = g_FogDepthTex.Sample(g_DiffuseSmp, vUV).x;

    float fThickness;

    fThickness = fFrontDepth - fBackDepth;

    float fDensity = min(1, (fThickness / g_fFogMaxScale));

    vColor = (vColor * (1.0f - fDensity)) + (g_vFogColor * fDensity);

    output.vTarget0 = vColor;

    return output;
}