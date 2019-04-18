#include "../ClientHeader.h"
#include "Battery.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Inventory.h"
#include "Component/ColliderSphere.h"
#include "BatteryIcon.h"

CBattery::CBattery()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
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

	CTransform*	pTr = m_pObject->GetTransform();

	pTr->SetWorldScale(1.f, 1.f, 1.f);

	SAFE_RELEASE(pTr);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("BatteryBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CBattery::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CBattery::MouseOut);

	pBody->SetInfo(Vector3::Zero, 2.f);

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
			if (KEYPRESS("LButton"))
			{
				m_bOnInven = true;

				m_pInvenObj = CGameObject::FindObject("Inven");

				CGameObject*	pIconObj = CGameObject::CreateObject("Icon_Battery", m_pLayer);

				CBatteryIcon*	pIcon = pIconObj->AddComponent<CBatteryIcon>("Icon_Battery");

				m_pInven = m_pInvenObj->FindComponentFromTag<CInventory>("Inven");
				m_pInven->AddItem(pIconObj);
				m_pInven->AddInvenList(pIconObj);

				SAFE_RELEASE(pIcon);
				SAFE_RELEASE(pIconObj);

				//SAFE_RELEASE(m_pInvenObj);

				m_pObject->SetEnable(false);
				m_bUseInven = true;
			}
		}
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
	if (pDest->GetTag() == "MouseWorld")
	{
		m_bMouseOn = true;
	}
}

void CBattery::MouseOn(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CBattery::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWorld")
	{
		m_bMouseOn = false;
	}
}
