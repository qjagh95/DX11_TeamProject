#include "../ClientHeader.h"
#include "HitEffectAnim.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Human_Player.h"

CHitEffectAnim::CHitEffectAnim()
{
	m_eComType = (COMPONENT_TYPE)UT_EFFECT;
	m_pAnimation = nullptr;
}

CHitEffectAnim::CHitEffectAnim(const CHitEffectAnim & effect)	:
	CUserComponent(effect)
{
	m_pAnimation = nullptr;
}

CHitEffectAnim::~CHitEffectAnim()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pPlayerObj);
	SAFE_RELEASE(m_pPlayer);
}

void CHitEffectAnim::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CHitEffectAnim::AfterClone()
{
}

bool CHitEffectAnim::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("Zipper2Renderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "HitEffectAnim", TEXT("Effect/HitEffect_Empty.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(1280.f, 720.f, 1.f);

	SAFE_RELEASE(pTransform);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("HitEffectAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(1280.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Hit_Empty", A2D_ATLAS, AO_LOOP, 0.4f, vecClipFrame,
		"Empty_Hit", TEXT("Effect/HitEffect_Empty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800.f, 475.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Hit1", A2D_ATLAS, AO_LOOP, 0.4f, vecClipFrame,
		"1Hit", TEXT("Effect/bloodyrect1.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f , 0.f);
	tFrame.vRB = Vector2(800.f, 475.f);
	vecClipFrame.push_back(tFrame);	

	m_pAnimation->AddClip("Hit2", A2D_ATLAS, AO_LOOP, 0.4f, vecClipFrame,
		"2Hit", TEXT("Effect/HitEffect2.png"));

	vecClipFrame.clear();

	for (int i = 0; i < 2; ++i)
	{
		tFrame.vLT = Vector2(0.f + i * 800.f, 0.f);
		tFrame.vRB = Vector2(800.f + i * 800.f, 475.f);
		vecClipFrame.push_back(tFrame);
	}

	m_pAnimation->AddClip("Hit_Anim", A2D_ATLAS, AO_LOOP, 0.8f, vecClipFrame,
		"Anim_Hit", TEXT("Effect/HitEffect3.png"));

	vecClipFrame.clear();

	for (int i = 0; i < 2; ++i)
	{
		tFrame.vLT = Vector2(0.f + i * 800.f, 0.f);
		tFrame.vRB = Vector2(800.f + i * 800.f, 475.f);
		vecClipFrame.push_back(tFrame);
	}

	m_pAnimation->AddClip("Hit4", A2D_ATLAS, AO_LOOP, 0.4f, vecClipFrame,
		"Anim4_Hit", TEXT("Effect/bloodyrect4_new.png"));

	vecClipFrame.clear();

	m_pPlayerObj = CGameObject::FindObject("Player");

	return true;
}

int CHitEffectAnim::Input(float fTime)
{
	return 0;
}

int CHitEffectAnim::Update(float fTime)
{	
	return 0;
}

int CHitEffectAnim::LateUpdate(float fTime)
{
	return 0;
}

void CHitEffectAnim::Collision(float fTime)
{
}

void CHitEffectAnim::Render(float fTime)
{
}

CHitEffectAnim * CHitEffectAnim::Clone()
{
	return new CHitEffectAnim(*this);
}
