#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderPoint :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& com);
	virtual ~CColliderPoint();

private:
	Vector3		m_vRelativePos;
	Vector3		m_vPos;

public:
	Vector3 GetInfo()	const;
	void SetInfo(const Vector3& vRelativePos);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderPoint* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END