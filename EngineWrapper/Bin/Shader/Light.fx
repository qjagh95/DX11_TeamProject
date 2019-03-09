
#include "Share.fx"

SamplerState g_GBufferSmp : register(s10);
Texture2D g_GBufferAlbedoTex : register(t10);
Texture2D g_GBufferNormalTex : register(t11);
Texture2D g_GBufferDepthTex : register(t12);
Texture2D g_GBufferMaterialTex : register(t13);

Texture2D g_LightDifTex : register(t14);
Texture2D g_LightSpcTex : register(t15);

struct PS_OUTPUT_LIGHTACC
{
    float4 vDif : SV_TARGET;
    float4 vSpc : SV_TARGET1;
};

VS_OUTPUT_TEX LightDirVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    output.vPos = float4(vNullPos[iVertexID], 0.f, 1.f);
    output.vUV = vNullUV[iVertexID];

    return output;
}

VS_OUTPUT_TEX LightPointVS(VS_INPUT_POS input)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

    float4 vProjPos = mul(float4(input.vPos, 1.f), g_matWVP);
    output.vPos = vProjPos;

    output.vUV.x = vProjPos.x / vProjPos.w * 0.5f + 0.5f;
    output.vUV.y = -vProjPos.y / vProjPos.w * 0.5f + 0.5f;

    return output;
}

PS_OUTPUT_LIGHTACC LightAccPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_LIGHTACC output = (PS_OUTPUT_LIGHTACC) 0;

    //SV_POSITION = 현재 정점에 WVP변환 후 ViewPortSize가 곱해져서 들어옴.
    float2 UV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vDepth = g_GBufferDepthTex.Sample(g_GBufferSmp, UV);

	// 정점 위치를 구한다. 인자로 들어온 UV값은 화면공간에서의 비율이다.
	// 0 ~ 1 사이로 들어오게 되는 것이다.
	// 이 값을 -1 ~ 1로 변화면 투영공간 좌표에서 w를 나눈 값이 나오게 된다.
	// 이 값을 w를 곱해서 투영공간 좌표로 변환한다.
    //- 정규좌표계(NDCS:Normalized Device Coordinate System, 정규화 장치 좌표계)
    //* 절단 좌표계의 동차좌표(x, y, z, w)를 3 차원 좌표(x / w, y / w, z / w, 1)로 변환하는
    // 원근분할을 가한 좌표
    //- 동차좌표: 사형 기하학의 일종인 그것이 아니고 그래픽스에서 점과 벡터를 구분하기 위한 수치
    //- > v = { x, y, z, w } 의4 개의 변수로 표기할 시에 마지막의 w에 해당하는 변수이다.
    //- > x, y, z좌표에 있는 특정 점 P는 원점 O에서 OP의 벡터로도 표현할 수 있으며 P인점으로도 구분된다.
    //- > 이를 구분하기 위해 동차 W를 사용한다.
    //- > 0.f 이면 벡터, 1.f 이면 점으로 표기한다.

    float4 vProjPos;
    vProjPos.x = input.vUV.x * 2.f - 1.f;
	// y는 uv공간은 아래가 +값이다. 이 값을 3차원 공간에서는 위고 y+ 값이므로
	// -2를 곱해서 아래가 -값이 될 수 있도록 해준다.
    vProjPos.y = input.vUV.y * -2.f + 1.f;
    vProjPos.z = vDepth.r;
    vProjPos.w = 1.f;       

    vProjPos *= vDepth.a;

    //if (g_iLightType == LIGHT_DIR)
    //{
    //    if (vDepth.w == 0.0f)
    //        clip(-1);
    //}

	// 뷰공간으로 변환한다.
    float3 vViewPos = mul(vProjPos, g_matInvProj).xyz;
    float4 vNormalCol = g_GBufferNormalTex.Sample(g_GBufferSmp, UV);
    float3 vNormal = vNormalCol.xyz;
	// 재질정보를 압축을 해제해서 구해준 재질 정보를 이용해서 계산한다.
    float4 vMaterial = g_GBufferMaterialTex.Sample(g_GBufferSmp, UV);
    float3 ToCamera = normalize(-vViewPos);

    _tagLightInfo tLight = ComputeLight(vViewPos, vNormal, vMaterial, vNormalCol.w, ToCamera);

    output.vDif = tLight.vDif + tLight.vAmb;
    output.vSpc = tLight.vSpc;

    output.vSpc.a = 1.f;
    output.vDif.a = 1.f;

    return output;
}

PS_OUTPUT_SINGLE LightBlendPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    float2 UV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vAlbedo = g_GBufferAlbedoTex.Sample(g_GBufferSmp, UV);

    float4 vDif = g_LightDifTex.Sample(g_GBufferSmp, UV);
    float4 vSpc = g_LightSpcTex.Sample(g_GBufferSmp, UV);

    output.vTarget0 = vAlbedo * vDif + vSpc;

    return output;
}
