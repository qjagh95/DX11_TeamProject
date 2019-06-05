#include "../ClientHeader.h"
#include "PaperGongji.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "DocxInven.h"
#include "Component/ColliderSphere.h"
#include "DocxGongji.h"
#include "Human_Player.h"
#include "../GameManager.h"

CPaperGongji::CPaperGongji()
{
	m_bUseInven = false;
	m_bMouseOn = false;
	m_bOnInven = false;
	m_bMotion = false;
}

CPaperGongji::CPaperGongji(const CPaperGongji & paper)	:
	CUserComponent(paper)
{
}

CPaperGongji::~CPaperGongji()
{
	SAFE_RELEASE(m_pOutLineObj);
	SAFE_RELEASE(m_pBigObj);
	SAFE_RELEASE(m_pOutLineTr);
	SAFE_RELEASE(m_pBigTr);

	if (m_bUseInven)
	{
		SAFE_RELEASE(m_pDocxInven);
		SAFE_RELEASE(m_pDocxInvenObj);
	}
}

void CPaperGongji::AfterClone()
{
}

bool CPaperGongji::Init()
{
	m_pOutLineObj = CGameObject::CreateObject("GongjiOutLine", m_pLayer, true);

	CRenderer*	pOutRenderer = m_pOutLineObj->AddComponent<CRenderer>("GongjiRenderer");

	pOutRenderer->SetMesh("GongjiOutLine", TEXT("FileEx.msh"));

	SAFE_RELEASE(pOutRenderer);

	CMaterial*	pOutMat = m_pOutLineObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pOutMat->SetMaterial(1.f, 1.f, 1.f, 3.2f, 5.f);
	pOutMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pOutMat);

	m_pOutLineTr = m_pOutLineObj->GetTransform();

	m_pOutLineTr->SetWorldScale(0.203f, 0.202f, 0.202f);
	m_pOutLineTr->SetWorldRot(0.f, 0.f, 90.f);

	m_pBigObj = CGameObject::CreateObject("GongjiBig", m_pLayer);

	CRenderer*	pBigRenderer = m_pBigObj->AddComponent<CRenderer>("GongjiBigRenderer");

	pBigRenderer->SetMesh("GongjiBig", TEXT("FileEx.msh"));

	SAFE_RELEASE(pBigRenderer);

	CMaterial*	pBigMat = m_pBigObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pBigMat->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pBigMat);

	m_pBigTr = m_pBigObj->GetTransform();

	m_pBigTr->SetWorldScale(0.2f);
	m_pBigTr->SetWorldRot(0.f, 0.f, 90.f);

	SetOutLineVisible(false);

	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("FileRender");
	pRenderer->SetMesh("File_Gongji", TEXT("FileEx.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldRot(0.f, 0.f, 90.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("File_GongjiBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPaperGongji::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPaperGongji::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_DOC_GONGJI);

	pBody->SetInfo(Vector3::Zero, 2.f);

	SAFE_RELEASE(pBody);

	return true;
}

int CPaperGongji::Input(float fTime)
{
	return 0;
}

int CPaperGongji::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (!m_bOnInven)
		{
			if (KEYUP("F"))
			{
				m_bOnInven = true;

				m_pDocxInvenObj = CGameObject::FindObject("DocxInven");

				CGameObject*	pSMObj = CGameObject::CreateObject("DocxGongji", m_pLayer, true);

				CDocxGongji*	pSM = pSMObj->AddComponent<CDocxGongji>("DocxGongji");

				m_pDocxInven = m_pDocxInvenObj->FindComponentFromTag<CDocxInven>("DocxInven");
				m_pDocxInven->AddItem(pSMObj);

				SAFE_RELEASE(pSM);
				SAFE_RELEASE(pSMObj);

				CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

				CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
				pPlayer->ChangeRayAnim("AimOff");
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
				SetOutLineVisible(false);

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pPlayerObj);

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

int CPaperGongji::LateUpdate(float fTime)
{
	return 0;
}

void CPaperGongji::Collision(float fTime)
{
}

void CPaperGongji::Render(float fTime)
{
}

CPaperGongji * CPaperGongji::Clone()
{
	return new CPaperGongji(*this);
}

void CPaperGongji::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
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

void CPaperGongji::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}

void CPaperGongji::SetOutLineVisible(bool bEnable)
{
	m_pOutLineObj->SetEnable(bEnable);
	m_pBigObj->SetEnable(bEnable);
}

void CPaperGongji::SetOutLinePos(const Vector3 & vPos)
{
	m_pOutLineTr->SetWorldPos(vPos);
	m_pBigTr->SetWorldPos(vPos);
}

void CPaperGongji::SetOutLinePos(float x, float y, float z)
{
	m_pOutLineTr->SetWorldPos(Vector3(x + 0.15f, y, z));
	m_pBigTr->SetWorldPos(Vector3(x, y, z));
}
