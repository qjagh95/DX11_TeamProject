#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CTestSceneYH
	:public CSceneComponent
{
private:
	CTransform*	m_pTr;
	CLayer*		m_pUILayer;
	CTransform*	m_pGhostTr;
	bool		m_bGhostOn;
	bool		m_bIncrease;
	float		m_fPlayTime;
	float		m_fX;
	float		m_fY;

public:
	virtual bool Init();
	virtual int Update(float fTime);
public:
	void Move(float fScale, float fTime);
private:
	void CreatePrototype();

public:
	void NextScene(float fTime);
public:
	CTestSceneYH();
	~CTestSceneYH();
};

