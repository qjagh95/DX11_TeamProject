#include "Share.fx"

cbuffer Collider	: register(b8)
{
	float4	g_vColor;
}

VS_OUTPUT_POS ColliderVS(VS_INPUT_POS input)
{
	VS_OUTPUT_POS	output = (VS_OUTPUT_POS)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	return output;
}

PS_OUTPUT_SINGLE ColliderPS()
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = g_vColor;

	return output;
}