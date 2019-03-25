#pragma once
#include "Scene/SceneComponent.h"

PUN_BEGIN

class CTransform;
class PUN_DLL EditorScene : public CSceneComponent
{
public:
	EditorScene();
	~EditorScene();

private:
	CTransform*	m_pTr;

private:
	void CreatePrototype();

public:
	void Move(float fScale, float fTime);

public:
	bool Init()					override;
	int  Update(float fTime)	override;
};

PUN_END