#include "../ClientHeader.h"
#include "PaperMsgTH.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "DocxInven.h"
#include "Component/ColliderSphere.h"
#include "MessageTH.h"
#include "Human_Player.h"

CPaperMsgTH::CPaperMsgTH()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CPaperMsgTH::CPaperMsgTH(const CPaperMsgTH & msg)	:
	CUserComponent(msg)
{
}


CPaperMsgTH::~CPaperMsgTH()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pDocxInven);
		SAFE_RELEASE(m_pDocxInvenObj);
	}
}

void CPaperMsgTH::AfterClone()
{
}

bool CPaperMsgTH::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("PaperRender");
	pRenderer->SetMesh("Papeer_msgth", TEXT("Paper.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(10.f, 10.f, 10.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("Papar_msgthBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPaperMsgTH::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPaperMsgTH::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_DOC_MESSAGETH);

	pBody->SetInfo(Vector3::Zero, 2.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CPaperMsgTH::Input(float fTime)
{
	return 0;
}

int CPaperMsgTH::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYPRESS("LButton"))
			{
				m_bOnInven = true;

				m_pDocxInvenObj = CGameObject::FindObject("DocxInven");

				CGameObject*	pSMObj = CGameObject::CreateObject("MessageTH", m_pLayer);

				CMessageTH*	pSM = pSMObj->AddComponent<CMessageTH>("MessageTH");

				m_pDocxInven = m_pDocxInvenObj->FindComponentFromTag<CDocxInven>("DocxInven");
				m_pDocxInven->AddItem(pSMObj);

				SAFE_RELEASE(pSM);
				SAFE_RELEASE(pSMObj);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

				m_pObject->SetEnable(false);
				m_bUseInven = true;
			}
		}
	}

	if (m_bMotion)
	{
		CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

		CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
		pPlayer->ChangeRayAnim("AimOff");

		m_bMotion = false;

		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	return 0;
}

int CPaperMsgTH::LateUpdate(float fTime)
{
	return 0;
}

void CPaperMsgTH::Collision(float fTime)
{
}

void CPaperMsgTH::Render(float fTime)
{
}

CPaperMsgTH * CPaperMsgTH::Clone()
{
	return new CPaperMsgTH(*this);
}

void CPaperMsgTH::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

	CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	Vector3 vPlayerPos = pPlayerTr->GetWorldPos();

	float fDist = m_pTransform->GetWorldPos().Distance(vPlayerPos);

	if (fDist < 50.f)
	{
		if (pDest->GetColliderID() == UCI_PLAYER_RAY)
		{
			m_bMouseOn = true;
			pPlayer->ChangeRayAnim("AimOn");
		}
	}

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
}

void CPaperMsgTH::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}