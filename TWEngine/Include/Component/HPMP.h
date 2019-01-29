#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CHPMP :
	public CUI
{
	friend class CGameObject;

protected:
	CHPMP();
	CHPMP(const CHPMP& hpmp);
	virtual ~CHPMP();

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHPMP* Clone();
};

PUN_END