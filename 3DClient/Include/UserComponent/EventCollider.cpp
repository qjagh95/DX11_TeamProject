#include "../ClientHeader.h"
#include "EventCollider.h"
#include "Component/ColliderOBB3D.h"
#include "EventMessage.h"
#include "Human_Player.h"
#include "Rendering/RenderManager.h"

CEventCollider::CEventCollider()	:
	m_pPressCtrlObj(nullptr),
	m_pPressGObj(nullptr),
	m_pPressShiftObj(nullptr),
	m_pPressShift2Obj(nullptr),
	m_pPressShift3Obj(nullptr),
	m_pPressMissionObj(nullptr),
	m_pTriggerObj(nullptr),
	m_pMessageObj(nullptr),
	m_pMessage(nullptr)
{
	m_eComType = (COMPONENT_TYPE)UT_EVENTBOX;
	m_bCtrl = false;
	m_bG = false;
	m_bShift = false;
	m_bMission = false;
	m_bPlay = false;
	m_bTrigger = false;
	m_bMade = false;
	m_bTutorial = false;
	m_bNext = false;
	m_fActiveTime = 0.f;
}

CEventCollider::CEventCollider(const CEventCollider & ebox)	:
	CUserComponent(ebox)
{
}

CEventCollider::~CEventCollider()
{
	if (m_bMade)
	{
		SAFE_RELEASE(m_pPressGObj);
	}

	if (m_bTutorial)
	{
		SAFE_RELEASE(m_pPressCtrlObj);
		SAFE_RELEASE(m_pPressShiftObj);
		SAFE_RELEASE(m_pPressShift2Obj);
		SAFE_RELEASE(m_pPressShift3Obj);
		SAFE_RELEASE(m_pPressMissionObj);
		SAFE_RELEASE(m_pTriggerObj);
		SAFE_RELEASE(m_pMessage);
		SAFE_RELEASE(m_pMessageObj);
	}
}

void CEventCollider::AfterClone()
{
}

bool CEventCollider::Init()
{
	m_pMessageObj = CGameObject::CreateObject("EventMessage", m_pLayer);
	m_pMessage = m_pMessageObj->AddComponent<CEventMessage>("EventMessage");

	m_pMessage->ChangeClip("Message_Empty");
	
	CTransform*	pMsgTr = m_pMessageObj->GetTransform();

	pMsgTr->SetWorldScale(468.f, 88.f, 1.f);
	pMsgTr->SetWorldPos(630.f, 600.f, 0.f);

	SAFE_RELEASE(pMsgTr);

	return true;
}

int CEventCollider::Input(float fTime)
{
	return 0;
}

int CEventCollider::Update(float fTime)
{
	if (m_bCtrl)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Press_Ctrl");
		m_bPlay = true;
		m_bCtrl = false;
	}	

	else if (m_bShift)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Press_Shift");
		m_bPlay = true;
		m_bShift = false;
	}

	else if (m_bMission)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Tutorial_Mission");
		m_bPlay = true;
	}

	if (m_bMade)
	{
		if (m_bG)
		{
			m_pMessage->SetVisible(true);
			m_pMessage->ChangeClip("Press_G");
			m_bPlay = true;
			m_bG = false;
		}
	}

	if (m_bTrigger)
	{
		CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

		CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromTag<CHuman_Player>("Player");

		pPlayer->SetNaviY(false);

		CTransform*	pTr = pPlayerObj->GetTransform();

		pTr->Move(Vector3::Axis[AXIS_Y], -300.f, fTime);


		if (m_bNext)
		{
			m_bTrigger = false;
			Vector3 vPlayerPos = pTr->GetWorldPos();
			pTr->SetWorldPos(vPlayerPos);

			GET_SINGLE(CRenderManager)->SetFadeAmount(0.001f, fTime);
		}

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	if (m_bPlay)
	{
		m_fActiveTime += fTime;

		if (m_fActiveTime >= 5.f)
		{
			m_fActiveTime = 0.f;
			m_bPlay = false;
			m_pMessage->SetVisible(false);
		}
	}

	return 0;
}

int CEventCollider::LateUpdate(float fTime)
{
	return 0;
}

void CEventCollider::Collision(float fTime)
{
}

void CEventCollider::Render(float fTime)
{
}

CEventCollider * CEventCollider::Clone()
{
	return new CEventCollider(*this);
}

bool CEventCollider::SetTutorial()
{
	// Ctrl 키
	m_pPressCtrlObj = CGameObject::CreateObject("PressCtrl", m_pLayer);

	CTransform*	pPressCtrlTr = m_pPressCtrlObj->GetTransform();

	pPressCtrlTr->SetWorldPos(311.f, 18.f, 617.f);

	SAFE_RELEASE(pPressCtrlTr);

	CColliderOBB3D*	pOBB = m_pPressCtrlObj->AddComponent<CColliderOBB3D>("PressCtrlBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, Vector3(30.f, 5.f, 5.f));
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// Shift 키
	m_pPressShiftObj = CGameObject::CreateObject("PressShift", m_pLayer);
	  
	CTransform*	pPressShiftTr = m_pPressShiftObj->GetTransform();

	pPressShiftTr->SetWorldPos(242.f, 9.f, 718.f);

	SAFE_RELEASE(pPressShiftTr);

	pOBB = m_pPressShiftObj->AddComponent<CColliderOBB3D>("PressShiftBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 10.f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	m_pPressShift2Obj = CGameObject::CreateObject("PressShift2", m_pLayer);

	pPressShiftTr = m_pPressShift2Obj->GetTransform();

	pPressShiftTr->SetWorldPos(319.f, 8.f, 718.f);

	SAFE_RELEASE(pPressShiftTr);

	pOBB = m_pPressShift2Obj->AddComponent<CColliderOBB3D>("PressShift2Body");

	pOBB->SetInfo(2.5f, Vector3::Axis, 10.f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	m_pPressShift3Obj = CGameObject::CreateObject("PressShift3", m_pLayer);

	pPressShiftTr = m_pPressShift3Obj->GetTransform();

	pPressShiftTr->SetWorldPos(391.f, 10.f, 718.f);

	SAFE_RELEASE(pPressShiftTr);

	pOBB = m_pPressShift3Obj->AddComponent<CColliderOBB3D>("PressShift3Body");

	pOBB->SetInfo(2.5f, Vector3::Axis, 10.f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// Mission 알림
	m_pPressMissionObj = CGameObject::CreateObject("Mission", m_pLayer);

	CTransform*	pMissionTr = m_pPressMissionObj->GetTransform();

	pMissionTr->SetWorldPos(350.f, 3.f, 730.f);

	SAFE_RELEASE(pMissionTr);

	pOBB = m_pPressMissionObj->AddComponent<CColliderOBB3D>("MissionBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// 트리거 이벤트
	m_pTriggerObj = CGameObject::CreateObject("TriggerBox", m_pLayer);

	CTransform*	pTriggerTr = m_pTriggerObj->GetTransform();

	pTriggerTr->SetWorldPos(309.f, 28.f, 549.f);

	SAFE_RELEASE(pTriggerTr);

	pOBB = m_pTriggerObj->AddComponent<CColliderOBB3D>("TriggerBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	CGameObject*	pNextObj = CGameObject::CreateObject("NextSceneBox", m_pLayer);

	pTriggerTr = pNextObj->GetTransform();

	pTriggerTr->SetWorldPos(309.f, -1028.f, 549.f);

	SAFE_RELEASE(pTriggerTr);

	pOBB = pNextObj->AddComponent<CColliderOBB3D>("NextSceneBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);
	SAFE_RELEASE(pNextObj);

	m_bTutorial = true;

	return true;
}

bool CEventCollider::NoticeCamera()
{
	m_bMade = true;
	// G 키
	m_pPressGObj = CGameObject::CreateObject("PressG", m_pLayer);

	CTransform*	pPressGTr = m_pPressGObj->GetTransform();

	pPressGTr->SetWorldPos(320.f, 13.f, 700.f);

	SAFE_RELEASE(pPressGTr);

	CColliderOBB3D*	pOBB = m_pPressGObj->AddComponent<CColliderOBB3D>("PressGBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	return true;
}

void CEventCollider::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bCtrl = true;
		m_pPressCtrlObj->SetEnable(false);
	}

	if (m_bMade)
	{
		if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
		{
			m_bG = true;
			m_pPressGObj->SetEnable(false);
		}
	}

	if (((pSrc->GetTag() == "PressShiftBody") || (pSrc->GetTag() == "PressShift2Body") ||
		(pSrc->GetTag() == "PressShift3Body")) && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bShift = true;
		m_pPressShiftObj->SetEnable(false);
		m_pPressShift2Obj->SetEnable(false);
		m_pPressShift3Obj->SetEnable(false);
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bMission = true;
	}

	if (pSrc->GetTag() == "TriggerBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bTrigger = true; 
		//m_pTriggerObj->SetEnable(false);
	}

	if (pSrc->GetTag() == "NextSceneBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bNext = true;
	}
}

void CEventCollider::Out(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bCtrl = false;
	}

	if (m_bMade)
	{
		if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
		{
			m_bG = false;
		}
	}

	if (((pSrc->GetTag() == "PressShiftBody") || (pSrc->GetTag() == "PressShift2Body") ||
		(pSrc->GetTag() == "PressShift3Body")) && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bShift = false;
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bMission = false;
	}

	if (pSrc->GetTag() == "TriggerBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		//m_bTrigger = false;
	}
}
