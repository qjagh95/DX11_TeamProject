#include "EngineHeader.h"
#include "RenderManager.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthState.h"
#include "RenderTarget.h"
#include "RasterizerState.h"
#include "MultiRenderTarget.h"
#include "../Device.h"
#include "../Resource/Sampler.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"
#include "ComputeShader.h"
#include "PostEffect.h"
#include "ViewManager.h"
#include "DownScale.h"

PUN_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_pCreateState(nullptr),
	m_bDeferred(true),
	m_pSkyObj(nullptr)
{
	m_eGameMode = GM_3D;
	m_pSphereVolum = NULLPTR;
	m_pCornVolum = NULLPTR;
	m_pGBufferMultiTarget = NULLPTR;
	m_pLightMultiTarget = NULLPTR;

	memset(m_pState, 0, sizeof(CRenderState*) * STATE_END);
	memset(m_pTarget, 0, sizeof(CRenderTarget*) * TARGET_END);
	memset(m_pShader, 0, sizeof(CShader*) * SHADER_END);
	memset(m_pFilter, 0, sizeof(CCSFilter*) * CFT_END);

	m_bWireFrame = true;

	m_tCBuffer = {};
}

CRenderManager::~CRenderManager()
{
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
	m_pShader[SHADER_FULL_SCREEN]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(FULLSCREEN_SHADER);
	m_pShader[SHADER_FINAL_PASS]	= GET_SINGLE(CShaderManager)->FindShaderNonCount(FINAL_PASS_SHADER);

	m_pTarget[TARGET_ALBEDO]		= GET_SINGLE(CViewManager)->FindRenderTarget("Albedo");
	m_pTarget[TARGET_DEPTH]			= GET_SINGLE(CViewManager)->FindRenderTarget("Depth");
	m_pTarget[TARGET_BACK]			= GET_SINGLE(CViewManager)->FindRenderTarget("SecondBackBuffer");
	m_pTarget[TARGET_ACC_DIFF]		= GET_SINGLE(CViewManager)->FindRenderTarget("LightAccDif");
	m_pTarget[TARGET_ACC_SPC]		= GET_SINGLE(CViewManager)->FindRenderTarget("LightAccSpc");
	m_pTarget[TARGET_BLEND]			= GET_SINGLE(CViewManager)->FindRenderTarget("LightBlend");
	m_pTarget[TARGET_FINAL]			= GET_SINGLE(CViewManager)->FindRenderTarget("Final");

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

	((CDepthState*)m_pState[STATE_DEPTH_GRATOR])->SetStencilRef(1); //스텐실값을 1로 채운다.

	m_pGBufferMultiTarget	= GET_SINGLE(CViewManager)->FindMRT("GBuffer");
	m_pLightMultiTarget		= GET_SINGLE(CViewManager)->FindMRT("LightAcc");

	for (int i = CFT_DOWNSCALE; i < CFT_END; ++i)
	{
		m_pFilter[i] = GET_SINGLE(CViewManager)->FindCSFilter((CS_FILTER_TYPE)i);
		
	}

	m_pFilter[1]->Disable();
	m_pFilter[2]->Disable();
	//m_pFilter[3]->Disable();
	//m_tFinalCBuffer.iHDR = 1;
	m_tFinalCBuffer.iBlur = 1;

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
		RenderDeferred(fTime);

	GET_SINGLE(CViewManager)->Render(fTime);
}

void CRenderManager::RenderDeferred(float fTime)
{
	// GBuffer를 만들어준다.
	RenderGBuffer(fTime);
	// 조명 누적버퍼를 만들어준다.
	RenderLightAcc(fTime);
	// 조명타겟과 Albedo 를 합성한다.
	RenderLightBlend(fTime);
	// 스카이 오브젝트와 조명 처리된 타겟을 합쳐 그린다.
	RenderSkyObj(fTime);
	// HDR 등 다양한 화면 효과를 계산한다.
	RenderComputeProcess(fTime);
	// 최종 합성된 타겟을 화면에 출력한다.
#ifdef _DEBUG
	RenderFinalPassDebug(fTime);
#else
	RenderFinalPass(fTime);
#endif
	
	GET_SINGLE(CCollisionManager)->Render(fTime);

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
}

void CRenderManager::RenderGBuffer(float fTime)
{
	// GBuffer MRT로 타겟을 교체한다.
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

	m_pState[STATE_DEPTH_DISABLE]->SetState();

	m_pGBufferSampler->SetShader(10);
	m_pTarget[TARGET_ALBEDO]->SetShader(10);

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
	m_pTarget[TARGET_ACC_DIFF]->ResetShader(14);
	m_pTarget[TARGET_ACC_SPC]->ResetShader(15);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();
	m_pTarget[TARGET_BLEND]->ResetTarget();
}

void CRenderManager::RenderSkyObj(float _fTime)
{
	m_pTarget[TARGET_BACK]->ClearTarget();
	m_pTarget[TARGET_BACK]->SetTarget();

	m_pSkyObj->Render(_fTime);

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


	m_pTarget[TARGET_BACK]->ResetTarget();
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

	// 풀 스크린 출력

	m_pShader[SHADER_FULL_SCREEN]->SetShader();
	m_pTarget[TARGET_FINAL]->SetShader(0);

	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pState[STATE_DEPTH_DISABLE]->ResetState();

	m_pTarget[TARGET_FINAL]->ResetShader(0);
}

void CRenderManager::RenderComputeProcess(float fTime)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!m_pFilter[i]->GetEnable())
			continue;

		m_pFilter[i]->Dispatch();
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
