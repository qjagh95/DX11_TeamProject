#include "Share.fx"

Texture2D<float4> OriginTex         : register(t0);
Texture2D<float4> DepthTex          : register(t1);
Texture2D<float4> BlurTex           : register(t2); //모션블러와 동시에 켜지지 않게?
Texture2D<float4> BloomTex          : register(t3);

StructuredBuffer<float> AvgLum    : register(t10);


static const float2 varrNullPos[4] =
{
    float2(-1.f, 1.f),
    float2(1.f, 1.f),
    float2(-1.f, -1.f),
    float2(1.f, -1.f)
};

static const float2 varrNullUV[4] =
{
    float2(0.f, 0.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 1.f)
};

cbuffer BloomConstants : register(b8)
{
    float fBloomScale;
    float3 vEmpty;
}

cbuffer FinalPass : register(b9)
{
    int     g_iHDR;
    int     g_iBlur;
    int     g_iDepthFog;
    int     g_iBloom;
}

float3 ToneMapping(float3 vHDRColor)
{
    // 현재 픽셀에 대한 휘도 스케일 계산
    float fLScale = dot(vHDRColor, LUM_FACTOR);
    fLScale *= g_fMiddleGrey / AvgLum[0];
    fLScale = (fLScale + fLScale * fLScale / g_fLumWhiteSqr) / (1.f + fLScale);

    // 휘도 스케일을 픽셀 색상에 적용
    return vHDRColor * fLScale;
}

//static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

VS_OUTPUT_TEX FullScreenQuadVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(varrNullPos[iVertexID].xy, 0.f, 1.f);
    output.vUV = varrNullUV[iVertexID].xy;

    return output;
}

PS_OUTPUT_SINGLE FinalPassPS(VS_OUTPUT_TEX Input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    float4 vDepth = DepthTex.Sample(PointSampler, Input.vUV.xy);
    float3 vColor = OriginTex.Sample(PointSampler, Input.vUV.xy).xyz;

    float Focus = vDepth.y;
    float fDepth = vDepth.w;

    if(g_iBlur == 1)
    {
        //if (Focus < 0.5f)
        vColor = BlurTex.Sample(PointSampler, Input.vUV.xy).xyz;
    }

    if (g_iDepthFog == 1)
    {
        //카메라 필터 상수버퍼에 색깔, 시작, 끝 값 넣어서 쓰기.
        float3 vFogColor = float3(0.4f, 0.4f, 0.1f);

        float fDensity = max(0, (100.f - fDepth) / 100.0f);

        vColor = (vColor * fDensity) + (vFogColor * (1.0f - fDensity));
    }
    
    // Bloom 을 적용할 건지
    if(g_iBloom == 1)
    {        
    // Bloom Contribution 을 추가한다
        vColor += fBloomScale * BloomTex.Sample(g_DiffuseSmp, Input.vUV.xy).xyz;

    // 톤 매핑
        if(g_iHDR != 1)
            vColor = ToneMapping(vColor);
    }
    
    // 톤 매핑(HDR 색을 LDR색으로 변환)
    if (g_iHDR == 1)
        vColor = ToneMapping(vColor);

    output.vTarget0 = float4(vColor, 1.f);

    // LDR 값 출력
    return output;
}
