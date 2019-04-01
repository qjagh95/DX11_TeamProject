#include "../EngineHeader.h"
#include "ViewManager.h"
#include "UAV.h"
#include "CSBlur.h"
#include "DownScale.h"
#include "BlendState.h"
#include "DepthState.h"
#include "RenderState.h"
#include "CSHDRFilter.h"
#include "RenderTarget.h"
#include "RasterizerState.h"
#include "MotionBlurFilter.h"
#include "MultiRenderTarget.h"
#include "../Device.h"

PUN_USING

DEFINITION_SINGLE(CViewManager)

CViewManager::CViewManager()	:
	m_pCreateState(nullptr)
{
}

CViewManager::~CViewManager()
{
	Safe_Delete_Map(m_mapUAV);
	Safe_Delete_Map(m_mapMultiTarget);
	Safe_Delete_Map(m_mapRenderState);

	unordered_map<string, CRenderState*>::iterator iter2;
	unordered_map<string, CRenderState*>::iterator iter2End = m_mapRenderState.end();

	for (iter2 = m_mapRenderState.begin(); iter2 != iter2End; ++iter2)
	{
		SAFE_DELETE(iter2->second);
	}

	m_mapRenderState.clear();

	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	m_mapRenderTarget.clear();

	unordered_map<CS_FILTER_TYPE, CCSFilter*>::iterator	iter1;
	unordered_map<CS_FILTER_TYPE, CCSFilter*>::iterator	iterEnd1 = m_mapCSFilter.end();

	for (iter1 = m_mapCSFilter.begin(); iter1 != iterEnd1; ++iter1)
		SAFE_DELETE(iter1->second);

	m_mapCSFilter.clear();	
}

bool CViewManager::Init()
{
	if (!CreateState())
		return false;

	if (!CreateRenderTargetView())
		return false;
	
	if (!CreateUAV())
		return false;

	if (!CreateFilter())
		return false;

	return true;
}

void CViewManager::AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend, 
	D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend, 
	D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp, UINT8 iWriteMask)
{
	if (!m_pCreateState)
		m_pCreateState = new CBlendState;

	m_pCreateState->AddTargetDesc(bEnable, srcBlend, destBlend, blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp, iWriteMask);
}

bool CViewManager::CreateBlendState(const string & strName, BOOL bAlphaCoverage, BOOL bIndependent)
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

bool CViewManager::CreateDepthStencilState(const string & strKey, BOOL bDepthEnable, 
	D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable, 
	UINT8 iStencilReadMask, UINT8 iStencilWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontFace, 
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

bool CViewManager::CreateRasterizerState(const string & strKey, D3D11_FILL_MODE eFill, 
	D3D11_CULL_MODE eCull, BOOL bFrontCounterClockwise, int iDepthBias, float fDepthBiasClamp, 
	float fSlopeScaledDepthBias, BOOL bDepthClipEnable, BOOL bScissorEnable, 
	BOOL bMultisampleEnable, BOOL bAntialiasedLineEnable)
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

bool CViewManager::CreateRenderTarget(const string & strName, DXGI_FORMAT eTargetFmt, 
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

bool CViewManager::CreateMultiTarget(const string & MultiKey)
{
	CMultiRenderTarget* newTarget = FindMRT(MultiKey);

	if (newTarget != NULLPTR)
		return false;

	newTarget = new CMultiRenderTarget();

	m_mapMultiTarget.insert(make_pair(MultiKey, newTarget));
	return true;
}

bool CViewManager::AddMultiRenderTarget(const string & MultiKey, const string & TargetKey)
{
	CMultiRenderTarget* getMulti = FindMRT(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddRenderTargetView(TargetKey);
	return true;
}

bool CViewManager::AddMultiRenderTargetDepthView(const string & MultiKey, const string & TargetKey)
{
	CMultiRenderTarget* getMulti = FindMRT(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddDepthView(TargetKey);

	return true;
}

bool CViewManager::CreateUAV(const string & strName, const string & strShaderKey,
	int iNumX, int iNumY, int iNumZ, int iWidth, int iHeight,
	DXGI_FORMAT eFormat, GPU_RESOURCE_TYPE eType)
{
	CUAV* pUAV = new CUAV;
	
	int ResourceWidth = iWidth;
	int ResourceHeight = iHeight;

	if (iWidth == 0)
		ResourceWidth = _RESOLUTION.iWidth;
	if (iHeight == 0)
		ResourceHeight = _RESOLUTION.iHeight;

	if (!pUAV->CreateView(strName, strShaderKey, eType, iNumX, iNumY, iNumZ,
		ResourceWidth, ResourceHeight, eFormat))
		return false;

	m_mapUAV.insert(make_pair(strName, pUAV));

	return true;
}

CRenderState* CViewManager::FindRenderState(const string & strStateName)
{
	unordered_map<string, CRenderState*>::iterator iter = m_mapRenderState.find(strStateName);

	if (iter == m_mapRenderState.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CRenderState * CViewManager::FindRenderStateNonCount(const string & strStateName)
{
	unordered_map<string, CRenderState*>::iterator iter = m_mapRenderState.find(strStateName);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}

CRenderTarget* CViewManager::FindRenderTarget(const string & strTargetName)
{
	unordered_map<string, CRenderTarget*>::iterator iter = m_mapRenderTarget.find(strTargetName);

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

CMultiRenderTarget* CViewManager::FindMRT(const string & strMRTName)
{
	unordered_map<string, CMultiRenderTarget*>::iterator iter = m_mapMultiTarget.find(strMRTName);

	if (iter == m_mapMultiTarget.end())
		return nullptr;

	return iter->second;
}

CCSFilter* CViewManager::FindCSFilter(CS_FILTER_TYPE eType)
{
	unordered_map<CS_FILTER_TYPE, CCSFilter*>::iterator iter = m_mapCSFilter.find(eType);

	if (iter == m_mapCSFilter.end())
		return nullptr;

	return iter->second;
}

CUAV* CViewManager::FindUAV(const string & strUAVName)
{
	unordered_map<string, CUAV*>::iterator iter = m_mapUAV.find(strUAVName);

	if (iter == m_mapUAV.end())
		return nullptr;

	return iter->second;
}

bool CViewManager::CreateUAV()
{
	Resolution tRS = _RESOLUTION;

	if (!CreateUAV("DownScale2x2", DOWNSCALE_SHADER, tRS.iWidth / 8, tRS.iHeight / 8, 1, 640, 360))
		return false;

	if (!CreateUAV("DownScale4x4", DOWNSCALE_SHADER, tRS.iWidth / 8, tRS.iHeight / 8, 1, 320, 180))
		return false;

	if (!CreateUAV("DownScale8x8", DOWNSCALE_SHADER, tRS.iWidth / 8, tRS.iHeight / 8, 1, 160, 90))
		return false;	

	if (!CreateUAV("HorizontalBlur", HORIZONTAL_BLUR_SHADER, 1, tRS.iHeight / 2, 1, tRS.iWidth / 2, tRS.iHeight / 2))
		return false;

	if (!CreateUAV("VerticalBlur", VERTICAL_BLUR_SHADER, tRS.iWidth / 2, 1, 1, tRS.iWidth / 2, tRS.iHeight / 2))
		return false;

	if (!CreateUAV("MotionBlur", MOTION_BLUR_SHADER, tRS.iWidth / 10, tRS.iHeight / 10, 1, tRS.iWidth, tRS.iHeight))
		return false;

	if (!CreateUAV("HDRFirstPass", HDR_COMPUTE_SHADER, 57, 1, 1, 57, 1, DXGI_FORMAT_R32_FLOAT, GRT_BUFFER))
		return false;
	
	if (!CreateUAV("HDRSecondPass", HDR_SECOND_COMPUTE_SHADER, 1, 1, 1, 1, 1, DXGI_FORMAT_R32_FLOAT, GRT_BUFFER))
	return false;

	return true;
}

bool CViewManager::CreateState()
{
	D3D11_DEPTH_STENCILOP_DESC First = {};
	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_NEVER;

	D3D11_DEPTH_STENCILOP_DESC Second = {};
	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_ALWAYS; //����������ɼ�

	//�޸鸸 ����ϰڴ�.
	if (!CreateDepthStencilState(DEPTH_GRATOR, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_COMPARISON_GREATER, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK,
		D3D11_DEFAULT_STENCIL_READ_MASK, First, Second))
		return false;

	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_EQUAL; //��������

	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_NEVER; //��� X

	if (!CreateDepthStencilState(DEPTH_LESS, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, 
		D3D11_COMPARISON_LESS, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, 
		D3D11_DEFAULT_STENCIL_READ_MASK, First, Second))
		return false;

	if (!CreateDepthStencilState(DEPTH_LESSEQUAL, TRUE,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL))
		return false;

	//////////////////////Blend//////////////////////////////
	AddBlendTargetDesc(TRUE);
	if (!CreateBlendState(ALPHA_BLEND))
		return false;

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	if (!CreateBlendState(ACC_BLEND))
		return false;

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, 0);
	if (!CreateBlendState(ZERO_BLEND))
		return false;

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);
	if (!CreateBlendState(ALL_BLEND))
		return false;
	///////////////////////////////////////////////////////

	//////////////////////Depth//////////////////////////
	if (!CreateDepthStencilState(DEPTH_DISABLE, FALSE))
		return false;
	//////////////////////////////////////////////////////

	if(!CreateRasterizerState(CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE))
		return false;
	if(!CreateRasterizerState(FRONT_CULL, D3D11_FILL_SOLID, D3D11_CULL_FRONT))
		return false;
	if(!CreateRasterizerState(BACK_CULL, D3D11_FILL_SOLID, D3D11_CULL_BACK))
		return false;
	if(!CreateRasterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME))
		return false;

	

	return true;
}

bool CViewManager::CreateRenderTargetView()
{// ����Ʈ ����Ʈ�� ������Ÿ�� ����
	Vector3	vPos;
	vPos.x = _RESOLUTION.iWidth - 100.f;

	//if (!CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM, vPos, Vector3(100.f, 100.f, 1.f), true))
	//	return false;

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

	// Tangent
	vPos.x = 0.f;
	vPos.y = 400.f;
	if (!CreateRenderTarget("Tangent", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	// Binormal
	vPos.x = 0.f;
	vPos.y = 500.f;
	if (!CreateRenderTarget("Binormal", DXGI_FORMAT_R32G32B32A32_FLOAT,
		vPos, Vector3(100.f, 100.f, 1.f), true))
		return false;

	CreateMultiTarget("GBuffer");
	AddMultiRenderTarget("GBuffer", "Albedo");
	AddMultiRenderTarget("GBuffer", "Normal");
	AddMultiRenderTarget("GBuffer", "Depth");
	AddMultiRenderTarget("GBuffer", "Material");
	AddMultiRenderTarget("GBuffer", "Tangent");
	AddMultiRenderTarget("GBuffer", "Binormal");

	CreateMultiTarget("SSDBuffer");
	AddMultiRenderTarget("SSDBuffer", "Albedo");
	AddMultiRenderTarget("SSDBuffer", "Normal");
	AddMultiRenderTarget("SSDBuffer", "Material");

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

	if (!CreateMultiTarget("LightAcc"))
		return false;

	AddMultiRenderTarget("LightAcc", "LightAccDif");
	AddMultiRenderTarget("LightAcc", "LightAccSpc");

	vPos.x = 100.f;
	vPos.y = 200.f;
	if (!CreateRenderTarget("VolumeFogDepth", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true, Vector4::LightCyan))
		return false;

	// Light Blend
	vPos.x = 200.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("LightBlend", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true, Vector4::LightCyan))
		return false;

	vPos.x = 300.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("Final", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true, Vector4::LightCyan))
		return false;

	vPos.x = 300.f;
	vPos.y = 100.f;
	if (!CreateRenderTarget("Sky", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true, Vector4::LightCyan))
		return false;

	vPos.x = 400.f;
	vPos.y = 0.f;
	if (!CreateRenderTarget("SecondBackBuffer", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.f, 100.f, 1.f), true, Vector4::LightCyan))
		return false;

	return true;
}

bool CViewManager::CreateFilter()
{
	CDownScale* pDownScale = CreateFilter<CDownScale>(CFT_DOWNSCALE);

	if (!pDownScale)
		return false;

	pDownScale->AddUAV("DownScale2x2");
	pDownScale->AddUAV("DownScale4x4");
	pDownScale->AddUAV("DownScale8x8");
	
	CCSBlur* pBlur = CreateFilter<CCSBlur>(CFT_BLUR);

	if (!pBlur)
		return false;

	pBlur->AddUAV("HorizontalBlur");
	pBlur->AddUAV("VerticalBlur");

	CMotionBlurFilter* pMBFilter = CreateFilter<CMotionBlurFilter>(CFT_MOTIONBLUR);

	if (!pMBFilter)
		return false;

	pMBFilter->AddUAV("MotionBlur");
	
	CCSHDRFilter* pHDR = CreateFilter<CCSHDRFilter>(CFT_HDR);

	if (!pHDR)
		return false;

	pHDR->AddUAV("HDRFirstPass");
	pHDR->AddUAV("HDRSecondPass");


	return true;
}

void CViewManager::Render(float fTime)
{
	unordered_map<string, CRenderTarget*>::iterator	iter;
	unordered_map<string, CRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}
}