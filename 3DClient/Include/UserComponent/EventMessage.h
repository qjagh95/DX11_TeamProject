#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CEventMessage :
	public CUserComponent
{
	friend class CGameObject;

private:
	CEventMessage();
	CEventMessage(const CEventMessage& eventmsg);
	virtual ~CEventMessage();

private:
	CAnimation2D*	m_pAnimation;
	bool	m_bPlay;

public:
	void ChangeClip(const string& strName);
	void SetVisible(bool bEnable);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEventMessage* Clone();
};

