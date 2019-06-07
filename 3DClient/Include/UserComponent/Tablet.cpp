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
	SAFE_RELEASE(m_pOutLineTr);
	SAFE_RELEASE(m_pOutLineObj);
	SAFE_RELEASE(m_pBigTr);
	SAFE_RELEASE(m_pBigObj);
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObjItemIcon);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pObjInventory);
}

void CTablet::AfterInit()
{
}

bool CTablet::Init()
{
	m_pOutLineObj = CGameObject::CreateObject("TabletOutLine", m_pLayer);

	CRenderer*	pOutRenderer = m_pOutLineObj->AddComponent<CRenderer>("TabletOutLineRenderer");

	pOutRenderer->SetMesh("TabletOutLine", TEXT("Tablet.msh"));

	SAFE_RELEASE(pOutRenderer);

	CMaterial*	pOutMat = m_pOutLineObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pOutMat->SetMaterial(1.f, 1.f, 1.f, 3.2f, 5.f);
	pOutMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pOutMat);

	m_pOutLineTr = m_pOutLineObj->GetTransform();

	m_pOutLineTr->SetWorldScale(68.f, 67.f, 68.f);

	m_pBigObj = CGameObject::CreateObject("TabletBig", m_pLayer);

	CRenderer*	pBigRenderer = m_pBigObj->AddComponent<CRenderer>("TabletBigRenderer");

	pBigRenderer->SetMesh("TabletBig", TEXT("Tablet.msh"));

	SAFE_RELEASE(pBigRenderer);

	CMaterial*	pBigMat = m_pBigObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pBigMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pBigMat);

	m_pBigTr = m_pBigObj->GetTransform();

	m_pBigTr->SetWorldScale(66.f);

	SetOutLineVisible(false);

	// Trasnform 
	//m_pTransform->SetWorldScale(20.f, 20.f, 20.f);
	m_pTransform->SetWorldScale(22.f);

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
		SetOutLineVisible(false);

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
			SetOutLineVisible(true);
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

				// 'Inventory' 객체 얻기
				m_pObjInventory = m_pScene->FindObject("Inven");
				m_pInventory = m_pObjInventory->FindComponentFromTag<CInventory>("Inven");

				CLayer*	pUILayer = m_pScene->FindLayer("UI");
				// 아이템 아이콘 생성
				// 'HealingPackIcon' 객체 생성
				string strIconName = "";
				m_pObjItemIcon = CGameObject::CreateObject("Icon_Tablet", pUILayer, true);
				CTabletIcon* pTabletIcon = m_pObjItemIcon->AddComponent<CTabletIcon>("Icon_Tablet");
				m_pInventory->AddItem(m_pObjItemIcon);
				SAFE_RELEASE(pTabletIcon);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
				SetOutLineVisible(false);

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);
				SAFE_RELEASE(pUILayer);

				GET_SINGLE(CGameManager)->AddChangedListItemObj(m_pObject);

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

void CTablet::SetOutLineVisible(bool bEnable)
{
	m_pOutLineObj->SetEnable(bEnable);
	m_pBigObj->SetEnable(bEnable);
}

void CTablet::SetOutLinePos(const Vector3 & vPos)
{
	m_pOutLineTr->SetWorldPos(vPos);
	m_pBigTr->SetWorldPos(vPos);
}

void CTablet::SetOutLinePos(float x, float y, float z)
{
	m_pOutLineTr->SetWorldPos(Vector3(x, y, z));
	m_pBigTr->SetWorldPos(Vector3(x, y, z));
}
