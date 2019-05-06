#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderOBB3D :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderOBB3D();
	CColliderOBB3D(const CColliderOBB3D& com);
	virtual ~CColliderOBB3D();

private:
	OBB3DInfo	m_tRelativeInfo;
	OBB3DInfo	m_tInfo;

public:
	OBB3DInfo GetInfo()	const;
	void SetInfo(const Vector3& vCenter, Vector3 vAxis[3],
		const Vector3& vLength);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderOBB3D* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END
