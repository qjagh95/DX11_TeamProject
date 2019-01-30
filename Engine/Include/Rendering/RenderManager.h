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

	~_tagRenderGroup()
	{
		delete[]	pList;
	}
}RenderGroup, *PRenderGroup;

typedef struct PUN_DLL _tagMRT
{
	vector<class CRenderTarget*>	vecTarget;
	vector<ID3D11RenderTargetView*>	vecOldTarget;
	ID3D11DepthStencilView*	pDepth;
	ID3D11DepthStencilView*	pOldDepth;
}MRT, *PMRT;

class PUN_DLL CRenderManager
{
private:
	unordered_map<string, class CRenderState*>	m_mapRenderState;
	unordered_map<string, class CRenderTarget*>	m_mapRenderTarget;
	std::unordered_map<std::string, PMRT> m_mapMRT;
	RenderGroup			m_tRenderObj[RG_END];
	RenderGroup			m_tLightGroup;
	class CBlendState*	m_pCreateState;
	GAME_MODE			m_eGameMode;
	bool				m_bDeferred;
	class CRenderState* m_pDepthDisable;
	class CSampler*     m_pGBufferSampler;
	class CShader*		m_pLightAccDirShader;
	class CShader*		m_pLightAccPointShader;
	class CShader*		m_pLightAccSpotShader;
	class CShader*      m_pLightBlendShader;
	class CShader*		m_pFullScreenShader;
	class CMesh*		m_pPointLightVolume;                                            
	class CRenderTarget* m_pAlbedoTarget;
	class CRenderTarget* m_pLightBlendTarget;
	class CRenderTarget* m_pLightAccDifTarget;
	class CRenderTarget* m_pLightAccSpcTarget;

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


	class CRenderState* FindRenderState(const string& strName);

	class CRenderState * FindRenderStateNonCount(const string & strName);

	bool CreateRenderTarget(const string& strName, DXGI_FORMAT eTargetFmt,
		const Vector3& vPos, const Vector3& vScale,
		bool bDrawDebug = true, const Vector4& vClearColor = Vector4(0.f, 0.f, 0.f, 0.f),
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);

	class CRenderTarget* FindRenderTarget(const string& strName);
	
	bool AddMRT(const string& strMRTKey, const string& strTargetKey);
	bool AddMRTDepth(const string& strMRTKey, const string& strTargetKey);
	void ClearMRT(const string& strMRTKey, float fClearColor[4]);
	void SetMRT(const string& strMRTKey);
	void ResetMRT(const string& strMRTKey);
	PMRT FindMRT(const string& strMRTKey);

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