#pragma once
#include "Component/UserComponent.h"

PUN_USING

class CHidable	:
	public CUserComponent
{
	friend class CGameObject;

protected:
	CHidable();
	CHidable(const CHidable& battery);
	virtual ~CHidable();

protected:
	HIDABLE_TYPE m_eType;

public:
	HIDABLE_TYPE GetHidableType() const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHidable* Clone();
};

