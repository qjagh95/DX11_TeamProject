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
	m_bCtrl = false;
	m_bG = false;
	m_bShift = false;
	m_bMission = false;
	m_bPlay = false;
	m_fActiveTime = 0.f;
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
	/*CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("ECRender");

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);*/

	// Ctrl 키
	m_pPressCtrlObj = CGameObject::CreateObject("PressCtrl", m_pLayer);

	CTransform*	pPressCtrlTr = m_pPressCtrlObj->GetTransform();

	pPressCtrlTr->SetWorldPos(300.f, 4.f, 735.f);

	SAFE_RELEASE(pPressCtrlTr);

	CColliderOBB3D*	pOBB = m_pPressCtrlObj->AddComponent<CColliderOBB3D>("PressCtrlBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// G 키
	m_pPressGObj = CGameObject::CreateObject("PressG", m_pLayer);

	CTransform*	pPressGTr = m_pPressGObj->GetTransform();

	pPressGTr->SetWorldPos(320.f, 13.f, 700.f);

	SAFE_RELEASE(pPressGTr);

	pOBB = m_pPressGObj->AddComponent<CColliderOBB3D>("PressGBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CEventCollider::Hit);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CEventCollider::Out);

	SAFE_RELEASE(pOBB);

	// Shift 키
	m_pPressShiftObj = CGameObject::CreateObject("PressShift", m_pLayer);

	CTransform*	pPressShiftTr = m_pPressShiftObj->GetTransform();

	pPressShiftTr->SetWorldPos(340.f, 3.f, 730.f);

	SAFE_RELEASE(pPressShiftTr);

	pOBB = m_pPressShiftObj->AddComponent<CColliderOBB3D>("PressShiftBody");

	pOBB->SetInfo(2.5f, Vector3::Axis, 2.5f);
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

	m_pMessageObj = CGameObject::CreateObject("EventMessage", m_pLayer);
	m_pMessage = m_pMessageObj->AddComponent<CEventMessage>("EventMessage");

	//m_pMessageObj->SetEnable(false);	

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
	}

	else if (m_bG)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Press_G");
		m_bPlay = true;
	}

	else if (m_bShift)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Press_Shift");
		m_bPlay = true;
	}

	else if (m_bMission)
	{
		m_pMessage->SetVisible(true);
		m_pMessage->ChangeClip("Tutorial_Mission");
		m_bPlay = true;
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

void CEventCollider::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bCtrl = true;
	}

	if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bG = true;
	}

	if (pSrc->GetTag() == "PressShiftBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bShift = true;
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bMission = true;
	}
}

void CEventCollider::Out(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pSrc->GetTag() == "PressCtrlBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bCtrl = false;
	}

	if (pSrc->GetTag() == "PressGBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bG = false;
	}

	if (pSrc->GetTag() == "PressShiftBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bShift = false;
	}

	if (pSrc->GetTag() == "MissionBody" && pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bMission = false;
	}
}
