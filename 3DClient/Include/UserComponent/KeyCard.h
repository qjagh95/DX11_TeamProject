#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CKeyCard :
	public CUserComponent
{
	friend class CGameObject;

private:
	CKeyCard();
	CKeyCard(const CKeyCard& key);
	virtual ~CKeyCard();

private:
	CGameObject*		m_pKeyInvenObj;
	class CKeyInven*	m_pKeyInven;
	bool			m_bMouseOn;
	bool			m_bUseInven;
	bool			m_bOnInven;
	int				m_iCount;
	bool			m_bMotion;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CKeyCard* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

