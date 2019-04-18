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

CBatteryIcon::CBatteryIcon()	:
	 m_pInvenObj(nullptr)
	,m_pInven(nullptr)
{
	m_eComType = (COMPONENT_TYPE)UT_ICON;
	m_bMouseOn = false;
	m_bUse = false;
}

CBatteryIcon::CBatteryIcon(const CBatteryIcon & Icon1)	:
	CUserComponent(Icon1)
{
}

CBatteryIcon::~CBatteryIcon()
{
	SAFE_RELEASE(m_pInvenObj);
	SAFE_RELEASE(m_pInven);
}

bool CBatteryIcon::GetUse() const
{
	return m_bUse;
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
	pTransform->SetWorldPos(600.f, 429.f, 0.f);

	SAFE_RELEASE(pTransform);

	CColliderRect* pBody = AddComponent<CColliderRect>("BatteryIconBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CBatteryIcon::Hit);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));

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
		if (KEYPRESS("RButton"))
		{
			m_bUse = true;
			m_pInvenObj = CGameObject::FindObject("Inven");

			m_pInven = m_pInvenObj->FindComponentFromTag<CInventory>("Inven");
			m_pInven->DeleteItem(m_pObject);

			SAFE_RELEASE(m_pInvenObj);

			CGameObject*	pHandycamObj = CGameObject::FindObject("Handycam");

			CHandycam*	pHandycam = pHandycamObj->FindComponentFromTag<CHandycam>("Handycam");

			pHandycam->ChangeBattery(true);

			SAFE_RELEASE(pHandycam);
			SAFE_RELEASE(pHandycamObj);

			m_pObject->SetEnable(false);
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
