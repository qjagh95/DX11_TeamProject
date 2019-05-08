#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CMasterKey :
	public CUserComponent
{
	friend class CGameObject;

private:
	CMasterKey();
	CMasterKey(const CMasterKey& key);
	virtual ~CMasterKey();

private:
	bool			m_bMouseOn;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMasterKey* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

