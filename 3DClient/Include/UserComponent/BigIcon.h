#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CBigIcon :
	public CUserComponent
{
	friend class CGameObject;

private:
	CBigIcon();
	CBigIcon(const CBigIcon& bigicon);
	virtual ~CBigIcon();

private:
	CAnimation2D*	m_pAnimation;

public:
	void ChangeClip(const string& strName);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBigIcon* Clone();
};

