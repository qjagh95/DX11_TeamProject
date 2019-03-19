#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CEditScene : public CSceneComponent
{
public:
	CEditScene();
	~CEditScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};

