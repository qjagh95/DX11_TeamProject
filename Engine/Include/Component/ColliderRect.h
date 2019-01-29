#pragma once
#include "Collider.h"

PUN_BEGIN

class PUN_DLL CColliderRect :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderRect();
	CColliderRect(const CColliderRect& com);
	virtual ~CColliderRect();

private:
	BoxInfo m_tRelativeInfo;
	BoxInfo m_tInfo;

public:
	BoxInfo GetInfo()	const;
	void	SetInfo(const Vector3& vMin, const Vector3& vMax);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderRect* Clone();

public:
	virtual bool Collision(CCollider* pDest, float fTime);
};

PUN_END