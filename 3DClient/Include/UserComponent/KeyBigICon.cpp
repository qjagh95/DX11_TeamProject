#include "../ClientHeader.h"
#include "KeyBigICon.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Input.h"

CKeyBigICon::CKeyBigICon()
{
	m_eComType = (COMPONENT_TYPE)IT_KEYICON;
	m_pAnimation = nullptr;
}

CKeyBigICon::CKeyBigICon(const CKeyBigICon & keybigicon)	:
	CUserComponent(keybigicon)
{
	m_pAnimation = nullptr;
}

CKeyBigICon::~CKeyBigICon()
{
	SAFE_RELEASE(m_pAnimation);
}

void CKeyBigICon::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CKeyBigICon::AfterClone()
{
}

bool CKeyBigICon::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("KeyBigIconRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "KeyBigIcon", TEXT("UI/Key/Key_Center.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(500.f, 528.f, 1.f);
	pTransform->SetWorldPos(560.f, 80.f, 0.f);

	SAFE_RELEASE(pTransform);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("KeyBigIconAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(221.f, 328.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_Empty", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"Empty_KeyBigIcon", TEXT("UI/Key/Key_Empty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(500.f, 528.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeyMaster", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeyMaster_KeyBig", TEXT("UI/Key/MasterKey_Detail.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(221.f, 328.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeyPiano", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeyPiano_KeyBig", TEXT("UI/Key/Key_Piano.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(221.f, 328.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeySmall", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeySmall_KeyBig", TEXT("UI/Key/Key_Small.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(221.f, 328.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeyTotal", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeyTotal_KeyBig", TEXT("UI/Key/Key_Total.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(221.f, 328.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeyCenter", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeyCenter_KeyBig", TEXT("UI/Key/Key_Center.png"));

	vecClipFrame.clear();

	m_pObject->SetEnable(false);

	return true;
}

int CKeyBigICon::Input(float fTime)
{
	return 0;
}

int CKeyBigICon::Update(float fTime)
{
	return 0;
}

int CKeyBigICon::LateUpdate(float fTime)
{
	return 0;
}

void CKeyBigICon::Collision(float fTime)
{
}

void CKeyBigICon::Render(float fTime)
{
}

CKeyBigICon * CKeyBigICon::Clone()
{
	return new CKeyBigICon(*this);
}