#include "../ClientHeader.h"
#include "Inventory.h"
#include "Tablet.h"
#include "TabletIcon.h"
#include "Human_Player.h"
#include <Component/ColliderRect.h>

CTabletIcon::CTabletIcon()
{
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
	m_pTransform->SetWorldScale(90.f, 29.f, 1.f);
	m_pTransform->SetWorldPos(600.f, 429.f, 0.f);

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
	pCollider->SetCollisionCallback(CCT_STAY, this, &CTabletIcon::HitStay);
	SAFE_RELEASE(pCollider);

	return true;
}

int CTabletIcon::Input(float _fTime)
{
	return 0;
}

int CTabletIcon::Update(float _fTime)
{
	return 0;
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