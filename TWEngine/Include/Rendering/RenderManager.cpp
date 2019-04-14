#include "EngineHeader.h"
#include "RenderManager.h"
#include "Core.h"
#include "Shader.h"
#include "CSBlur.h"
#include "DownScale.h"
#include "BlendState.h"
#include "DepthState.h"
#include "PostEffect.h"
#include "CSFilter.h"
#include "CSAdaptFilter.h"
#include "CSBloomFilter.h"
#include "ViewManager.h"
#include "EditManager.h"
#include "RenderTarget.h"
#include "ComputeShader.h"
#include "RasterizerState.h"
#include "MultiRenderTarget.h"
#include "../Device.h"
#include "../Resource/Sampler.h"
#include "../Resource/Texture.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"

PUN_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_pCreateState(nullptr),
	m_bDeferred(true),
	m_bFogEnable(false),
	m_bSSAOEnable(true),
	m_pSkyObj(nullptr),
	m_pFogDepthSRV(nullptr),
	m_accShakeTime(0.f),
	m_isGetCameraObj(false),
	m_pNoiseTex(nullptr),
	m_pFogDepthTex(nullptr)
{
	m_eGameMode = GM_3D;
	m_pSphereVolum = NULLPTR;
	m_pCornVolum = NULLPTR;
	m_pGBufferMultiTarget = NULLPTR;
	m_pLightMultiTarget = NULLPTR;
	m_pDownScaledGBufferMultiTarget = NULLPTR;
	m_pPostEffect = nullptr;

	memset(m_pState, 0, sizeof(CRenderState*) * STATE_END);
	memset(m_pTarget, 0, sizeof(CRenderTarget*) * TARGET_END);
	memset(m_pShader, 0, sizeof(CShader*) * SHADER_END);
	memset(m_pFilter, 0, sizeof(CCSFilter*) * CFT_END);
	
	m_bLightWireFrame = true;

	m_tCBuffer = {};

	// ============= Magic Number용 =================
	m_bHDR = false;
	m_bAdaptation = false;
	m_bBloom = false;
	m_bBlur = false;
	m_bMotionBlur = false;
	m_bSLC = false;
	m_bDepthFog = false;

	m_fMiddleGrey = 0.863f;
	m_fLumWhite = 1.53f;
	m_fAdaptation = 3.f;
	m_fBloomThreshold = 1.1f;
	m_fBloomScale = 0.74f;

	m_fDepthFogEnd = 100.f;
	m_fDepthFogStart = 0.f;

	m_vDepthFogColor = Vector4(0.2f, 0.2f, 0.2f, 1.f);

#ifdef _DEBUG
	m_bMagic = true;
#else
	m_bMagic = false;
#endif
	// =================================================
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_pPostEffect);
	SAFE_RELEASE(m_pRandomNormalTex);
	SAFE_RELEASE(m_pNoiseTex);
	SAFE_RELEASE(m_pFogDepthSRV);
	SAFE_RELEASE(m_pFogDepthTex);
	SAFE_RELEASE(m_pGBufferSampler);
	DESTROY_SINGLE(CShaderManager);
	DESTROY_SINGLE(CViewManager);
}

GAME_MODE CRenderManager::GetGameMode() const
{
	return m_eGameMode;
}

bool CRenderManager::GetRenderingMode() const
{
	return m_bDeferred;
}

void CRenderManager::SetGameMode(GAME_MODE eMode)
{
	m_eGameMode = eMode;
}

void CRenderManager::EnableDeferredRendering()
{
	m_bDeferred = true;
}

bool CRenderManager::SetSkyObject(CGameObject* pObj)
{
	m_pSkyObj = pObj;

	return true;
}

bool CRenderManager::Init()
{
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	if (!GET_SINGLE(CViewManager)->Init())
		return false;

	m_tCBuffer.ViewPortSize.x = (float)CDevice::GetInst()->GetResolution().iWidth;
	m_tCBuffer.ViewPortSize.y = (float)CDevice::GetInst()->GetResolution().iHeight;
	m_tCBuffer.isDeferred = m_bDeferred;

	m_pGBufferSampler				= GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_POINT);
	m_pSphereVolum					= GET_SINGLE(CResourcesManager)->FindMeshNonCount(SPHERE_VOLUME);
	m_pCornVolum					= GET_SINGLE(CResourcesManager)->FindMeshNonCount(CORN_VOLUME);
	m_pPointLightLayout				= GET_SINGLE(CShaderManager)->FindInputLayout(POS_LAYOUT);

	m_pShader[SHADER_ACC_DIR]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(LIGHT_DIR_ACC_SHADER);
	m_pShader[SHADER_ACC_POINT]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(LIGHT_POINT_ACC_SHADER);
	m_pShader[SHADER_ACC_SPOT]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(LIGHT_SPOT_ACC_SHADER);
	m_pShader[SHADER_BLEND]			= GET_SINGLE(CShaderManager)->FindShaderNonCount(LIGHT_BLEND_SHADER);
	m_pShader[SHADER_FOG_FRONT]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(VOLUME_FOG_FIRST_SHADER);
	m_pShader[SHADER_FOG_BACK]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(VOLUME_FOG_BACK_SHADER);
	m_pShader[SHADER_FOG_COLOR]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(VOLUME_FOG_SECOND_SHADER);
	m_pShader[SHADER_FULL_SCREEN]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(FULLSCREEN_SHADER);
	m_pShader[SHADER_DS_GBUFFER]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(DS_GBUFFER_SHADER);
	m_pShader[SHADER_FINAL_PASS]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(FINAL_PASS_SHADER);
	m_pShader[SHADER_SHADOW]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(SHADOWMAP_SHADER);
	m_pShader[SHADER_SSAO]			= GET_SINGLE(CShaderManager)->FindShaderNonCount(SSAO_SHADER);
	m_pShader[SHADER_STARLIGHT_SCOPE] = GET_SINGLE(CShaderManager)->FindShaderNonCount(STARLIGHTSCOPE_SHADER);
	m_pShader[SHADER_LAND_EDITOR]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(LAND_NAVI_EDITOR_SHADER);
	m_pShader[SHADER_OBJ_EDITOR]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(NAVI_EDITOR_SHADER);
	m_pShader[SHADER_SHADOW_TEX]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(SHADOWTEX_SHADER);
	m_pShader[SHADER_DS_SHADOW]		= GET_SINGLE(CShaderManager)->FindShaderNonCount(DOWNSCALE_SHADOW_SHADER);

	m_pTarget[TARGET_ALBEDO]		= GET_SINGLE(CViewManager)->FindRenderTarget("Albedo");
	m_pTarget[TARGET_DEPTH]			= GET_SINGLE(CViewManager)->FindRenderTarget("Depth");
	m_pTarget[TARGET_NORMAL]		= GET_SINGLE(CViewManager)->FindRenderTarget("Normal");
	m_pTarget[TARGET_BACK]			= GET_SINGLE(CViewManager)->FindRenderTarget("SecondBackBuffer");
	m_pTarget[TARGET_SKY]			= GET_SINGLE(CViewManager)->FindRenderTarget("Sky");
	m_pTarget[TARGET_FOG_DEPTH]		= GET_SINGLE(CViewManager)->FindRenderTarget("VolumeFogDepth");
	m_pTarget[TARGET_ACC_DIFF]		= GET_SINGLE(CViewManager)->FindRenderTarget("LightAccDif");
	m_pTarget[TARGET_ACC_SPC]		= GET_SINGLE(CViewManager)->FindRenderTarget("LightAccSpc");
	m_pTarget[TARGET_BLEND]			= GET_SINGLE(CViewManager)->FindRenderTarget("LightBlend");
	m_pTarget[TARGET_FINAL]			= GET_SINGLE(CViewManager)->FindRenderTarget("Final");
	m_pTarget[TARGET_TANGENT]		= GET_SINGLE(CViewManager)->FindRenderTarget("Tangent");
	m_pTarget[TARGET_BINORMAL]		= GET_SINGLE(CViewManager)->FindRenderTarget("Binormal");
	m_pTarget[TARGET_SHADOWMAP]		= GET_SINGLE(CViewManager)->FindRenderTarget("ShadowMap");
	m_pTarget[TARGET_DS_DEPTH]		= GET_SINGLE(CViewManager)->FindRenderTarget("DownScaledDepth");
	m_pTarget[TARGET_DS_NORMAL]		= GET_SINGLE(CViewManager)->FindRenderTarget("DownScaledNormal");
	m_pTarget[TARGET_SSAO]			= GET_SINGLE(CViewManager)->FindRenderTarget("SSAO");
	m_pTarget[TARGET_STARLIGHT_SCOPE] = GET_SINGLE(CViewManager)->FindRenderTarget("StarLightScope");
	m_pTarget[TARGET_SHADOW]		= GET_SINGLE(CViewManager)->FindRenderTarget("ShadowTarget");
	m_pTarget[TARGET_SHADOW_TEX]	= GET_SINGLE(CViewManager)->FindRenderTarget("ShadowTexture");
	m_pTarget[TARGET_DS_SHADOW]		= GET_SINGLE(CViewManager)->FindRenderTarget("DownScaledShadow");

	m_pTarget[TARGET_SHADOW_TEX]->SetClearColor(Vector4::One);
	m_pTarget[TARGET_BACK]->SetClearColor(Vector4::Zero);

	m_pState[STATE_DEPTH_GRATOR]	= GET_SINGLE(CViewManager)->FindRenderStateNonCount(DEPTH_GRATOR);
	m_pState[STATE_DEPTH_LESS]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(DEPTH_LESS);
	m_pState[STATE_ACC_BLEND]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(ACC_BLEND);
	m_pState[STATE_FRONT_CULL]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(FRONT_CULL);
	m_pState[STATE_BACK_CULL]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(BACK_CULL);
	m_pState[STATE_WIRE_FRAME]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(WIRE_FRAME);
	m_pState[STATE_CULL_NONE]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(CULL_NONE);
	m_pState[STATE_ZERO_BLEND]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(ZERO_BLEND);
	m_pState[STATE_ALL_BLEND]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(ALL_BLEND);
	m_pState[STATE_DEPTH_DISABLE]	= GET_SINGLE(CViewManager)->FindRenderStateNonCount(DEPTH_DISABLE);
	m_pState[STATE_DEPTH_READ_ONLY] = GET_SINGLE(CViewManager)->FindRenderStateNonCount(DEPTH_DISABLE);
	m_pState[STATE_ALPHA_BLEND]		= GET_SINGLE(CViewManager)->FindRenderStateNonCount(ALPHA_BLEND);


	((CDepthState*)m_pState[STATE_DEPTH_GRATOR])->SetStencilRef(1); //스텐실값을 1로 채운다.

	m_pGBufferMultiTarget			= GET_SINGLE(CViewManager)->FindMRT("GBuffer");
	m_pLightMultiTarget				= GET_SINGLE(CViewManager)->FindMRT("LightAcc");
	m_pDecalMultiTarget				= GET_SINGLE(CViewManager)->FindMRT("SSDBuffer");
	m_pDownScaledGBufferMultiTarget = GET_SINGLE(CViewManager)->FindMRT("DownScaleGBuffer");

	for (int i = CFT_DOWNSCALE; i < CFT_END; ++i)
	{
		m_pFilter[i] = GET_SINGLE(CViewManager)->FindCSFilter((CS_FILTER_TYPE)i);
		m_pFilter[i]->Disable();
	}
	m_pFilter[0]->Enable();
	m_pFilter[1]->Enable();

	ID3D11Texture2D* pTex = m_pTarget[TARGET_FOG_DEPTH]->GetTexture();

	D3D11_TEXTURE2D_DESC tDesc;
	pTex->GetDesc(&tDesc);

	DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pFogDepthTex);
	DEVICE->CreateShaderResourceView(m_pFogDepthTex, nullptr, &m_pFogDepthSRV);

	m_pRandomNormalTex = GET_SINGLE(CResourcesManager)->FindTexture("RandomNormal");

	m_tCBuffer.iSSAOEnable = 1;


	// 야간 투시경에 필요한 정보
	srand((unsigned int)time(NULL));
	if (CResourcesManager::GetInst()->CreateTexture("Noise", TEXT("noise6.png")))
	{
		m_pNoiseTex = CResourcesManager::GetInst()->FindTexture("Noise");
	}
	SetStarLightScope(0);

	m_pPostEffect = new CPostEffect;

	if (!m_pPostEffect->Init())
		return false;
	
	m_tDepthFogCBuffer.vFogColor = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	m_tDepthFogCBuffer.fStartDepth = 0.0f;
	m_tDepthFogCBuffer.fEndDepth = 100.0f;

	return true;
}

CRenderState * CRenderManager::FindRenderState(const string & strName)
{
	CRenderState* pState = GET_SINGLE(CViewManager)->FindRenderState(strName);

	return pState;
}

CRenderState * CRenderManager::FindRenderStateNonCount(const string & strName)
{
	CRenderState* pState = GET_SINGLE(CViewManager)->FindRenderStateNonCount(strName);

	return pState;
}

CRenderTarget * CRenderManager::FindRenderTarget(const string & strName)
{
	return GET_SINGLE(CViewManager)->FindRenderTarget(strName);
}

void CRenderManager::AddRenderObj(CGameObject * pObj)
{
	RENDER_GROUP	rg = pObj->GetRenderGroup();

	if (rg == RG_GIZMO)
		return;
	//{
	//	if (m_tGizmoGroup.iSize == m_tGizmoGroup.iCapacity)
	//	{
	//		m_tGizmoGroup.iCapacity *= 2;

	//		CGameObject**	pList = new CGameObject*[m_tGizmoGroup.iCapacity];

	//		memcpy(pList, m_tGizmoGroup.pList, sizeof(CGameObject*) * m_tGizmoGroup.iSize);

	//		SAFE_DELETE_ARRAY(m_tGizmoGroup.pList);

	//		m_tGizmoGroup.pList = pList;
	//	}
	//	m_tGizmoGroup.pList[m_tGizmoGroup.iSize] = pObj;
	//	++m_tGizmoGroup.iSize;
	//}

	if (rg == RG_LIGHT)
	{
		if (m_tLightGroup.iSize == m_tLightGroup.iCapacity)
		{
			m_tLightGroup.iCapacity *= 2;

			CGameObject**	pList = new CGameObject*[m_tLightGroup.iCapacity];

			memcpy(pList, m_tLightGroup.pList, sizeof(CGameObject*) * m_tLightGroup.iSize);

			SAFE_DELETE_ARRAY(m_tLightGroup.pList);

			m_tLightGroup.pList = pList;
		}
		m_tLightGroup.pList[m_tLightGroup.iSize] = pObj;
		++m_tLightGroup.iSize;

		/*
		밑의 부분은 '그려질 오브젝트이면서 빛을 가지고 있는 오브젝트'의 경우
		(EX. 횃불, 전구 (근데 이건 조명따로 횃불따로 해도 될 것 같은.. 읍읍) 
			또는 파티클)
		렌더러를 가지고 있을 것이기 때문에 렌더러 유무 체크를 해주어야 한다고
		판단해서 추가함.
		렌더러를 가지고 있지않은 순수 조명의 경우 m_tLightGroup에서만 처리되게 하고
		렌더러를 가진 오브젝트의 경우 m_tLightGroup에서 처리해줌과 동시에
		렌더 그룹에 포함시켜 화면에 렌더한다.
		일단 디폴트로 NORMAL에 넣었지만 아마 EFFECT라든지 그런 렌더 그룹을
		따로 만들어서 넣어줘야 할 지도 모르겠다.
		*/
		if (!pObj->CheckComponentFromType(CT_RENDERER))
			return;
		else
			rg = RG_NORMAL;
	}

	else if (rg != RG_UI && rg != RG_FOG)
	{
		if (CCore::GetInst()->m_bEditorMode == false)
		{
			if (pObj->FrustumCull())
			{
				return;
			}
		}
	}
	if (rg != RG_GIZMO)
	{
		if (m_tRenderObj[rg].iSize == m_tRenderObj[rg].iCapacity)
		{
			m_tRenderObj[rg].iCapacity *= 2;

			CGameObject**	pList = new CGameObject*[m_tRenderObj[rg].iCapacity];

			memcpy(pList, m_tRenderObj[rg].pList, sizeof(CGameObject*) * m_tRenderObj[rg].iSize);

			SAFE_DELETE_ARRAY(m_tRenderObj[rg].pList);

			m_tRenderObj[rg].pList = pList;
		}
		m_tRenderObj[rg].pList[m_tRenderObj[rg].iSize] = pObj;
		++m_tRenderObj[rg].iSize;
	}
}

void CRenderManager::Render(float fTime)
{
	m_tCBuffer.DeltaTime = fTime;
	m_tCBuffer.PlusedDeltaTime += fTime;
	m_tCBuffer.isWireFrame = m_bLightWireFrame;

	CShaderManager::GetInst()->UpdateCBuffer("PublicCBuffer", &m_tCBuffer);

	switch (m_eGameMode)
	{
	case GM_2D:
		Render2D(fTime);
		break;

	case GM_3D:
		Render3D(fTime);
		break;
	}
}

void CRenderManager::Render2D(float fTime)
{
	// 포스트 이펙트 처리용 타겟으로 교체한다.
	CRenderTarget*	pTarget = GET_SINGLE(CViewManager)->FindRenderTarget("PostEffect");

	pTarget->ClearTarget();
	pTarget->SetTarget();

	for (int i = 0; i <= RG_ALPHA3; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}

		m_tRenderObj[i].iSize = 0;
	}

	pTarget->ResetTarget();

	// 여기에서 포스트 이펙트를 처리한다.
	CRenderState*	pAlphaBlend = GET_SINGLE(CViewManager)->FindRenderState(ALPHA_BLEND);

	// 여기에서 포스트이펙트 처리가 된 타겟을 전체 크기로 화면에 출력한다.
	pTarget->RenderFullScreen();

	// UI출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}

		m_tRenderObj[i].iSize = 0;
	}

	pAlphaBlend->SetState();

	GET_SINGLE(CViewManager)->Render(fTime);

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

void CRenderManager::Render3D(float fTime)
{
	// Forward Rendering 처리
	if (!m_bDeferred)
		RenderForward(fTime);
	else
	{
		if (GET_SINGLE(CCore)->m_bEditorMode)
		{
			if (GET_SINGLE(CEditManager)->IsNaviEditorMode())
				RenderNaviEditorMode(fTime);
			else
				RenderDeferred(fTime);
		}
		else
			RenderDeferred(fTime);
	}

	GET_SINGLE(CViewManager)->Render(fTime);
}

void CRenderManager::RenderDeferred(float fTime)
{
	// MainCamera를 얻어온다.
	FindMagicNumber(fTime);

	CCamera* pMainCamera = GET_SINGLE(CSceneManager)->GetMainCameraNoneCount();
	
	// 그림자 맵을 그려준다.
	if (pMainCamera->IsShadow())
		RenderShadowMap(fTime);
	// GBuffer를 만들어준다.
	RenderGBuffer(fTime);
	// GBuffer를 이용해 데칼을 GBuffer에 추가하여 그린다.
	RenderDecal(fTime);

	//그림자 맵을 토대로 그림자 텍스쳐를 만들어 준다.
	if (pMainCamera->IsShadow())
		RenderShadowTexture(fTime);

	// SSAO를 계산해준다.
	if(m_bSSAOEnable)
		RenderSSAO(fTime);
	// 조명 누적버퍼를 만들어준다.
	RenderLightAcc(fTime);
	// 조명타겟과 Albedo 를 합성한다.
	RenderLightBlend(fTime);
	// 스카이 오브젝트와 조명 처리된 타겟을 합쳐 그린다.
	RenderSkyObj(fTime);
	// 볼륨안개를 그린다.
	if(m_bFogEnable)
		RenderFog(fTime);
	// HDR 등 다양한 화면 효과를 계산한다.
	RenderComputeProcess(fTime);
	// 최종 합성된 타겟을 화면에 출력한다.

	
#ifdef _DEBUG
	RenderFinalPassDebug(fTime);
#else
	RenderFinalPass(fTime);
#endif
	

	if (CCore::GetInst()->m_bEditorMode == true)
	{
		m_pState[STATE_DEPTH_DISABLE]->SetState();
		for (size_t i = 0; i < m_tRenderObj[RG_LANDSCAPE].iSize; i++)
		{
			m_tRenderObj[RG_LANDSCAPE].pList[i]->Render(fTime);
		}
		m_pState[STATE_DEPTH_DISABLE]->ResetState();

		CEditManager::GetInst()->Render(fTime);
	}
	// UI출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}

	m_tLightGroup.iSize = 0;

	for (int i = RG_LANDSCAPE; i < RG_END; ++i)
	{
		m_tRenderObj[i].iSize = 0;
	}
	m_tGizmoGroup.iSize = 0;

	GET_SINGLE(CCollisionManager)->Render(fTime);
}

void CRenderManager::RenderGBuffer(float fTime)
{
	// GBuffer MRT로 타겟을 교체한다.
	float	fClearColor[4] = {0.f , 0.f , 0.f , 0.f};
	m_pGBufferMultiTarget->ClearRenderTarget(fClearColor);
	m_pGBufferMultiTarget->SetTarget();

	if (CCore::GetInst()->m_bEditorMode == true)
	{
		for (int j = 0; j < m_tRenderObj[RG_NORMAL].iSize; ++j)
			m_tRenderObj[RG_NORMAL].pList[j]->Render(fTime);
	}
	else
	{
		for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
		{
			for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
				m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}

	m_pGBufferMultiTarget->ResetTarget();
}

void CRenderManager::RenderDecal(float fTime)
{
	m_pDecalMultiTarget->SetTarget();

	m_pTarget[TARGET_DEPTH]->SetShader(12);
	m_pTarget[TARGET_TANGENT]->SetShader(13);
	m_pTarget[TARGET_BINORMAL]->SetShader(14);

	for (int j = 0; j < m_tRenderObj[RG_DECAL].iSize; ++j)
	{
		m_tRenderObj[RG_DECAL].pList[j]->Render(fTime);
	}

	m_pTarget[TARGET_DEPTH]->ResetShader(12);
	m_pTarget[TARGET_TANGENT]->ResetShader(13);
	m_pTarget[TARGET_BINORMAL]->ResetShader(14);

	m_pDecalMultiTarget->ResetTarget();
}

void CRenderManager::DownScaleGBuffer(float fTime)
{
	float fClearColor[4] = {};
	m_pDownScaledGBufferMultiTarget->ClearRenderTarget(fClearColor);
	m_pDownScaledGBufferMultiTarget->SetTarget();

	m_pShader[SHADER_DS_GBUFFER]->SetShader();

	m_pTarget[TARGET_DEPTH]->SetShader(10);
	m_pTarget[TARGET_NORMAL]->SetShader(11);

	CONTEXT->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	m_pTarget[TARGET_DEPTH]->ResetShader(10);
	m_pTarget[TARGET_NORMAL]->ResetShader(11);

	m_pDownScaledGBufferMultiTarget->ResetTarget();
}

void CRenderManager::RenderSSAO(float fTime)
{
	DownScaleGBuffer(fTime);

	m_pTarget[TARGET_SSAO]->ClearTarget();
	m_pTarget[TARGET_SSAO]->SetTarget();

	m_pTarget[TARGET_DS_DEPTH]->SetShader(10);
	m_pTarget[TARGET_DS_NORMAL]->SetShader(11);
	m_pRandomNormalTex->SetShader(12);

	m_pShader[SHADER_SSAO]->SetShader();
	
	CONTEXT->Draw(4, 0);

	m_pTarget[TARGET_DS_DEPTH]->ResetShader(10);
	m_pTarget[TARGET_DS_NORMAL]->ResetShader(11);
	m_pRandomNormalTex->ResetShader(12);

	m_pTarget[TARGET_SSAO]->ResetTarget();

	int iEnable = 0;
	int iDOF = 0;
	if (!m_pFilter[CFT_BLUR]->GetEnable())
	{
		iEnable = 1;
		m_pFilter[CFT_BLUR]->Enable();
		if (((CCSBlur*)m_pFilter[CFT_BLUR])->GetDOFEnable())
		{
			((CCSBlur*)m_pFilter[CFT_BLUR])->DisableDOF();
			iDOF = 1;
		}
	}

	m_pFilter[CFT_BLUR]->ChangeSourceSRV(m_pTarget[TARGET_SSAO]->GetShaderResourceView());

	m_pFilter[CFT_BLUR]->Dispatch(fTime);
	
	m_pFilter[CFT_BLUR]->ResetSourceSRV();

	if (iEnable == 1)
		m_pFilter[CFT_BLUR]->Disable();
	if(iDOF == 1)
		((CCSBlur*)m_pFilter[CFT_BLUR])->EnableDOF();
}

void CRenderManager::RenderLightAcc(float fTime)
{
	float fClearColor[4] = {};
	m_pLightMultiTarget->ClearRenderTarget(fClearColor);
	m_pLightMultiTarget->SetTarget();

	m_pShader[SHADER_ACC_DIR]->SetShader();

	m_pState[STATE_ACC_BLEND]->SetState();
	m_pState[STATE_DEPTH_DISABLE];

	m_pGBufferSampler->SetShader(10);

	m_pGBufferMultiTarget->SetShaderResource(10);

	for (int i = 0; i < m_tLightGroup.iSize; ++i)
	{
		CLight*	pLight = m_tLightGroup.pList[i]->FindComponentFromType<CLight>(CT_LIGHT);

		switch (pLight->GetLightType())
		{
		case LT_DIR:
			RenderLightDir(fTime, pLight);
			break;
		case LT_POINT:
			RenderLightPoint(fTime, pLight);
			break;
		case LT_SPOT:
			RenderLightSpot(fTime, pLight);
			break;
		}

		SAFE_RELEASE(pLight);
	}

	m_pState[STATE_DEPTH_DISABLE]->ResetState();
	m_pState[STATE_ACC_BLEND]->ResetState();
	m_pGBufferMultiTarget->ResetShaderResource(10);

	m_pLightMultiTarget->ResetTarget();
}

void CRenderManager::RenderLightDir(float fTime, CLight * pLight)
{
	m_pShader[SHADER_ACC_DIR]->SetShader();

	// 조명 정보를 상수버퍼에 넘겨준다.
	pLight->UpdateLightCBuffer();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CONTEXT->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);
}

void CRenderManager::RenderLightPoint(float fTime, CLight * pLight)
{
	m_pShader[SHADER_ACC_POINT]->SetShader();

	pLight->UpdateLightCBuffer();

	CScene* pScene = CSceneManager::GetInst()->GetSceneNonCount();
	CCamera* getCamera = NULLPTR;

	Matrix	matScale, matPos;
	matScale.Scaling(pLight->GetLightInfo().fRange, pLight->GetLightInfo().fRange, pLight->GetLightInfo().fRange);
	matPos.Translation(pLight->GetLightInfo().vPos);

	getCamera = pScene->GetMainCameraNonCount();

	TransformCBuffer cBuffer = {};
	cBuffer.matWorld = matScale * matPos;
	cBuffer.matView = getCamera->GetViewMatrix();
	cBuffer.matProj = getCamera->GetProjMatrix();

	cBuffer.matWV = cBuffer.matWorld * cBuffer.matView;
	cBuffer.matWVP = cBuffer.matWV * cBuffer.matProj;

	cBuffer.matWorld.Transpose();
	cBuffer.matView.Transpose();
	cBuffer.matProj.Transpose();
	cBuffer.matWV.Transpose();
	cBuffer.matWVP.Transpose();

	CONTEXT->IASetInputLayout(m_pPointLightLayout);

	CShaderManager::GetInst()->UpdateCBuffer("Transform", &cBuffer);

	m_pState[STATE_FRONT_CULL]->SetState();	
	{
		m_pState[STATE_DEPTH_GRATOR]->SetState();		
		{
			m_pState[STATE_ALL_BLEND]->SetState();			
			{
				m_pSphereVolum->Render();
			}
			m_pState[STATE_ALL_BLEND]->ResetState();
		}
		m_pState[STATE_DEPTH_GRATOR]->ResetState();
	}
	m_pState[STATE_FRONT_CULL]->ResetState();


	m_pState[STATE_BACK_CULL]->SetState();
	{
		m_pState[STATE_ZERO_BLEND]->SetState();
		{
			m_pState[STATE_DEPTH_LESS]->SetState();
			{
				m_pSphereVolum->Render();
			}
			m_pState[STATE_DEPTH_LESS]->ResetState();
		}
		m_pState[STATE_ZERO_BLEND]->ResetState();
	}
	m_pState[STATE_BACK_CULL]->ResetState();

	if (m_bLightWireFrame == false)
		return;

	//m_pState[STATE_DEPTH_DISABLE]->SetState();
	m_pState[STATE_CULL_NONE]->SetState();
	m_pState[STATE_WIRE_FRAME]->SetState();
	{
		m_pSphereVolum->Render();
	}
	m_pState[STATE_WIRE_FRAME]->ResetState();
	m_pState[STATE_CULL_NONE]->ResetState();
	//m_pState[STATE_DEPTH_DISABLE]->ResetState();
}

void CRenderManager::RenderLightSpot(float fTime, CLight * pLight)
{
	m_pShader[SHADER_ACC_SPOT]->SetShader();

	pLight->UpdateLightCBuffer();

	CScene* pScene = CSceneManager::GetInst()->GetSceneNonCount();
	CCamera* getCamera = NULLPTR;

	Matrix	matScale, matPos, matRot, matLocal;

	matScale.Scaling(pLight->GetLightInfo().fRange, pLight->GetLightInfo().fRange, pLight->GetLightInfo().fRange);
	matPos.Translation(pLight->GetLightInfo().vPos);
	matRot.Rotation(pLight->GetTransformNonCount()->GetWorldRot());
	matLocal = pLight->GetTransformNonCount()->GetLocalMatrix();

	getCamera = pScene->GetMainCameraNonCount();

	TransformCBuffer cBuffer = {};
	cBuffer.matWorld = matLocal * matScale* matRot * matPos;
	cBuffer.matView = getCamera->GetViewMatrix();
	cBuffer.matProj = getCamera->GetProjMatrix();

	cBuffer.matWV = cBuffer.matWorld * cBuffer.matView;
	cBuffer.matWVP = cBuffer.matWV * cBuffer.matProj;

	cBuffer.matWorld.Transpose();
	cBuffer.matView.Transpose();
	cBuffer.matProj.Transpose();
	cBuffer.matWV.Transpose();
	cBuffer.matWVP.Transpose();

	CShaderManager::GetInst()->UpdateCBuffer("Transform", &cBuffer);

	CONTEXT->IASetInputLayout(m_pPointLightLayout);

	m_pState[STATE_FRONT_CULL]->SetState();
	{
		m_pState[STATE_DEPTH_GRATOR]->SetState();
		{
			m_pState[STATE_ALL_BLEND]->SetState();
			{
				m_pCornVolum->Render();
			}
			m_pState[STATE_ALL_BLEND]->ResetState();
		}
		m_pState[STATE_DEPTH_GRATOR]->ResetState();
	}
	m_pState[STATE_FRONT_CULL]->ResetState();


	m_pState[STATE_BACK_CULL]->SetState();
	{
		m_pState[STATE_ZERO_BLEND]->SetState();
		{
			m_pState[STATE_DEPTH_LESS]->SetState();
			{
				m_pCornVolum->Render();
			}
			m_pState[STATE_DEPTH_LESS]->ResetState();
		}
		m_pState[STATE_ZERO_BLEND]->ResetState();
	}
	m_pState[STATE_BACK_CULL]->ResetState();

	if (m_bLightWireFrame == false)
		return;

	//m_pState[STATE_DEPTH_DISABLE]->SetState();
	m_pState[STATE_CULL_NONE]->SetState();
	m_pState[STATE_WIRE_FRAME]->SetState();
	{
		m_pCornVolum->Render();
	}
	m_pState[STATE_WIRE_FRAME]->ResetState();
	m_pState[STATE_CULL_NONE]->ResetState();
	//m_pState[STATE_DEPTH_DISABLE]->ResetState();
}

void CRenderManager::RenderLightBlend(float _fTime)
{
	m_pTarget[TARGET_BLEND]->ClearTarget();
	m_pTarget[TARGET_BLEND]->SetTarget();

	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = GET_SINGLE(CSceneManager)->GetMainCamera();

	if (pMainCamera->IsShadow())
		tCBuffer.iShadowEnable = 1;
	else
		tCBuffer.iShadowEnable = 0;

	tCBuffer.matWorld.Identity();
	tCBuffer.matView = pMainCamera->GetViewMatrix();
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.matWLP = tCBuffer.matWorld * pMainCamera->GetShadowViewMatrix() * pMainCamera->GetShadowProjMatrix();
	tCBuffer.matInvWVP = tCBuffer.matWVP;
	tCBuffer.matInvWVP.Inverse();
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
	tCBuffer.matVP = tCBuffer.matView * tCBuffer.matProj;

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();
	tCBuffer.matWLP.Transpose();
	tCBuffer.matInvWVP.Transpose();
	tCBuffer.matInvProj.Transpose();
	tCBuffer.matVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &tCBuffer);
	SAFE_RELEASE(pMainCamera);


	m_pState[STATE_DEPTH_DISABLE]->SetState();

	m_pGBufferSampler->SetShader(10);
	m_pTarget[TARGET_DS_SHADOW]->SetShader(16);

	m_pFilter[CFT_BLUR]->SetShaderResourceTo(9, 1);

	m_pTarget[TARGET_ALBEDO]->SetShader(10);
	m_pTarget[TARGET_DEPTH]->SetShader(12);

	m_pTarget[TARGET_ACC_DIFF]->SetShader(14);
	m_pTarget[TARGET_ACC_SPC]->SetShader(15);

	m_pShader[SHADER_BLEND]->SetShader();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pTarget[TARGET_ALBEDO]->ResetShader(10);
	
	m_pFilter[CFT_BLUR]->ResetShaderResourceFrom(9, 1);

	m_pTarget[TARGET_ACC_DIFF]->ResetShader(14);
	m_pTarget[TARGET_ACC_SPC]->ResetShader(15);
	m_pTarget[TARGET_DEPTH]->ResetShader(12);
	m_pTarget[TARGET_DS_SHADOW]->ResetShader(16);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();
	m_pTarget[TARGET_BLEND]->ResetTarget();
}

void CRenderManager::RenderSkyObj(float _fTime)
{
	if (m_bFogEnable)
		RenderSkyObj(TARGET_SKY, _fTime);
	else
		RenderSkyObj(TARGET_BACK, _fTime);
}

void CRenderManager::RenderSkyObj(FAVORITE_TARGET eTarget, float fTime)
{
	m_pTarget[eTarget]->ClearTarget();
	m_pTarget[eTarget]->SetTarget();

	m_pSkyObj->Render(fTime);

	m_pTarget[TARGET_BLEND]->SetShader(0);
	m_pState[STATE_DEPTH_DISABLE]->SetState();

	m_pShader[SHADER_FULL_SCREEN]->SetShader();

	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pTarget[TARGET_BLEND]->ResetShader(0);

	// Alpha 출력
	m_pTarget[TARGET_DEPTH]->SetShader(11);

	m_pState[STATE_DEPTH_DISABLE]->SetState();
	
	for (int i = RG_ALPHA1; i <= RG_ALPHA3; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}

	m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pTarget[TARGET_DEPTH]->ResetShader(11);

	m_pTarget[eTarget]->ResetTarget();
}

void CRenderManager::RenderFog(float _fTime)
{
	//첫번째 패스 시작
	m_pTarget[TARGET_FOG_DEPTH]->ClearTarget();

	m_pTarget[TARGET_BACK]->ClearTarget();

	CONTEXT->CopyResource(m_pTarget[TARGET_BACK]->GetTexture(), m_pTarget[TARGET_SKY]->GetTexture());

	m_pGBufferSampler->SetShader(9);
	m_pState[STATE_DEPTH_DISABLE]->SetState();

	for (int i = 0; i < m_tRenderObj[RG_FOG].iSize; ++i)
	{
		m_pTarget[TARGET_FOG_DEPTH]->SetTarget();

		m_pState[STATE_FRONT_CULL]->SetState();

		m_pShader[SHADER_FOG_FRONT]->SetShader();

		m_tRenderObj[RG_FOG].pList[i]->Render(_fTime);

		m_pState[STATE_FRONT_CULL]->ResetState();

		CONTEXT->CopyResource(m_pFogDepthTex, m_pTarget[TARGET_FOG_DEPTH]->GetTexture());

		//두번째 패스 시작
		m_pState[STATE_BACK_CULL]->SetState();

		m_pShader[SHADER_FOG_BACK]->SetShader();

		CONTEXT->PSSetShaderResources(9, 1, &m_pFogDepthSRV);

		m_tRenderObj[RG_FOG].pList[i]->Render(_fTime);

		ID3D11ShaderResourceView* pSRV = nullptr;

		CONTEXT->PSSetShaderResources(9, 1, &pSRV);

		m_pState[STATE_BACK_CULL]->ResetState();

		m_pTarget[TARGET_FOG_DEPTH]->ResetTarget();

		//세번째 패스 시작
		m_pTarget[TARGET_BACK]->SetTarget();

		m_pTarget[TARGET_FOG_DEPTH]->SetShader(10);
		m_pTarget[TARGET_SKY]->SetShader(11);

		m_pShader[SHADER_FOG_COLOR]->SetShader();

		m_pState[STATE_CULL_NONE]->SetState();

		m_tRenderObj[RG_FOG].pList[i]->Render(_fTime);

		m_pState[STATE_CULL_NONE]->ResetState();

		m_pTarget[TARGET_FOG_DEPTH]->ResetShader(10);
		m_pTarget[TARGET_SKY]->ResetShader(11);

		m_pTarget[TARGET_BACK]->ResetTarget();
	}
	m_pState[STATE_DEPTH_DISABLE]->ResetState();
}

void CRenderManager::RenderFinalPass(float _fTime)
{
	m_pState[STATE_DEPTH_DISABLE]->SetState();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("FinalPass", &m_tFinalCBuffer);

	m_pTarget[TARGET_BACK]->SetShader(0);
	m_pTarget[TARGET_DEPTH]->SetShader(1);
	for (int i = CFT_HDR; i < CFT_END; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->SetShaderResourceTo();
	}

	m_pShader[SHADER_FINAL_PASS]->SetShader();
	m_pGBufferSampler->SetShader(0);

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();
	m_pTarget[TARGET_BACK]->ResetShader(0);
	m_pTarget[TARGET_DEPTH]->ResetShader(1);

	for (int i = CFT_HDR; i < CFT_END; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->ResetShaderResourceFrom();
	}
}

void CRenderManager::RenderFinalPassDebug(float _fTime)
{
	m_pState[STATE_DEPTH_DISABLE]->SetState();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("FinalPass", &m_tFinalCBuffer);

	m_pTarget[TARGET_BACK]->SetShader(0);

	m_pTarget[TARGET_DEPTH]->SetShader(1);
	for (int i = CFT_HDR; i < CFT_END; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->SetShaderResourceTo();
	}

	m_pShader[SHADER_FINAL_PASS]->SetShader();
	m_pGBufferSampler->SetShader(9);

	m_pTarget[TARGET_FINAL]->ClearTarget();
	m_pTarget[TARGET_FINAL]->SetTarget();

	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pTarget[TARGET_DEPTH]->ResetShader(1);
	m_pTarget[TARGET_BACK]->ResetShader(0);

	for (int i = CFT_HDR; i < CFT_END; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->ResetShaderResourceFrom();
	}

	m_pTarget[TARGET_FINAL]->ResetTarget();

	/***************************************************************************************/
	// 야간 투시경 

	// 상수 버퍼 업데이트
	m_accShakeTime += _fTime;
	float quakeTime = 0.3f;
	float quakeDelay = 0.01f;
	if (quakeDelay >= quakeDelay && m_accShakeTime <= quakeTime)
	{
		// Random
		int defaultNum = 8;
		int randNum = RandomRange(0, 2);
		defaultNum += randNum;
		Vector2 vPos((1.f / defaultNum), (1.f / defaultNum));

		// Sign
		int randSign = 0;
		while (true)
		{
			randSign = RandomRange(-1, 1);
			if (randSign != 0)
			{
				break;
			}
		}

		m_tStarLightScope.isShake = 1;
		m_tStarLightScope.isSign = randSign;
		m_tStarLightScope.vRandomPos = vPos;
		GET_SINGLE(CShaderManager)->UpdateCBuffer("StarLightScope", &m_tStarLightScope);
	}
	else if (m_accShakeTime > (quakeDelay + quakeTime))
	{
		m_accShakeTime = 0.f;
		m_tStarLightScope.isShake = 0;
		GET_SINGLE(CShaderManager)->UpdateCBuffer("StarLightScope", &m_tStarLightScope);
	}

	// Public 상수 버퍼 초기화
	if (m_isGetCameraObj == false)
	{
		m_isGetCameraObj = true;
		CSceneManager* pSceneMgr = CSceneManager::GetInst();
		CScene* pScene = pSceneMgr->GetSceneNonCount();
		float cameraNear = pScene->GetMainCameraNonCount()->GetCameraNear();
		float cameraFar  = pScene->GetMainCameraNonCount()->GetCameraFar();
		m_tCBuffer.vNearFar = Vector2(cameraNear, cameraFar);
		CShaderManager::GetInst()->UpdateCBuffer("PublicCBuffer", &m_tCBuffer);
	}

	// IA 단계는 위에 설정된 그대로 Null Buffer 설정으로 유지한다.

	// 셰이더 진입점 설정
	m_pShader[SHADER_STARLIGHT_SCOPE]->SetShader();

	// 셰이더에서 사용할 자원을 넘겨준다.
	// - FINAL_TARGET(렌더 타겟), TARET_DEPTH을 SRV(Texture2D)로 연결시킨다.
	m_pTarget[TARGET_FINAL]->SetShader(0);
	m_pTarget[TARGET_DEPTH]->SetShader(2);

	// 노이즈 텍스처 SRV
	if (m_pNoiseTex != nullptr)
	{
		m_pNoiseTex->SetShader(1);
	}

	// OM 단계에서 출력을 위한 자원인 RTV(StarLightScope)를 연결시킨다.
	m_pTarget[TARGET_STARLIGHT_SCOPE]->ClearTarget();
	m_pTarget[TARGET_STARLIGHT_SCOPE]->SetTarget();

	// 렌더링 파이프라인 시작
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	// Reset SRV, RTV
	m_pNoiseTex->SetShader(1);
	m_pTarget[TARGET_FINAL]->ResetShader(0);
	m_pTarget[TARGET_DEPTH]->ResetShader(2);
	m_pTarget[TARGET_STARLIGHT_SCOPE]->ResetTarget();

	// 풀 스크린 출력
	m_pShader[SHADER_FULL_SCREEN]->SetShader();
	m_pTarget[TARGET_STARLIGHT_SCOPE]->SetShader(0);
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);
	m_pState[STATE_DEPTH_DISABLE]->ResetState();
	m_pTarget[TARGET_STARLIGHT_SCOPE]->ResetShader(0);
}

void CRenderManager::RenderShadowMap(float fTime)
{
	m_pTarget[TARGET_SHADOW]->ClearTarget();
	m_pTarget[TARGET_SHADOW]->SetTarget();

	m_pShader[SHADER_SHADOW]->SetShader();

	for (int j = 0; j < m_tRenderObj[RG_NORMAL].iSize; ++j)
	{
		m_tRenderObj[RG_NORMAL].pList[j]->RenderShadow(fTime);
	}

	m_pTarget[TARGET_SHADOW]->ResetTarget();
}

void CRenderManager::RenderShadowTexture(float fTime)
{
	TransformCBuffer	tCBuffer = {};

	CCamera* pCamera = GET_SINGLE(CSceneManager)->GetMainCameraNoneCount();

	tCBuffer.matVP		= pCamera->GetVP();
	tCBuffer.matInvVP = tCBuffer.matVP;
	tCBuffer.matInvVP.Inverse();
	tCBuffer.matLP = pCamera->GetShadowVP();

	tCBuffer.matVP.Transpose();
	tCBuffer.matInvVP.Transpose();
	tCBuffer.matLP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &tCBuffer);

	m_pTarget[TARGET_SHADOW_TEX]->ClearTarget();
	m_pTarget[TARGET_SHADOW_TEX]->SetTarget();

	m_pTarget[TARGET_SHADOW]->SetShader(10);
	m_pTarget[TARGET_DEPTH]->SetShader(11);

	m_pShader[SHADER_SHADOW_TEX]->SetShader();

	m_pState[STATE_DEPTH_DISABLE]->SetState();

	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pTarget[TARGET_SHADOW]->ResetShader(10);
	m_pTarget[TARGET_DEPTH]->ResetShader(11);

	m_pTarget[TARGET_SHADOW_TEX]->ResetTarget();

	DownScaleShadowTexture(fTime);
}

void CRenderManager::DownScaleShadowTexture(float fTime)
{
	m_pTarget[TARGET_DS_SHADOW]->ClearTarget();
	m_pTarget[TARGET_DS_SHADOW]->SetTarget();

	m_pTarget[TARGET_SHADOW_TEX]->SetShader(10);
	m_pShader[SHADER_DS_SHADOW]->SetShader();

	m_pState[STATE_DEPTH_DISABLE]->SetState();

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pTarget[TARGET_SHADOW_TEX]->ResetShader(10);

	m_pTarget[TARGET_DS_SHADOW]->ResetTarget();

	int iEnable = 0;
	int iDOF = 0;
	if (!m_pFilter[CFT_BLUR]->GetEnable())
	{
		iEnable = 1;
		m_pFilter[CFT_BLUR]->Enable();
	}

	if (((CCSBlur*)m_pFilter[CFT_BLUR])->GetDOFEnable())
	{
		((CCSBlur*)m_pFilter[CFT_BLUR])->DisableDOF();
		iDOF = 1;
	}

	m_pFilter[CFT_BLUR]->ChangeSourceSRV(m_pTarget[TARGET_DS_SHADOW]->GetShaderResourceView());

	m_pFilter[CFT_BLUR]->Dispatch(fTime);

	m_pFilter[CFT_BLUR]->ResetSourceSRV();

	if (iEnable == 1)
		m_pFilter[CFT_BLUR]->Disable();
	if (iDOF == 1)
		((CCSBlur*)m_pFilter[CFT_BLUR])->EnableDOF();

	CONTEXT->CopyResource(m_pTarget[TARGET_DS_SHADOW]->GetTexture(), ((CCSBlur*)m_pFilter[CFT_BLUR])->GetTexture());
}

void CRenderManager::RenderNaviEditorMode(float fTime)
{
	m_pTarget[TARGET_BACK]->ClearTarget();
	m_pTarget[TARGET_BACK]->SetTarget();

	m_pSkyObj->Render(fTime);

	//m_pState[STATE_DEPTH_DISABLE]->SetState();
	for (size_t i = 0; i < m_tRenderObj[RG_LANDSCAPE].iSize; i++)
	{
		m_tRenderObj[RG_LANDSCAPE].pList[i]->Render(fTime);
	}

	//m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pState[STATE_ALPHA_BLEND]->SetState();

	for (int j = 0; j < m_tRenderObj[RG_NORMAL].iSize; ++j)
	{
		m_tRenderObj[RG_NORMAL].pList[j]->RenderNaviEditorMode(fTime);
	}

	m_pTarget[TARGET_BACK]->ResetTarget();
	m_pState[STATE_ALPHA_BLEND]->ResetState();

	CEditManager::GetInst()->Render(fTime);

	m_pTarget[TARGET_BACK]->SetShader(0);
	m_pShader[SHADER_FULL_SCREEN]->SetShader();

	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	CCollisionManager::GetInst()->Render(fTime);
	CInput::GetInst()->SelectNaviBoxRender(fTime);

	for (int i = RG_LANDSCAPE; i < RG_END; ++i)
	{
		m_tRenderObj[i].iSize = 0;
	}
	m_tGizmoGroup.iSize = 0;
}

void CRenderManager::RenderComputeProcess(float fTime)
{
	for (int i = 0; i < 6; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->Dispatch(fTime);
	}
}

void CRenderManager::RenderForward(float fTime)
{
	// 포스트 이펙트 처리용 타겟으로 교체한다.
	CRenderTarget*	pTarget = FindRenderTarget("PostEffect");

	pTarget->ClearTarget();
	pTarget->SetTarget();

	if (m_tLightGroup.iSize > 0)
	{
		CLight*	pLight = m_tLightGroup.pList[0]->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
		pLight->UpdateLightCBuffer();
	}

	for (int i = 0; i <= RG_ALPHA3; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
			m_tRenderObj[i].pList[j]->Render(fTime);

		m_tRenderObj[i].iSize = 0;
	}

	pTarget->ResetTarget();

	CRenderState*	pAlphaBlend = FindRenderState(ALPHA_BLEND);

	// 여기에서 포스트이펙트 처리가 된 타겟을 전체 크기로 화면에 출력한다.
	pTarget->RenderFullScreen();

	// UI출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
			m_tRenderObj[i].pList[j]->Render(fTime);

		m_tRenderObj[i].iSize = 0;
	}

	m_tLightGroup.iSize = 0;

	pAlphaBlend->SetState();

	GET_SINGLE(CViewManager)->Render(fTime);

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

CMultiRenderTarget * CRenderManager::FindMultiTarget(const string & MultiKey)
{
	return GET_SINGLE(CViewManager)->FindMRT(MultiKey);
}

void CRenderManager::EnableFilter(CS_FILTER_TYPE eType)
{
	m_pFilter[eType]->Enable();

	switch (eType)
	{
	case CFT_HDR:
		m_tFinalCBuffer.iHDR = 1;
		break;

	case CFT_ADAPTATION:
		m_tFinalCBuffer.iAdaptation = 1;
		break;

	case CFT_BLUR:
		m_tFinalCBuffer.iBlur = 1;
		break;

	case CFT_MOTIONBLUR:
		m_tFinalCBuffer.iMotionBlur = 1;
		break;

	case CFT_BLOOM:
		m_tFinalCBuffer.iBloom = 1;
		break;
	}
}

void CRenderManager::DisableFilter(CS_FILTER_TYPE eType)
{
	if (eType == CFT_DOWNSCALE)
		return;

	m_pFilter[eType]->Disable();

	switch (eType)
	{
	case CFT_HDR:
		m_tFinalCBuffer.iHDR = 0;
		break;

	case CFT_ADAPTATION:
		m_tFinalCBuffer.iAdaptation = 0;
		break;

	case CFT_BLUR:
		m_tFinalCBuffer.iBlur = 0;
		break;

	case CFT_MOTIONBLUR:
		m_tFinalCBuffer.iMotionBlur = 0;
		break;

	case CFT_BLOOM:
		m_tFinalCBuffer.iBloom = 0;
		break;
	}
}

void CRenderManager::SetStarLightScope(int _flag)
{
	m_tStarLightScope.isStarLightScope = _flag;
}

void CRenderManager::FindMagicNumber(float fTime)
{
	if (m_bMagic == true)
	{
		if (CCore::GetInst()->m_bEditorMode == true)
			return;

		ImGui::Begin("ShaderOption");

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("ShaderOption");
			ImGui::EndTooltip();
		}

		ImGui::Checkbox("StarLight Scope", &m_bSLC);

		if (m_bSLC)
		{
			SetStarLightScope(1);
		}
		else
		{
			SetStarLightScope(0);
		}

		ImGui::Checkbox("HDR", &m_bHDR);

		if (m_bHDR)
		{
			EnableFilter(CFT_HDR);
			ImGui::SliderFloat("MiddleGrey", &m_fMiddleGrey, 0.f, 6.f);
			ImGui::Text("MiddleGrey : %f", m_fMiddleGrey);
			ImGui::SliderFloat("LumWhite", &m_fLumWhite, 0.f, 6.f);
			ImGui::Text("LumWhite : %f", m_fLumWhite);

			m_pPostEffect->SetFinalPassCB(m_fMiddleGrey, m_fLumWhite, fTime);
			m_pPostEffect->UpdateCBuffer(CFT_HDR);
		}
		else
			DisableFilter(CFT_HDR);

		ImGui::Checkbox("Adaptation", &m_bAdaptation);

		if (m_bAdaptation)
		{
			EnableFilter(CFT_ADAPTATION);
			ImGui::SliderFloat("Adaptation", &m_fAdaptation, 0.f, 10.f);
			ImGui::Text("Adaptation : %f", m_fAdaptation);

			CCSAdaptFilter*	pFilter = (CCSAdaptFilter*)GET_SINGLE(CViewManager)->FindCSFilter(CFT_ADAPTATION);

			pFilter->SetAdaptationTime(m_fAdaptation);
		}
        else
			DisableFilter(CFT_ADAPTATION);
		
		ImGui::Checkbox("Bloom", &m_bBloom);

		if (m_bBloom)			
		{
			EnableFilter(CFT_BLOOM);
			ImGui::SliderFloat("BloomThreshold", &m_fBloomThreshold, 0.f, 2.5f);
			ImGui::Text("BoomThreshold : %f", m_fBloomThreshold);
			ImGui::SliderFloat("BloomScale", &m_fBloomScale, 0.f, 2.f);
			ImGui::Text("BloomScale : %f", m_fBloomScale);

			CCSBloomFilter*	pFilter = (CCSBloomFilter*)GET_SINGLE(CViewManager)->FindCSFilter(CFT_BLOOM);
			pFilter->SetBloomThreshold(m_fBloomThreshold);
			pFilter->SetBloomScale(m_fBloomScale);
		}
		else
			DisableFilter(CFT_BLOOM);

		ImGui::Checkbox("Blur", &m_bBlur);

		if (m_bBlur)
			EnableFilter(CFT_BLUR);
		else
			DisableFilter(CFT_BLUR);

		ImGui::Checkbox("MotionBlur", &m_bMotionBlur);

		if (m_bMotionBlur)
			EnableFilter(CFT_MOTIONBLUR);
		else
			DisableFilter(CFT_MOTIONBLUR);

		ImGui::Checkbox("SSAO", &m_bSSAOEnable);
		if (m_bSSAOEnable)
			m_tCBuffer.iSSAOEnable = 1;
		else
			m_tCBuffer.iSSAOEnable = 0;

		ImGui::Checkbox("DepthFog", &m_bDepthFog);

		if (m_bDepthFog)
		{
			m_tFinalCBuffer.iDepthFog = 1;
			ImGui::SliderFloat("DepthFog_R", &m_vDepthFogColor.r, 0.f, 1.f);
			ImGui::Text("DepthFog_R : %f", m_vDepthFogColor.r);
			ImGui::SliderFloat("DepthFog_G", &m_vDepthFogColor.g, 0.f, 1.f);
			ImGui::Text("DepthFog_G : %f", m_vDepthFogColor.g);
			ImGui::SliderFloat("DepthFog_B", &m_vDepthFogColor.b, 0.f, 1.f);
			ImGui::Text("DepthFog_B : %f", m_vDepthFogColor.b);
			ImGui::SliderFloat("DepthFog_StartPos", &m_fDepthFogStart, 0.f, 1000.f);
			ImGui::Text("DepthFog_StartPos : %f", m_fDepthFogStart);
			ImGui::SliderFloat("DepthFog_EndPos", &m_fDepthFogEnd, 0.f, 1000.f);
			ImGui::Text("DepthFog_EndPos : %f", m_fDepthFogEnd);

			m_tDepthFogCBuffer.fStartDepth = m_fDepthFogStart;
			m_tDepthFogCBuffer.fEndDepth = m_fDepthFogEnd;
			m_tDepthFogCBuffer.vFogColor = m_vDepthFogColor;

			GET_SINGLE(CShaderManager)->UpdateCBuffer("DepthFog", &m_tDepthFogCBuffer);
		}
		else
			m_tFinalCBuffer.iDepthFog = 0;

		ImGui::End();
	}
}
