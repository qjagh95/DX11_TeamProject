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

public:
	void ResetClip();

private:
	CGameObject*	m_pInvenObj;
	CAnimation2D*	m_pAnimation;

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

