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
	m_eComType = (COMPONENT_TYPE)IT_BATTERY;
	m_bMouseOn = false;
	m_bUse = false;
	m_iCount = 0;
}

CBatteryIcon::CBatteryIcon(const CBatteryIcon & Icon1)	:
	CItem(Icon1)
{
}

CBatteryIcon::~CBatteryIcon()
{
	SAFE_RELEASE(m_pNumber);
	SAFE_RELEASE(m_pNumObj);
	SAFE_RELEASE(m_pInvenObj);
	SAFE_RELEASE(m_pInven);
}

void CBatteryIcon::Add(int iCount)
{
	m_iCount = iCount;

	int	iResult = 0;
	int iNumber = iCount;
/*
	while (iNumber > 0)
	{
		iResult = iNumber % 10;
		iNumber /= 10;
	}*/

	m_pNumber->SetNumber(iNumber);
	m_pNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

	Vector3	vIconPos = m_pTransform->GetWorldPos();

	CTransform*	pNumTr = m_pNumObj->GetTransform();

	pNumTr->SetWorldPos(vIconPos.x + 80.f, vIconPos.y, 0.f);

	SAFE_RELEASE(pNumTr);
}

void CBatteryIcon::Use()
{
	--m_iCount;

	if (m_iCount < 0)
		m_iCount = 0;


	int	iResult = 0;
	int iNumber = m_iCount;

	while (iNumber > 0)
	{
		iResult = iNumber % 10;
		iNumber /= 10;
	}

	m_pNumber->SetNumber(iResult);
	m_pNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

	Vector3	vIconPos = m_pTransform->GetWorldPos();

	CTransform*	pNumTr = m_pNumObj->GetTransform();

	pNumTr->SetWorldPos(vIconPos.x + 80.f, vIconPos.y, 0.f);

	SAFE_RELEASE(pNumTr);
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

	m_pNumObj = CGameObject::CreateObject("BatteryNumber", m_pLayer);
	m_pNumber = m_pNumObj->AddComponent<CNumber>("BatteryNumber");

	m_pNumber->SetNumber(0);
	m_pNumber->SetNumberPivot(0.5f, 0.5f, 0.f);

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

			Use();

			if (m_iCount == 0)
			{
				m_pNumObj->SetEnable(false);
				m_pObject->SetEnable(false);
			}
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
