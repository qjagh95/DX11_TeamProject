#include "Share.fx"

// �߰����ð�� ��� ����
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

    // Aspect Ratio (��Ⱦ��)
    float ratio = g_ViewPortSize.y / g_ViewPortSize.x;

    if (isFlag == 1)
    {
        // �߰����ð� ����
        float red, green, blue = 0.f;
        red   =   1.f / 255.f;
        green = 150.f / 255.f;
        blue  =  30.f / 255.f;
        float4 filterColor = float4(red, green, blue, 1.f);        

        //ȭ�� �߾��� 0,0���� �����ֱ� ���� ����
        float2 cvtUV = input.vUV * 2.0f - 1.0f;
        //��Ⱦ�� �ڵ����� ������ �ȵż� Ÿ�������� �����⶧���� ���� ���Ѵ�.
        float powX = pow((cvtUV.x / ratio), 2);
        float powY = pow(cvtUV.y, 2);
        float distance = abs(sqrt(powX + powY));
        distance *= 0.8f;

        float fEnd = g_cameraNearFar.y - 950.f;
        //���̿����� DepthFog����� ȿ���� ������ �Ѵ�.
        float zInten = saturate(vDepthColor.w / fEnd); // z intencity

        // �Ÿ� ���� ����Ͽ� ��Ӱ� ó���Ѵ�.
        red   *= saturate(1.0f - distance) * (1.0f - zInten);
        green *= saturate(1.0f - distance) * (1.0f - zInten);
        blue  *= saturate(1.0f - distance) * (1.0f - zInten);
        filterColor = float4(red, green, blue, 1.f);

        // ������ ȿ��
        vNoiseColor *= 200.0f;
        
        // ���� ���� ��
        vColor *= filterColor * vNoiseColor;
    }
    
    output.vTarget0 = vColor * fOnOff;
    return output;
}

