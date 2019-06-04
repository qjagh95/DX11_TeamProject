#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CLogoScene
	:public CSceneComponent
{
private:
	float	m_fChangeTime;
	bool	m_bNextScene;
	bool	m_bStart;
	CLayer*	m_pUILayer;
	CTransform*	m_pPMsgTr;
	CGameObject*	m_pPlayerObj;
	CGameObject*	m_pSButtonObj;
	CGameObject*	m_pCButtonObj;
	CGameObject*	m_pEButtonObj;
	CGameObject*	m_pMsgObj;

public:
	virtual void AfterInit();
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void NextScene(float fTime);
	void StartBtnCallback(float fTime);
	void Continue(float fTime);
	void Exit(float fTime);

public:
	CLogoScene();
	~CLogoScene();
};

