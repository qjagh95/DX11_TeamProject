#pragma once
#include "Ref.h"

PUN_BEGIN

class PUN_DLL CSceneComponent :
	public CRef
{
	friend class CScene;

protected:
	CSceneComponent();
	virtual ~CSceneComponent() = 0;

protected:
	class CScene*	m_pScene;
	class CLayer*	m_pLayer;

public:
	virtual void Start();
	virtual bool Init() = 0;
	virtual void AfterInit();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
};

PUN_END