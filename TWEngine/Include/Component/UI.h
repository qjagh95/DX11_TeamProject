#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CUI :
	public CComponent
{
	friend class CGameObject;

protected:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI() = 0;

protected:
	UI_TYPE		m_eUIType;

public:
	UI_TYPE	GetUIType()	const;

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUI* Clone() = 0;
};

PUN_END