#include "stdafx.h"
#include "ClubTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../SoundManager.h"
#include "Animation2D.h"

PUN_USING

CClubTab::CClubTab()
{
	m_eUIType = UT_TAB;
	SetTag("ClubTab");
	m_pAnimation = nullptr;
}

CClubTab::CClubTab(const CClubTab & cltab)	:
	CUI(cltab)
{
	m_eState = CLTS_NORMAL;

	for (int i = 0; i < CLTS_END; ++i)
	{
		m_strSound[i] = cltab.m_strSound[i];
	}
}

CClubTab::~CClubTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CClubTab::Disable()
{
	m_eState = CLTS_DISABLE;
}

void CClubTab::Enable()
{
	m_eState = CLTS_NORMAL;
}

void CClubTab::SetSound(const string & strFileName, CLUB_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CClubTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("CLTabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CClubTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CClubTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("CLTabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "CLTab", TEXT("UI/RightSide/Club1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("CTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("CLTab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_CLTab", TEXT("UI/RightSide/Club1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("CLTab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_CLTab", TEXT("UI/RightSide/Club2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("CLTabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CClubTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CClubTab::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = CLTS_NORMAL;

	return true;
}

int CClubTab::Input(float fTime)
{
	if (m_eState == CLTS_MOUSEON || m_eState == CLTS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = CLTS_CLICK;
			if (!m_strSound[CLTS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[CLTS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == CLTS_MOUSEON || m_eState == CLTS_CLICK)
	{
		m_pAnimation->ChangeClip("CLTab_On");
	}
	else
		m_pAnimation->ChangeClip("CLTab_Idle");

	return 0;
}

int CClubTab::Update(float fTime)
{

	return 0;
}

int CClubTab::LateUpdate(float fTime)
{
	return 0;
}

void CClubTab::Collision(float fTime)
{
}

void CClubTab::Render(float fTime)
{
}

CClubTab * CClubTab::Clone()
{
	return new CClubTab(*this);
}

void CClubTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CLTS_MOUSEON;

		if (!m_strSound[CLTS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[CLTS_MOUSEON - 1]);
	}
}

void CClubTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CLTS_NORMAL;
	}
}

void CClubTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
