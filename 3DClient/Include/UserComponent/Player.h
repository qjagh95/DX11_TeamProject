#pragma once
#include "Component/UserComponent.h"
#include "Component/Animation.h"

PUN_USING

class CPlayer
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
	bool				m_bParkour;
	bool				m_bNaviOn;
	float				m_fParkourYPos;
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

	//Sound Test
	void YoDjPumpThisParty(float fTime);
	void KingDdenGodTtack(float fTime);
	void ParkourOut(CCollider * pSrc, CCollider * pDest, float fTime);
	void ParkourIn(CCollider * pSrc, CCollider * pDest, float fTime);
};

