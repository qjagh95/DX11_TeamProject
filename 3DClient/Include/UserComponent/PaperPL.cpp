#include "../ClientHeader.h"
#include "PaperPL.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "DocxInven.h"
#include "Component/ColliderSphere.h"
#include "MeesagePL.h"
#include "Human_Player.h"

CPaperPL::CPaperPL()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CPaperPL::CPaperPL(const CPaperPL & paper)	:
	CUserComponent(paper)
{
}

CPaperPL::~CPaperPL()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pDocxInven);
		SAFE_RELEASE(m_pDocxInvenObj);
	}
}

void CPaperPL::AfterClone()
{
}

bool CPaperPL::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("PaperRender");
	pRenderer->SetMesh("Paper_PL", TEXT("Paper.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(10.f, 10.f, 10.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("PaperPLBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPaperPL::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPaperPL::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_DOC_MESSAGEPL);

	pBody->SetInfo(Vector3::Zero, 2.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CPaperPL::Input(float fTime)
{
	return 0;
}

int CPaperPL::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYPRESS("LButton"))
			{
				m_bOnInven = true;

				m_pDocxInvenObj = CGameObject::FindObject("DocxInven");

				CGameObject*	pSMObj = CGameObject::CreateObject("PaperPL", m_pLayer);

				CMeesagePL*	pSM = pSMObj->AddComponent<CMeesagePL>("PaperPL");

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

int CPaperPL::LateUpdate(float fTime)
{
	return 0;
}

void CPaperPL::Collision(float fTime)
{
}

void CPaperPL::Render(float fTime)
{
}

CPaperPL * CPaperPL::Clone()
{
	return new CPaperPL(*this);
}

void CPaperPL::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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

void CPaperPL::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}