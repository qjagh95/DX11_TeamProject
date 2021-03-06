#pragma once

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
class CCSFilter;
class CTexture;
class PUN_DLL CRenderManager
{
private:
	RenderGroup					m_tRenderObj[RG_END];
	RenderGroup					m_tLightGroup;
	RenderGroup					m_tGizmoGroup;

	GAME_MODE					m_eGameMode;
	bool						m_bDeferred;
	bool						m_bFogEnable;
	bool						m_isGetCameraObj;
	int							m_iScopeFlag;
	float						m_accShakeTime;
	CTexture*					m_pRandomNormalTex;
	CTexture*					m_pNoiseTex;
	ID3D11Texture2D*			m_pFogDepthTex;
	ID3D11ShaderResourceView*	m_pFogDepthSRV;
	ID3D11InputLayout*			m_pPointLightLayout;
	CSampler*					m_pPointSmp;
	CBlendState*				m_pCreateState;
	
	CSampler*					m_pGBufferSampler;

	CShader*					m_pShader[SHADER_END];
	CRenderTarget*				m_pTarget[TARGET_END];
	CRenderState*				m_pState[STATE_END];
	CCSFilter*					m_pFilter[CFT_END];

	CMultiRenderTarget*			m_pGBufferMultiTarget;
	CMultiRenderTarget*			m_pLightMultiTarget;
	CMultiRenderTarget*			m_pDecalMultiTarget;
	CMultiRenderTarget*			m_pDownScaledGBufferMultiTarget;

	CMesh*						m_pSphereVolum;
	CMesh*						m_pCornVolum;

	CGameObject*				m_pSkyObj;

	PublicCBuffer				m_tCBuffer;
	FinalPassCB					m_tFinalCBuffer;
	StarLightScopeCB			m_tStarLightScope;
	DepthFogCBuffer				m_tDepthFogCBuffer;

	// Magic Number 찾는 용도의 변수들
	class CPostEffect*	m_pPostEffect;
	bool				m_bMagic;
	bool				m_bHDR;
	bool				m_bHDROn;
	bool				m_bAdaptation;
	bool				m_bAdaptationOn;
	bool				m_bBloom;
	bool				m_bBloomOn;
	bool				m_bBlur;
	bool				m_bBlurOn;
	bool				m_bMotionBlur;
	bool				m_bMotionBlurOn;
	bool				m_bSLC;
	bool				m_bSSAOEnable;
	bool				m_bSSAOEnableOn;
	bool				m_bDepthFog;
	bool				m_bDepthFogOn;
	bool				m_AdaptOn;
	bool				m_bDOF;
	bool				m_bDistortion;
	float				m_fMiddleGrey;
	float				m_fLumWhite;
	float				m_fAdaptation;
	float				m_fBloomThreshold;
	float				m_fBloomScale;
	float				m_fBlurDensity;
	Vector3				m_vRimColor;
	float				m_fRimColor;
	Vector4				m_vDepthFogColor;
	float				m_fDepthFogStart;
	float				m_fDepthFogEnd;
	float				m_fOnOff;
	float				m_fDofEnd;
	int					m_iNumPixel;

	float				m_fWaveLimitTime;
	float				m_fTime;

public:
	GAME_MODE GetGameMode()	const;
	bool GetRenderingMode()	const;
	bool m_bLightWireFrame;

public:
	void SetGameMode(GAME_MODE eMode);
	void EnableDeferredRendering();
	bool SetSkyObject(CGameObject* pObj);
	bool Init();

	class CRenderState* FindRenderState(const string& strName);
	class CRenderState * FindRenderStateNonCount(const string & strName);
	class CRenderTarget* FindRenderTarget(const string& strName);
	CMultiRenderTarget* FindMultiTarget(const string& MultiKey);

	void EnableFilter(CS_FILTER_TYPE eType);
	void DisableFilter(CS_FILTER_TYPE eType);
	void SetStarLightScope(int _flag);
	void SetOnOff(float iOnOff);

public:
	void SetHDRValue(float fMiddleGrey, float fLumWhite, bool bOnOff = true);
	void SetHDRValue(float fMiddleGrey, float fLumWhite, float fTime, bool bOnOff = true);
	void SetAdaptValue(float fAdaptation, bool bOnOff = false);
	void SetAdaptValue(float fAdaptation, float fTime, bool bOnOff = false);
	void SetBloomValue(float fBloomScale, bool bOnOff = false);
	void SetBloomValue(float fBloomScale, float fTime, bool bOnOff = false);
	void SetFadeAmount(float fFadeAmount, float fTime);
	void SetDepthFog(bool bOnOff, Vector4 vFogColor = Vector4::Black, float fStartDepth = 0.f, float fEndDepth = 0.f);
	void SetDepthFog(bool bOnOff, float r, float g, float b, float fStartDepth = 0.f, float fEndDepth = 0.f);

public:
	void AddRenderObj(class CGameObject* pObj);
	void Render(float fTime);

private:
	void Render2D(float fTime);
	void Render3D(float fTime);

	void RenderForward(float fTime);
	void RenderDeferred(float fTime);

	void RenderGBuffer(float fTime);
	void RenderDecal(float fTime);
	void DownScaleGBuffer(float fTime);
	void RenderSSAO(float fTime);
	void RenderLightAcc(float fTime);
	void RenderLightDir(float fTime, class CLight* pLight);
	void RenderLightPoint(float fTime, class CLight* pLight);
	void RenderLightSpot(float fTime, class CLight* pLight);
	void RenderLightBlend(float _fTime);
	void RenderSkyObj(float _fTime);
	void RenderSkyObj(FAVORITE_TARGET eTarget, float fTime);
	void RenderFog(float _fTime);
	void RenderComputeProcess(float fTime);
	void RenderFinalPass(float _fTime);
	void RenderFinalPassDebug(float _fTime);
	void RenderShadowMap(float fTime);
	void RenderShadowTexture(float fTime);
	void DownScaleShadowTexture(float fTime);

	void RenderNaviEditorMode(float fTime);

public:
	void FindMagicNumber(float fTime);

	void SetMotionBlur(bool bEnable, int iMotionBlur);
	void SetSkyEnable(bool bEnable);

	void EnableDistortion();
	void Distortion(float fTime);

	DECLARE_SINGLE(CRenderManager)
};

PUN_END

//체비 쇼프 부등식 공부 해보자.