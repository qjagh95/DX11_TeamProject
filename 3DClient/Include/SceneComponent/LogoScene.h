#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CLogoScene
	:public CSceneComponent
{
private:
	float m_fChangeTime;
public:
	virtual bool Init();
	virtual int Update(float fTime);
private:
	void CreatePrototype();
public:
	void NextScene(float fTime);
public:
	CLogoScene();
	~CLogoScene();
};

