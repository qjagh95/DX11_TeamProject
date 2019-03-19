
#include "Share.fx"

Texture2DArray	g_SplatDif		: register(t11);
Texture2DArray	g_SplatNrm		: register(t12);
Texture2DArray	g_SplatSpc		: register(t13);
Texture2DArray	g_SplatAlpha	: register(t14);
SamplerState	g_SplatSmp		: register(s11);

cbuffer LandScape	: register(b11)
{
	float		g_fDetailLevel;
	int			g_iSplatCount;
	float2		g_vLandEmpty;
}

VS_OUTPUT_3D LandScapeVS(VS_INPUT_3D input)
{
	VS_OUTPUT_3D	output = (VS_OUTPUT_3D)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vViewPos = mul(float4(vPos, 1.f), g_matWV);
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
	PS_OUTPUT_GBUFFER	output = (PS_OUTPUT_GBUFFER)0;

	float2	vDetailUV = input.vUV * g_fDetailLevel;

	float3	vNormal = (float3)0.f;
	float4	vSpc = (float4)0.f;

	if (g_vMtrlDif.w == 1.f)
	{
		vNormal = g_NormalTex.Sample(g_DiffuseSmp, vDetailUV).xyz;
	}

	if (g_vMtrlAmb.w == 1.f)
	{
		vSpc = g_SpecularTex.Sample(g_DiffuseSmp, vDetailUV);
	}

	float4	vColor = g_DiffuseTex.Sample(g_DiffuseSmp, vDetailUV);
	float	fAlpha = 0.f;
	float4	vSplatDif = (float4)0.f;
	float3	vSplatNrm = (float3)0.f;
	float4	vSplatSpc = (float4)0.f;

	for (int i = 0; i < g_iSplatCount; ++i)
	{
		fAlpha = g_SplatAlpha.Sample(g_SplatSmp, float3(input.vUV, i)).x;
		vSplatDif = g_SplatDif.Sample(g_SplatSmp, float3(vDetailUV, i));
		vSplatNrm = g_SplatNrm.Sample(g_SplatSmp, float3(vDetailUV, i)).xyz;
		vSplatSpc = g_SplatSpc.Sample(g_SplatSmp, float3(vDetailUV, i));

		vNormal = vNormal * float3(1.f - fAlpha, 1.f - fAlpha, 1.f - fAlpha) +
			vSplatNrm * float3(fAlpha, fAlpha, fAlpha);
		vColor = vColor * float4(1.f - fAlpha, 1.f - fAlpha, 1.f - fAlpha, 1.f - fAlpha) +
			vSplatDif * float4(fAlpha, fAlpha, fAlpha, fAlpha);
		vSpc = vSpc * float4(1.f - fAlpha, 1.f - fAlpha, 1.f - fAlpha, 1.f - fAlpha) +
			vSplatSpc * float4(fAlpha, fAlpha, fAlpha, fAlpha);
	}

	if (g_vMtrlDif.w == 1.f)
	{
		vNormal = vNormal * 2.f - 1.f;
		vNormal = normalize(vNormal);

		float3x3	mat =
		{
			input.vTangent,
			input.vBinormal,
			input.vNormal
		};

		vNormal = normalize(mul(vNormal, mat));
	}

    if (g_isDeferred == RENDER_FORWARD)
	{
		_tagLightInfo	tLight = ComputeLight(input.vViewPos, vNormal, input.vUV);

		output.vAlbedo = vColor * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
	}

	else
	{
		output.vAlbedo = vColor;
		output.vNormal.xyz = vNormal;
		output.vNormal.w = g_vMtrlSpc.w;
		output.vDepth.rgb = (float3)(input.vProjPos.z / input.vProjPos.w);
		output.vDepth.a = input.vProjPos.w;

		if (g_vMtrlAmb.w == 0.f)
		{
			vSpc = g_vMtrlSpc;
		}

		output.vMaterial.r = ConvertColor(g_vMtrlDif);
		output.vMaterial.g = ConvertColor(g_vMtrlAmb);
		output.vMaterial.b = ConvertColor(vSpc);
		output.vMaterial.a = ConvertColor(g_vMtrlEmv);
	}

	return output;
}