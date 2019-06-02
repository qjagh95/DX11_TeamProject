#include "../ClientHeader.h"
#include "Battery.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Inventory.h"
#include "Component/ColliderSphere.h"
#include "BatteryIcon.h"
#include "Human_Player.h"
#include "../GameManager.h"

CBattery::CBattery()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CBattery::CBattery(const CBattery & battery)	:
	CUserComponent(battery)
{
}

CBattery::~CBattery()
{
	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pInven);
		SAFE_RELEASE(m_pInvenObj);
	}
}

void CBattery::AfterClone()
{
}

bool CBattery::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("Battery", TEXT("Battery.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetEmissive(0.f);
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(0.1f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("BatteryBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CBattery::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CBattery::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_ITEM_BATTERY);

	pBody->SetInfo(Vector3::Zero, 1.f);

	SAFE_RELEASE(pBody);
	
	return true;
}

int CBattery::Input(float fTime)
{
	return 0;
}

int CBattery::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYUP("F"))
			{
				m_bOnInven = true;

				m_pInvenObj = CGameObject::FindObject("Inven");

				CGameObject*	pIconObj = CGameObject::CreateObject("Icon_Battery", m_pLayer);

				CBatteryIcon*	pIcon = pIconObj->AddComponent<CBatteryIcon>("Icon_Battery");

				m_pInven = m_pInvenObj->FindComponentFromTag<CInventory>("Inven");
				m_pInven->AddItem(pIconObj);

				SAFE_RELEASE(pIcon);
				SAFE_RELEASE(pIconObj);		

				m_pObject->SetEnable(false);
				m_bUseInven = true;

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);
			}
		}
	}

	if(m_bMotion)
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

int CBattery::LateUpdate(float fTime)
{
	return 0;
}

void CBattery::Collision(float fTime)
{
}

void CBattery::Render(float fTime)
{
}

CBattery * CBattery::Clone()
{
	return new CBattery(*this);
}

void CBattery::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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

void CBattery::MouseOn(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CBattery::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}	
}
