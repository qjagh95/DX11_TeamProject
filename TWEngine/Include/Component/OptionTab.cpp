#include "stdafx.h"
#include "OptionTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../SoundManager.h"
#include "Animation2D.h"

PUN_USING

COptionTab::COptionTab()
{
	m_eUIType = UT_TAB;
	SetTag("OptionTab");
	m_pAnimation = nullptr;
}

COptionTab::COptionTab(const COptionTab & otab)	:
	CUI(otab)
{
	m_eState = OTS_NORMAL;

	for (int i = 0; i < OTS_END; ++i)
	{
		m_strSound[i] = otab.m_strSound[i];
	}
}

COptionTab::~COptionTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void COptionTab::Disable()
{
	m_eState = OTS_DISABLE;
}

void COptionTab::Enable()
{
	m_eState = OTS_NORMAL;
}

void COptionTab::SetSound(const string & strFileName, OPTION_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void COptionTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("OTabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &COptionTab::Hit);

	SAFE_RELEASE(pRC);
}

bool COptionTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("OTabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "OTab", TEXT("UI/RightSide/Option1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("OTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("OTab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_OTab", TEXT("UI/RightSide/Option1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("OTab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_OTab", TEXT("UI/RightSide/Option2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("OTabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &COptionTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &COptionTab::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = OTS_NORMAL;

	return true;
}

int COptionTab::Input(float fTime)
{
	if (m_eState == OTS_MOUSEON || m_eState == OTS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = OTS_CLICK;
			if (!m_strSound[OTS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[OTS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == OTS_MOUSEON || m_eState == OTS_CLICK)
	{
		m_pAnimation->ChangeClip("OTab_On");
	}
	else
		m_pAnimation->ChangeClip("OTab_Idle");

	return 0;
}

int COptionTab::Update(float fTime)
{
	return 0;
}

int COptionTab::LateUpdate(float fTime)
{
	return 0;
}

void COptionTab::Collision(float fTime)
{
}

void COptionTab::Render(float fTime)
{
}

COptionTab * COptionTab::Clone()
{
	return new COptionTab(*this);
}

void COptionTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = OTS_MOUSEON;

		if (!m_strSound[OTS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[OTS_MOUSEON - 1]);
	}
}

void COptionTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = OTS_NORMAL;
	}
}

void COptionTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
