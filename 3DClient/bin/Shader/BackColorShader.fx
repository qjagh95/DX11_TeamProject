#include "Share.fx"

VS_OUTPUT_TEX BackColorVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX)0;

	float3 vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.0f), g_matWVP);

	return output;
}

PS_OUTPUT_SINGLE BackColorPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = g_vMtrlDif;

	return output;
}