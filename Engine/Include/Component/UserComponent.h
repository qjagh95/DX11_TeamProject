#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CUserComponent :
	public CComponent
{
	friend class CGameObject;

public:
	CUserComponent();
	CUserComponent(const CUserComponent& com);
	virtual ~CUserComponent() = 0;

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUserComponent* Clone() = 0;
};

PUN_END