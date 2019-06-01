#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CDocxGenerator :
	public CUserComponent
{
	friend class CGameObject;

private:
	CDocxGenerator();
	CDocxGenerator(const CDocxGenerator& doc);
	virtual ~CDocxGenerator();

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
	virtual CDocxGenerator* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

