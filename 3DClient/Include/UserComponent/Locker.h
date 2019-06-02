#pragma once
#include "Hidable.h"
#include "Component/ColliderOBB3D.h"

PUN_USING

class CLocker :
	public CHidable
{
	friend class CGameObject;

private:
	CLocker();
	CLocker(const CLocker& battery);
	virtual ~CLocker();

private:
	Vector3 m_vRelativePos;

	class CDoor* m_pDoor;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLocker* Clone();

	void Interact(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void OpenDoor();
};
