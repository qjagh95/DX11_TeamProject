#pragma once
#include "Component\UserComponent.h"
#include "../Client.h"

PUN_USING

class CEventCollider :
	public CUserComponent
{
	friend class CGameObject;

private:
	CEventCollider();
	CEventCollider(const CEventCollider& ebox);
	virtual ~CEventCollider();

private:
	CGameObject*	m_pPressCtrlObj;
	CGameObject*	m_pPressGObj;
	CGameObject*	m_pPressShiftObj;
	CGameObject*	m_pPressShift2Obj;
	CGameObject*	m_pPressShift3Obj;
	CGameObject*	m_pPressMissionObj;
	CGameObject*	m_pTriggerObj;
	CGameObject*	m_pMessageObj;
	class CEventMessage*	m_pMessage;

	bool m_bCtrl;
	bool m_bG;
	bool m_bShift;
	bool m_bMission;
	bool m_bPlay;
	bool m_bTrigger;
	bool m_bNext;

	bool m_bMade;
	bool m_bTutorial;

	float m_fActiveTime;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEventCollider* Clone();

public:
	bool SetTutorial();
	bool NoticeCamera();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void Out(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

