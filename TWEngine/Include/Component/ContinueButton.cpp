#include "EngineHeader.h"
#include "ContinueButton.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "ColliderRect.h"
#include "../GameObject.h"

PUN_USING

CContinueButton::CContinueButton()
{
	m_eComType = CT_UI;
	m_eUIType = UT_BUTTON;
	SetTag("ContinueButton");
}

CContinueButton::CContinueButton(const CContinueButton & button)	:
	CUI(button)
{
	m_eState = CS_NORMAL;
	memcpy(m_vBSColor, button.m_vBSColor, sizeof(Vector4) * CS_END);

	for (int i = 0; i < CS_END; ++i)
	{
		m_strSound[i] = button.m_strSound[i];
	}
}

CContinueButton::~CContinueButton()
{
}

void CContinueButton::Disable()
{
	m_eState = CBS_DISABLE;
}

void CContinueButton::Enable()
{
	m_eState = CS_NORMAL;
}

void CContinueButton::SetStateColor(CBUTTON_STATE eState, const Vector4 & vColor)
{
	m_vBSColor[eState] = vColor;
}

void CContinueButton::SetStateColor(CBUTTON_STATE eState, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vBSColor[eState] = Vector4(r, g, b, a);
}

void CContinueButton::SetSound(const string & strFileName, CBUTTON_STATE eState)
{
	m_strSound[eState - 1] = strFileName;
}

void CContinueButton::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CColliderRect>("ContinueButtonBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CContinueButton::Hit);

	SAFE_RELEASE(pRC);
}

bool CContinueButton::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("MouseOn", TEXT("commonMusic/MouseClick.wav"));

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("ContinueButtonRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetShader(BUTTON_SHADER);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("Button", sizeof(ButtonCBuffer));
	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "ContinueButton", TEXT("Scene/Continue.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(258.f, 55.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	CColliderRect*	pRC = AddComponent<CColliderRect>("ContinueButtonBody");

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(258.f, 55.f, 1.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CContinueButton::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CContinueButton::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = CS_NORMAL;
	m_vBSColor[CBS_DISABLE] = Vector4(0.5f, 0.5f, 0.5f, 1.f);
	m_vBSColor[CS_NORMAL] = Vector4(0.7f, 0.7f, 0.7f, 1.f);
	m_vBSColor[CS_MOUSEON] = Vector4(2.f, 2.f, 2.f, 1.f);
	m_vBSColor[CS_CLICK] = Vector4(0.7f, 0.7f, 0.7f, 1.f);

	m_pObject->SetRenderGroup(RG_UI);

	return true;
}

int CContinueButton::Input(float fTime)
{
	if (m_eState == CS_MOUSEON || m_eState == CS_CLICK)
	{
		if (CInput::GetInst()->KeyPress("LButton"))
			m_eState = CS_CLICK;

		else if (CInput::GetInst()->KeyRelease("LButton"))
			m_ClickCallback(fTime);
	}

	return 0;
}

int CContinueButton::Update(float fTime)
{
	return 0;
}

int CContinueButton::LateUpdate(float fTime)
{
	return 0;
}

void CContinueButton::Collision(float fTime)
{
}

void CContinueButton::Render(float fTime)
{
	m_tCBuffer.vColor = m_vBSColor[m_eState];

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Button", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
}

CContinueButton * CContinueButton::Clone()
{
	return new CContinueButton(*this);
}

void CContinueButton::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CS_MOUSEON;

		CSoundManager::GetInst()->SoundPlay("MouseOn");
		//if (!m_strSound[BS_MOUSEON - 1].empty())
		//	GET_SINGLE(CSoundManager)->Play(m_strSound[BS_MOUSEON - 1]);
	}
}

void CContinueButton::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = CS_NORMAL;

		//if (!m_strSound[BS_NORMAL - 1].empty())
		//	GET_SINGLE(CSoundManager)->Play(m_strSound[BS_NORMAL - 1]);
	}
}

void CContinueButton::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
