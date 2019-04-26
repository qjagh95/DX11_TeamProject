#pragma once

#include "../Client.h"
#include "Component/Item.h"
#include "Component/Number.h"

PUN_USING

class CBatteryIcon :
	public CItem
{
	friend class CGameObject;

private:
	CBatteryIcon();
	CBatteryIcon(const CBatteryIcon& Icon1);
	virtual ~CBatteryIcon();

private:
	CGameObject*	m_pInvenObj;
	class CInventory*	m_pInven;
	CGameObject*	m_pNumObj;
	class CNumber*	m_pNumber;
	bool	m_bMouseOn;
	bool	m_bUse;
	int		m_iCount;

public:
	virtual void Add(int iCount);
	virtual void Use();
	bool	GetUse()	const;

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

