#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/ColliderRect.h"

PUN_USING

class CZipper :
	public CUserComponent
{
	friend class CGameObject;

private:
	CZipper();
	CZipper(const CZipper& zip);
	virtual ~CZipper();

private:
	CGameObject*	m_pInvenObj;


public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CZipper* Clone();
};

