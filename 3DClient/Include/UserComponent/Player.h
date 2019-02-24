#pragma once
#include "Component/UserComponent.h"
#include "Component/Animation.h"
PUN_USING

class CPlayer
	:public CUserComponent
{
	friend class CGameObject;

private:
	CAnimation* m_pAnimation;
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
	void Rotation(float fScale, float fTime);
};

