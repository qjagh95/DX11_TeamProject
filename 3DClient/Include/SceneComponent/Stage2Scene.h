#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CStage2Scene
	:public CSceneComponent
{
public:
	CStage2Scene();
	~CStage2Scene();

private:
	float	m_fFade;

public:
	virtual bool Init();
	virtual void AfterInit();
	virtual int Update(float fTime);
};

