#include "EngineHeader.h"
#include "ComboTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../SoundManager.h"
#include "Animation2D.h"

PUN_USING

CComboTab::CComboTab()
{
	m_eUIType = UT_TAB;
	SetTag("ComboTab");
	m_pAnimation = nullptr;
}

CComboTab::CComboTab(const CComboTab & cctab)	:
	CUI(cctab)
{
	m_eState = COTS_NORMAL;

	for (int i = 0; i < COTS_END; ++i)
	{
		m_strSound[i] = cctab.m_strSound[i];
	}
}

CComboTab::~CComboTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CComboTab::Disable()
{
	m_eState = COTS_DISABLE;
}

void CComboTab::Enable()
{
	m_eState = COTS_NORMAL;
}

void CComboTab::SetSound(const string & strFileName, COMBO_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CComboTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("CoTabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CComboTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CComboTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("CoTabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "CoTab", TEXT("UI/RightSide/Combo1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("CoTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("CoTab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_CoTab", TEXT("UI/RightSide/Combo1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("CoTab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_CoTab", TEXT("UI/RightSide/Combo2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("CoTabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CComboTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CComboTab::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = COTS_NORMAL;

	return true;
}

int CComboTab::Input(float fTime)
{
	//if (m_eState == COTS_MOUSEON || m_eState == COTS_CLICK)
	//{
	//	if (KEYPUSH("LButton"))
	//	{
	//		m_eState = COTS_CLICK;
	//		if (!m_strSound[COTS_CLICK - 1].empty())
	//			GET_SINGLE(CSoundManager)->Play(m_strSound[COTS_CLICK - 1]);
	//	}

	//	else if (KEYUP("LButton"))
	//	{
	//		//m_ClickCallback(fTime);
	//	}
	//}

	//if (m_eState == COTS_MOUSEON || m_eState == COTS_CLICK)
	//{
	//	m_pAnimation->ChangeClip("CoTab_On");
	//}
	//else
	//	m_pAnimation->ChangeClip("CoTab_Idle");

	return 0;
}

int CComboTab::Update(float fTime)
{
	return 0;
}

int CComboTab::LateUpdate(float fTime)
{
	return 0;
}

void CComboTab::Collision(float fTime)
{
}

void CComboTab::Render(float fTime)
{
}

CComboTab * CComboTab::Clone()
{
	return new CComboTab(*this);
}

void CComboTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = COTS_MOUSEON;

		if (!m_strSound[COTS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[COTS_MOUSEON - 1]);
	}
}

void CComboTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = COTS_NORMAL;
	}
}

void CComboTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
