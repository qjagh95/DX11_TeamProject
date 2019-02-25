#include "EngineHeader.h"
#include "RenderManager.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthState.h"
#include "RenderTarget.h"
#include "RasterizerState.h"
#include "MultiRenderTarget.h"
#include "../Device.h"
#include "../GameObject.h"
#include "../Resource/Sampler.h"
#include "../Resource/ResourcesManager.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

PUN_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_pCreateState(nullptr),
	m_bDeferred(true)
{
	m_eGameMode = GM_3D;
	m_pAlbedoTarget = NULLPTR;
	m_pLightBlendTarget = NULLPTR;
	m_pLightAccDifTarget = NULLPTR;
	m_pLightAccSpcTarget = NULLPTR;

	m_pAddBlend = NULLPTR;
	m_pDepthDisable = NULLPTR;
	m_pDepthGrator = NULLPTR;
	m_pDepthLess = NULLPTR;
	m_pFrontCull = NULLPTR;
	m_pBackCull = NULLPTR;
	m_pWireFrame = NULLPTR;
	m_pCullNone = NULLPTR;
	m_pZeroBlend = NULLPTR;
	m_pAllBlend = NULLPTR;
	m_pSphereVolum = NULLPTR;
	m_pCornVolum = NULLPTR;
	m_pGBufferMultiTarget = NULLPTR;
	m_pLightMultiTarget = NULLPTR;

	m_bWireFrame = true;

	m_tCBuffer = {};
}

CRenderManager::~CRenderManager()
{
	SAFE_RELEASE(m_pFullScreenShader);
	SAFE_RELEASE(m_pLightBlendShader);
	SAFE_RELEASE(m_pLightAccSpotShader);
	SAFE_RELEASE(m_pLightAccPointShader);
	SAFE_RELEASE(m_pLightAccDirShader);
	SAFE_RELEASE(m_pGBufferSampler);

	Safe_Delete_Map(m_mapMultiTarget);
	Safe_Release_Map(m_mapRenderState);

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	m_mapRenderTarget.clear();

	for (int i = 0; i < RG_END; ++i)
	{
		for (int j = 0; j < m_tRenderObj[i].iSize; ++j)
			SAFE_RELEASE(m_tRenderObj[i].pList[j]);

		m_tRenderObj[i].iSize = 0;
	}


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

	m_tCBuffer.ViewPortSize.x = (float)CDevice::GetInst()->GetResolution().iWidth;
	m_tCBuffer.ViewPortSize.y = (float)CDevice::GetInst()->GetResolution().iHeight;
	m_tCBuffer.isDeferred = m_bDeferred;

	D3D11_DEPTH_STENCILOP_DESC First = {};
	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_NEVER;

	D3D11_DEPTH_STENCILOP_DESC Second = {};
	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_ALWAYS; //무조건통과옵션

	//뒷면만 통과하겠다.
	CreateDepthStencilState(DEPTH_GRATOR, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_GREATER, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_READ_MASK, First, Second);

	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_EQUAL; //같을때만

	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_NEVER; //통과 X

	//앞면만 통과시키겠다.
	CreateDepthStencilState(DEPTH_LESS, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_READ_MASK, First, Second);	

	//////////////////////Blend//////////////////////////////
	AddBlendTargetDesc(TRUE);
	CreateBlendState(ALPHA_BLEND);
	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState(ACC_BLEND);

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, 0);
	CreateBlendState(ZERO_BLEND);
	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);
	CreateBlendState(ALL_BLEND);
	///////////////////////////////////////////////////////

	//////////////////////Depth//////////////////////////
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);
	//////////////////////////////////////////////////////

	CreateRasterizerState(CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateRasterizerState(FRONT_CULL, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateRasterizerState(BACK_CULL, D3D11_FILL_SOLID, D3D11_CULL_BACK);
	CreateRasterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME);

	// 포스트 이펙트용 렌더링타겟 생성
	Vector3	vPos;
	vPos.x = _RESOLUTION.iWidth - 100.f;

	if (!CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Albedo
	vPos.x = 0.f;
	if (!CreateRenderTarget("Albedo", DXGI_FORMAT_R32G32B32A32_FLOAT, Vector3::Zero, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Normal
	vPos.x = 0.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("Normal", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Depth
	vPos.x = 0.f;
	vPos.y = 200.f;
	if (!CreateRenderTarget("Depth", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Material
	vPos.x = 0.f;
	vPos.y = 300.f;
	if (!CreateRenderTarget("Material", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	CreateMultiTarget("GBuffer");
	AddMultiRenderTarget("GBuffer", "Albedo");
	AddMultiRenderTarget("GBuffer", "Normal");
	AddMultiRenderTarget("GBuffer", "Depth");
	AddMultiRenderTarget("GBuffer", "Material");

	// Light Dif
	vPos.x = 100.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("LightAccDif", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Light Spc
	vPos.x = 100.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("LightAccSpc", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	CreateMultiTarget("LightAcc");
	AddMultiRenderTarget("LightAcc", "LightAccDif");
	AddMultiRenderTarget("LightAcc", "LightAccSpc");

	// Light Blend
	vPos.x = 200.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("LightBlend", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true , Vector4::LightCyan))
		return false;

	m_pGBufferSampler = CResourcesManager::GetInst()->FindSampler(SAMPLER_POINT);
	m_pLightAccDirShader = CShaderManager::GetInst()->FindShader(LIGHT_DIR_ACC_SHADER);
	m_pLightAccPointShader = CShaderManager::GetInst()->FindShader(LIGHT_POINT_ACC_SHADER);
	m_pLightAccSpotShader = CShaderManager::GetInst()->FindShader(LIGHT_SPOT_ACC_SHADER);
	m_pLightBlendShader = CShaderManager::GetInst()->FindShader(LIGHT_BLEND_SHADER);
	m_pFullScreenShader = CShaderManager::GetInst()->FindShader(FULLSCREEN_SHADER);

	m_pAlbedoTarget = FindRenderTarget("Albedo");
	m_pLightBlendTarget = FindRenderTarget("LightBlend");
	m_pLightAccDifTarget = FindRenderTarget("LightAccDif");
	m_pLightAccSpcTarget = FindRenderTarget("LightAccSpc");

	m_pPointLightLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_LAYOUT);

	m_pSphereVolum = CResourcesManager::GetInst()->FindMeshNonCount(SPHERE_VOLUME);
	m_pCornVolum = CResourcesManager::GetInst()->FindMeshNonCount(CORN_VOLUME);

	m_pDepthGrator = FindRenderStateNonCount(DEPTH_GRATOR);
	m_pAddBlend = FindRenderStateNonCount(ACC_BLEND);
	m_pDepthLess = FindRenderStateNonCount(DEPTH_LESS);
	m_pFrontCull = FindRenderStateNonCount(FRONT_CULL);
	((CDepthState*)m_pDepthGrator)->SetStencilRef(1); //스텐실값을 1로 채운다.
	m_pBackCull = FindRenderStateNonCount(BACK_CULL);
	m_pWireFrame = FindRenderStateNonCount(WIRE_FRAME);
	m_pCullNone = FindRenderStateNonCount(CULL_NONE);
	m_pZeroBlend = FindRenderStateNonCount(ZERO_BLEND);
	m_pAllBlend = FindRenderStateNonCount(ALL_BLEND);
	m_pDepthDisable = FindRenderStateNonCount(DEPTH_DISABLE);

	m_pGBufferMultiTarget = FindMultiTarget("GBuffer");
	m_pLightMultiTarget = FindMultiTarget("LightAcc");

	CreateDepthStencilState(DEPTH_LESSEQUAL, TRUE,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL);
	return true;
}

void CRenderManager::AddBlendTargetDesc(BOOL bEnable,D3D11_BLEND srcBlend, D3D11_BLEND destBlend,D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend,D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp,UINT8 iWriteMask)
{
	if (!m_pCreateState)
		m_pCreateState = new CBlendState;

	m_pCreateState->AddTargetDesc(bEnable, srcBlend, destBlend,	blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp,	iWriteMask);
}

bool CRenderManager::CreateBlendState(const string & strName, BOOL bAlphaCoverage, BOOL bIndependent)
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

bool CRenderManager::CreateDepthStencilState(const string & strKey, BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask,D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable,UINT8 iStencilReadMask, UINT8 iStencilWriteMask,D3D11_DEPTH_STENCILOP_DESC tFrontFace,D3D11_DEPTH_STENCILOP_DESC tBackFace)
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

bool CRenderManager::CreateRasterizerState(const string & strKey, D3D11_FILL_MODE eFill, D3D11_CULL_MODE eCull, BOOL bFrontCounterClockwise, int iDepthBias, float fDepthBiasClamp, float fSlopeScaledDepthBias, BOOL bDepthClipEnable, BOOL bScissorEnable, BOOL bMultisampleEnable, BOOL bAntialiasedLineEnable)
{
	CRasterizerState*	pState = (CRasterizerState*)FindRenderState(strKey);

	if (pState)
		return false;

	pState = new CRasterizerState;

	if (!pState->CreateState(eFill, eCull,
		bFrontCounterClockwise, iDepthBias, fDepthBiasClamp, fSlopeScaledDepthBias,
		bDepthClipEnable, bScissorEnable, bMultisampleEnable, bAntialiasedLineEnable))
	{
		SAFE_RELEASE(pState);
		return false;
	}

	m_mapRenderState.insert(make_pair(strKey, pState));

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


bool CRenderManager::CreateRenderTarget(const string & strName, DXGI_FORMAT eTargetFmt,const Vector3 & vPos, const Vector3 & vScale, bool bDrawDebug,const Vector4 & vClearColor, DXGI_FORMAT eDepthFmt)
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

void CRenderManager::AddRenderObj(CGameObject * pObj)
{
	if (m_eGameMode == GM_3D)
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

	else
	{
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
	 //Sky 출력
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetSceneNonCount();

	CGameObject* pSkyObj = pScene->GetSkyObj();
	pSkyObj->Render(fTime);

	SAFE_RELEASE(pSkyObj);

	// Forward Rendering 처리
	if (!m_bDeferred)
		RenderForward(fTime);
	else
		RenderDeferred(fTime);

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}
}

void CRenderManager::RenderDeferred(float fTime)
{
	// GBuffer를 만들어준다.
	RenderGBuffer(fTime);
	// 조명 누적버퍼를 만들어준다.
	RenderLightAcc(fTime);
	// 조명타겟과 Albedo 를 합성한다.
	RenderLightBlend(fTime);
	// 최종 합성된 타겟을 화면에 출력한다.
	RenderLightFullScreen(fTime);

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

	m_pLightAccDirShader->SetShader();

	m_pAddBlend->SetState();
	m_pDepthDisable->SetState();
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

	m_pDepthDisable->ResetState();
	m_pAddBlend->ResetState();
	m_pGBufferMultiTarget->ResetShaderResource(10);

	m_pLightMultiTarget->ResetTarget();
}

void CRenderManager::RenderLightDir(float fTime, CLight * pLight)
{
	m_pLightAccDirShader->SetShader();

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
	m_pLightAccPointShader->SetShader();

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

	m_pFrontCull->SetState();
	{
		m_pDepthGrator->SetState();
		{
			m_pAllBlend->SetState();
			{
				m_pSphereVolum->Render();
			}
			m_pAllBlend->ResetState();
		}
		m_pDepthGrator->ResetState();
	}
	m_pFrontCull->ResetState();


	m_pBackCull->SetState();
	{
		m_pZeroBlend->SetState();
		{
			m_pDepthLess->SetState();
			{
				m_pSphereVolum->Render();
			}
			m_pDepthLess->ResetState();
		}
		m_pZeroBlend->ResetState();
	}
	m_pBackCull->ResetState();

	if (m_bWireFrame == false)
		return;

	m_pWireFrame->SetState();
	{
		m_pSphereVolum->Render();
	}
	m_pWireFrame->ResetState();
}

void CRenderManager::RenderLightSpot(float fTime, CLight * pLight)
{
	m_pLightAccSpotShader->SetShader();

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

	m_pFrontCull->SetState();
	{
		m_pDepthGrator->SetState();
		{
			m_pAllBlend->SetState();
			{
				m_pCornVolum->Render();
			}
			m_pAllBlend->ResetState();
		}
		m_pDepthGrator->ResetState();
	}
	m_pFrontCull->ResetState();

	m_pBackCull->SetState();
	{
		m_pZeroBlend->SetState();
		{
			m_pDepthLess->SetState();
			{
				m_pCornVolum->Render();
			}
			m_pDepthLess->ResetState();
		}
		m_pZeroBlend->ResetState();
	}
	m_pBackCull->ResetState();

	if (m_bWireFrame == false)
		return;

	m_pWireFrame->SetState();
	{
		m_pCornVolum->Render();
	}
	m_pWireFrame->ResetState();
}

void CRenderManager::RenderLightBlend(float _fTime)
{
	m_pLightBlendTarget->ClearTarget();
	m_pLightBlendTarget->SetTarget();

	m_pDepthDisable->SetState();

	m_pGBufferSampler->SetShader(10);
	m_pAlbedoTarget->SetShader(10);

	m_pLightAccDifTarget->SetShader(14);
	m_pLightAccSpcTarget->SetShader(15);

	m_pLightBlendShader->SetShader();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pAlbedoTarget->ResetShader(10);
	m_pLightAccDifTarget->ResetShader(14);
	m_pLightAccSpcTarget->ResetShader(15);

	m_pDepthDisable->ResetState();
	m_pLightBlendTarget->ResetTarget();
}

void CRenderManager::RenderLightFullScreen(float _fTime)
{
	m_pDepthDisable->SetState();

	m_pLightBlendTarget->SetShader(0);
	m_pFullScreenShader->SetShader();
	m_pGBufferSampler->SetShader(0);

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CDevice::GetInst()->GetContext()->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_pDepthDisable->ResetState();
	m_pLightBlendTarget->ResetShader(0);
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

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
		iter->second->Render(fTime);

	pAlphaBlend->ResetState();

	SAFE_RELEASE(pAlphaBlend);
}

bool CRenderManager::CreateMultiTarget(const string & MultiKey)
{
	CMultiRenderTarget* newTarget = FindMultiTarget(MultiKey);

	if (newTarget != NULLPTR)
		return false;

	newTarget = new CMultiRenderTarget();

	m_mapMultiTarget.insert(make_pair(MultiKey, newTarget));
	return true;
}

bool CRenderManager::AddMultiRenderTarget(const string & MultiKey, const string & TargetKey)
{
	CMultiRenderTarget* getMulti = FindMultiTarget(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddRenderTargetView(TargetKey);
	return true;
}

bool CRenderManager::AddMultiRenderTargetDepthView(const string & MultiKey, const string & TargetKey)
{
	CMultiRenderTarget* getMulti = FindMultiTarget(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddDepthView(TargetKey);
	return true;
}

CMultiRenderTarget * CRenderManager::FindMultiTarget(const string & MultiKey)
{
	unordered_map<string, CMultiRenderTarget*>::iterator FindIter = m_mapMultiTarget.find(MultiKey);

	if (FindIter == m_mapMultiTarget.end())
		return NULLPTR;

	return FindIter->second;
}