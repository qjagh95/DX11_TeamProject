#include "Share.fx"

cbuffer Animation2D	: register(b8)
{
	float2	g_vAnimvUVLT;
	float2	g_vAnimvUVRB;
	int		g_iAnimFrame;
	float3	g_vAnimEmpty;
}

VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = input.vColor;

	return output;
}

VS_OUTPUT_NORMAL_COLOR StandardNormalColorVS(VS_INPUT_NORMAL_COLOR input)
{
	VS_OUTPUT_NORMAL_COLOR	output = (VS_OUTPUT_NORMAL_COLOR)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	//output.vPos = output.vProjPos;
    output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	// Normal을 View로 변환한다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);

	return output;
}

PS_OUTPUT_GBUFFER StandardNormalColorPS(VS_OUTPUT_NORMAL_COLOR input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    if (g_isDeferred == RENDER_FORWARD)
    {
        _tagLightInfo tLight = ComputeLight(input.vViewPos, input.vNormal, float2(0.f, 0.f));

        output.vAlbedo = input.vColor * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
    }
    else
    {
        output.vAlbedo = input.vColor;
        output.vNormal.xyz = input.vNormal;
        output.vNormal.w = g_vLightSpc.w;
        output.vDepth.r = (input.vProjPos.z / input.vProjPos.w);
        output.vDepth.g = g_iFocus;
        output.vDepth.b = g_iDecalEnable;
        output.vDepth.a = input.vProjPos.w;
        output.vMaterial.r = ConvertColor(g_vMtrlDif);
        output.vMaterial.g = ConvertColor(g_vMtrlAmb);
        output.vMaterial.b = ConvertColor(g_vMtrlSpc);
        output.vMaterial.a = ConvertColor(g_vMtrlEmv);
    }

    return output;
}

// ========== Texture Shader ============
VS_OUTPUT_TEX StandardTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	if (g_iAnimation2DEnable == 1)
	{
		if (input.vUV.x == 0.f)
			output.vUV.x = g_vAnimvUVLT.x;

		else
			output.vUV.x = g_vAnimvUVRB.x;

		if (input.vUV.y == 0.f)
			output.vUV.y = g_vAnimvUVLT.y;

		else
			output.vUV.y = g_vAnimvUVRB.y;
	}

	else
		output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE StandardTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float3 vColor = (float3)0;

	if (g_iAnimation2DEnable == 1)
	{
		if (g_iAnimationType == A2D_ATLAS)
			output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * float4(g_vMtrlDif.xyz, 1.0f);

		else
			output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame)) * float4(g_vMtrlDif.xyz, 1.0f);
	}

	else
		output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * float4(g_vMtrlDif.xyz, 1.0f);

	return output;
}

VS_OUTPUT_TEX StandardTexStaticVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE StandardTexStaticPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

	return output;
}

VS_OUTPUT_3D StandardBumpVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0; 

    float3 vPos = input.vPos - g_vPivot * g_vLength; 

    output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz; 
    output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	//output.vPos = output.vProjPos;
    output.vPos = mul(float4(vPos, 1.f), g_matWVP); // 현재 Pixel의 Position에 투영변환된 Matrix를 곱한다.
    output.vUV = input.vUV; 

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
    output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
    output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);

    return output;
}

VS_OUTPUT_3D StandardBumpAnimVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    _tagSkinning tSkinning = Skinning(vPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vBlendWeight,
		input.vBlendIndex);

    output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
    output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;
    output.vUV = input.vUV;

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.f), g_matWV).xyz);
    output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.f), g_matWV).xyz);
    output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.f), g_matWV).xyz);

    return output;
}

PS_OUTPUT_GBUFFER StandardBumpPS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

	float3	vNormal = input.vNormal;

	if (g_vMtrlDif.w == 1.f)
	{
		float4 vNormalCol = g_NormalTex.Sample(g_DiffuseSmp, input.vUV);

		vNormalCol.xyz = vNormalCol.xyz * 2.f - 1.f;

		float3x3	mat =
		{
			input.vTangent,
			input.vBinormal,
			input.vNormal
		};

		vNormal = normalize(mul(vNormalCol.xyz, mat));
	}

    if (g_isDeferred == RENDER_FORWARD)
    {
        _tagLightInfo tLight = ComputeLight(input.vViewPos, vNormal, input.vUV);

        output.vAlbedo = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
    }
    else if(g_isDeferred == RENDER_DEFERRED)
    {
        if (g_vMtrlDif.w == 1.f)
        {
            output.vTangent.xyz = input.vTangent;
            output.vTangent.w = 1.f;

            output.vBinormal.xyz = input.vBinormal;
            output.vBinormal.w = 1.f;
        }

		output.vAlbedo = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
		output.vNormal.xyz = vNormal;
		output.vNormal.w = g_vMtrlSpc.w;
		output.vDepth.r = (input.vProjPos.z / input.vProjPos.w);
        output.vDepth.g = g_iFocus;
        output.vDepth.b = g_iDecalEnable;
		output.vDepth.a = input.vProjPos.w;

		float4	vSpc = g_vMtrlSpc;

		if (g_vMtrlAmb.w == 1.f)
		{
			vSpc = g_SpecularTex.Sample(g_DiffuseSmp, input.vUV);
		}

		output.vMaterial.r = ConvertColor(g_vMtrlDif);
		output.vMaterial.g = ConvertColor(g_vMtrlAmb);
		output.vMaterial.b = ConvertColor(vSpc);
		output.vMaterial.a = ConvertColor(g_vMtrlEmv);
	}

	return output;
}

VS_OUTPUT_3D StandardNaviEditorVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
    output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;
    output.vUV = input.vUV;

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);

    return output;
}

PS_OUTPUT_SINGLE StandardNaviEditorPS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

    vColor.a = 0.5f;

    output.vTarget0 = vColor;

    return output;
}

VS_OUTPUT_3D StandardTexNormalVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
    output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;
    output.vUV = input.vUV;

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);

    return output;
}

VS_OUTPUT_3D StandardTexNormalAnimVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    float3 vPos = input.vPos - g_vPivot * g_vLength;

    _tagSkinning tSkinning = Skinning(vPos, input.vNormal,
		input.vBlendWeight, input.vBlendIndex);

    output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
    output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
    output.vPos = output.vProjPos;
    output.vUV = input.vUV;

	// Normal을 View로 변환한다.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.f), g_matWV).xyz);

    return output;
}

// =========== NullBuffer 출력용 Shader ============
VS_OUTPUT_TEX FullScreenVS(uint iVertexID : SV_VertexID)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	output.vPos = float4(vNullPos[iVertexID], 0.f, 1.f);
	output.vUV = vNullUV[iVertexID];

	return output;
}

PS_OUTPUT_SINGLE FullScreenPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4 vColor = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

    if(vColor.a == 0.f)
        clip(-1);
    
    output.vTarget0 = vColor;

	return output;
}

Texture2D g_ShadowTex : register(t10);
Texture2D g_DepthTex : register(t11);

PS_OUTPUT_SINGLE ShadowPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 vDepth = g_DepthTex.Sample(PointSampler, input.vUV);

    float4 vProjPos = (float4) 0;

    vProjPos.x = input.vUV.x * 2.0f - 1.0f;
    vProjPos.y = input.vUV.y * -2.0f + 1.0f;
    vProjPos.z = vDepth.x;
    vProjPos.w = 1.0f;

    vProjPos *= vDepth.w;

    float4 vShadowProjPos = mul(vProjPos, g_matInvVP);
    vShadowProjPos = mul(vShadowProjPos, g_matLP);
    
    float2 vShadowUV = vShadowProjPos.xy / vShadowProjPos.w;

    vShadowUV.x = saturate(vShadowUV.x * 0.5f + 0.5f);
    vShadowUV.y = saturate(vShadowUV.y * -0.5f + 0.5f);

    float4 vShadowDepth = g_ShadowTex.Sample(PointSampler, vShadowUV);

    if(vShadowDepth.w == 0.0f)
        clip(-1);

    float4 vColor = (float4) 0;

    float fShadowBias = 1.0f;

    float fPixelToLight = vShadowProjPos.w;
    float fAmbientPixel = vShadowDepth.w + fShadowBias;

    if (fPixelToLight > fAmbientPixel)
        vColor = float4(0.2f, 0.2f, 0.2f, 1.0f);
    else
        vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    output.vTarget0 = vColor;

    return output;
}

PS_OUTPUT_SINGLE DownScaleShadowPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float2 vUV = input.vUV * 2.0f;

    float4 vColor = g_ShadowTex.Sample(PointSampler, vUV);

    output.vTarget0 = vColor;

    return output;
}

Texture2D g_DepthTexture : register(t10);
Texture2D g_NormalTexture : register(t11);
Texture2D g_RandomNormalTex : register(t12);

PS_OUTPUT_DS_GBUFFER DownScaleGBufferPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_DS_GBUFFER output = (PS_OUTPUT_DS_GBUFFER) 0;

    float2 vUV = input.vUV * 2.0f;

    float4 vDepth   = g_DepthTexture.Sample(g_DiffuseSmp, vUV);
    float4 vNormal  = g_NormalTexture.Sample(g_DiffuseSmp, vUV);

    if (vDepth.a == 0.f)
        clip(-1);
    
    output.vDepth = vDepth;
    output.vNormal = vNormal;

    return output;
}

static float3 g_vSampleDir[8] =
{
    float3(1.0f, 1.0f, 1.0f)    * 0.125f,
    float3(1.0f, -1.0f, 1.0f)   * 0.25f,
    float3(-1.0f, 1.0f, 1.0f)   * 0.375f,
    float3(-1.0f, -1.0f, 1.0f)  * 0.5f,
    float3(1.0f, 1.0f,   -1.0f) * 0.625f,
    float3(1.0f, -1.0f,  -1.0f) * 0.75f,
    float3(-1.0f, 1.0f,  -1.0f) * 0.875f,
    float3(-1.0f, -1.0f, -1.0f) * 1.0f
};

PS_OUTPUT_SINGLE SSAmbientOcclusionPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float2 vUV = input.vUV * 2.0f;

    float4 vDepth = g_DepthTexture.Sample(g_DiffuseSmp, vUV);
    float3 vNormal = g_NormalTexture.Sample(g_DiffuseSmp, vUV);

    if (vDepth.a == 0.0f)
        clip(-1);

    float4 vViewPos = float4(vUV * 2.0f, vDepth.x, 1.0f);

    vViewPos.x = vViewPos.x - 1.0f;
    vViewPos.y = 1.0f - vViewPos.y;
    vViewPos *= vDepth.w;
    vViewPos = mul(vViewPos, g_matInvProj);

    float fRadius = 20.0f;
    float fRatio = fRadius / 1000.0f;

    float fOcclusion = 0.0f;
    
    float3 vRandomNormal = g_RandomNormalTex.Sample(PointSampler, vUV).xyz;

    vRandomNormal = normalize(vRandomNormal * 2.0f - 1.0f);

    for (int j = 0; j < 8; ++j)
    {
        float3 vRotatedRandomNormal = reflect(g_vSampleDir[j], vRandomNormal);
        vRotatedRandomNormal = normalize(vRotatedRandomNormal) * fRatio;
        float2 CurUV = saturate(vUV + vRotatedRandomNormal.xy);
        float4 CurDepth = g_DepthTexture.Sample(g_DiffuseSmp, CurUV);
        
        float4 CurViewPos = float4(CurUV * 2.0f, CurDepth.x, 1.0f);
        CurViewPos.x = CurViewPos.x - 1.0f;
        CurViewPos.y = 1.0f - CurViewPos.y;
        CurViewPos *= CurDepth.w;

        CurViewPos = mul(CurViewPos, g_matInvProj);

        float3 vViewDir = CurViewPos.xyz - vViewPos.xyz;
        float fLength = length(vViewDir);

        float fAngleFactor = 1.0f - dot(vViewDir / fLength, vNormal);
        float fDistanceFactor = fLength / fRadius;

        fOcclusion += saturate(max(fAngleFactor, fDistanceFactor));
    }

    fOcclusion /= 8.0f;

    output.vTarget0 = fOcclusion;

    return output;
}