#include "../ClientHeader.h"
#include "BatteryIcon.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Input.h"
#include "Inventory.h"
#include "Handycam.h"
#include "BigIcon.h"

CBatteryIcon::CBatteryIcon()	
{
	m_eComType = (COMPONENT_TYPE)IT_BATTERY;
	m_bMouseOn = false;
	m_bUse = false;
}

CBatteryIcon::CBatteryIcon(const CBatteryIcon & Icon1)	:
	CItem(Icon1)
{
}

CBatteryIcon::~CBatteryIcon()
{
}

void CBatteryIcon::AfterClone()
{
}

bool CBatteryIcon::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("Icon1Renderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "BatteryIcon", TEXT("UI/Icon/Icon_Battery.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();

	pTransform->SetWorldScale(90.f, 29.f, 1.f);
	//pTransform->SetWorldPos(600.f, 429.f, 0.f);

	SAFE_RELEASE(pTransform);

	CColliderRect* pBody = AddComponent<CColliderRect>("BatteryIconBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CBatteryIcon::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CBatteryIcon::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, -30.f, 0.f), Vector3(100.f, 70.f, 0.f));

	SAFE_RELEASE(pBody);	

	return true;
}

int CBatteryIcon::Input(float fTime)
{
	return 0;
}

int CBatteryIcon::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (KEYPRESS("LButton"))
		{
			CGameObject*	pBigIconObj = CGameObject::FindObject("BigIcon");
			pBigIconObj->SetEnable(true);

			CBigIcon* pBigIcon = pBigIconObj->FindComponentFromType<CBigIcon>((COMPONENT_TYPE)IT_BIGICON);

			pBigIcon->ChangeClip("Battery_Detail");

			SAFE_RELEASE(pBigIcon);
			SAFE_RELEASE(pBigIconObj);
		}

		if (KEYPRESS("RButton"))
		{
			m_bUse = true;
			CGameObject*	pInvenObj = CGameObject::FindObject("Inven");

			CInventory*	pInven = pInvenObj->FindComponentFromTag<CInventory>("Inven");
			pInven->UseItem(m_pObject);
			
			SAFE_RELEASE(pInven);
			SAFE_RELEASE(pInvenObj);

			CGameObject*	pHandycamObj = CGameObject::FindObject("Handycam");

			CHandycam*	pHandycam = pHandycamObj->FindComponentFromTag<CHandycam>("Handycam");

			pHandycam->ChangeBattery(true);

			SAFE_RELEASE(pHandycam);
			SAFE_RELEASE(pHandycamObj);
		}
	}
	
	return 0;
}

int CBatteryIcon::LateUpdate(float fTime)
{
	return 0;
}

void CBatteryIcon::Collision(float fTime)
{
}

void CBatteryIcon::Render(float fTime)
{
}

CBatteryIcon * CBatteryIcon::Clone()
{
	return new CBatteryIcon(*this);
}

void CBatteryIcon::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CBatteryIcon::MouseOn(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CBatteryIcon::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
