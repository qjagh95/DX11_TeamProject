#include "../ClientHeader.h"
#include "PaperBQ.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "DocxInven.h"
#include "Component/ColliderSphere.h"
#include "MessageBQ.h"
#include "Human_Player.h"
#include "../GameManager.h"

CPaperBQ::CPaperBQ()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CPaperBQ::CPaperBQ(const CPaperBQ & paper)	:
	CUserComponent(paper)
{
}

CPaperBQ::~CPaperBQ()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pDocxInven);
		SAFE_RELEASE(m_pDocxInvenObj);
	}
}

void CPaperBQ::AfterClone()
{
}

bool CPaperBQ::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("PaperRender");
	pRenderer->SetMesh("PaperBQ", TEXT("Paper.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(10.f, 10.f, 10.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("PaperBQBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPaperBQ::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPaperBQ::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_DOC_MESSAGEBQ);

	pBody->SetInfo(Vector3::Zero, 2.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CPaperBQ::Input(float fTime)
{
	return 0;
}

int CPaperBQ::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYUP("F"))
			{
				m_bOnInven = true;

				m_pDocxInvenObj = CGameObject::FindObject("DocxInven");

				CGameObject*	pSMObj = CGameObject::CreateObject("MessageBQ", m_pLayer);

				CMessageBQ*	pSM = pSMObj->AddComponent<CMessageBQ>("MessageBQ");

				m_pDocxInven = m_pDocxInvenObj->FindComponentFromTag<CDocxInven>("DocxInven");
				m_pDocxInven->AddItem(pSMObj);

				SAFE_RELEASE(pSM);
				SAFE_RELEASE(pSMObj);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

				GET_SINGLE(CGameManager)->AddChangedListItemObj(m_pObject);

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
		GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

		m_bMotion = false;

		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	return 0;
}

int CPaperBQ::LateUpdate(float fTime)
{
	return 0;
}

void CPaperBQ::Collision(float fTime)
{
}

void CPaperBQ::Render(float fTime)
{
}

CPaperBQ * CPaperBQ::Clone()
{
	return new CPaperBQ(*this);
}

void CPaperBQ::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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
			GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_F_Pickup");
		}
	}

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
}

void CPaperBQ::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}
