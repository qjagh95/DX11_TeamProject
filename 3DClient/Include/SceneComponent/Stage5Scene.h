#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CStage5Scene
	:public CSceneComponent
{
public:
	CStage5Scene();
	~CStage5Scene();

	bool m_isTemp;

private:
	float	m_fFade;

public:
	bool Init() override;
	void AfterInit() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	void Render(float fTime) override;
};

