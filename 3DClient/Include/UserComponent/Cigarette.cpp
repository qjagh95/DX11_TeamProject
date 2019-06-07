#include "../ClientHeader.h"
#include "Inventory.h"
#include "Cigarette.h"
#include "CigaretteIcon.h"
#include "Human_Player.h"
#include <Component/ColliderSphere.h>
#include "../GameManager.h"

CCigarette::CCigarette() :
	m_isInvenInItem(false),
	m_bMotion(false),
	m_bGetItem(false),
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
	SAFE_RELEASE(m_pBigTr);
	SAFE_RELEASE(m_pOutLineTr);
	SAFE_RELEASE(m_pBigObj);
	SAFE_RELEASE(m_pOutLineObj);
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObjItemIcon);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pObjInventory);
}

void CCigarette::AfterInit()
{
}

bool CCigarette::Init()
{
	m_pOutLineObj = CGameObject::CreateObject("CigaOutLine", m_pLayer);

	CRenderer*	pOutRenderer = m_pOutLineObj->AddComponent<CRenderer>("CigaOutLineRenderer");

	pOutRenderer->SetMesh("CigaOutLine", TEXT("Daema.msh"));

	SAFE_RELEASE(pOutRenderer);

	CMaterial*	pOutMat = m_pOutLineObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pOutMat->SetMaterial(1.f, 1.f, 1.f, 3.2f, 5.f);
	pOutMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pOutMat);

	m_pOutLineTr = m_pOutLineObj->GetTransform();

	m_pOutLineTr->SetWorldScale(32.f, 24.f, 32.f);

	m_pBigObj = CGameObject::CreateObject("CigaBig", m_pLayer);

	CRenderer*	pBigRenderer = m_pBigObj->AddComponent<CRenderer>("CigaBigRenderer");

	pBigRenderer->SetMesh("CigaBig", TEXT("Daema.msh"));

	SAFE_RELEASE(pBigRenderer);

	CMaterial*	pBigMat = m_pBigObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pBigMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pBigMat);

	m_pBigTr = m_pBigObj->GetTransform();

	m_pBigTr->SetWorldScale(30.f);

	SetOutLineVisible(false);

	// Trasnform 
	//m_pTransform->SetWorldScale(40.f, 40.f, 40.f);
	m_pTransform->SetWorldScale(10.f, 10.f, 10.f);
	m_pTransform->SetWorldRotX(180.f);

	// Renderer
	m_pRenderer = m_pObject->AddComponent<CRenderer>("Renderer");
	m_pRenderer->SetMesh("Cigarette", TEXT("Daema.msh"));
	
	// Sphere Collider
	CColliderSphere* pCollider = m_pObject->AddComponent<CColliderSphere>("Cigarette_Collider");
	pCollider->SetInfo(Vector3::Zero, 1.f);
	pCollider->SetCollisionCallback(CCT_ENTER,  this, &CCigarette::Hit);
	pCollider->SetCollisionCallback(CCT_STAY, this, &CCigarette::HitStay);
	pCollider->SetCollisionCallback(CCT_LEAVE, this, &CCigarette::MouseOut);
	pCollider->SetColliderID((COLLIDER_ID)UCI_ITEM_DAMBAE);
	SAFE_RELEASE(pCollider);

	/**********************************************************************************************/

	return true;
}

int CCigarette::Input(float _fTime)
{
	return 0;
}

int CCigarette::Update(float _fTime)
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

void CCigarette::Hit(CCollider * _pSrc, CCollider * _pDest, float _fTime)
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

void CCigarette::HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime)
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
				m_pObjItemIcon = CGameObject::CreateObject("Icon_Cigarette", pUILayer, true);
				CCigaretteIcon* pCigaretteIcon = m_pObjItemIcon->AddComponent<CCigaretteIcon>("Icon_Cigarette");
				m_pInventory->AddItem(m_pObjItemIcon);
				SAFE_RELEASE(pCigaretteIcon);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);
				SAFE_RELEASE(pUILayer);

				GET_SINGLE(CGameManager)->AddChangedListItemObj(m_pObject);

				// 게임 화면에서 사라진다.
				m_pObject->SetEnable(false);
				SetOutLineVisible(false);
			}
		}
	}
}

void CCigarette::MouseOut(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (_pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMotion = true;
		m_bGetItem = false;
	}
}

void CCigarette::SetOutLineVisible(bool bEnable)
{
	m_pOutLineObj->SetEnable(bEnable);
	m_pBigObj->SetEnable(bEnable);
}

void CCigarette::SetOutLinePos(const Vector3 & vPos)
{
	m_pOutLineTr->SetWorldPos(vPos);
	m_pBigTr->SetWorldPos(vPos);
}

void CCigarette::SetOutLinePos(float x, float y, float z)
{
	m_pOutLineTr->SetWorldPos(Vector3(x, y, z));
	m_pBigTr->SetWorldPos(Vector3(x, y, z));
}
