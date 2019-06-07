
#include "Share.fx"

Texture2D g_GBufferDepth : register(t11);

cbuffer Animation2D : register(b8)
{
    float2  g_vAnimvUVLT;
    float2  g_vAnimvUVRB;
    int     g_iAnimFrame;
    float3  g_vAnimEmpty;
}

cbuffer Particle : register(b4)
{
    float3  g_vParticleCenter;
    float   g_fParticleSizeX;
    float3  g_vCameraAxisX;
    float   g_fParticleSizeY;
    float3  g_vCameraAxisY;
    float   g_fParticleEmpty;
}

struct GS_OUTPUT_PARTICLE
{
    float4 vPos     : SV_POSITION;
    float4 vProjPos : POSITION;
    float2 vUV      : TEXCOORD;
};

VS_OUTPUT_POS ParticleVS(VS_INPUT_POS input)
{
    VS_OUTPUT_POS output = (VS_OUTPUT_POS) 0;

    return output;
}

[maxvertexcount(4)]
void ParticleGS(point VS_OUTPUT_POS input[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> stream)
{
    float fHalfX = 0.5f * g_fParticleSizeX;
    float fHalfY = 0.5f * g_fParticleSizeY;

    float3 vPos[4];
    vPos[0] = g_vParticleCenter - g_vCameraAxisX * fHalfX - g_vCameraAxisY * fHalfY;
    vPos[1] = g_vParticleCenter - g_vCameraAxisX * fHalfX + g_vCameraAxisY * fHalfY;
    vPos[2] = g_vParticleCenter + g_vCameraAxisX * fHalfX - g_vCameraAxisY * fHalfY;
    vPos[3] = g_vParticleCenter + g_vCameraAxisX * fHalfX + g_vCameraAxisY * fHalfY;

    float2 vUV[4] =
    {
        float2(0.f, 1.f),
		float2(0.f, 0.f),
		float2(1.f, 1.f),
		float2(1.f, 0.f)
    };

    GS_OUTPUT_PARTICLE output;

    for (int i = 0; i < 4; ++i)
    {
        output.vProjPos = mul(float4(vPos[i], 1.f), g_matVP);
        output.vPos = output.vProjPos;

        if (g_iAnimation2DEnable == 1)
        {
            if (vUV[i].x == 0.f)
                output.vUV.x = g_vAnimvUVLT.x;
            else
                output.vUV.x = g_vAnimvUVRB.x;

            if (vUV[i].y == 0.f)
                output.vUV.y = g_vAnimvUVLT.y;
            else
                output.vUV.y = g_vAnimvUVRB.y;
        }
        else
            output.vUV = vUV[i];

        stream.Append(output);
    }
}

PS_OUTPUT_SINGLE ParticlePS(GS_OUTPUT_PARTICLE input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    if (g_iAnimation2DEnable == 1)
    {
        if (g_iAnimationType == A2D_ATLAS)
            output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
        else
            output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame));
    }
    else
        output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

    if (output.vTarget0.a == 0.f)
        clip(-1);

    float2 vDepthUV = input.vProjPos.xy / input.vProjPos.w;
    vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;
    vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

    float4 vDepth = g_GBufferDepth.Sample(g_DiffuseSmp, vDepthUV);

	// 뷰 공간의 Z값을 비교하여 거리를 구한다.
    float fDist = vDepth.w - input.vProjPos.w;

    if (vDepth.w == 0.f)
        fDist = 1.f;
    else if (fDist < 0.f)
        clip(-1);
    else if (fDist == 0.f)
        fDist = 0.4f;

    float fAlpha = fDist / 0.4f;
    fAlpha = min(fAlpha, 1.f);

    output.vTarget0.a *= fAlpha;

    return output;
}


PS_OUTPUT_SINGLE ParticlePS_GreenMat(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_iAnimation2DEnable == 1)
	{
		if (g_iAnimationType == A2D_ATLAS)
			output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
		else
			output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame));
	}
	else
		output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

	if (output.vTarget0.a == 0.f)
		clip(-1);

	float2 vDepthUV = input.vProjPos.xy / input.vProjPos.w;
	vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;
	vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

	float4 vDepth = g_GBufferDepth.Sample(g_DiffuseSmp, vDepthUV);

	//알파값 = r + b값 / 2.f, 그린이 1일 때
	output.vTarget0.a = 1 - output.vTarget0.g;
	// 뷰 공간의 Z값을 비교하여 거리를 구한다.

	//그린값 : 꼼수로 r + b 만큼 주되, 한쪽으로 쏠려있으면 줄이자
	output.vTarget0.g = ((output.vTarget0.r + output.vTarget0.b) - abs(2.f * (output.vTarget0.r - output.vTarget0.b))) / 2.f;
	float fDist = vDepth.w - input.vProjPos.w;

	if (vDepth.w == 0.f)
		fDist = 1.f;
	else if (fDist < 0.f)
		clip(-1);
	else if (fDist == 0.f)
		fDist = 0.4f;

	float fAlpha = fDist / 0.4f;
	fAlpha = min(fAlpha, 1.f);

	output.vTarget0.a *= fAlpha; //거리에 따른 알파 처리

	return output;
}

PS_OUTPUT_SINGLE ParticlePS_BlueMat(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_iAnimation2DEnable == 1)
	{
		if (g_iAnimationType == A2D_ATLAS)
			output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
		else
			output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame));
	}
	else
		output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

	if (output.vTarget0.a == 0.f)
		clip(-1);

	float2 vDepthUV = input.vProjPos.xy / input.vProjPos.w;
	vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;
	vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

	float4 vDepth = g_GBufferDepth.Sample(g_DiffuseSmp, vDepthUV);

	//알파값 = r + g값 / 2.f, 
	output.vTarget0.a = 1 - output.vTarget0.b;
	// 뷰 공간의 Z값을 비교하여 거리를 구한다.

	//그린값 : 꼼수로 r + g 만큼 주되, 한쪽으로 쏠려있으면 줄이자
	output.vTarget0.b = ((output.vTarget0.r + output.vTarget0.g) - abs(2.f * (output.vTarget0.r - output.vTarget0.g))) / 2.f;
	float fDist = vDepth.w - input.vProjPos.w;

	if (vDepth.w == 0.f)
		fDist = 1.f;
	else if (fDist < 0.f)
		clip(-1);
	else if (fDist == 0.f)
		fDist = 0.4f;

	float fAlpha = fDist / 0.4f;
	fAlpha = min(fAlpha, 1.f);

	output.vTarget0.a *= fAlpha; //거리에 따른 알파 처리

	return output;
}

PS_OUTPUT_SINGLE ParticlePS_BlackMat(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	if (g_iAnimation2DEnable == 1)
	{
		if (g_iAnimationType == A2D_ATLAS)
			output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
		else
			output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame));
	}
	else
		output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

	if (output.vTarget0.a == 0.f)
		clip(-1);

	float2 vDepthUV = input.vProjPos.xy / input.vProjPos.w;
	vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;
	vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

	float4 vDepth = g_GBufferDepth.Sample(g_DiffuseSmp, vDepthUV);

	//알파값 : 3.0f - 세 개 빛의 총합
	output.vTarget0.a = (output.vTarget0.r + output.vTarget0.g + output.vTarget0.b) / 3.f;
	float fDist = vDepth.w - input.vProjPos.w;
	if (vDepth.w == 0.f)
		fDist = 1.f;
	else if (fDist < 0.f)
		clip(-1);
	else if (fDist == 0.f)
		fDist = 0.4f;

	float fAlpha = fDist / 0.4f;
	fAlpha = min(fAlpha, 1.f);

	output.vTarget0.a *= fAlpha; //거리에 따른 알파 처리

	return output;
}