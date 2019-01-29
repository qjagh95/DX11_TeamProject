#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CStartScene :
	public CSceneComponent
{
public:
	CStartScene();
	~CStartScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void StartBtnCallback(float fTime);
};

