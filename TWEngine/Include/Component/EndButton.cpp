#include "EngineHeader.h"
#include "EndButton.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "ColliderRect.h"
#include "../GameObject.h"

PUN_USING

CEndButton::CEndButton()
{
	m_eComType = CT_UI;
	m_eUIType = UT_BUTTON;
	SetTag("UIEndButton");
}

CEndButton::CEndButton(const CEndButton & button)	:
	CUI(button)
{
	m_eState = ES_NORMAL;
	memcpy(m_vBSColor, button.m_vBSColor, sizeof(Vector4) * ES_END);


	for (int i = 0; i < ES_END; ++i)
	{
		m_strSound[i] = button.m_strSound[i];
	}
}

CEndButton::~CEndButton()
{
}

void CEndButton::Disable()
{
	m_eState = ES_DISABLE;
}

void CEndButton::Enable()
{
	m_eState = ES_NORMAL;
}

void CEndButton::SetStateColor(EBUTTON_STATE eState, const Vector4 & vColor)
{
	m_vBSColor[eState] = vColor;
}

void CEndButton::SetStateColor(EBUTTON_STATE eState, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vBSColor[eState] = Vector4(r, g, b, a);
}

void CEndButton::SetSound(const string & strFileName, EBUTTON_STATE eState)
{
	m_strSound[eState - 1] = strFileName;
}

void CEndButton::AfterClone()
{
	CColliderRect*	pRC = (CColliderRect*)FindComponentFromTag<CColliderRect>("EndButtonBody");

	pRC->SetCollisionCallback(CCT_ENTER, this, &CEndButton::Hit);

	SAFE_RELEASE(pRC);
}

bool CEndButton::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("MouseOn", TEXT("commonMusic/MouseClick.wav"));

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("EndButtonRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetShader(BUTTON_SHADER);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("Button", sizeof(ButtonCBuffer));
	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "EndButton", TEXT("Scene/End.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(258.f, 55.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	CColliderRect*	pRC = AddComponent<CColliderRect>("EndButtonBody");

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(258.f, 55.f, 1.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CEndButton::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CEndButton::MouseOut);

	SAFE_RELEASE(pRC);

	m_eState = ES_NORMAL;
	m_vBSColor[ES_DISABLE] = Vector4(0.5f, 0.5f, 0.5f, 1.f);
	m_vBSColor[ES_NORMAL] = Vector4(0.7f, 0.7f, 0.7f, 1.f);
	m_vBSColor[ES_MOUSEON] = Vector4(2.f, 2.f, 2.f, 1.f);
	m_vBSColor[ES_CLICK] = Vector4(0.7f, 0.7f, 0.7f, 1.f);

	m_pObject->SetRenderGroup(RG_UI);

	return true;
}

int CEndButton::Input(float fTime)
{
	if (m_eState == ES_MOUSEON || m_eState == ES_CLICK)
	{
		if (CInput::GetInst()->KeyPress("LButton"))
			m_eState = ES_CLICK;

		else if (CInput::GetInst()->KeyRelease("LButton"))
			m_ClickCallback(fTime);
	}

	return 0;
}

int CEndButton::Update(float fTime)
{
	return 0;
}

int CEndButton::LateUpdate(float fTime)
{
	return 0;
}

void CEndButton::Collision(float fTime)
{
}

void CEndButton::Render(float fTime)
{
	m_tCBuffer.vColor = m_vBSColor[m_eState];

	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Button", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
}

CEndButton * CEndButton::Clone()
{
	return new CEndButton(*this);
}

void CEndButton::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ES_MOUSEON;

		CSoundManager::GetInst()->SoundPlay("MouseOn");
	}
}

void CEndButton::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = ES_NORMAL;
	}
}

void CEndButton::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}
