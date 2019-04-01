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

class PUN_DLL CRenderManager
{
private:
	RenderGroup	m_tRenderObj[RG_END];
	RenderGroup	m_tLightGroup;

	GAME_MODE m_eGameMode;
	bool m_bDeferred;
	bool m_bFogEnable;
	ID3D11InputLayout* m_pPointLightLayout;
	CBlendState* m_pCreateState;
	
	CSampler*			m_pGBufferSampler;

	CShader*			m_pShader[SHADER_END];
	CRenderTarget*		m_pTarget[TARGET_END];
	CRenderState*		m_pState[STATE_END];
	CCSFilter*			m_pFilter[CFT_END];

	CMultiRenderTarget* m_pGBufferMultiTarget;
	CMultiRenderTarget* m_pLightMultiTarget;
	CMultiRenderTarget* m_pDecalMultiTarget;

	CMesh*				m_pSphereVolum;
	CMesh*				m_pCornVolum;

	CGameObject*		m_pSkyObj;

	PublicCBuffer		m_tCBuffer;
	FinalPassCB			m_tFinalCBuffer;


public:
	GAME_MODE GetGameMode()	const;
	bool GetRenderingMode()	const;
	bool m_bWireFrame;

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

	DECLARE_SINGLE(CRenderManager)
};

PUN_END

//체비 쇼프 부등식 공부 해보자.