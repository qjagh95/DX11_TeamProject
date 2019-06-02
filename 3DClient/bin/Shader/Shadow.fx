#include "Share.fx"

struct VS_OUTPUT_SHADOW
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;
};

VS_OUTPUT_SHADOW ShadowMapVS(VS_INPUT_3D input)
{
    VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    if (g_i3DAnimation == 1)
    {
        vPos = Skinning(vPos, input.vBlendWeight,
			input.vBlendIndex);
    }

    output.vProjPos = mul(float4(vPos, 1.f), g_matWLP);
    output.vPos = mul(float4(vPos, 1.f), g_matWLP);

    return output;
}

PS_OUTPUT_SINGLE ShadowMapPS(VS_OUTPUT_SHADOW input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float2 vProj = input.vProjPos.xy / input.vProjPos.w;

    float fDist = sqrt(vProj.x * vProj.x + vProj.y * vProj.y);
    
    if (fDist > 1.0f)
        clip(-1);

    output.vTarget0.rgb = (float3) (input.vProjPos.z / input.vProjPos.w);
    output.vTarget0.a = input.vProjPos.w;

    return output;
}

