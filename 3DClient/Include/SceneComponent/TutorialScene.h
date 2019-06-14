//#pragma once
//#include "Scene/SceneComponent.h"
//
//PUN_USING
//
//class CTutorialScene :
//	public CSceneComponent
//{
//public:
//	CTutorialScene();
//	~CTutorialScene();
//
//private:
//	float			m_fFade;
//	float			m_fAdaptationTime;
//
//public:
//	virtual void AfterInit();
//	virtual bool Init();
//	virtual int Update(float fTime);
//	virtual int LateUpdate(float fTime);
//};
//

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
	float			m_fFade;
	float			m_fAdaptationTime;

public:
	virtual void AfterInit();
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
};

