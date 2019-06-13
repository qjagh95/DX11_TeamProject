#include "../ClientHeader.h"
#include "KeyCard.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "KeyInven.h"
#include "Component/ColliderSphere.h"
#include "Human_Player.h"
#include "MasterKey.h"
#include "../GameManager.h"
#include "Door.h"

CKeyCard::CKeyCard()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CKeyCard::CKeyCard(const CKeyCard & key)	:
	CUserComponent(key)
{
}

CKeyCard::~CKeyCard()
{
	SAFE_RELEASE(m_pOutLineObj);
	SAFE_RELEASE(m_pBigObj);
	SAFE_RELEASE(m_pOutLineTr);
	SAFE_RELEASE(m_pBigTr);

	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pKeyInven);
		SAFE_RELEASE(m_pKeyInvenObj);
	}
}

void CKeyCard::AfterClone()
{
}

bool CKeyCard::Init()
{
	m_pOutLineObj = CGameObject::CreateObject("KeyCardOutLine", m_pLayer);

	CRenderer*	pOutRenderer = m_pOutLineObj->AddComponent<CRenderer>("KeyCardOutLineRenderer");

	pOutRenderer->SetMesh("KeyCard", TEXT("KeyCard.msh"));

	SAFE_RELEASE(pOutRenderer);

	CMaterial*	pOutMat = m_pOutLineObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pOutMat->SetMaterial(1.f, 1.f, 1.f, 3.2f, 5.f);
	pOutMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pOutMat);

	m_pOutLineTr = m_pOutLineObj->GetTransform();

	m_pOutLineTr->SetWorldScale(0.31f, 0.29f, 0.31f);

	m_pBigObj = CGameObject::CreateObject("KeyCardBig", m_pLayer);

	CRenderer*	pBigRenderer = m_pBigObj->AddComponent<CRenderer>("KeyCardBigRenderer");

	pBigRenderer->SetMesh("KeyCard", TEXT("KeyCard.msh"));

	SAFE_RELEASE(pBigRenderer);

	CMaterial*	pBigMat = m_pBigObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pBigMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pBigMat);

	m_pBigTr = m_pBigObj->GetTransform();

	m_pBigTr->SetWorldScale(0.3f);

	SetOutLineVisible(false);

	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
	{
		pRenderer = m_pObject->AddComponent<CRenderer>("KeyCardRender");
		pRenderer->SetMesh("KeyCard", TEXT("KeyCard.msh"));
	}

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(0.1f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("KeyCardBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CKeyCard::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CKeyCard::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_ITEM_KEY);

	pBody->SetInfo(Vector3::Zero, 1.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CKeyCard::Input(float fTime)
{
	return 0;
}

int CKeyCard::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYUP("F"))
			{
				m_bOnInven = true;

				CLayer*	pUILayer = m_pScene->FindLayer("UI");

				m_pKeyInvenObj = CGameObject::FindObject("KeyInven");

				CGameObject*	pMKObj = CGameObject::CreateObject("MasterKey", pUILayer, true);

				CMasterKey*	pMK = pMKObj->AddComponent<CMasterKey>("MasterKey");

				m_pKeyInven = m_pKeyInvenObj->FindComponentFromTag<CKeyInven>("KeyInven");
				m_pKeyInven->AddItem(pMKObj);

				SAFE_RELEASE(pMK);
				SAFE_RELEASE(pMKObj);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
				SetOutLineVisible(false);
				pPlayer->TakeItem();
				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);
				SAFE_RELEASE(pUILayer);

				GET_SINGLE(CGameManager)->AddKey(m_pObject->GetTag(), m_pObject);
				GET_SINGLE(CGameManager)->AddChangedListItemObj(m_pObject);

				m_pObject->SetEnable(false);
				m_bUseInven = true;
			}
		}
	}

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

int CKeyCard::LateUpdate(float fTime)
{
	return 0;
}

void CKeyCard::Collision(float fTime)
{
}

void CKeyCard::Render(float fTime)
{
}

CKeyCard * CKeyCard::Clone()
{
	return new CKeyCard(*this);
}

void CKeyCard::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

	CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	Vector3 vPlayerPos = pPlayerTr->GetWorldPos();

	float fDist = m_pTransform->GetWorldPos().Distance(vPlayerPos);

	if (fDist < 50.f)
	{
		if (pDest->GetColliderID() == UCI_PLAYER_RAY)
		{
			m_bMouseOn = true;
			pPlayer->ChangeRayAnim("AimOn");
			GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_F_Pickup");
			SetOutLineVisible(true);
		}
	}

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
}

void CKeyCard::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}

void CKeyCard::SetOutLineVisible(bool bEnable)
{
	m_pOutLineObj->SetEnable(bEnable);
	m_pBigObj->SetEnable(bEnable);
}

void CKeyCard::SetOutLinePos(const Vector3 & vPos)
{
	m_pOutLineTr->SetWorldPos(vPos);
	m_pBigTr->SetWorldPos(vPos);
}

void CKeyCard::SetOutLinePos(float x, float y, float z)
{
	m_pOutLineTr->SetWorldPos(Vector3(x, y, z));
	m_pBigTr->SetWorldPos(Vector3(x, y, z));
}
