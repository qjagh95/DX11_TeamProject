#pragma once
#include "Scene/SceneComponent.h"

PUN_BEGIN

class CTransform;
class PUN_DLL EditorScene : public CSceneComponent
{
private:
	CTransform*	m_pTr;

public:
	bool Init() override;
	int Update(float fTime) override;

public:
	void Move(float fScale, float fTime);

private:
	void CreatePrototype();

public:
	EditorScene();
	~EditorScene();
};

PUN_END
