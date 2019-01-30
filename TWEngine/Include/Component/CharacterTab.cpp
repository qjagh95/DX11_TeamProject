#include "EngineHeader.h"
#include "CharacterTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "Animation2D.h"

PUN_USING

CCharacterTab::CCharacterTab()
{
	m_eUIType = UT_TAB;
	SetTag("CharacterTab");
	m_pAnimation = nullptr;
}

CCharacterTab::CCharacterTab(const CCharacterTab & ctab)	:
	CUI(ctab)
{
	m_eState = CTS_NORMAL;

	for (int i = 0; i < CTS_END; ++i)
	{
		m_strSound[i] = ctab.m_strSound[i];
	}
}

CCharacterTab::~CCharacterTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CCharacterTab::Disable()
{
	m_eState = CTS_DISABLE;
}

void CCharacterTab::Enable()
{
	m_eState = CTS_NORMAL;
}

void CCharacterTab::SetSound(const string & strFileName, CHARACTER_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CCharacterTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("CTabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CCharacterTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CCharacterTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("CTabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "CTab", TEXT("UI/RightSide/Charc1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("CTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};
		
	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);	

	m_pAnimation->AddClip("CTab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_CTab", TEXT("UI/RightSide/Charc1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("CTab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_CTab", TEXT("UI/RightSide/Charc2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("CTabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CCharacterTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CCharacterTab::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = CTS_NORMAL;

	return true;
}

int CCharacterTab::Input(float fTime)
{
	if (m_eState == CTS_MOUSEON || m_eState == CTS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = CTS_CLICK;
			if (!m_strSound[CTS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[CTS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == CTS_MOUSEON || m_eState == CTS_CLICK)
	{
		m_pAnimation->ChangeClip("CTab_On");
	}
	else
		m_pAnimation->ChangeClip("CTab_Idle");

	return 0;
}

int CCharacterTab::Update(float fTime)
{
	return 0;
}

int CCharacterTab::LateUpdate(float fTime)
{
	return 0;
}

void CCharacterTab::Collision(float fTime)
{
}

void CCharacterTab::Render(float fTime)
{
}

CCharacterTab * CCharacterTab::Clone()
{
	return new CCharacterTab(*this);
}

void CCharacterTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CTS_MOUSEON;

		if (!m_strSound[CTS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[CTS_MOUSEON - 1]);
	}
}

void CCharacterTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CTS_NORMAL;
	}
}

void CCharacterTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
