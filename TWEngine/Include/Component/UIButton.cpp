#include "EngineHeader.h"
#include "UIButton.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "Animation2D.h"
#include "ColliderRect.h"
#include "../GameObject.h"

PUN_USING

CUIButton::CUIButton()
{
	m_eUIType = UT_BUTTON;
	SetTag("UIButton");
}

CUIButton::CUIButton(const CUIButton & button)	:
	CUI(button)
{
	m_eState = BS_NORMAL;
	memcpy(m_vBSColor, button.m_vBSColor, sizeof(Vector4) * BS_END);


	for (int i = 0; i < BS_END; ++i)
	{
		m_strSound[i] = button.m_strSound[i];
	}
}

CUIButton::~CUIButton()
{
	SAFE_RELEASE(m_pAnimation);
}

void CUIButton::Disable()
{
	m_eState = BS_DISABLE;
}

void CUIButton::Enable()
{
	m_eState = BS_NORMAL;
}

void CUIButton::SetStateColor(BUTTON_STATE eState, const Vector4 & vColor)
{
	m_vBSColor[eState] = vColor;
}

void CUIButton::SetStateColor(BUTTON_STATE eState, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vBSColor[eState] = Vector4(r, g, b, a);
}

void CUIButton::SetSound(const string & strFileName, BUTTON_STATE eState)
{
	m_strSound[eState - 1] = strFileName;

	//GET_SINGLE(CSoundManager)->LoadSound(m_pScene, false, strFileName.c_str());
}

void CUIButton::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CColliderRect>("ButtonBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CUIButton::Hit);

	SAFE_RELEASE(pRC);
}

bool CUIButton::Init()
{ 
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("ButtonRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Button", TEXT("StartScene/Start.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(115.f, 24.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("StartBtnAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(115.f, 24.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("StartBtn_Idle", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Idle_StartBtn", TEXT("StartScene/Start.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(115.f, 24.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("StartBtn_On", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"On_StartBtn", TEXT("StartScene/Start2.png"));

	vecClipFrame.clear();

	CColliderRect*	pRC = AddComponent<CColliderRect>("ButtonBody");

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(115.f, 24.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CUIButton::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CUIButton::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = BS_NORMAL;

	return true;
}

int CUIButton::Input(float fTime)
{
	//if (m_eState == BS_MOUSEON || m_eState == BS_CLICK)
	//{
	//	if (KEYPRESS("LButton"))
	//	{
	//		m_eState = BS_CLICK;

	//		if (!m_strSound[BS_CLICK - 1].empty())
	//			GET_SINGLE(CSoundManager)->Play(m_strSound[BS_CLICK - 1]);
	//	}

	//	else if (KEYUP("LButton"))
	//	{
	//		m_ClickCallback(fTime);
	//	}
	//}

	//if (m_eState == BS_MOUSEON || m_eState == BS_CLICK)
	//{
	//	m_pAnimation->ChangeClip("StartBtn_On");
	//}
	//else
	//	m_pAnimation->ChangeClip("StartBtn_Idle");

	return 0;
}

int CUIButton::Update(float fTime)
{
	return 0;
}

int CUIButton::LateUpdate(float fTime)
{
	return 0;
}

void CUIButton::Collision(float fTime)
{
}

void CUIButton::Render(float fTime)
{
}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = BS_MOUSEON;

		//if (!m_strSound[BS_MOUSEON - 1].empty())
		//	GET_SINGLE(CSoundManager)->Play(m_strSound[BS_MOUSEON - 1]);
	}
}

void CUIButton::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = BS_NORMAL;

		//if (!m_strSound[BS_NORMAL - 1].empty())
		//	GET_SINGLE(CSoundManager)->Play(m_strSound[BS_NORMAL - 1]);
	}
}

void CUIButton::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
