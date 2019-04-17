#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CBatteryIcon :
	public CUserComponent
{
	friend class CGameObject;

private:
	CBatteryIcon();
	CBatteryIcon(const CBatteryIcon& Icon1);
	virtual ~CBatteryIcon();

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBatteryIcon* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOn(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

