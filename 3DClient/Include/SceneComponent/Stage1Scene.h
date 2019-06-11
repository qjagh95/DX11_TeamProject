#pragma once
#include "Scene/SceneComponent.h"

PUN_USING

class CStage1Scene
	:public CSceneComponent
{
public:
	CStage1Scene();
	~CStage1Scene();

	static bool m_isEventColl;
	bool m_isTemp;

private:
	float	m_fFade;

	CGameObject* m_CollObject;
	CColliderOBB3D* m_EventColl;

	CGameObject* m_DefaultObject;
	ST_Default* m_Default;

private:
	void EventCallback(CCollider* Src, CCollider* Dest, float DeltaTime);

public:
	bool Init() override;
	void AfterInit() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	void Render(float fTime) override;
};

