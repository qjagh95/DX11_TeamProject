#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CTutorialScene :
	public CSceneComponent
{
public:
	CTutorialScene();
	~CTutorialScene();

private:
	vector<string>	m_vecAlpha;

public:
	virtual void AfterInit();
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);

private:
	void AdaptAlpha(const string& strName);
};

