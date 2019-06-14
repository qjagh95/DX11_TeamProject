#include "ComputeShare.fx"

Texture2D g_DepthTex : register(t1);

cbuffer Blur : register(b1)
{
	int     g_iNumPixel;
	int     g_iDepthOfField;
	float   g_fCameraFar;
	float   g_fBlurDensity;
}

groupshared float4 g_vSharedColor[2][2];
groupshared float g_vDepth[2][2];
groupshared float4 g_vBlurColorH[960];
groupshared float4 g_vBlurColorV[540];

[numthreads(960, 1, 1)]
void HorizontalBlur(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
	float4 vColor = (float4) 0;
	int iMaxCount = g_iNumPixel;

	if (iMaxCount % 2 == 0)
		iMaxCount -= 1;

	int iMaxOffset = iMaxCount * 0.5f;
	int3 vUV;

	vUV.xy = vDispatchThreadID.xy;
	vUV.z = 0;

	float4 vOriginColor = g_BaseTex.Load(vUV);

	if (!isfinite(vOriginColor.x) || !isfinite(vOriginColor.y) || !isfinite(vOriginColor.z) || !isfinite(vOriginColor.a))
		vOriginColor = float4(0.0f, 0.0f, 0.0f, 10.0f);


	g_vBlurColorH[vGroupThreadID.x] = vOriginColor;

	GroupMemoryBarrierWithGroupSync();

	int k = 0;

	float fMyBlur = 1.0f - g_fBlurDensity;

	if (fMyBlur > 1.0f)
		fMyBlur = 1.0f;
	else if (fMyBlur < 1.0f / (float)iMaxCount)
		fMyBlur = 1.0f / (float)iMaxCount;

	float fBlur = (1.0f - fMyBlur) / (float)iMaxCount;

	if (fBlur < 0.0f)
		fBlur = 0.0f;

	vColor = vOriginColor * fMyBlur;

	if (g_iDepthOfField == 1)
	{
		float fDepth = vOriginColor.a / g_fCameraFar;
		if (fDepth > 1.0f || fDepth < 0.000001f)
			fDepth = 0.9f;

		fBlur = fDepth / (float)(iMaxCount - 1);
		vColor = vOriginColor * (1.0f - fDepth);
	}

	for (int i = -iMaxOffset; i <= iMaxOffset; ++i)
	{
		k = vGroupThreadID.x + i;

		if (k >= 0 && k < 960)
		{
			if (i == 0)
				continue;

			vColor += g_vBlurColorH[k] * fBlur;
		}
	}

	g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}

[numthreads(1, 540, 1)]
void VerticalBlur(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
	float4 vColor = (float4) 0;
	int iMaxCount = g_iNumPixel;

	if (iMaxCount % 2 == 0)
		iMaxCount -= 1;

	int iMaxOffset = iMaxCount / 2;
	int3 vUV;
	vUV.xy = vDispatchThreadID.xy;
	vUV.z = 0;

	float4 vOriginColor = g_BaseTex.Load(vUV);

	if (!isfinite(vOriginColor.x) || !isfinite(vOriginColor.y) || !isfinite(vOriginColor.z))
		vOriginColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	g_vBlurColorV[vGroupThreadID.y] = vOriginColor;


	GroupMemoryBarrierWithGroupSync();

	float fMyBlur = 1.0f - g_fBlurDensity;

	if (fMyBlur > 1.0f)
		fMyBlur = 1.0f;
	else if (fMyBlur < 1.0f / (float)iMaxCount)
		fMyBlur = 1.0f / (float)iMaxCount;

	float fBlur = (1.0f - fMyBlur) / (float)iMaxCount;

	if (fBlur < 0.0f)
		fBlur = 0.0f;

	vColor = vOriginColor * fMyBlur;

	if (g_iDepthOfField == 1)
	{
		float fDepth = vOriginColor.a / g_fCameraFar;
		if (fDepth > 1.0f || fDepth < 0.000001f)
			fDepth = 0.9f;
		fBlur = fDepth / (float)(iMaxCount - 1);
		vColor = vOriginColor * (1.0f - fDepth);
	}

	int k = 0;

	for (int i = -iMaxOffset; i <= iMaxOffset; ++i)
	{
		k = vGroupThreadID.y + i;

		if (k >= 0 && k < 540)
		{
			if (i == 0)
				continue;

			vColor += g_vBlurColorV[k] * fBlur;
		}
	}

	g_RWOutputTex[vDispatchThreadID.xy] = vColor;
}