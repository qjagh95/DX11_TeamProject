#pragma once
#include "Component/UserComponent.h"

PUN_USING

class CParkour
	: public CUserComponent
{
	friend class CGameObject;

private:
	CParkour();
	CParkour(const CParkour& _Parkour);
	virtual ~CParkour();
public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CParkour* Clone();
};

