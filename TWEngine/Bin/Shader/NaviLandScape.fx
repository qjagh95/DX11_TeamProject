#include "Share.fx"

cbuffer NaviLandCBuffer : register(b12)
{
    float3 g_BrushPos;
    int g_BrushXRange;
    int g_BrushZRange;
    float4 g_BrushColor;
    float3 vEmpty;
};

cbuffer GridCBuffer : register(b10)
{
    int g_GridLineSize;
    int g_isVisibleGrid;
    float g_GridLineThickness;
    float Empty12313234;
    float4 g_GridColor;
}

float4 GetBrushColor(float3 vPos, float4 BasicColor)
{
    float XRange = g_BrushXRange * 0.5f;
    float ZRange = g_BrushZRange * 0.5f;

    if ((vPos.x >= g_BrushPos.x - XRange) &&
        (vPos.x <= g_BrushPos.x + XRange) &&
        (vPos.z >= g_BrushPos.z - ZRange) &&
        (vPos.z <= g_BrushPos.z + ZRange))

        return g_BrushColor;

    return BasicColor;
}

float4 GetGridColor(float3 vPos)
{
    if (g_isVisibleGrid == 0)
        return float4(0.0f, 0.0f, 0.0f, 0.0f);

    float2 Grid = vPos.xz / g_GridLineSize;
    float2 Range = abs(frac(Grid - 0.5f) - 0.5f);
    float2 Speed = fwidth(Grid);
    float2 Pixel = Range / Speed;
    float Weight = saturate(min(Pixel.x, Pixel.y) - g_GridLineThickness);

    return lerp(g_GridColor, float4(0.0f, 0.0f, 0.0f, 0.0f), Weight);
}

VS_OUTPUT_3D_COLOR LandScapeColorVS(VS_INPUT_3D_COLOR input)
{
    VS_OUTPUT_3D_COLOR output = (VS_OUTPUT_3D_COLOR) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    float3 sPos = mul(float4(vPos, 1.0f), g_matWorld);
    output.vPos = mul(float4(vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vColor = input.vColor;
    output.oPos = sPos;
    
    return output;
}

PS_OUTPUT_SINGLE LandScapeColorPS(VS_OUTPUT_3D_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    float4 vColor = GetBrushColor(input.oPos, input.vColor) + GetGridColor(input.oPos);

    vColor.a = -1.0f;

    output.vTarget0 = vColor;
    
    return output;
}