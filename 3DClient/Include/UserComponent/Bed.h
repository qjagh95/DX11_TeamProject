#pragma once
#include "Hidable.h"
#include <Component/ColliderOBB3D.h>

PUN_USING

class CBed :
	public CHidable
{
	friend class CGameObject;

private:
	CBed();
	CBed(const CBed& battery);
	virtual ~CBed();


public:
	static float fPlayerLocalOffsetY;
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBed* Clone();
	void OnCollisionWithPlayer(PUN::CCollider *pSrc, PUN::CCollider *pDest, float fTime);
	void OnCollisionExitWithPlayer(PUN::CCollider *pSrc, PUN::CCollider *pDest, float fTime);
};

