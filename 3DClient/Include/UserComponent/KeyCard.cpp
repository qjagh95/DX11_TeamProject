#include "../ClientHeader.h"
#include "KeyCard.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "KeyInven.h"
#include "Component/ColliderSphere.h"
#include "Human_Player.h"
#include "MasterKey.h"
#include "../GameManager.h"
#include "Door.h"

CKeyCard::CKeyCard()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CKeyCard::CKeyCard(const CKeyCard & key)	:
	CUserComponent(key)
{
}

CKeyCard::~CKeyCard()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pKeyInven);
		SAFE_RELEASE(m_pKeyInvenObj);
	}
}

void CKeyCard::AfterClone()
{
}

bool CKeyCard::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("KeyCardRender");
	pRenderer->SetMesh("KeyCard", TEXT("KeyCard.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(0.1f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("KeyCardBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CKeyCard::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CKeyCard::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_ITEM_KEY);

	pBody->SetInfo(Vector3::Zero, 1.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CKeyCard::Input(float fTime)
{
	return 0;
}

int CKeyCard::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYUP("F"))
			{
				m_bOnInven = true;

				m_pKeyInvenObj = CGameObject::FindObject("KeyInven");

				CGameObject*	pMKObj = CGameObject::CreateObject("MasterKey", m_pLayer);				

				CMasterKey*	pMK = pMKObj->AddComponent<CMasterKey>("MasterKey");

				m_pKeyInven = m_pKeyInvenObj->FindComponentFromTag<CKeyInven>("KeyInven");
				m_pKeyInven->AddItem(pMKObj);

				SAFE_RELEASE(pMK);
				SAFE_RELEASE(pMKObj);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

				GET_SINGLE(CGameManager)->AddKey(m_pObject->GetTag(), m_pObject);
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

int CKeyCard::LateUpdate(float fTime)
{
	return 0;
}

void CKeyCard::Collision(float fTime)
{
}

void CKeyCard::Render(float fTime)
{
}

CKeyCard * CKeyCard::Clone()
{
	return new CKeyCard(*this);
}

void CKeyCard::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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

void CKeyCard::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}
