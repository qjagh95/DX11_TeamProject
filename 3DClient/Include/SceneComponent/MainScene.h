#pragma once

#include "Scene/SceneComponent.h"
#include "Component/Stage2D.h"

PUN_USING

class CMainScene :
	public CSceneComponent
{
public:
	CMainScene();
	~CMainScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);

private:
	void CreatePrototype();

public:
	void NextScene(float fTime);
};

