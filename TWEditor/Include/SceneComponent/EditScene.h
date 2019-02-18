#pragma once

#include "Scene/SceneComponent.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

PUN_USING

class CEditScene :
	public CSceneComponent
{
public:
	CEditScene();
	~CEditScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};

