#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CFreeCamera :
	public CComponent
{
	friend class CGameObject;

protected:
	CFreeCamera();
	CFreeCamera(const CFreeCamera& camera);
	~CFreeCamera();

private:
	float		m_fSpeed;

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CFreeCamera* Clone();
};

PUN_END