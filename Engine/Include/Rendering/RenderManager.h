#pragma once

#include "../Engine.h"

PUN_BEGIN

typedef struct _tagRenderGroup
{
	class CGameObject**	pList;
	int					iSize;
	int					iCapacity;

	_tagRenderGroup()
	{
		iSize = 0;
		iCapacity = 100;
		pList = new CGameObject*[iCapacity];
	}

	~_tagRenderGroup() { delete[]	pList; }
}RenderGroup, *PRenderGroup;

class CMesh;
class CShader;
class CRenderTarget;
class CSampler;
class CRenderState;
class CBlendState;
class CMultiRenderTarget;
class PUN_DLL CRenderManager
{
private:
	unordered_map<string, CRenderState*> m_mapRenderState;
	unordered_map<string, CRenderTarget*> m_mapRenderTarget;
	unordered_map<string, CMultiRenderTarget*> m_mapMultiTarget;

	RenderGroup	m_tRenderObj[RG_END];
	RenderGroup	m_tLightGroup;
	GAME_MODE m_eGameMode;
	bool m_bDeferred;
	ID3D11InputLayout* m_pPointLightLayout;
	CBlendState* m_pCreateState;
	CRenderState*	m_pDepthDisable;
	CRenderState*	m_pCullNone;
	CSampler* m_pGBufferSampler;
	CShader* m_pLightAccDirShader;
	CShader* m_pLightAccPointShader;
	CShader* m_pLightAccSpotShader;
	CShader* m_pLightBlendShader;
	CShader* m_pFullScreenShader;
	CRenderTarget* m_pAlbedoTarget;
	CRenderTarget* m_pLightBlendTarget;
	CRenderTarget* m_pLightAccDifTarget;
	CRenderTarget* m_pLightAccSpcTarget;

	CRenderState* m_pAddBlend;
	CRenderState* m_pDepthGrator;
	CRenderState* m_pDepthLess;
	CRenderState* m_pFrontCull;
	CRenderState* m_pBackCull;
	CRenderState* m_pWireFrame;
	CRenderState* m_pZeroBlend;
	CRenderState* m_pAllBlend;

	CMultiRenderTarget* m_pGBufferMultiTarget;
	CMultiRenderTarget* m_pLightMultiTarget;

	bool m_bWireFrame;
	CMesh* m_pSphereVolum;
	CMesh* m_pCornVolum;

	PublicCBuffer m_tCBuffer;

public:
	GAME_MODE GetGameMode()	const;
	bool GetRenderingMode()	const;
public:
	void SetGameMode(GAME_MODE eMode);
	void EnableDeferredRendering();
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

	class CRenderState* FindRenderState(const string& strName);

	class CRenderState * FindRenderStateNonCount(const string & strName);

	bool CreateRenderTarget(const string& strName, DXGI_FORMAT eTargetFmt,
		const Vector3& vPos, const Vector3& vScale,
		bool bDrawDebug = true, const Vector4& vClearColor = Vector4(0.f, 0.f, 0.f, 0.f),
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);

	class CRenderTarget* FindRenderTarget(const string& strName);

	bool CreateMultiTarget(const string& MultiKey);
	bool AddMultiRenderTarget(const string& MultiKey, const string& TargetKey);
	bool AddMultiRenderTargetDepthView(const string& MultiKey, const string& TargetKey);

	CMultiRenderTarget* FindMultiTarget(const string& MultiKey);

public:
	void AddRenderObj(class CGameObject* pObj);
	void Render(float fTime);

private:
	void Render2D(float fTime);
	void Render3D(float fTime);
	void RenderForward(float fTime);
	void RenderDeferred(float fTime);
	void RenderGBuffer(float fTime);
	void RenderLightAcc(float fTime);
	void RenderLightDir(float fTime, class CLight* pLight);
	void RenderLightPoint(float fTime, class CLight* pLight);
	void RenderLightSpot(float fTime, class CLight* pLight);
	void RenderLightBlend(float _fTime);
	void RenderLightFullScreen(float _fTime);

	DECLARE_SINGLE(CRenderManager)
};

PUN_END