
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

    //SV_POSITION = ���� ������ WVP��ȯ �� ViewPortSize�� �������� ����.
    float2 UV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vDepth = g_GBufferDepthTex.Sample(g_GBufferSmp, UV);

	// ���� ��ġ�� ���Ѵ�. ���ڷ� ���� UV���� ȭ����������� �����̴�.
	// 0 ~ 1 ���̷� ������ �Ǵ� ���̴�.
	// �� ���� -1 ~ 1�� ��ȭ�� �������� ��ǥ���� w�� ���� ���� ������ �ȴ�.
	// �� ���� w�� ���ؼ� �������� ��ǥ�� ��ȯ�Ѵ�.
    //- ������ǥ��(NDCS:Normalized Device Coordinate System, ����ȭ ��ġ ��ǥ��)
    //* ���� ��ǥ���� ������ǥ(x, y, z, w)�� 3 ���� ��ǥ(x / w, y / w, z / w, 1)�� ��ȯ�ϴ�
    // ���ٺ����� ���� ��ǥ
    //- ������ǥ: ���� �������� ������ �װ��� �ƴϰ� �׷��Ƚ����� ���� ���͸� �����ϱ� ���� ��ġ
    //- > v = { x, y, z, w } ��4 ���� ������ ǥ���� �ÿ� �������� w�� �ش��ϴ� �����̴�.
    //- > x, y, z��ǥ�� �ִ� Ư�� �� P�� ���� O���� OP�� ���ͷε� ǥ���� �� ������ P�������ε� ���еȴ�.
    //- > �̸� �����ϱ� ���� ���� W�� ����Ѵ�.
    //- > 0.f �̸� ����, 1.f �̸� ������ ǥ���Ѵ�.

    float4 vProjPos;
    vProjPos.x = input.vUV.x * 2.f - 1.f;
	// y�� uv������ �Ʒ��� +���̴�. �� ���� 3���� ���������� ���� y+ ���̹Ƿ�
	// -2�� ���ؼ� �Ʒ��� -���� �� �� �ֵ��� ���ش�.
    vProjPos.y = input.vUV.y * -2.f + 1.f;
    vProjPos.z = vDepth.r;
    vProjPos.w = 1.f;       

    vProjPos *= vDepth.a;

    //if (g_iLightType == LIGHT_DIR)
    //{
    //    if (vDepth.w == 0.0f)
    //        clip(-1);
    //}

	// ��������� ��ȯ�Ѵ�.
    float3 vViewPos = mul(vProjPos, g_matInvProj).xyz;
    float4 vNormalCol = g_GBufferNormalTex.Sample(g_GBufferSmp, UV);
    float3 vNormal = vNormalCol.xyz;
	// ���������� ������ �����ؼ� ������ ���� ������ �̿��ؼ� ����Ѵ�.
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
