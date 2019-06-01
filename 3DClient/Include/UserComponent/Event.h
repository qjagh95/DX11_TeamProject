#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CEvent :
	public CUserComponent
{
	friend class CGameObject;

private:
	CEvent();
	CEvent(const CEvent& event);
	virtual ~CEvent();

private:
	CAnimation2D*	m_pAnimation;
	bool			m_bMouseOn;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEvent* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void Out(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

