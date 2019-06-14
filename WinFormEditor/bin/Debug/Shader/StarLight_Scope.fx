#include "Share.fx"

// 야간투시경용 상수 버퍼
cbuffer cbCameraFilter : register(b12)
{
    int isFlag;
    int isShake;
    int isSign;
    float fOnOff;
    float2 vRandomPos;
    float2 vcbCameraEmpty;
}

// Texture
Texture2D tex2D_FinalTarget : register(t0);
Texture2D tex2D_Noise       : register(t1);
Texture2D tex2D_Depth       : register(t2);

VS_OUTPUT_TEX StarLightScopeVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    output.vPos = float4(vNullPos[iVertexID].xy, 0.f, 1.f);
    output.vUV  = vNullUV[iVertexID];
    
    return output;
}

PS_OUTPUT_SINGLE StarLightScopePS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    float2 noiseUV = input.vUV;
    if (isShake == 1)
    {
        //noiseUV += vRandomPos * isSign;
        noiseUV.x += vRandomPos.x * isSign;
    }

    // Sampler
    float4 vColor      = tex2D_FinalTarget.Sample(g_DiffuseSmp, input.vUV);
    float4 vDepthColor = tex2D_Depth.Sample(PointSampler, input.vUV);
    float4 vNoiseColor = tex2D_Noise.Sample(g_DiffuseSmp, noiseUV);

    // Aspect Ratio (종횡비)
    float ratio = g_ViewPortSize.y / g_ViewPortSize.x;

    if (isFlag == 1)
    {
        // 야간투시경 색상
        float red, green, blue = 0.f;
        red   =   1.f / 255.f;
        green = 150.f / 255.f;
        blue  =  30.f / 255.f;
        float4 filterColor = float4(red, green, blue, 1.f);        

        //화면 중앙을 0,0으로 맞춰주기 위한 보정
        float2 cvtUV = input.vUV * 2.0f - 1.0f;
        //종횡비가 자동으로 보정이 안돼서 타원형으로 나오기때문에 직접 곱한다.
        float powX = pow((cvtUV.x / ratio), 2);
        float powY = pow(cvtUV.y, 2);
        float distance = abs(sqrt(powX + powY));
        distance *= 0.8f;

        float fEnd = g_cameraNearFar.y - 950.f;
        //깊이에따라서 DepthFog비슷한 효과를 내도록 한다.
        float zInten = saturate(vDepthColor.w / fEnd); // z intencity

        // 거리 값에 비례하여 어둡게 처리한다.
        red   *= saturate(1.0f - distance) * (1.0f - zInten);
        green *= saturate(1.0f - distance) * (1.0f - zInten);
        blue  *= saturate(1.0f - distance) * (1.0f - zInten);
        filterColor = float4(red, green, blue, 1.f);

        // 노이즈 효과
        vNoiseColor *= 200.0f;
        
        // 최종 색상 값
        vColor *= filterColor * vNoiseColor;
    }
    
    output.vTarget0 = vColor * fOnOff;
    return output;
}

