#pragma once
#include "EngineHeader.h"
#include "Scene/SceneComponent.h"

PUN_USING

class CTrdTestScene
	:public CSceneComponent
{
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
	CTrdTestScene();
	~CTrdTestScene();
};

