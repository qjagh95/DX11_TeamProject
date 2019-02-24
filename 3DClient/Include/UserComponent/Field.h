#pragma once

#include "Component/UserComponent.h"
PUN_USING

class CField
	:public CUserComponent
{
	friend class CGameObject;
protected:
	CField();
	CField(const CField& com);
	virtual ~CField();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CField* Clone();
};

