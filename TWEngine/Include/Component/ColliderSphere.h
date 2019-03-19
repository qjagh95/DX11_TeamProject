#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderSphere :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& com);
	virtual ~CColliderSphere();

private:
	SphereInfo	m_tRelativeInfo;
	SphereInfo	m_tInfo;

public:
	SphereInfo GetInfo()	const;
	void SetInfo(const Vector3& vCenter, float fRadius);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderSphere* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END
