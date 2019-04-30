#include "../ClientHeader.h"
#include "Inventory.h"
#include "Cigarette.h"
#include "CigaretteIcon.h"
#include "Human_Player.h"
#include <Component/ColliderSphere.h>

CCigarette::CCigarette() :
	m_isInvenInItem(false),
	m_pRenderer(nullptr),
	m_pObjInventory(nullptr),
	m_pInventory(nullptr),
	m_pObjItemIcon(nullptr)
{
}

CCigarette::CCigarette(const CCigarette& _cigarette)
{
}

CCigarette::~CCigarette()
{
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObjItemIcon);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pObjInventory);
}

bool CCigarette::Init()
{
	// Trasnform 
	//m_pTransform->SetWorldScale(40.f, 40.f, 40.f);
	m_pTransform->SetWorldScale(100.f, 100.f, 100.f);
	m_pTransform->SetWorldRotX(180.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");
	m_pRenderer->SetMesh("Cigarette", TEXT("Daema.msh"));
	
	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("Cigarette_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.5f);
	pCollider->SetCollisionCallback(CCT_STAY,  this, &CCigarette::HitStay);
	SAFE_RELEASE(pCollider);

	/**********************************************************************************************/

	// 'Inventory' 객체 얻기
	m_pObjInventory = m_pScene->FindObject("Inven");
	m_pInventory = m_pObjInventory->FindComponentFromTag<CInventory>("Inven");

	return true;
}

int CCigarette::Input(float _fTime)
{
	return 0;
}

int CCigarette::Update(float _fTime)
{
	return 0;
}

void CCigarette::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (KEYPRESS("LButton") == true)
	{
		if (m_isInvenInItem == false)
		{
			m_isInvenInItem = true;
			
			// 아이템 아이콘 생성
			// 'HealingPackIcon' 객체 생성
			string strIconName = "";
			m_pObjItemIcon = CGameObject::CreateObject("Icon_Cigarette", m_pLayer);
			CCigaretteIcon* pCigaretteIcon = m_pObjItemIcon->AddComponent<CCigaretteIcon>("Icon_Cigarette");
			m_pInventory->AddItem(m_pObjItemIcon);
			SAFE_RELEASE(pCigaretteIcon);

			// 게임 화면에서 사라진다.
			m_pObject->SetEnable(false);
		}
	}
}