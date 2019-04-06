#include "EngineHeader.h"
#include "RenderManager.h"
#include "Core.h"
#include "Shader.h"
#include "CSBlur.h"
#include "DownScale.h"
#include "BlendState.h"
#include "DepthState.h"
#include "PostEffect.h"
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
	m_bFogEnable(true),
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

	memset(m_pState, 0, sizeof(CRenderState*) * STATE_END);
	memset(m_pTarget, 0, sizeof(CRenderTarget*) * TARGET_END);
	memset(m_pShader, 0, sizeof(CShader*) * SHADER_END);
	memset(m_pFilter, 0, sizeof(CCSFilter*) * CFT_END);

	m_bWireFrame = true;

	m_tCBuffer = {};
}

CRenderManager::~CRenderManager()
{
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

	((CDepthState*)m_pState[STATE_DEPTH_GRATOR])->SetStencilRef(1); //���ٽǰ��� 1�� ä���.

	m_pGBufferMultiTarget			= GET_SINGLE(CViewManager)->FindMRT("GBuffer");
	m_pLightMultiTarget				= GET_SINGLE(CViewManager)->FindMRT("LightAcc");
	m_pDecalMultiTarget				= GET_SINGLE(CViewManager)->FindMRT("SSDBuffer");
	m_pDownScaledGBufferMultiTarget = GET_SINGLE(CViewManager)->FindMRT("DownScaleGBuffer");

	for (int i = CFT_DOWNSCALE; i < CFT_END; ++i)
	{
		m_pFilter[i] = GET_SINGLE(CViewManager)->FindCSFilter((CS_FILTER_TYPE)i);
		m_pFilter[i]->Disable();
	}

	ID3D11Texture2D* pTex = m_pTarget[TARGET_FOG_DEPTH]->GetTexture();

	D3D11_TEXTURE2D_DESC tDesc;
	pTex->GetDesc(&tDesc);

	DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pFogDepthTex);
	DEVICE->CreateShaderResourceView(m_pFogDepthTex, nullptr, &m_pFogDepthSRV);

	m_pRandomNormalTex = GET_SINGLE(CResourcesManager)->FindTexture("RandomNormal");

	m_tCBuffer.iSSAOEnable = 1;


	// �߰� ���ð濡 �ʿ��� ����
	srand((unsigned int)time(NULL));
	if (CResourcesManager::GetInst()->CreateTexture("Noise", TEXT("noise6.png")))
	{
		m_pNoiseTex = CResourcesManager::GetInst()->FindTexture("Noise");
	}
	SetStarLightScope(0);
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
		���� �κ��� '�׷��� ������Ʈ�̸鼭 ���� ������ �ִ� ������Ʈ'�� ���
		(EX. ȶ��, ���� (�ٵ� �̰� ������� ȶ�ҵ��� �ص� �� �� ����.. ����) 
			�Ǵ� ��ƼŬ)
		�������� ������ ���� ���̱� ������ ������ ���� üũ�� ���־�� �Ѵٰ�
		�Ǵ��ؼ� �߰���.
		�������� ������ �������� ���� ������ ��� m_tLightGroup������ ó���ǰ� �ϰ�
		�������� ���� ������Ʈ�� ��� m_tLightGroup���� ó�����ܰ� ���ÿ�
		���� �׷쿡 ���Խ��� ȭ�鿡 �����Ѵ�.
		�ϴ� ����Ʈ�� NORMAL�� �־����� �Ƹ� EFFECT����� �׷� ���� �׷���
		���� ���� �־���� �� ���� �𸣰ڴ�.
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

void CRenderManager::Render(float fTime)
{
	m_tCBuffer.DeltaTime = fTime;
	m_tCBuffer.PlusedDeltaTime += fTime;
	m_tCBuffer.isWireFrame = m_bWireFrame;

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
	// ����Ʈ ����Ʈ ó���� Ÿ������ ��ü�Ѵ�.
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

	// ���⿡�� ����Ʈ ����Ʈ�� ó���Ѵ�.
	CRenderState*	pAlphaBlend = GET_SINGLE(CViewManager)->FindRenderState(ALPHA_BLEND);

	// ���⿡�� ����Ʈ����Ʈ ó���� �� Ÿ���� ��ü ũ��� ȭ�鿡 ����Ѵ�.
	pTarget->RenderFullScreen();

	// UI���
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
	// Forward Rendering ó��
	if (!m_bDeferred)
		RenderForward(fTime);
	else
		RenderDeferred(fTime);

	GET_SINGLE(CViewManager)->Render(fTime);
}

void CRenderManager::RenderDeferred(float fTime)
{
	// MainCamera�� ���´�.
	CCamera* pMainCamera = GET_SINGLE(CSceneManager)->GetMainCameraNoneCount();
	
	// �׸��� ���� �׷��ش�.
	if (pMainCamera->IsShadow())
		RenderShadowMap(fTime);
	// GBuffer�� ������ش�.
	RenderGBuffer(fTime);
	// GBuffer�� �̿��� ��Į�� GBuffer�� �߰��Ͽ� �׸���.
	RenderDecal(fTime);
	// SSAO�� ������ش�.
	if(m_bSSAOEnable)
		RenderSSAO(fTime);
	// ���� �������۸� ������ش�.
	RenderLightAcc(fTime);
	// ����Ÿ�ٰ� Albedo �� �ռ��Ѵ�.
	RenderLightBlend(fTime);
	// ��ī�� ������Ʈ�� ���� ó���� Ÿ���� ���� �׸���.
	RenderSkyObj(fTime);
	// �����Ȱ��� �׸���.
	if(m_bFogEnable)
		RenderFog(fTime);
	// HDR �� �پ��� ȭ�� ȿ���� ����Ѵ�.
	RenderComputeProcess(fTime);
	// ���� �ռ��� Ÿ���� ȭ�鿡 ����Ѵ�.
#ifdef _DEBUG
	RenderFinalPassDebug(fTime);
#else
	RenderFinalPass(fTime);
#endif
	
	GET_SINGLE(CCollisionManager)->Render(fTime);
	if (CCore::GetInst()->m_bEditorMode == true)
	{
		CEditManager::GetInst()->Render(fTime);
	}
	// UI���
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
}

void CRenderManager::RenderGBuffer(float fTime)
{
	// GBuffer MRT�� Ÿ���� ��ü�Ѵ�.
	float	fClearColor[4] = {0.f , 0.f , 0.f , 0.f};
	m_pGBufferMultiTarget->ClearRenderTarget(fClearColor);
	m_pGBufferMultiTarget->SetTarget();

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
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

	// ���� ������ ������ۿ� �Ѱ��ش�.
	pLight->UpdateLightCBuffer();

	// NULL Buffer�� ��ü ȭ��ũ���� �簢���� ����Ѵ�.
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

	if (m_bWireFrame == false)
		return;

	m_pState[STATE_WIRE_FRAME]->SetState();
	{
		m_pSphereVolum->Render();
	}
	m_pState[STATE_WIRE_FRAME]->ResetState();
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

	if (m_bWireFrame == false)
		return;

	m_pState[STATE_WIRE_FRAME]->SetState();
	{
		m_pSphereVolum->Render();
	}
	m_pState[STATE_WIRE_FRAME]->ResetState();
}

void CRenderManager::RenderLightBlend(float _fTime)
{
	m_pTarget[TARGET_BLEND]->ClearTarget();
	m_pTarget[TARGET_BLEND]->SetTarget();

	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = GET_SINGLE(CSceneManager)->GetMainCamera();

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
	m_pTarget[TARGET_SHADOWMAP]->SetShader(16);

	m_pFilter[CFT_BLUR]->SetShaderResourceTo(9, 1);

	m_pTarget[TARGET_ALBEDO]->SetShader(10);
	m_pTarget[TARGET_DEPTH]->SetShader(12);

	m_pTarget[TARGET_ACC_DIFF]->SetShader(14);
	m_pTarget[TARGET_ACC_SPC]->SetShader(15);

	m_pShader[SHADER_BLEND]->SetShader();

	// NULL Buffer�� ��ü ȭ��ũ���� �簢���� ����Ѵ�.
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
	m_pTarget[TARGET_SHADOWMAP]->ResetShader(16);

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

	// Alpha ���
	m_pTarget[TARGET_DEPTH]->SetShader(11);

	for (int i = RG_ALPHA1; i <= RG_ALPHA3; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}
	m_pTarget[TARGET_DEPTH]->ResetShader(11);

	m_pTarget[eTarget]->ResetTarget();
}

void CRenderManager::RenderFog(float _fTime)
{
	//ù��° �н� ����
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

		//�ι�° �н� ����
		m_pState[STATE_BACK_CULL]->SetState();

		m_pShader[SHADER_FOG_BACK]->SetShader();

		CONTEXT->PSSetShaderResources(9, 1, &m_pFogDepthSRV);

		m_tRenderObj[RG_FOG].pList[i]->Render(_fTime);

		ID3D11ShaderResourceView* pSRV = nullptr;

		CONTEXT->PSSetShaderResources(9, 1, &pSRV);

		m_pState[STATE_BACK_CULL]->ResetState();

		m_pTarget[TARGET_FOG_DEPTH]->ResetTarget();

		//����° �н� ����
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

	// NULL Buffer�� ��ü ȭ��ũ���� �簢���� ����Ѵ�.
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
	// �߰� ���ð� 

	// ��� ���� ������Ʈ
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

	// Public ��� ���� �ʱ�ȭ
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

	// IA �ܰ�� ���� ������ �״�� Null Buffer �������� �����Ѵ�.

	// ���̴� ������ ����
	m_pShader[SHADER_STARLIGHT_SCOPE]->SetShader();

	// ���̴����� ����� �ڿ��� �Ѱ��ش�.
	// - FINAL_TARGET(���� Ÿ��), TARET_DEPTH�� SRV(Texture2D)�� �����Ų��.
	m_pTarget[TARGET_FINAL]->SetShader(0);
	m_pTarget[TARGET_DEPTH]->SetShader(2);

	// ������ �ؽ�ó SRV
	if (m_pNoiseTex != nullptr)
	{
		m_pNoiseTex->SetShader(1);
	}

	// OM �ܰ迡�� ����� ���� �ڿ��� RTV(StarLightScope)�� �����Ų��.
	m_pTarget[TARGET_STARLIGHT_SCOPE]->ClearTarget();
	m_pTarget[TARGET_STARLIGHT_SCOPE]->SetTarget();

	// ������ ���������� ����
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	// Reset SRV, RTV
	m_pNoiseTex->SetShader(1);
	m_pTarget[TARGET_FINAL]->ResetShader(0);
	m_pTarget[TARGET_DEPTH]->ResetShader(2);
	m_pTarget[TARGET_STARLIGHT_SCOPE]->ResetTarget();

	// Ǯ ��ũ�� ���
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
	GET_SINGLE(CDevice)->SetShadowVP();

	m_pTarget[TARGET_SHADOWMAP]->ClearTarget();
	m_pTarget[TARGET_SHADOWMAP]->SetTarget();

	m_pShader[SHADER_SHADOW]->SetShader();

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->RenderShadow(fTime);
		}
	}

	m_pTarget[TARGET_SHADOWMAP]->ResetTarget();

	GET_SINGLE(CDevice)->ResetVP();
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
	// ����Ʈ ����Ʈ ó���� Ÿ������ ��ü�Ѵ�.
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

	// ���⿡�� ����Ʈ����Ʈ ó���� �� Ÿ���� ��ü ũ��� ȭ�鿡 ����Ѵ�.
	pTarget->RenderFullScreen();

	// UI���
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

	case CFT_BLUR:
	case CFT_MOTIONBLUR:
		m_tFinalCBuffer.iBlur = 1;

		break;
	}
}

void CRenderManager::DisableFilter(CS_FILTER_TYPE eType)
{
	m_pFilter[eType]->Disable();

	switch (eType)
	{
	case CFT_HDR:
		m_tFinalCBuffer.iHDR = 0;
		break;

	case CFT_BLUR:
	case CFT_MOTIONBLUR:
		m_tFinalCBuffer.iBlur = 0;

		break;
	}
}

void CRenderManager::SetStarLightScope(int _flag)
{
	m_tStarLightScope.isStarLightScope = _flag;
}