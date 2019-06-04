#include "EngineHeader.h"
#include "UIButton.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "ColliderRect.h"
#include "../GameObject.h"
#include "SoundManager.h"

PUN_USING

CUIButton::CUIButton()
{
	m_eComType = CT_UI;
	m_eUIType = UT_BUTTON;
	SetTag("UIButton");
}

CUIButton::CUIButton(const CUIButton & button) :
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
	CSoundManager::GetInst()->CreateSoundEffect("MouseOn", TEXT("commonMusic/MouseClick.wav"));
		
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("StartButtonRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetShader(BUTTON_SHADER);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("Button", sizeof(m_tCBuffer));
	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "StartButton", TEXT("Scene/Start.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(258.f, 55.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	CColliderRect*	pRC = AddComponent<CColliderRect>("StartButtonBody");

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(258.f, 55.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CUIButton::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CUIButton::MouseOut);

	SAFE_RELEASE(pRC);	

	m_eState = BS_NORMAL;
	m_vBSColor[BS_DISABLE] = Vector4(0.5f, 0.5f, 0.5f, 1.f);
	m_vBSColor[BS_NORMAL] = Vector4(0.7f, 0.7f, 0.7f, 1.f);
	m_vBSColor[BS_MOUSEON] = Vector4(2.f, 2.f, 2.f, 1.f);
	m_vBSColor[BS_CLICK] = Vector4(0.7f, 0.7f, 0.7f, 1.f);

	m_pObject->SetRenderGroup(RG_UI);

	return true;
}

int CUIButton::Input(float fTime)
{
	if (m_eState == BS_MOUSEON || m_eState == BS_CLICK)
	{
		if (CInput::GetInst()->KeyPress("LButton"))
			m_eState = BS_CLICK;

		else if (CInput::GetInst()->KeyRelease("LButton"))
			m_ClickCallback(fTime);
	}

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
	m_tCBuffer.vColor = m_vBSColor[m_eState];

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Button", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
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
		CSoundManager::GetInst()->SoundPlay("MouseOn");
	}
}

void CUIButton::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = BS_NORMAL;
	}
}

void CUIButton::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
