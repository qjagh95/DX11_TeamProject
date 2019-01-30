#pragma once
#include "../stdafx.h"
#include "Component/UserComponent.h"

PUN_USING

class CPlayer
	:public CUserComponent
{
	friend class CGameObject;

protected:
	CPlayer();
	CPlayer(const CPlayer& com);
	virtual ~CPlayer();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CPlayer* Clone();

public:
	void Move(float fScale, float fTime);
	void Fire(float fTime);
	void Fire1(float fTime);
	void Fire1Release(float fTime);
};

