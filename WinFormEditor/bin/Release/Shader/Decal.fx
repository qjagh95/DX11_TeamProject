#include "Share.fx"

Texture2D g_GBufferDepthTex : register(t12);
Texture2D g_GBufferTangentTex : register(t13);
Texture2D g_GBufferBinormalTex : register(t14);

struct VS_OUTPUT_DECAL
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;
};

struct PS_OUTPUT_DECAL
{
    float4 vAlbedo      : SV_TARGET;
    float4 vNormal      : SV_TARGET1;
    float4 vMaterial    : SV_TARGET2;
};

VS_OUTPUT_DECAL SSDVS(VS_INPUT_POS input)
{
    VS_OUTPUT_DECAL output = (VS_OUTPUT_DECAL) 0;

    output.vProjPos = mul(float4(input.vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;

    return output;
}

PS_OUTPUT_DECAL SSDPS(VS_OUTPUT_DECAL input)
{
    PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL) 0;

    float2 vUV = input.vProjPos.xy / input.vProjPos.w;
    vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

    float4 vDepth = g_GBufferDepthTex.Sample(g_DiffuseSmp, vUV);

    if (vDepth.a == 0.0f)
        clip(-1);

	// Decal을 적용 안해야 될 경우
    if (vDepth.b == 0.0f)
        clip(-1);

    float4 vPos;
    //래스터화기를 거치기 전의 투영공간 좌표를 구한다.
    vPos.x = vUV.x * 2.f - 1.f;
    vPos.y = vUV.y * -2.f + 1.f;
    vPos.z = vDepth.r;
    vPos.xyz *= vDepth.w;
    vPos.w = vDepth.w;

	// 데칼 상자의 로컬 공간으로 변환한다.
    float3 vLocalPos = mul(vPos, g_matInvWVP).xyz;

    clip(0.5f - abs(vLocalPos));

    float2 vScreenUV = vLocalPos.xz + float2(0.5f, 0.5f);
    vScreenUV.y = 1.f - vScreenUV.y;

    float4 vColor = g_DiffuseTex.Sample(g_DiffuseSmp, vScreenUV);

    vColor.a *= 5.5f;
    

    if (vColor.a == 0.f)
        clip(-1);
    else if(vColor.a > 1.0f)
        vColor.a = 1.0f;

    output.vAlbedo = vColor;

    float3 vTangent = g_GBufferTangentTex.Sample(g_DiffuseSmp, vUV).xyz;
    float3 vBinormal = g_GBufferBinormalTex.Sample(g_DiffuseSmp, vUV).xyz;
    float3 vVtxNormal = normalize(cross(vTangent, vBinormal));

    if (g_vMtrlDif.w == 1.f)
    {
        float3 vNormal = g_NormalTex.Sample(g_DiffuseSmp, vScreenUV).xyz;

        vNormal = vNormal * 2.f - 1.f;
        vNormal = normalize(lerp(float3(0.0f, 0.0f, 1.0f), vNormal, g_fBumpScale));

        float3x3 mat =
        {
            vTangent,
			vBinormal,
			vVtxNormal
        };

        vNormal = mul(vNormal, mat);
        vNormal = normalize(vNormal);

        output.vNormal.xyz = vNormal;
        output.vNormal.w = 1.f;
    }
    else
    {
        output.vNormal.xyz = vVtxNormal;
        output.vNormal.w = 1.f;
    }

    float4 vMtrlSpc = g_vMtrlSpc;

    if (g_vMtrlAmb.w != 0.f)
    {
        vMtrlSpc.xyz = g_SpecularTex.Sample(g_DiffuseSmp, vScreenUV).xyz;
        vMtrlSpc.w = g_vMtrlSpc.w;
    }

    output.vMaterial.r = ConvertColor(g_vMtrlDif);
    output.vMaterial.g = ConvertColor(g_vMtrlAmb);
    output.vMaterial.b = ConvertColor(vMtrlSpc);
    output.vMaterial.a = ConvertColor(g_vMtrlEmv);

    return output;
}
