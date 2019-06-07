#include "../ClientHeader.h"
#include "Inventory.h"
#include "HealingPack.h"
#include "HealingPackIcon.h"
#include "Human_Player.h"
#include "BigIcon.h"
#include <Component/ColliderRect.h>

CHealingPackIcon::CHealingPackIcon() :
	m_isUseItem(false),
	m_pHealingPack(nullptr)
{
	 m_bLunchBoxMouseOn = false;
	 m_bMedicalKitMouseOn = false;
}

CHealingPackIcon::CHealingPackIcon(const CHealingPackIcon& _healingPackIcon)
{
}

CHealingPackIcon::~CHealingPackIcon()
{
	SAFE_RELEASE(m_pCollider);
}

bool CHealingPackIcon::Init()
{
	// Transform
	m_pTransform->SetWorldScale(90.f, 91.f, 1.f);
	//m_pTransform->SetWorldPos(600.f, 429.f, 0.f);

	// Renderer
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("IconRendrer");
	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->Enable2DRenderer();
	m_pObject->SetRenderGroup(RG_UI);
	SAFE_RELEASE(pRenderer);

	// Collider
	m_pCollider = AddComponent<CColliderRect>("IconCollider");
	m_pCollider->SetCollisionGroup("UI");
	m_pCollider->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));
	m_pCollider->SetCollisionCallback(CCT_ENTER, this, &CHealingPackIcon::Hit);
	m_pCollider->SetCollisionCallback(CCT_STAY, this, &CHealingPackIcon::HitStay);
	m_pCollider->SetCollisionCallback(CCT_LEAVE, this, &CHealingPackIcon::MouseOut);

	return true;
}

int CHealingPackIcon::Input(float _fTime)
{
	return 0;
}

int CHealingPackIcon::Update(float _fTime)
{
	if ((m_bLunchBoxMouseOn == true) || (m_bMedicalKitMouseOn == true))
	{
		if (KEYUP("LButton"))
		{
			CGameObject*	pBigIconObj = CGameObject::FindObject("BigIcon");
			pBigIconObj->SetEnable(true);

			CBigIcon* pBigIcon = pBigIconObj->FindComponentFromType<CBigIcon>((COMPONENT_TYPE)IT_BIGICON);

			if(m_bMedicalKitMouseOn == true)
				pBigIcon->ChangeClip("FoodCan_Detail");

			else if (m_bLunchBoxMouseOn == true)
				pBigIcon->ChangeClip("Cola_Detail");

			SAFE_RELEASE(pBigIcon);
			SAFE_RELEASE(pBigIconObj);
		}
	}

	return 0;
}

void CHealingPackIcon::SetHealingPackInst(const CHealingPack* _pInst)
{
	m_pHealingPack = const_cast<CHealingPack*>(_pInst);
}

void CHealingPackIcon::SetMaterial()
{
	// Material 정보는 m_pHealingPack 객체를 이용하기 때문에 별도로 호출해서 처리한다.
	// Init 함수에서 m_pHealingPck 객체를 초기화할 수 있는 방법이 없다.
	CMaterial* pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	string m_strMeshName = m_pHealingPack->GetMeshKey();
	if (m_strMeshName == "MedicalKit")
	{
		m_eComType = (COMPONENT_TYPE)IT_MEDICALKIT;
		m_pCollider->SetColliderID((COLLIDER_ID)UCI_ITEM_FAK);
		pMaterial->SetDiffuseTex(0, "MedicalKitIcon", TEXT("UI/Icon/FirstAidKit.png"));
	}
	else if (m_strMeshName == "LunchBox")
	{
		m_eComType = (COMPONENT_TYPE)IT_LUNCHBOX;
		m_pCollider->SetColliderID((COLLIDER_ID)UCI_ITEM_LUNCHBOX);
		pMaterial->SetDiffuseTex(0, "LaunchBoxIcon", TEXT("UI/Icon/Lunchbox.png"));
	}
	pMaterial->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pMaterial);
}

void CHealingPackIcon::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		if (m_eComType == (COMPONENT_TYPE)IT_MEDICALKIT)
		{
			m_bMedicalKitMouseOn = true;
		}

		else if (m_eComType == (COMPONENT_TYPE)IT_LUNCHBOX)
		{
			m_bLunchBoxMouseOn = true;
		}
	}
}

void CHealingPackIcon::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (KEYUP("RButton") == true)
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

		// 플레이어 회복
		CGameObject* pObjPlayer = CGameObject::FindObject("Player");
		CHuman_Player* pHumanPlayer = pObjPlayer->FindComponentFromTag<CHuman_Player>("Player");
		pHumanPlayer->RecoveryHP(m_pHealingPack->GetHPAmount());
		SAFE_RELEASE(pObjPlayer);
		SAFE_RELEASE(pHumanPlayer);
	}
}

void CHealingPackIcon::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		if (m_eComType == (COMPONENT_TYPE)IT_MEDICALKIT)
		{
			m_bMedicalKitMouseOn = false;
		}

		else if (m_eComType == (COMPONENT_TYPE)IT_LUNCHBOX)
		{
			m_bLunchBoxMouseOn = false;
		}
	}
}
