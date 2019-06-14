#pragma once
#include "Scene/SceneComponent.h"
#include "Component/ColliderOBB3D.h"

PUN_USING

class CStage2Scene
	:public CSceneComponent
{
public:
	CStage2Scene();
	~CStage2Scene();

private:
	float	m_fFade;

	vector<CGameObject*> m_vecRealObj;
	vector<CGameObject*> m_vecIllusionObj;

	bool m_bStart;

public:
	virtual bool Init();
	virtual void AfterInit();
	virtual int Update(float fTime);

	void InteractIllusion(CCollider* pSrc, CCollider* pDest, float fTime);
	void InteractReal(CCollider* pSrc, CCollider* pDest, float fTime);
};

