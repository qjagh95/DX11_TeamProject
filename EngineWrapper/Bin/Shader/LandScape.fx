
#include "Share.fx"

VS_OUTPUT_3D LandScapeVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
    output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	//output.vPos = output.vProjPos;
    output.vPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vUV = input.vUV;

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
    output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
    output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);

    return output;
}

PS_OUTPUT_GBUFFER LandScapePS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    float3 vNormal = input.vNormal;

    if (g_vMtrlDif.w == 1.f)
    {
        float4 vNormalCol = g_NormalTex.Sample(g_DiffuseSmp, input.vUV);

        vNormalCol.xyz = vNormalCol.xyz * 2.f - 1.f;

        float3x3 mat =
        {
            input.vTangent,
			input.vBinormal,
			input.vNormal
        };

        vNormal = normalize(mul(vNormalCol.xyz, mat));
    }

    if (g_iDeferredEnable == RENDER_FORWARD)
    {
        _tagLightInfo tLight = ComputeLight(input.vViewPos, vNormal, input.vUV);

        output.vAlbedo = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
    }
    else
    {
        output.vAlbedo = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
        output.vNormal.xyz = vNormal;
        output.vNormal.w = g_vMtrlSpc.w;
        output.vDepth.rgb = (float3) (input.vProjPos.z / input.vProjPos.w);
        output.vDepth.a = input.vProjPos.w;

        float4 vSpc = g_vMtrlSpc;

        if (g_vMtrlAmb.w == 1.f)
        {
            vSpc = g_SpecularTex.Sample(g_DiffuseSmp, input.vUV);
        }

        output.vMaterial.r = ConvertColor(g_vMtrlDif);
        output.vMaterial.g = ConvertColor(g_vMtrlAmb);
        output.vMaterial.b = ConvertColor(vSpc);
        output.vMaterial.a = ConvertColor(g_vMtrlEmv);
    }

    return output;
}