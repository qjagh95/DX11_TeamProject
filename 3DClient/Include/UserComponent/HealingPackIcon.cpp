#include "../ClientHeader.h"
#include "Inventory.h"
#include "HealingPack.h"
#include "HealingPackIcon.h"
#include "Human_Player.h"
#include <Component/ColliderRect.h>

CHealingPackIcon::CHealingPackIcon() :
	m_isUseItem(false),
	m_pHealingPack(nullptr)
{
}

CHealingPackIcon::CHealingPackIcon(const CHealingPackIcon& _healingPackIcon)
{
}

CHealingPackIcon::~CHealingPackIcon()
{
}

bool CHealingPackIcon::Init()
{
	// Transform
	m_pTransform->SetWorldScale(90.f, 91.f, 1.f);
	m_pTransform->SetWorldPos(600.f, 429.f, 0.f);

	// Renderer
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("IconRendrer");
	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->Enable2DRenderer();
	m_pObject->SetRenderGroup(RG_UI);
	SAFE_RELEASE(pRenderer);

	// Collider
	CColliderRect* pCollider = AddComponent<CColliderRect>("IconCollider");
	pCollider->SetCollisionGroup("UI");
	pCollider->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));
	pCollider->SetCollisionCallback(CCT_STAY, this, &CHealingPackIcon::HitStay);
	SAFE_RELEASE(pCollider);

	return true;
}

int CHealingPackIcon::Input(float _fTime)
{
	return 0;
}

int CHealingPackIcon::Update(float _fTime)
{
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
	string strMeshName = m_pHealingPack->GetMeshKey();
	if (strMeshName == "MedicalKit")
	{
		pMaterial->SetDiffuseTex(0, "MedicalKitIcon", TEXT("UI/Icon/FoodCan.png"));
	}
	else if (strMeshName == "LunchBox")
	{
		pMaterial->SetDiffuseTex(0, "LaunchBoxIcon", TEXT("UI/Icon/ColaCan.png"));
	}
	pMaterial->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pMaterial);
}

void CHealingPackIcon::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
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

		// 플레이어 회복
		CGameObject* pObjPlayer = CGameObject::FindObject("Player");
		CHuman_Player* pHumanPlayer = pObjPlayer->FindComponentFromTag<CHuman_Player>("Player");
		pHumanPlayer->RecoveryHP(m_pHealingPack->GetHPAmount());
		SAFE_RELEASE(pObjPlayer);
		SAFE_RELEASE(pHumanPlayer);
	}
}