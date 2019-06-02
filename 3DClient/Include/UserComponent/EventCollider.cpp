#include "../ClientHeader.h"
#include "EventCollider.h"
#include "Component/ColliderOBB3D.h"
#include "EventMessage.h"

CEventCollider::CEventCollider()	:
	m_pPressCtrlObj(nullptr),
	m_pPressGObj(nullptr),
	m_pPressShiftObj(nullptr),
	m_pPressMissionObj(nullptr),
	m_pMessageObj(nullptr),
	m_pMessage(nullptr)
{
	m_eComType = (COMPONENT_TYPE)UT_EVENTBOX;
}

CEventCollider::CEventCollider(const CEventCollider & ebox)	:
	CUserComponent(ebox)
{
}

CEventCollider::~CEventCollider()
{
	SAFE_RELEASE(m_pPressCtrlObj);
	SAFE_RELEASE(m_pPressGObj);
	SAFE_RELEASE(m_pPressShiftObj);
	SAFE_RELEASE(m_pPressMissionObj);
	SAFE_RELEASE(m_pMessage);
	SAFE_RELEASE(m_pMessageObj);
}

void CEventCollider::AfterClone()
{
}

bool CEventCollider::Init()
{
	m_pMessageObj = CGameObject::CreateObject("EventMessage", m_pLayer);

	m_pMessageObj->SetEnable(false);

	m_pMessage = m_pMessageObj->AddComponent<CEventMessage>("EventMessage");

	m_pMessage->ChangeClip("Message_Empty");

	CTransform*	pMessageTr = m_pMessageObj->GetTransform();

	pMessageTr->SetWorldPos(520.f, 600.f, 0.f);
	
	SAFE_RELEASE(pMessageTr);

	// Ctrl 키
	m_pPressCtrlObj = CGameObject::CreateObject("PressCtrl", m_pLayer);

	CTransform*	pPressCtrlTr = m_pPressCtrlObj->GetTransform();

	pPressCtrlTr->SetWorldPos(300.f, 20.f, 700.f);

	SAFE_RELEASE(pPressCtrlTr);

	CColliderOBB3D*	pOBB = m_pPressCtrlObj->AddComponent<CColliderOBB3D>("PressCtrlBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// G 키
	m_pPressGObj = CGameObject::CreateObject("PressG", m_pLayer);

	CTransform*	pPressGTr = m_pPressGObj->GetTransform();

	pPressGTr->SetWorldPos(320.f, 20.f, 700.f);

	SAFE_RELEASE(pPressGTr);

	pOBB = m_pPressGObj->AddComponent<CColliderOBB3D>("PressGBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// Shift 키
	m_pPressShiftObj = CGameObject::CreateObject("PressShift", m_pLayer);

	CTransform*	pPressShiftTr = m_pPressShiftObj->GetTransform();

	SAFE_RELEASE(pPressShiftTr);

	pOBB = m_pPressShiftObj->AddComponent<CColliderOBB3D>("PressShiftBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// Mission 알림
	m_pPressMissionObj = CGameObject::CreateObject("Mission", m_pLayer);

	CTransform*	pMissionTr = m_pPressMissionObj->GetTransform();

	pMissionTr->SetWorldPos(350.f, 20.f, 700.f);

	SAFE_RELEASE(pMissionTr);

	pOBB = m_pPressMissionObj->AddComponent<CColliderOBB3D>("MissionBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	return true;
}

int CEventCollider::Input(float fTime)
{
	return 0;
}

int CEventCollider::Update(float fTime)
{
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

void CEventCollider::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "PressShiftBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}
}

void CEventCollider::Out(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "PressShiftBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
	}
}
