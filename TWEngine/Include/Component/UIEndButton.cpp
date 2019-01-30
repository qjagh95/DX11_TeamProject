#include "EngineHeader.h"
#include "UIEndButton.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"

PUN_USING

CUIEndButton::CUIEndButton()
{
	m_eUIType = UT_BUTTON;
	SetTag("UIEndButton");
}

CUIEndButton::CUIEndButton(const CUIEndButton & endbutton)	:
	CUI(endbutton)
{
	m_eState = EBS_NORMAL;
	memcpy(m_vBSColor, endbutton.m_vBSColor, sizeof(Vector4) * EBS_END);


	for (int i = 0; i < BS_END; ++i)
	{
		m_strSound[i] = endbutton.m_strSound[i];
	}
}

CUIEndButton::~CUIEndButton()
{
	SAFE_RELEASE(m_pAnimation);
}

void CUIEndButton::Disable()
{
	m_eState = EBS_DISABLE;
}

void CUIEndButton::Enable()
{
	m_eState = EBS_NORMAL;
}

void CUIEndButton::SetSound(const string & strFileName, END_BUTTON_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CUIEndButton::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CColliderRect>("EndButtonBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CUIEndButton::Hit);

	SAFE_RELEASE(pRC);
}

bool CUIEndButton::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("EndButtonRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "EndButton", TEXT("StartScene/Exit.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(115.f, 24.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("EndBtnAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(115.f, 24.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("EndBtn_Idle", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Idle_EndBtn", TEXT("StartScene/Exit.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(115.f, 24.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("EndBtn_On", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"On_EndBtn", TEXT("StartScene/Exit2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("EndButtonBody");

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(115.f, 24.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CUIEndButton::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CUIEndButton::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = EBS_NORMAL;

	return true;
}

int CUIEndButton::Input(float fTime)
{
	//if (m_eState == EBS_MOUSEON || m_eState == EBS_CLICK)
	//{
	//	if (KEYPRESS("LButton"))
	//	{
	//		m_eState = EBS_CLICK;

	//		if (!m_strSound[EBS_CLICK - 1].empty())
	//			GET_SINGLE(CSoundManager)->Play(m_strSound[EBS_CLICK - 1]);
	//	}

	//	else if (KEYUP("LButton"))
	//	{
	//		m_ClickCallback(fTime);
	//	}
	//}

	//if (m_eState == EBS_MOUSEON || m_eState == EBS_CLICK)
	//{
	//	m_pAnimation->ChangeClip("EndBtn_On");
	//}
	//else
	//	m_pAnimation->ChangeClip("EndBtn_Idle");

	return 0;
}

int CUIEndButton::Update(float fTime)
{
	return 0;
}

int CUIEndButton::LateUpdate(float fTime)
{
	return 0;
}

void CUIEndButton::Collision(float fTime)
{
}

void CUIEndButton::Render(float fTime)
{
}

CUIEndButton * CUIEndButton::Clone()
{
	return new CUIEndButton(*this);
}

void CUIEndButton::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = EBS_MOUSEON;

		if (!m_strSound[EBS_MOUSEON - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[EBS_MOUSEON - 1]);
	}
}

void CUIEndButton::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = EBS_NORMAL;

		if (!m_strSound[EBS_NORMAL - 1].empty())
			GET_SINGLE(CSoundManager)->Play(m_strSound[EBS_NORMAL - 1]);
	}
}

void CUIEndButton::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
