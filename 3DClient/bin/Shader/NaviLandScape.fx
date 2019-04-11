#include "Share.fx"

VS_OUTPUT_3D_COLOR LandScapeColorVS(VS_INPUT_3D_COLOR input)
{
    VS_OUTPUT_3D_COLOR output = (VS_OUTPUT_3D_COLOR) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    output.vPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vUV = input.vUV;
    output.vColor = input.vColor;

    return output;
}

PS_OUTPUT_SINGLE LandScapeColorPS(VS_OUTPUT_3D_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 vColor = input.vColor;

    if (vColor.r > 0.998f)
    {
        vColor.r = 1.0f;
        vColor.g = 0.0f;
        vColor.b = 0.0f;

        output.vTarget0 = vColor;
        return output;
    }

    if (vColor.r > 0.00000001f)
    {
        vColor.r = 0.0f;
        vColor.g = 1.0f;
        vColor.b = 0.5f;

        output.vTarget0 = vColor;
        return output;
    }

    output.vTarget0 = vColor;
    
    return output;
}