#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderOBB :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderOBB();
	CColliderOBB(const CColliderOBB& com);
	virtual ~CColliderOBB();

private:
	OBB2DInfo	m_tRelativeInfo;
	OBB2DInfo	m_tInfo;

public:
	OBB2DInfo GetInfo()	const;
	void SetInfo(const Vector3& vCenter, Vector3 vAxis[2],
		float fLength[2]);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderOBB* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END