#pragma once
#include "Component/UserComponent.h"

PUN_USING

class CBed :
	public CUserComponent
{
	friend class CGameObject;

private:
	CBed();
	CBed(const CBed& battery);
	virtual ~CBed();


public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBed* Clone();
};

