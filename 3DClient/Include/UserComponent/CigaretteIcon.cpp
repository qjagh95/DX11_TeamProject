#include "../ClientHeader.h"
#include "Inventory.h"
#include "Cigarette.h"
#include "CigaretteIcon.h"
#include "Human_Player.h"
#include "BigIcon.h"
#include <Component/ColliderRect.h>

CCigaretteIcon::CCigaretteIcon() :
	m_isUseItem(false),
	m_pCigarette(nullptr)
{
	m_eComType = (COMPONENT_TYPE)IT_DAEMA;
	m_bMouseOn = false;
}

CCigaretteIcon::CCigaretteIcon(const CCigaretteIcon& _cigaretteIcon)
{
}

CCigaretteIcon::~CCigaretteIcon()
{
}

bool CCigaretteIcon::Init()
{
	// Transform
	m_pTransform->SetWorldScale(86.f, 94.f, 1.f);
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
	pMaterial->SetDiffuseTex(0, "CigaretteIcon", TEXT("UI/Icon/Cigarette.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pMaterial);

	// Collider
	CColliderRect* pCollider = AddComponent<CColliderRect>("IconCollider");
	pCollider->SetCollisionGroup("UI");
	pCollider->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));
	pCollider->SetCollisionCallback(CCT_ENTER, this, &CCigaretteIcon::Hit);
	pCollider->SetCollisionCallback(CCT_STAY, this, &CCigaretteIcon::HitStay);
	pCollider->SetCollisionCallback(CCT_LEAVE, this, &CCigaretteIcon::MouseOut);
	pCollider->SetColliderID((COLLIDER_ID)UCI_ITEM_DAMBAE);
	SAFE_RELEASE(pCollider);

	return true;
}

int CCigaretteIcon::Input(float _fTime)
{
	return 0;
}

int CCigaretteIcon::Update(float _fTime)
{
	if (m_bMouseOn)
	{
		if (KEYPRESS("LButton"))
		{
			CGameObject*	pBigIconObj = CGameObject::FindObject("BigIcon");
			pBigIconObj->SetEnable(true);

			CBigIcon* pBigIcon = pBigIconObj->FindComponentFromType<CBigIcon>((COMPONENT_TYPE)IT_BIGICON);

			pBigIcon->ChangeClip("Dambae_Detail");

			SAFE_RELEASE(pBigIcon);
			SAFE_RELEASE(pBigIconObj);
		}
	}

	return 0;
}

void CCigaretteIcon::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CCigaretteIcon::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	OutputDebugString(L"Icon Hit Enter\n");
	if (KEYPRESS("RButton") == true)
	{
		if (m_isUseItem == true)
		{
			return;
		}
		m_isUseItem = true;
		cout << "들어옴" << endl;
		CGameObject* pObjInventory = CGameObject::FindObject("Inven");
		CInventory*	pInventory = pObjInventory->FindComponentFromTag<CInventory>("Inven");
		pInventory->UseItem(m_pObject);
		SAFE_RELEASE(pObjInventory);
		SAFE_RELEASE(pInventory);

		// 플레이어 체력1을 소모하여 이동속도 증가
		CGameObject* pObjPlayer = CGameObject::FindObject("Player");
		CHuman_Player* pHumanPlayer = pObjPlayer->FindComponentFromTag<CHuman_Player>("Player");
		if (pHumanPlayer->GetHP() > 2)
		{
			pHumanPlayer->RecoveryHP(-1);
		}
		pHumanPlayer->SetAccelState(true);
		pHumanPlayer->SetAccelSpeed(2.f);
		pHumanPlayer->SetAccelDuration(10.f);
		SAFE_RELEASE(pObjPlayer);
		SAFE_RELEASE(pHumanPlayer);
	}
}

void CCigaretteIcon::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
