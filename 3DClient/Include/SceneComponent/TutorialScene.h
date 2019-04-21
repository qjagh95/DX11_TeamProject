#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CTutorialScene :
	public CSceneComponent
{
public:
	CTutorialScene();
	~CTutorialScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};

