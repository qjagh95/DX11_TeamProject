#include "stdafx.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "BlendState.h"
#include "DepthState.h"
#include "RenderTarget.h"
#include "../GameObject.h"
#include "../Component/Light.h"
#include "../Device.h"

PUN_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_pCreateState(nullptr),
	m_bDeferred(true)
{
	m_eGameMode = GM_3D;
}


CRenderManager::~CRenderManager()
{
	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapRenderTarget.clear();

	for (int i = 0; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			SAFE_RELEASE(m_tRenderObj[i].pList[j]);
		}

		m_tRenderObj[i].iSize = 0;
	}

	Safe_Release_Map(m_mapRenderState);
	DESTROY_SINGLE(CShaderManager);
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

bool CRenderManager::Init()
{
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	AddBlendTargetDesc(TRUE);
	CreateBlendState(ALPHA_BLEND);
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);

	Vector3 vPos;
	vPos.x = CDevice::GetInst()->GetResolution().iWidth - 100.f;

	// 포스트 이펙트용 렌더링타겟 생성
	if (!CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM, 
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	////////Albedo Target 생성///////////////////////
	if (!CreateRenderTarget("Albedo", DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector3::Zero , Vector3(100.f, 100.f, 1.f), true))
		return false;

	////////Normal Target 생성///////////////////////
	vPos.x = 0.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("Normal", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	////////Depth Target 생성///////////////////////
	vPos.x = 0.f;
	vPos.y = 200.f;
	if (!CreateRenderTarget("Depth", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	////////Material(Specular) Target 생성///////////////////////
	vPos.x = 0.f;
	vPos.y = 300.f;
	if (!CreateRenderTarget("Material", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	AddMRT("GBuffer", "Albedo");
	AddMRT("GBuffer", "Normal");
	AddMRT("GBuffer", "Depth");
	AddMRT("GBuffer", "Material");

	m_bDeferred = true;
	return true;
}

void CRenderManager::AddBlendTargetDesc(BOOL bEnable,
	D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
	D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend,
	D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp,
	UINT8 iWriteMask)
{
	if (!m_pCreateState)
		m_pCreateState = new CBlendState;

	m_pCreateState->AddTargetDesc(bEnable, srcBlend, destBlend,
		blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp,
		iWriteMask);
}

bool CRenderManager::CreateBlendState(const string & strName,
	BOOL bAlphaCoverage, BOOL bIndependent)
{
	if (!m_pCreateState)
		return false;

	if (!m_pCreateState->CreateState(bAlphaCoverage, bIndependent))
	{
		SAFE_RELEASE(m_pCreateState);
		return false;
	}

	m_mapRenderState.insert(make_pair(strName, m_pCreateState));
	m_pCreateState = nullptr;

	return true;
}

bool CRenderManager::CreateDepthStencilState(const string & strKey,
	BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask,
	D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable,
	UINT8 iStencilReadMask, UINT8 iStencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC tFrontFace,
	D3D11_DEPTH_STENCILOP_DESC tBackFace)
{
	CDepthState*	pDepth = (CDepthState*)FindRenderState(strKey);

	if (pDepth)
		return false;

	pDepth = new CDepthState;

	if (!pDepth->CreateState(bDepthEnable, eMask,
		eDepthFunc, bStencilEnable, iStencilReadMask, iStencilWriteMask,
		tFrontFace, tBackFace))
	{
		SAFE_RELEASE(pDepth);
		return false;
	}

	m_mapRenderState.insert(make_pair(strKey, pDepth));

	return true;
}

CRenderState * CRenderManager::FindRenderState(const string & strName)
{
	unordered_map<string, CRenderState*>::iterator	iter = m_mapRenderState.find(strName);

	if (iter == m_mapRenderState.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CRenderManager::CreateRenderTarget(const string & strName, DXGI_FORMAT eTargetFmt,
	const Vector3 & vPos, const Vector3 & vScale, bool bDrawDebug,
	const Vector4 & vClearColor, DXGI_FORMAT eDepthFmt)
{
	CRenderTarget*	pTarget = FindRenderTarget(strName);

	if (pTarget)
		return false;

	pTarget = new CRenderTarget;

	if (!pTarget->CreateRenderTarget(eTargetFmt, vPos, vScale, eDepthFmt))
	{
		SAFE_DELETE(pTarget);
		return false;
	}

	pTarget->SetClearColor(vClearColor);
	pTarget->ONDrawDebug(bDrawDebug);

	m_mapRenderTarget.insert(make_pair(strName, pTarget));

	return true;
}

CRenderTarget * CRenderManager::FindRenderTarget(const string & strName)
{
	unordered_map<string, CRenderTarget*>::iterator	iter = m_mapRenderTarget.find(strName);

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

bool CRenderManager::AddMRT(const string & strMRTKey, const string & strTargetKey)
{
	PMRT pMRT = FindMRT(strMRTKey);

	if (!pMRT)
	{
		pMRT = new MRT;
		pMRT->pDepth = nullptr;
		//m_mapMRT.insert(std::make_pair(strMRTKey, pMRT));
	}

	CRenderTarget* pTarget = FindRenderTarget(strTargetKey);

	if (pTarget == nullptr)
	{
		return false;
	}

	pMRT->vecTarget.push_back(pTarget->GetRenderTargetView());

	return true;
}

bool CRenderManager::AddMRTDepth(const string & strMRTKey, const string & strTargetKey)
{

	PMRT pMRT = FindMRT(strMRTKey);

	if (!pMRT)
	{
		pMRT = new MRT;
		pMRT->pDepth = nullptr;
	}

	CRenderTarget* pTarget = FindRenderTarget(strTargetKey);

	if (pTarget == nullptr)
	{
		pMRT->pDepth = nullptr;
	}
	else
	{
		pMRT->pDepth = pTarget->GetDepthView();
	}

	return true;
}

void CRenderManager::SetMRT(const string & strMRTKey)
{
	PMRT pMRT = FindMRT(strMRTKey);
	
	if (pMRT == nullptr)
	{
		return;
	}

	if (pMRT->vecOldTarget.empty())
		pMRT->vecOldTarget.resize(pMRT->vecTarget.size());

	ID3D11DepthStencilView* pDepth = pMRT->pDepth;
	
	if (pDepth == nullptr)
	{
		pDepth = pMRT->pOldDepth;
	}

	CDevice::GetInst()->GetContext()->OMSetRenderTargets(pMRT->vecTarget.size(), &pMRT->vecTarget[0], pDepth);
}

void CRenderManager::ResetMRT(const string & strMRTKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	if (!pMRT)
		return;

	CONTEXT->OMSetRenderTargets(pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0],
		pMRT->pOldDepth);

	for (size_t i = 0; i < pMRT->vecOldTarget.size(); ++i)
	{
		SAFE_RELEASE(pMRT->vecOldTarget[i]);
	}

	SAFE_RELEASE(pMRT->pOldDepth);
}

PMRT CRenderManager::FindMRT(const string & strMRTKey)
{
	unordered_map<string, PMRT>::iterator	iter = m_mapMRT.find(strMRTKey);

	if (iter == m_mapMRT.end())
		return nullptr;

	return iter->second;
}

void CRenderManager::AddRenderObj(CGameObject * pObj)
{
	if (m_eGameMode == GM_3D)
	{
		RENDER_GROUP	rg = RG_NORMAL;

		if (pObj->CheckComponentFromType(CT_STAGE2D))
		{
			rg = RG_LANDSCAPE;
		}

		else if (pObj->CheckComponentFromType(CT_UI))
		{
			rg = RG_UI;
		}

		else if (pObj->CheckComponentFromType(CT_LIGHT))
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

	else
	{
		RENDER_GROUP	rg = RG_NORMAL;

		if (pObj->CheckComponentFromType(CT_STAGE2D))
		{
			rg = RG_LANDSCAPE;
		}

		else if (pObj->CheckComponentFromType(CT_UI))
		{
			rg = RG_UI;
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
}

void CRenderManager::Render(float fTime)
{
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
	CRenderTarget*	pTarget = FindRenderTarget("PostEffect");

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


	CRenderState*	pAlphaBlend = FindRenderState(ALPHA_BLEND);

	//pAlphaBlend->SetState();

	// 여기에서 포스트이펙트 처리가 된 타겟을 전체 크기로 화면에 출력한다.
	pTarget->RenderFullScreen();

	//pAlphaBlend->ResetState();

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

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

void CRenderManager::Render3D(float fTime)
{
	// Forward Rendering 처리
	if (!m_bDeferred)
	{
		RenderForward(fTime);
	}

	else
	{
		RenderDeferred(fTime);
	}

	CRenderState*	pAlphaBlend = FindRenderState(ALPHA_BLEND);

	pAlphaBlend->SetState();

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

void CRenderManager::RenderForward(float fTime)
{
	// 포스트 이펙트 처리용 타겟으로 교체한다.
	CRenderTarget*	pTarget = FindRenderTarget("PostEffect");

	pTarget->ClearTarget();
	pTarget->SetTarget();

	if (m_tLightGroup.iSize > 0)
	{
		CLight*	pLight = m_tLightGroup.pList[0]->FindComponentFromType<CLight>(CT_LIGHT);

		pLight->SetShader();

		SAFE_RELEASE(pLight);
	}

	for (int i = 0; i <= RG_ALPHA3; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}

		m_tRenderObj[i].iSize = 0;
	}

	pTarget->ResetTarget();

	CRenderState*	pAlphaBlend = FindRenderState(ALPHA_BLEND);

	//pAlphaBlend->SetState();

	// 여기에서 포스트이펙트 처리가 된 타겟을 전체 크기로 화면에 출력한다.
	pTarget->RenderFullScreen();

	//pAlphaBlend->ResetState();

	// UI출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}

		m_tRenderObj[i].iSize = 0;
	}

	m_tLightGroup.iSize = 0;

	pAlphaBlend->SetState();

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

void CRenderManager::RenderDeferred(float fTime)
{
	// GBuffer를 만들어준다.
	RenderGBuffer(fTime);


	for (int i = RG_LANDSCAPE; i < RG_END; ++i)
	{
		m_tRenderObj[i].iSize = 0;
	}
}

void CRenderManager::RenderGBuffer(float fTime)
{
	// GBuffer MRT로 타겟을 교체한다.
	SetMRT("GBuffer");

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}

	ResetMRT("GBuffer");
}
