#pragma once
#include "Component/UserComponent.h"
#include "Component/Animation.h"

PUN_USING

class BlackCow
	:public CUserComponent
{
	friend class CGameObject;

private:
	CAnimation*			m_pAnimation;
	class CGameObject*	m_pInvenObj;
	class CInventory*	m_pInven;
	class CTransform*	m_pInvenTr;
	class CGameObject*	m_pHandycamObj;
	class CHandycam*	m_pHandycam;

protected:
	BlackCow();
	BlackCow(const BlackCow& com);
	virtual ~BlackCow();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual BlackCow* Clone();

public:
	void Move(float fScale, float fTime);
	void Rotation(float fScale, float fTime);

	//Sound Test
	void YoDjPumpThisParty(float fTime);
	void KingDdenGodTtack(float fTime);
};

