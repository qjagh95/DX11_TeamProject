
#include "EngineHeader.h"
#include "RenderManager.h"
#include "BlendState.h"
#include "DepthState.h"
#include "RenderTarget.h"
#include "../GameObject.h"
#include "../Component/Light.h"
#include "../Device.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Sampler.h"
#include "Shader.h"

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
	SAFE_RELEASE(m_pLightAccDirShader);
	SAFE_RELEASE(m_pGBufferSampler);
	SAFE_RELEASE(m_pDepthDisable);


	Safe_Delete_Map(m_mapMRT);

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

	//////////////////////Blend//////////////////////////////
	AddBlendTargetDesc(TRUE);
	CreateBlendState(ALPHA_BLEND);
	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, 
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE , D3D11_BLEND_ONE);

	CreateBlendState(ACC_BLEND);
	///////////////////////////////////////////////////////

	//////////////////////Depth//////////////////////////
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);
	m_pDepthDisable = FindRenderState(DEPTH_DISABLE);
	//////////////////////////////////////////////////////
	// 포스트 이펙트용 렌더링타겟 생성
	Vector3	vPos;
	vPos.x = _RESOLUTION.iWidth - 100.f;

	if (!CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Albedo
	vPos.x = 0.f;
	if (!CreateRenderTarget("Albedo", DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector3::Zero, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Normal
	vPos.x = 0.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("Normal", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Depth
	vPos.x = 0.f;
	vPos.y = 200.f;
	if (!CreateRenderTarget("Depth", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Material
	vPos.x = 0.f;
	vPos.y = 300.f;
	if (!CreateRenderTarget("Material", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	AddMRT("GBuffer", "Albedo");
	AddMRT("GBuffer", "Normal");
	AddMRT("GBuffer", "Depth");
	AddMRT("GBuffer", "Material");

	// Light Dif
	vPos.x = 100.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("LightAccDif", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Light Spc
	vPos.x = 100.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("LightAccSpc", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	AddMRT("LightAcc", "Albedo");
	AddMRT("LightAcc", "Normal");

	m_pGBufferSampler = GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_POINT);

	m_pLightAccDirShader = GET_SINGLE(CShaderManager)->FindShader(LIGHT_DIR_ACC_SHADER);

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

CRenderState * CRenderManager::FindRenderStateNonCount(const string & strName)
{
	unordered_map<string, CRenderState*>::iterator	iter = m_mapRenderState.find(strName);

	if (iter == m_mapRenderState.end())
		return nullptr;

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
		m_mapMRT.insert(std::make_pair(strMRTKey, pMRT));
	}

	CRenderTarget* pTarget = FindRenderTarget(strTargetKey);

	if (pTarget == nullptr)
	{
		return false;
	}

	pMRT->vecTarget.push_back(pTarget);

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

void CRenderManager::ClearMRT(const string & strMRTKey, float fClearColor[4])
{
	PMRT pMRT = FindMRT(strMRTKey);

	if (!pMRT)
		return;

	for (size_t i = 0; i < pMRT->vecTarget.size(); ++i)
	{
		pMRT->vecTarget[i]->SetClearColor(fClearColor);
		pMRT->vecTarget[i]->ClearTarget();
	}

	if (pMRT->pDepth)
		CDevice::GetInst()->GetContext()->ClearDepthStencilView(pMRT->pDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CRenderManager::SetMRT(const string & strMRTKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	if (!pMRT)
		return;

	if (pMRT->vecOldTarget.empty())
		pMRT->vecOldTarget.resize(pMRT->vecTarget.size());

	CONTEXT->OMGetRenderTargets(pMRT->vecTarget.size(), &pMRT->vecOldTarget[0],
		&pMRT->pOldDepth);

	ID3D11DepthStencilView*	pDepth = pMRT->pDepth;

	if (!pDepth)
		pDepth = pMRT->pOldDepth;

	vector<ID3D11RenderTargetView*>	vecTarget;

	for (size_t i = 0; i < pMRT->vecTarget.size(); ++i)
	{
		vecTarget.push_back(pMRT->vecTarget[i]->GetRenderTargetView());
	}

	CONTEXT->OMSetRenderTargets(pMRT->vecTarget.size(), &vecTarget[0],
		pDepth);
}

void CRenderManager::ResetMRT(const string & strMRTKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	if (!pMRT)
		return;

	CONTEXT->OMSetRenderTargets((UINT)pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0],
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
		//RENDER_GROUP	rg = RG_NORMAL;

		//if (pObj->CheckComponentFromType(CT_STAGE2D))
		//{
		//	rg = RG_LANDSCAPE;
		//}

		//else if (pObj->CheckComponentFromType(CT_UI))
		//{
		//	rg = RG_UI;
		//}

		//else if (pObj->CheckComponentFromType(CT_LIGHT))

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

	else
	{
		//RENDER_GROUP	rg = RG_NORMAL;

		//if (pObj->CheckComponentFromType(CT_STAGE2D))
		//{
		//	rg = RG_LANDSCAPE;
		//}

		//else if (pObj->CheckComponentFromType(CT_UI))
		//{
		//	rg = RG_UI;
		//}

		RENDER_GROUP	rg = pObj->GetRenderGroup();

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

	//pAlphaBlend->SetState();

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}

	//pAlphaBlend->ResetState();

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

	// 조명 누적버퍼를 만들어준다.
	//RenderLightAcc(fTime);

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
	float	fClearColor[4] = {1.f , 0.f , 0.f , 1.f};
	ClearMRT("GBuffer", fClearColor);
	SetMRT("GBuffer");
	//CRenderTarget*	pTarget = FindRenderTarget("Albedo");

	//pTarget->SetClearColor(Vector4::Yellow);
	//pTarget->ClearTarget();
	//pTarget->SetTarget();

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
		{
			m_tRenderObj[i].pList[j]->Render(fTime);
		}
	}

	//pTarget->ResetTarget();

	ResetMRT("GBuffer");

	//pTarget->RenderFullScreen();
}

void CRenderManager::RenderLightAcc(float fTime)
{
	float	fClearColor[4] = {};
	ClearMRT("LightAcc", fClearColor);
	SetMRT("LightAcc");

	m_pLightAccDirShader->SetShader();

	CRenderState*	pAccBlend = FindRenderState(ACC_BLEND);

	pAccBlend->SetState();

	m_pDepthDisable->SetState();

	m_pGBufferSampler->SetShader(10);

	// GBuffer를 얻어온다.
	PMRT	pGBuffer = FindMRT("GBuffer");

	pGBuffer->vecTarget[1]->SetShader(11);
	pGBuffer->vecTarget[2]->SetShader(12);

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

	pGBuffer->vecTarget[1]->ResetShader(11);
	pGBuffer->vecTarget[2]->ResetShader(12);

	m_pDepthDisable->ResetState();

	pAccBlend->ResetState();

	SAFE_RELEASE(pAccBlend);

	ResetMRT("LightAcc");
}

void CRenderManager::RenderLightDir(float fTime, CLight * pLight)
{
	// 조명 정보를 상수버퍼에 넘겨준다.
	pLight->SetShader();

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
}

void CRenderManager::RenderLightSpot(float fTime, CLight * pLight)
{
}
