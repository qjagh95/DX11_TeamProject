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
        output.vDepth.rb = (float3) (input.vProjPos.z / input.vProjPos.w);
        output.vDepth.g = 0.0f;
        if(g_iFocus == 1)
            output.vDepth.g = 1.0f;
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

	if (g_iAnimation2DEnable == 1)
	{
		if (g_iAnimationType == A2D_ATLAS)
			output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * g_vMtrlDif;

		else
			output.vTarget0 = g_DiffuseTexArray.Sample(g_DiffuseSmp, float3(input.vUV, g_iAnimFrame)) * g_vMtrlDif;
	}

	else
		output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * g_vMtrlDif;

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
    else
    {
		output.vAlbedo = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);
		output.vNormal.xyz = vNormal;
		output.vNormal.w = g_vMtrlSpc.w;
		output.vDepth.rb = (float3)(input.vProjPos.z / input.vProjPos.w);
        output.vDepth.g = 0.0f;
        if (g_iFocus == 1)
            output.vDepth.g = 1.0f;

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
