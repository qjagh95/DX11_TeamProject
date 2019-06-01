#pragma once
#include "Component\UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CDocxGongji :
	public CUserComponent
{
	friend class CGameObject;

private:
	CDocxGongji();
	CDocxGongji(const CDocxGongji& doc);
	virtual ~CDocxGongji();

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
	virtual CDocxGongji* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

