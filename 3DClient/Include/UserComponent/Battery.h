#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CBattery :
	public CUserComponent
{
	friend class CGameObject;

private:
	CBattery();
	CBattery(const CBattery& battery);
	virtual ~CBattery();

private:
	CGameObject*	m_pInvenObj;
	class CInventory*	m_pInven;
	bool			m_bMouseOn;
	bool			m_bUseInven;
	bool			m_bOnInven;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBattery* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOn(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

