struct VS_INPUT_COLOR
{
	// semantic은 이름 뒤에 숫자를 붙여준다. 안붙여줄 경우 기본으로
	// 0으로 지정된다.
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	float4	vPos	: SV_POSITION;
	float4	vProjPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_INPUT_NORMAL_COLOR
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_NORMAL_COLOR
{
	float4	vPos	: SV_POSITION;
	float4	vProjPos	: POSITION;
	float3	vViewPos	: POSITION1;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
};

struct VS_INPUT_POS
{
	float3	vPos	: POSITION;
};

struct VS_OUTPUT_POS
{
	float4	vPos	: SV_POSITION;
};

struct VS_INPUT_TEX
{
	// semantic은 이름 뒤에 숫자를 붙여준다. 안붙여줄 경우 기본으로
	// 0으로 지정된다.
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
};

struct VS_OUTPUT_TEX
{
	// semantic은 이름 뒤에 숫자를 붙여준다. 안붙여줄 경우 기본으로
	// 0으로 지정된다.
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
    float4 vProjPos : POSITION;
};

struct VS_INPUT_3D
{
    float3 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vUV          : TEXCOORD;
    float3 vTangent     : TANGENT;
    float3 vBinormal    : BINORMAL;
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex  : BLENDINDICES;
};

struct VS_OUTPUT_3D
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;
    float3 vViewPos : POSITION1;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;
};

struct VS_INPUT_3D_COLOR
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR0;
};

struct VS_OUTPUT_3D_COLOR
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR0;
    float3 oPos : POSITION1;
};

struct PS_OUTPUT_SINGLE
{
	float4	vTarget0	: SV_Target;
};
struct PS_OUTPUT_GBUFFER
{
    float4 vAlbedo      : SV_TARGET;
    float4 vNormal      : SV_TARGET1;
    float4 vDepth       : SV_TARGET2;
    float4 vMaterial    : SV_TARGET3;
    float4 vTangent     : SV_Target4;
    float4 vBinormal    : SV_Target5;
};
// 16바이트 패딩을 맞춰 주어야한다

struct PS_OUTPUT_DS_GBUFFER
{
    float4 vDepth       : SV_TARGET;
    float4 vNormal      : SV_TARGET1;
};

static const float2 vNullPos[4] =
{
    float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

static const float2 vNullUV[4] =
{
    float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
    matrix  g_matWLP;
    matrix  g_matInvWVP;
    matrix  g_matInvProj;
    matrix  g_matVP;
    matrix  g_matInvVP;
    matrix  g_matLP;
	float3	g_vPivot;
    int     g_iShadowEnable;
	float3	g_vLength;
	float	g_fEmptyTr1;
}

cbuffer Material	: register(b1)
{
	float4	g_vMtrlDif;
	float4	g_vMtrlAmb;
	float4	g_vMtrlSpc;
	float4	g_vMtrlEmv;
}

cbuffer Component	: register(b2)
{
	int		g_iAnimation2DEnable;
	int		g_iAnimationType;
    int     g_iDeferredEnable;
    int     g_iFocus;
    int     g_iDecalEnable;
    int     g_i3DAnimation;
    int     g_iAlphaEnable;
    float   g_fBumpScale;
}

cbuffer Public : register(b5)
{
    float   g_DeltaTime;
    float   g_PlusedDeltaTime;
    int     g_isDeferred;
    int     g_isWireFrame;
    float2  g_cameraNearFar;
    float2  g_ViewPortSize;
    int     g_iSSAOEnable;
    float3  g_empty;
}

cbuffer Light	: register(b3)
{
	float4	g_vLightDif;
	float4	g_vLightAmb;
	float4	g_vLightSpc;
	int		g_iLightType;
	float3	g_vLightPos;
	float3	g_vLightDir;
	float	g_fLightRange;
	float	g_fLightInAngle;
	float	g_fLightOutAngle;
	float   g_fFallOff;
	int     g_iRimLight;
}

cbuffer RimLight : register(b4)
{
    float3  g_vRimColor;
    float   g_fRimPower;
}

#define	RENDER_FORWARD	    0
#define	RENDER_DEFERRED	    1
#define RENDER_NAVIEDITOR   2

#define	A2D_ATLAS	0
#define	A2D_FRAME	1

Texture2D		g_DiffuseTex	    : register(t0);
Texture2D		g_NormalTex		    : register(t1);
Texture2D		g_SpecularTex	    : register(t2);
Texture2D       g_BoneTex           : register(t3);

Texture2DArray	g_DiffuseTexArray	: register(t4);

Texture2D		g_MultiTex1		    : register(t5);
Texture2D		g_MultiTex2		    : register(t6);
Texture2D		g_MultiTex3		    : register(t7);
Texture2D		g_MultiTex4		    : register(t8);

SamplerState	g_DiffuseSmp	    : register(s0);

SamplerState	g_MultiSmp1		    : register(s5);
SamplerState	g_MultiSmp2		    : register(s6);
SamplerState	g_MultiSmp3		    : register(s7);
SamplerState	g_MultiSmp4		    : register(s8);

SamplerState PointSampler : register(s9);
//SamplerState LinearSampler : register(s1);

static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

cbuffer FinalPassConstants : register(b10)
{
    float   g_fMiddleGrey;
    float   g_fLumWhiteSqr;
    float2  fEmpty;
}

struct _tagLightInfo
{
	float4	vDif;
	float4	vAmb;
	float4	vSpc;
    float4  vEmv;
};

#define	LIGHT_DIR	0
#define	LIGHT_POINT	1
#define	LIGHT_SPOT	2

_tagLightInfo ComputeLight(float3 vViewPos, float3 vViewNormal , float2 vUV)
{
	_tagLightInfo	tInfo = (_tagLightInfo)0; // Lightinfo 지역변수 선언

	float3	vLightDir = (float3)0; // Lightdir 지역변수 선언.
	float	fIntensity = 1.f; // 빛의 세기를 정의할 Intensity 변수 선언

	// 조명 타입을 구분한다.
	if (g_iLightType == LIGHT_DIR)
	{
		// 조명 방향을 뷰공간으로 변환한다.
		vLightDir = -normalize(mul(float4(g_vLightDir, 0.f), g_matView).xyz);
	}

	if (g_iLightType == LIGHT_POINT)
	{
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// 조명과 정점사이의 거리를 구한다
		float	fDist = distance(vLightPos, vViewPos);

		fIntensity = 1.f - fDist / g_fLightRange;

		// 최소 강도를 0.3으로 잡아주었다(임의로)
		fIntensity = max(0, fIntensity) * 0.7f + 0.3f;
	}

	if (g_iLightType == LIGHT_SPOT)
	{
		// 정점에서 조명을 향하는 벡터를 구한다
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// 조명에서 정점을 향하는 벡터를 구한다
		float3	vDir = -vLightDir;

		// 조명이 비추는 방향을 구한다
		float3	vLightCenterDir = normalize(mul(float4(g_vLightDir, 0.f), g_matView).xyz);

		// CenterDir과 vDir을 내적하여 cos값을 구해준다
		float	fDot = dot(vDir, vLightCenterDir);

		// 점과 조명사이의 거리를 구한다
		float	fDist = distance(vLightPos, vViewPos);

		// 조명 범위를 벗어난 것이다
		if (fDot < g_fLightOutAngle || fDist >= g_fLightRange)
			fIntensity = 0.3f;

		// 안쪽 범위 안에 들어오는지를 판단한다
		else if (fDot >= g_fLightInAngle)
			fIntensity = 1.f;

		else
		{
			// 안쪽과 바깥쪽의 범위를 구한다
			float	fAngleDist = g_fLightInAngle - g_fLightOutAngle;
			// 안쪽에서 현재 구해준 cos값을 빼서 얼마나 벗어났는지 판단한다
			float	fAngle = g_fLightInAngle - fDot;

			fIntensity = (1.f - fAngle / fAngleDist) * 0.7f + 0.3f;
		}
	}

	// Diffuse를 구한다.
	float	fRamb = dot(vLightDir, vViewNormal);

	if (fRamb < 0.f)
		fIntensity = 0.f;

	tInfo.vDif = g_vMtrlDif * g_vLightDif * max(0, fRamb) * fIntensity;
	tInfo.vAmb = g_vMtrlAmb * g_vLightAmb * min(0.2f, fIntensity);

	float3	vR = 2.f * vViewNormal * dot(vLightDir, vViewNormal) - vLightDir;
	//float3 vR = reflect(vViewNormal, vLightDir);
	vR = normalize(vR);

	float3	vView = -normalize(vViewPos);

	float vSpc = g_vMtrlSpc.w;
	if (g_vMtrlAmb.w == 1.f)
	{
		vSpc = g_SpecularTex.Sample(g_DiffuseSmp, vUV);
	}
	tInfo.vSpc = vSpc * g_vLightSpc * pow(max(0, dot(vView, vR)), g_vMtrlSpc.w) * fIntensity;

	return tInfo;
}

float ConvertColor(float4 vColor)
{
	/*
	참고 : https://www.h-schmidt.net/FloatConverter/IEEE754.html
	AMD 및 Intel의 그래픽스에서는 asuint와 asfloat를 통한 강제 형변환이 원활하게 이뤄지지 않고 있음
	특정 장치들이 ConvertColor로 나온 값들을 저장하는 과정에서 변형이 생기는 것으로 파악됨

	asfloat 이후의 float값 비트는 정상적으로 압축되나,
	이를 저장하고 다시 불러오는 과정에서 Nan 또는 infinity에 포함되는 비트가 잘려서
	빨간 맛, 노랑 맛, 혹은 퍼렁맛의 에러가 발생했던 것으로 보입니다

	4바이트 - 4바이트 사이의 형변환이지만, 와중에 부동 소수점(IEEE 754) 무결성 검사나 IsNan /Infinity 등등이
	걸릴 수 있으니 신중히 할 것
	*/

    uint4 vColor1 = (uint4) 0;
    vColor1.r = uint(floor(vColor.r * 255.f));
    vColor1.g = uint(floor(vColor.g * 255.f));
    vColor1.b = uint(floor(vColor.b * 255.f));
    vColor1.a = uint(floor(vColor.a * 255.f));

    uint iColor = 0;
    iColor = vColor1.a;
    iColor = (iColor << 8);
    iColor += vColor1.r;
    iColor = (iColor << 8);
    iColor += vColor1.g;
    iColor = (iColor << 8);
    iColor += vColor1.b;

    float fColor = asfloat(iColor);

    if (!isfinite(fColor))
    {
		//nan : 지수부분(Exponent)의 모든 비트가 1이면서(여기까진 infinity) 가수부분(Mantissa)의 비트가 0이 아님
		//비 Nvidia를 위한 뼈를 주고 살을 깎는 대책 : Alpha 값을 줄여(가장 끝부분만 줄이자) Nan 막기
		//Alpha의 값 1만 바뀌어 255 -> 254가 되므로, 피해를 최소화할 수 있다.
		//이게 아깝다면 비 Nvidia용 셰이더를 따로 만들어 두고,
		//IDXGIAdapter2::GetDesc2 함수를 이용해 가져오는 DXGI_ADAPTER_DESC2 구조체에서
		//UINT VendorId 를 가져오면 파악 가능하다 >> 0x10DE , 즉 4318이 Nvidia ID라고 함
        iColor = iColor & 0xfeffffff;
        fColor = asfloat(iColor);
    }
	
	
    if (isinf(fColor))
    {
        if (vColor1.a & 0x00000080)
        {
            fColor = -8.507059E37;
        }
        else
        {
            fColor = 8.507059E37;
        }
    }

    return fColor;
}

float4 ConvertColor(float fColor)
{
	
    float4 vColor;
    uint iColor = asuint(fColor);
	
    vColor.b = (iColor & 0x000000ff) / 255.f;
    vColor.g = ((iColor >> 8) & 0x000000ff) / 255.f;
    vColor.r = ((iColor >> 16) & 0x000000ff) / 255.f;
    vColor.a = ((iColor >> 24) & 0x000000ff) / 255.f;

	//보통 불투명도가 확실할 때 처리되는 절차가 많을 수 있으므로, 알파값 254값은 오차범위 내 255로 쳐주자
    if (((iColor >> 24) & 0x000000ff) == 0x000000fe)
    {
        vColor.a = 1.f;
    }	
	
    return vColor;
}

_tagLightInfo ComputeLight(float3 vViewPos, float3 vViewNormal, float4 vMaterial,
	float fSpcPower, float3 ToCamera)
{
	_tagLightInfo tInfo = (_tagLightInfo)0;

	// 재질정보를 분리한다.
	float4 vMtrlDif = ConvertColor(vMaterial.r);
	float4 vMtrlAmb = ConvertColor(vMaterial.g);
	float4 vMtrlSpc = ConvertColor(vMaterial.b);
	float4 vMtrlEmv = ConvertColor(vMaterial.a);

	float3 vLightDir = (float3) 0;
	float fIntensity = 1.0f;
	float SpotStrong = 1.0f;

	float3 vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;
	vLightDir = vLightPos - vViewPos;
	vLightDir = normalize(vLightDir);

	float3 HalfWay = normalize(vLightPos + ToCamera);

    tInfo.vEmv = vMtrlSpc * vMtrlEmv;

	if (g_iLightType == LIGHT_DIR)
	{
		vLightDir = -normalize(mul(float4(g_vLightDir, 0.f), g_matView).xyz);

        if (g_iRimLight == 1)
        {
            float fLength = distance(vViewPos, float3(0.f, 0.f, 0.f));

            if(fLength < 18)
            {           

			// 카메라방향과 노말벡터를 내적하여 어둡게 해줄 외각을 찾는다
                float fRim = saturate(dot(vViewNormal, ToCamera));

			// 0.3보다 크게 되면 Rim을 없애준다
                if (fRim > 0.2 || fRim < 0.0f)
                    fRim = 1;

			// 0.3보다 작은 값만 Rim을 처리해준다
			// 더 뚜렷한 외각선을 얻기 위함이다
                else
                    fRim = -1;

                tInfo.vEmv = vMtrlSpc * vMtrlEmv + float4(pow(1 - fRim, g_fRimPower) * g_vRimColor, 1.f);
            }
        }
    }

	if (g_iLightType == LIGHT_POINT)
	{
		// 조명과 정점사이의 거리를 구한다.
		float fDist = distance(vLightPos, vViewPos);

		fIntensity = 1.f - fDist / g_fLightRange;
		fIntensity = max(0, fIntensity) * 0.7f + 0.3f;
	}

	if (g_iLightType == LIGHT_SPOT)
	{
        float3 vDir = -vLightDir;
        float3 vLightCenterDir = normalize(mul(float4(g_vLightDir, 0.0f), g_matView)).xyz;
        float fDot = dot(vDir, vLightCenterDir);
        float fDist = distance(vLightPos, vViewPos);

        if (fDot < g_fLightOutAngle || fDist >= g_fLightRange)
            fIntensity = 0.0f;
        
        else if (fDot >= g_fLightInAngle)
            fIntensity = 1.f;

        else
        {
            float fAngleDist = g_fLightInAngle - g_fLightOutAngle;
            float fAngle = g_fLightInAngle - fDot;
            fIntensity = (1.f - fAngle / fAngleDist) * 0.7f + 0.3f;
        }
    }

	float fRamb = dot(vLightDir, vViewNormal);

	if (fRamb < 0.001f)
		fIntensity = 0.0f;

    float3 vR = 2.f * vViewNormal * dot(vLightDir, vViewNormal) - vLightDir;
	//float3	vR = reflect(vViewNormal, vLightDir);
    vR = normalize(vR);

	tInfo.vDif = vMtrlDif * g_vLightDif * max(0, fRamb) * fIntensity;
    tInfo.vSpc = float4(vMtrlSpc.xyz, 1.0f) * g_vLightSpc * pow(max(0.0f, dot(vR, ToCamera)), vMtrlSpc.w) * fIntensity /** SpotStrong*/;
    tInfo.vAmb = vMtrlAmb * g_vLightAmb * min(0.2f, fIntensity);

	return tInfo;
}

VS_OUTPUT_3D Vertex3DVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    return output;
}

PS_OUTPUT_SINGLE Vertex3DPS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    return output;
}

struct _tagSkinning
{
    float3 vPos;
    float3 vNormal;
    float3 vTangent;
    float3 vBinormal;
};

matrix GetBoneMatrix(int idx)
{
    matrix matBone =
    {
        g_BoneTex.Load(int3(idx * 4, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 1, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 2, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 3, 0, 0))
    };

    return matBone;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float3 vTangent,
	float3 vBinormal, float4 vWeights, float4 vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.f), matBone).xyz;
        tSkinning.vTangent += fWeights[i] * mul(float4(vTangent, 0.f), matBone).xyz;
        tSkinning.vBinormal += fWeights[i] * mul(float4(vBinormal, 0.f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);
    tSkinning.vTangent = normalize(tSkinning.vTangent);
    tSkinning.vBinormal = normalize(tSkinning.vBinormal);

    return tSkinning;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float4 vWeights,
	float4 vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);

    return tSkinning;
}

float3 Skinning(float3 vPos, float4 vWeights, float4 vIndices)
{
    float3 vSkinningPos = (float3) 0.f;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        vSkinningPos += fWeights[i] * mul(float4(vPos, 1.f), matBone).xyz;
    }

    return vSkinningPos;
}