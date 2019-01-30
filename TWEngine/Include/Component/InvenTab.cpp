#include "EngineHeader.h"
#include "InvenTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../SoundManager.h"

PUN_USING

CInvenTab::CInvenTab()
{
	m_eUIType = UT_TAB;
	SetTag("InvenTab");
	m_pAnimation = nullptr;
}

CInvenTab::CInvenTab(const CInvenTab & itab)	:
	CUI(itab)
{
	m_eState = ITS_NORMAL;

	for (int i = 0; i < ITS_END; ++i)
	{
		m_strSound[i] = itab.m_strSound[i];
	}
}

CInvenTab::~CInvenTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CInvenTab::Disable()
{
	m_eState = ITS_DISABLE;
}

void CInvenTab::Enable()
{
	m_eState = ITS_NORMAL;
}

void CInvenTab::SetSound(const string & strFileName, INVEN_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CInvenTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("ITabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CInvenTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CInvenTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("ITabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "ITab", TEXT("UI/RightSide/Inven1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("ITabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("ITab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_ITab", TEXT("UI/RightSide/Inven1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("ITab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_ITab", TEXT("UI/RightSide/Inven2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("ITabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CInvenTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CInvenTab::MouseOut);
	SAFE_RELEASE(pRC);

	return true;
}

int CInvenTab::Input(float fTime)
{
	return 0;
}

int CInvenTab::Update(float fTime)
{
	if (m_eState == ITS_MOUSEON || m_eState == ITS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = ITS_CLICK;
			if (!m_strSound[ITS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[ITS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == ITS_MOUSEON || m_eState == ITS_CLICK)
	{
		m_pAnimation->ChangeClip("ITab_On");
	}
	else
		m_pAnimation->ChangeClip("ITab_Idle");

	return 0;
}

int CInvenTab::LateUpdate(float fTime)
{
	return 0;
}

void CInvenTab::Collision(float fTime)
{
}

void CInvenTab::Render(float fTime)
{
}

CInvenTab * CInvenTab::Clone()
{
	return new CInvenTab(*this);
}

void CInvenTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ITS_MOUSEON;

		if (!m_strSound[ITS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[ITS_MOUSEON - 1]);
	}
}

void CInvenTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ITS_NORMAL;
	}
}

void CInvenTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
