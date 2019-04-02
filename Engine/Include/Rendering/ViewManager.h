#pragma once

PUN_BEGIN

class CRenderState;
class CRenderTarget;
class CMultiRenderTarget;
class CUAV;
class CCSFilter;

class PUN_DLL CViewManager
{
private:
	CBlendState*	m_pCreateState;

	unordered_map<string, CUAV*>				m_mapUAV;
	unordered_map<string, CRenderState*>		m_mapRenderState;
	unordered_map<string, CRenderTarget*>		m_mapRenderTarget;
	unordered_map<string, CMultiRenderTarget*>	m_mapMultiTarget;
	unordered_map<CS_FILTER_TYPE, CCSFilter*>	m_mapCSFilter;

public:
	bool Init();

	void AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphaBlend = D3D11_BLEND_ONE,
		D3D11_BLEND destAlphaBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

	bool CreateBlendState(const string& strName, BOOL bAlphaCoverage = FALSE,
		BOOL bIndependent = FALSE);

	bool CreateDepthStencilState(const string& strKey, BOOL bDepthEnable,
		D3D11_DEPTH_WRITE_MASK eMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		BOOL bStencilEnable = FALSE,
		UINT8 iStencilReadMask = 0, UINT8 iStencilWriteMask = 0,
		D3D11_DEPTH_STENCILOP_DESC tFrontFace = {},
		D3D11_DEPTH_STENCILOP_DESC tBackFace = {});

	bool CreateRasterizerState(const string& strKey, D3D11_FILL_MODE eFill = D3D11_FILL_SOLID, D3D11_CULL_MODE eCull = D3D11_CULL_BACK,
		BOOL bFrontCounterClockwise = FALSE, int iDepthBias = 0,
		float fDepthBiasClamp = 0.f, float fSlopeScaledDepthBias = 0.f,
		BOOL bDepthClipEnable = TRUE, BOOL bScissorEnable = FALSE,
		BOOL bMultisampleEnable = FALSE, BOOL bAntialiasedLineEnable = FALSE);

	bool CreateRenderTarget(const string& strName, DXGI_FORMAT eTargetFmt,
		const Vector3& vPos, const Vector3& vScale,
		const Resolution& tRS,
		bool bDrawDebug = true, const Vector4& vClearColor = Vector4(0.f, 0.f, 0.f, 0.f),
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);

	bool CreateMultiTarget(const string& MultiKey);
	bool AddMultiRenderTarget(const string& MultiKey, const string& TargetKey);
	bool AddMultiRenderTargetDepthView(const string& MultiKey, const string& TargetKey);

	bool CreateUAV(const string& strName, const string& strShaderKey,
		int iNumX, int iNumY, int iNumZ, int iWidth = 0, int iHeight = 0,
		DXGI_FORMAT eFormat = DXGI_FORMAT_R32G32B32A32_FLOAT, 
		GPU_RESOURCE_TYPE eType = GRT_TEXTURE2D);

	CUAV* FindUAV(const string& strUAVName);
	CRenderState* FindRenderState(const string& strStateName);
	CRenderState* FindRenderStateNonCount(const string& strStateName);
	CRenderTarget* FindRenderTarget(const string& strTargetName);
	CMultiRenderTarget* FindMRT(const string& strMRTName);
	CCSFilter* FindCSFilter(CS_FILTER_TYPE eType);

	bool CreateUAV();
	bool CreateState();
	bool CreateRenderTargetView();
	bool CreateFilter();

	void Render(float fTime);

	template <typename T>
	T* CreateFilter(CS_FILTER_TYPE eType)
	{
		T* pFilter = new T;

		if (!pFilter->Init())
			return nullptr;
		
		m_mapCSFilter.insert(make_pair(eType, pFilter));

		return pFilter;
	}

	DECLARE_SINGLE(CViewManager)
};

PUN_END