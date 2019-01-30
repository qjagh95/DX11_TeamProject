#include "EngineHeader.h"
#include "EquipTab.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../SoundManager.h"
#include "../Input.h"

PUN_USING

CEquipTab::CEquipTab()
{
	m_eUIType = UT_TAB;
	SetTag("EquipTab");
	m_pAnimation = nullptr;
}

CEquipTab::CEquipTab(const CEquipTab & etab)	:
	CUI(etab)
{
	m_eState = ETS_NORMAL;

	for (int i = 0; i < ETS_END; ++i)
	{
		m_strSound[i] = etab.m_strSound[i];
	}
}

CEquipTab::~CEquipTab()
{
	SAFE_RELEASE(m_pCollider);
	SAFE_RELEASE(m_pAnimation);
}

void CEquipTab::Disable()
{
	m_eState = ETS_DISABLE;
}

void CEquipTab::Enable()
{
	m_eState = ETS_NORMAL;
}

void CEquipTab::SetSound(const string & strFileName, EQUIP_TAB_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CEquipTab::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CCollider>("ETabBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CEquipTab::Hit);

	SAFE_RELEASE(pRC);
}

bool CEquipTab::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("ETabRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "ETab", TEXT("UI/RightSide/Equip1.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(20.f, 56.f, 1.f);
	m_pTransform->SetWorldPivot(1.f, 1.f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("CTabAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("ETab_Idle", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"Idle_ETab", TEXT("UI/RightSide/Equip1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(18.f, 51.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("ETab_On", A2D_ATLAS, AO_LOOP, 2.f, vecClipFrame,
		"On_ETab", TEXT("UI/RightSide/Equip2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("ETabBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(20.f, 56.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CEquipTab::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CEquipTab::MouseOut);
	SAFE_RELEASE(pRC);

	return true;
}

int CEquipTab::Input(float fTime)
{
	return 0;
}

int CEquipTab::Update(float fTime)
{
	if (m_eState == ETS_MOUSEON || m_eState == ETS_CLICK)
	{
		if (KEYPUSH("LButton"))
		{
			m_eState = ETS_CLICK;
			if (!m_strSound[ETS_CLICK - 1].empty())
				GET_SINGLE(CSoundManager)->Play(m_strSound[ETS_CLICK - 1]);
		}

		else if (KEYUP("LButton"))
		{
			//m_ClickCallback(fTime);
		}
	}

	if (m_eState == ETS_MOUSEON || m_eState == ETS_CLICK)
	{
		m_pAnimation->ChangeClip("ETab_On");
	}
	else
		m_pAnimation->ChangeClip("ETab_Idle");

	return 0;
}

int CEquipTab::LateUpdate(float fTime)
{
	return 0;
}

void CEquipTab::Collision(float fTime)
{
}

void CEquipTab::Render(float fTime)
{
}

CEquipTab * CEquipTab::Clone()
{
	return new CEquipTab(*this);
}

void CEquipTab::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ETS_MOUSEON;

		if (!m_strSound[ETS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[ETS_MOUSEON - 1]);
	}
}

void CEquipTab::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ETS_NORMAL;
	}
}

void CEquipTab::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
