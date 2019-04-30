#include "../ClientHeader.h"
#include "Inventory.h"
#include "Tablet.h"
#include "TabletIcon.h"
#include "Human_Player.h"
#include <Component/ColliderSphere.h>

CTablet::CTablet() :
	m_isInvenInItem(false),
	m_pRenderer(nullptr),
	m_pObjInventory(nullptr),
	m_pInventory(nullptr),
	m_pObjItemIcon(nullptr)
{
}

CTablet::CTablet(const CTablet& _tablet)
{
}

CTablet::~CTablet()
{
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObjItemIcon);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pObjInventory);
}

bool CTablet::Init()
{
	// Trasnform 
	//m_pTransform->SetWorldScale(20.f, 20.f, 20.f);
	m_pTransform->SetWorldScale(100.f, 100.f, 100.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");
	m_pRenderer->SetMesh("Tablet", TEXT("Tablet.msh"));

	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("Tablet_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.5f);
	pCollider->SetCollisionCallback(CCT_STAY, this, &CTablet::HitStay);
	SAFE_RELEASE(pCollider);

	/**********************************************************************************************/

	// 'Inventory' 객체 얻기
	m_pObjInventory = m_pScene->FindObject("Inven");
	m_pInventory = m_pObjInventory->FindComponentFromTag<CInventory>("Inven");

	return true;
}

int CTablet::Input(float _fTime)
{
	return 0;
}

int CTablet::Update(float _fTime)
{
	return 0;
}

void CTablet::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (KEYPRESS("LButton") == true)
	{
		if (m_isInvenInItem == false)
		{
			m_isInvenInItem = true;

			// 아이템 아이콘 생성
			// 'HealingPackIcon' 객체 생성
			string strIconName = "";
			m_pObjItemIcon = CGameObject::CreateObject("Icon_Tablet", m_pLayer);
			CTabletIcon* pTabletIcon = m_pObjItemIcon->AddComponent<CTabletIcon>("Icon_Tablet");
			m_pInventory->AddItem(m_pObjItemIcon);
			SAFE_RELEASE(pTabletIcon);

			// 게임 화면에서 사라진다.
			m_pObject->SetEnable(false);
		}
	}
}