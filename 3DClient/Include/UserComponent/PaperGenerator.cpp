#include "../ClientHeader.h"
#include "PaperGenerator.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "DocxInven.h"
#include "Component/ColliderSphere.h"
#include "DocxGenerator.h"
#include "Human_Player.h"

CPaperGenerator::CPaperGenerator()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CPaperGenerator::CPaperGenerator(const CPaperGenerator & paper)	:
	CUserComponent(paper)
{
}

CPaperGenerator::~CPaperGenerator()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pDocxInven);
		SAFE_RELEASE(m_pDocxInvenObj);
	}
}

void CPaperGenerator::AfterClone()
{
}

bool CPaperGenerator::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("FileRender");
	pRenderer->SetMesh("File_Generator", TEXT("FileEx.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldRot(90.f, 0.f, 0.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("File_GeneratorBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPaperGenerator::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPaperGenerator::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_DOC_GENERATOR);

	pBody->SetInfo(Vector3::Zero, 2.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CPaperGenerator::Input(float fTime)
{
	return 0;
}

int CPaperGenerator::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYPRESS("LButton"))
			{
				m_bOnInven = true;

				m_pDocxInvenObj = CGameObject::FindObject("DocxInven");

				CGameObject*	pSMObj = CGameObject::CreateObject("DocxGenerator", m_pLayer);

				CDocxGenerator*	pSM = pSMObj->AddComponent<CDocxGenerator>("DocxGenerator");

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

int CPaperGenerator::LateUpdate(float fTime)
{
	return 0;
}

void CPaperGenerator::Collision(float fTime)
{
}

void CPaperGenerator::Render(float fTime)
{
}

CPaperGenerator * CPaperGenerator::Clone()
{
	return new CPaperGenerator(*this);
}

void CPaperGenerator::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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

void CPaperGenerator::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}
