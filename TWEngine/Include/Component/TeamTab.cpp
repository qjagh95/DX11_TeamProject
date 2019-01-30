#include "EngineHeader.h"
#include "TeamTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../SoundManager.h"
#include "Animation2D.h"

PUN_USING

CTeamTab::CTeamTab()
{
	m_eUIType = UT_TAB;
	SetTag("TeamTab");
	m_pAnimation = nullptr;
}

CTeamTab::CTeamTab(const CTeamTab & ttab)	:
	CUI(ttab)
{
	m_eState = TTS_NORMAL;

	for (int i = 0; i < TTS_END; ++i)
	{
		m_strSound[i] = ttab.m_strSound[i];
	}
}

CTeamTab::~CTeamTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CTeamTab::Disable()
{
	m_eState = TTS_DISABLE;
}

void CTeamTab::Enable()
{
	m_eState = TTS_NORMAL;
}

void CTeamTab::SetSound(const string & strFileName, TEAM_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CTeamTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("TTabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CTeamTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CTeamTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("TTabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "TTab", TEXT("UI/RightSide/Team1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("TTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("TTab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_TTab", TEXT("UI/RightSide/Team1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("TTab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_TTab", TEXT("UI/RightSide/Team2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("TTabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CTeamTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CTeamTab::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = TTS_NORMAL;

	return true;
}

int CTeamTab::Input(float fTime)
{
	if (m_eState == TTS_MOUSEON || m_eState == TTS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = TTS_CLICK;
			if (!m_strSound[TTS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[TTS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == TTS_MOUSEON || m_eState == TTS_CLICK)
	{
		m_pAnimation->ChangeClip("TTab_On");
	}
	else
		m_pAnimation->ChangeClip("TTab_Idle");

	return 0;
}

int CTeamTab::Update(float fTime)
{
	return 0;
}

int CTeamTab::LateUpdate(float fTime)
{
	return 0;
}

void CTeamTab::Collision(float fTime)
{
}

void CTeamTab::Render(float fTime)
{
}

CTeamTab * CTeamTab::Clone()
{
	return new CTeamTab(*this);
}

void CTeamTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = TTS_MOUSEON;

		if (!m_strSound[TTS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[TTS_MOUSEON - 1]);
	}
}

void CTeamTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = TTS_NORMAL;
	}
}

void CTeamTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
