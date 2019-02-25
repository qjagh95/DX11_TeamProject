#pragma once

#include "Component.h"

PUN_BEGIN

class PUN_DLL CArm :
	public CComponent
{
	friend class CGameObject;

protected:
	CArm();
	CArm(const CArm& arm);
	~CArm();

private:
	float		m_fSpeed;
	float		m_fDistance;
	bool		m_bMouseAction;
	class CTransform*	m_pTarget;

public:
	void SetTarget(class CGameObject* pTarget);
	void SetTarget(class CComponent* pTarget);

public:
	void EnableMouse();
	void MouseRotation(float fTime);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CArm* Clone();
};


PUN_END
