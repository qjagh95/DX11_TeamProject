#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderRay :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderRay();
	CColliderRay(const CColliderRay& com);
	virtual ~CColliderRay();

private:
	RayInfo	m_tRelativeInfo;
	RayInfo	m_tInfo;
	bool	m_bMouseEnable;

public:
	RayInfo GetInfo()	const;
	void SetInfo(const Vector3& vPos, const Vector3& vDir,
		float fDist = -1.f);
	void MouseEnable();

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderRay* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END
