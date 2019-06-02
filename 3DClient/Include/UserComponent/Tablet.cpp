#include "../ClientHeader.h"
#include "Inventory.h"
#include "Tablet.h"
#include "TabletIcon.h"
#include "Human_Player.h"
#include <Component/ColliderSphere.h>
#include "../GameManager.h"

CTablet::CTablet() :
	m_isInvenInItem(false),
	m_bMotion(false),
	m_bGetItem(false),
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
	m_pTransform->SetWorldScale(66.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");
	m_pRenderer->SetMesh("Tablet", TEXT("Tablet.msh"));

	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("Tablet_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.f);
	pCollider->SetCollisionCallback(CCT_ENTER, this, &CTablet::Hit);
	pCollider->SetCollisionCallback(CCT_STAY, this, &CTablet::HitStay);
	pCollider->SetCollisionCallback(CCT_LEAVE, this, &CTablet::MouseOut);
	pCollider->SetColliderID((COLLIDER_ID)UCI_ITEM_TABLET);
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
	if (m_bMotion)
	{
		CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

		CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
		pPlayer->ChangeRayAnim("AimOff");
		GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

		m_bMotion = false;

		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	return 0;
}

void CTablet::Hit(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

	CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	Vector3 vPlayerPos = pPlayerTr->GetWorldPos();

	float fDist = m_pTransform->GetWorldPos().Distance(vPlayerPos);

	if (fDist < 50.f)
	{
		if (_pDest->GetColliderID() == UCI_PLAYER_RAY)
		{
			pPlayer->ChangeRayAnim("AimOn");
			GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_F_Pickup");
			m_bGetItem = true;
		}
	}

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
}

void CTablet::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
{
	if (m_bGetItem)
	{
		if (KEYUP("F") == true)
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

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

				// 게임 화면에서 사라진다.
				m_pObject->SetEnable(false);
			}
		}
	}
}

void CTablet::MouseOut(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (_pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMotion = true;
		m_bGetItem = false;
	}
}
