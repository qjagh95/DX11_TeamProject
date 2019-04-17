#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CHandycam :
	public CUserComponent
{
	friend class CGameObject;

private:
	CHandycam();
	CHandycam(const CHandycam& handycam);
	virtual ~CHandycam();

private:
	int		m_iFlag;
	bool	m_bVisible;

public:
	void SetVisible(bool bVisible);
	void SetVisible();

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHandycam* Clone();
};

