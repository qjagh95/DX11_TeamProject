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
    int     g_iMotionBlur;
    int     g_iDepthFog;
    int     g_iBloom;
    int     g_iAdaptation;
	int		g_iDistortion;
    float   g_fFadeAmount;
	float	g_fWaveScale;
	float	g_fWaveSpeed;
	float2	g_vWaveCenter;
}

cbuffer DepthFog : register(b13)
{
    float4 g_vFogColor;
    float  g_fStartDepth;
    float  g_fEndDepth;
    float2 g_vDepthFogEmpty;
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
 
	float2 vUV = Input.vUV;

	if (g_iDistortion)
	{
		if (g_fWaveScale > 0)
		{
			float2 p = float2(vUV.x, vUV.y - 1.f);
			p.x -= g_vWaveCenter.x;
			p.y += g_vWaveCenter.y;

			float lenth = sqrt(pow(p.x, 2) + pow(p.y, 2));

			float fScale = (1.f - lenth) * g_fWaveScale;

			if (fScale < 0.000001f)
			{
				fScale = 0.000000f;
			}

			vUV = vUV + (((p / lenth) * cos(g_DeltaTime * g_fWaveSpeed) * fScale) + (fScale * 0.5f));
		}
	}

    float4 vDepth = DepthTex.Sample(PointSampler, vUV);
    float3 vColor = OriginTex.Sample(PointSampler, vUV).xyz;

    float Focus = vDepth.y;
    float fDepth = vDepth.w;



    if (g_iBlur == 1 || g_iMotionBlur == 1)
    {
        if (Focus < 0.5f)
            vColor = BlurTex.Sample(PointSampler, vUV).xyz;
    }

    if (g_iDepthFog == 1)
    {
        //카메라 필터 상수버퍼에 색깔, 시작, 끝 값 넣어서 쓰기.

        float fDensity = saturate((g_fEndDepth - fDepth) / (g_fEndDepth - g_fStartDepth));

        vColor = (vColor * fDensity) + (g_vFogColor.xyz * (1.0f - fDensity));
    }
    
    // Bloom 을 적용할 건지
    if(g_iBloom == 1)
    {
    // Bloom Contribution 을 추가한다
        vColor += fBloomScale * BloomTex.Sample(g_DiffuseSmp, vUV).xyz;
    }
    
    // 톤 매핑(HDR 색을 LDR색으로 변환)
    if (g_iHDR == 1 || g_iAdaptation == 1)
        vColor = ToneMapping(vColor);

    output.vTarget0 = float4(vColor, 1.f) * g_fFadeAmount;

    // LDR 값 출력
    return output;
}
