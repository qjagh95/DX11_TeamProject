#pragma once
#include "../Client.h"
#include "Component/UserComponent.h"
#include "Component/Animation2D.h"

PUN_USING

class CDocument :
	public CUserComponent
{
	friend class CGameObject;

private:
	CDocument();
	CDocument(const CDocument& doc);
	virtual ~CDocument();

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
	virtual CDocument* Clone();
};

