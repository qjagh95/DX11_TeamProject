#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CStage1Scene
	:public CSceneComponent
{
public:
	CStage1Scene();
	~CStage1Scene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};

