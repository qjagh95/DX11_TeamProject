#include "../ClientHeader.h"
#include "HitEffectAlpha.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Rendering/RenderManager.h"

CHitEffectAlpha::CHitEffectAlpha()
{
	m_eComType = (COMPONENT_TYPE)UT_EFFECT;
	m_fElapsedTime = 0.f;
	m_fPlayTime = 1.f;
	m_fAlpha = 1.f;
	m_iFlag = -1;
}

CHitEffectAlpha::CHitEffectAlpha(const CHitEffectAlpha & effect)	:
	CUserComponent(effect)
{
}

CHitEffectAlpha::~CHitEffectAlpha()
{
}

void CHitEffectAlpha::AfterClone()
{
}

bool CHitEffectAlpha::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("HitEffectAlphaRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "HitEffectAlpha", TEXT("Effect/bloodyrect4.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*	pTr = m_pTransform->GetTransform();

	pTr->SetWorldScale(1280.f, 720.f, 0.f);

	SAFE_RELEASE(pTr);

	return true;
}

int CHitEffectAlpha::Input(float fTime)
{
	return 0;
}

int CHitEffectAlpha::Update(float fTime)
{
	m_fElapsedTime += fTime;

	m_fAlpha -= m_fElapsedTime;
	//GET_SINGLE(CRenderManager)->SetHitEffectAlpha(m_fAlpha, fTime, true);

	if (m_fElapsedTime > 1.f)
	{
		m_fElapsedTime = 0.f;
	}

	return 0;
}

int CHitEffectAlpha::LateUpdate(float fTime)
{
	return 0;
}

void CHitEffectAlpha::Collision(float fTime)
{
}

void CHitEffectAlpha::Render(float fTime)
{
}

CHitEffectAlpha * CHitEffectAlpha::Clone()
{
	return nullptr;
}
