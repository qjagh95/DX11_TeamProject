#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CMainScene :
	public CSceneComponent
{
private:
	CTransform*	m_pTr;

public:
	CMainScene();
	~CMainScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
public:
	void Move(float fScale, float fTime);
private:
	void CreatePrototype();

public:
};

