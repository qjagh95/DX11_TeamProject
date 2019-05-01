#include "../ClientHeader.h"
#include "Inventory.h"
#include "Tablet.h"
#include "TabletIcon.h"
#include "Human_Player.h"
#include "BigIcon.h"
#include <Component/ColliderRect.h>

CTabletIcon::CTabletIcon()
{
	m_eComType = (COMPONENT_TYPE)IT_TABLET;
	m_bMouseOn = false;
}

CTabletIcon::CTabletIcon(const CTabletIcon & _tabletIcon)
{
}

CTabletIcon::~CTabletIcon()
{
}

bool CTabletIcon::Init()
{
	// Transform
	m_pTransform->SetWorldScale(90.f, 90.f, 1.f);
	//m_pTransform->SetWorldPos(600.f, 429.f, 0.f);

	// Renderer, Material
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("IconRendrer");
	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->Enable2DRenderer();
	m_pObject->SetRenderGroup(RG_UI);
	SAFE_RELEASE(pRenderer);
	CMaterial* pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	pMaterial->SetDiffuseTex(0, "TabletIcon", TEXT("UI/Icon/Pills.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pMaterial);

	// Collider
	CColliderRect* pCollider = AddComponent<CColliderRect>("IconCollider");
	pCollider->SetCollisionGroup("UI");
	pCollider->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));
	pCollider->SetCollisionCallback(CCT_ENTER, this, &CTabletIcon::Hit);
	pCollider->SetCollisionCallback(CCT_STAY, this, &CTabletIcon::HitStay);
	pCollider->SetCollisionCallback(CCT_LEAVE, this, &CTabletIcon::MouseOut);
	SAFE_RELEASE(pCollider);

	return true;
}

int CTabletIcon::Input(float _fTime)
{
	return 0;
}

int CTabletIcon::Update(float _fTime)
{
	if (m_bMouseOn)
	{
		if (KEYPRESS("LButton"))
		{
			CGameObject*	pBigIconObj = CGameObject::FindObject("BigIcon");

			CBigIcon* pBigIcon = pBigIconObj->FindComponentFromType<CBigIcon>((COMPONENT_TYPE)IT_BIGICON);

			pBigIcon->ChangeClip("Tablet_Detail");
			pBigIconObj->SetEnable(true);

			SAFE_RELEASE(pBigIcon);
			SAFE_RELEASE(pBigIconObj);
		}
	}

	return 0;
}

void CTabletIcon::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CTabletIcon::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	OutputDebugString(L"Icon Hit Enter\n");
	if (KEYPRESS("RButton") == true)
	{
		if (m_isUseItem == true)
		{
			return;
		}
		m_isUseItem = true;

		CGameObject* pObjInventory = CGameObject::FindObject("Inven");
		CInventory*	pInventory = pObjInventory->FindComponentFromTag<CInventory>("Inven");
		pInventory->UseItem(m_pObject);
		SAFE_RELEASE(pObjInventory);
		SAFE_RELEASE(pInventory);

		// 플레이어 무적 상태 돌입
		CGameObject* pObjPlayer = CGameObject::FindObject("Player");
		CHuman_Player* pHumanPlayer = pObjPlayer->FindComponentFromTag<CHuman_Player>("Player");
		pHumanPlayer->SetInvincibility(true);
		pHumanPlayer->SetEnvincibilityTime(5.f);
		SAFE_RELEASE(pObjPlayer);
		SAFE_RELEASE(pHumanPlayer);
	}
}

void CTabletIcon::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
