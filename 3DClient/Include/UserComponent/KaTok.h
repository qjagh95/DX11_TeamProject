#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CKaTok :
	public CUserComponent
{
	friend class CGameObject;

private:
	CKaTok();
	CKaTok(const CKaTok& doc);
	virtual ~CKaTok();
	
public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CKaTok* Clone();
};

