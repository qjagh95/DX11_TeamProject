#include "../ClientHeader.h"
#include "KeyBigICon.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Input.h"
#include "Device.h"

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

	//m_pTransform->SetWorldScale(750.f, 792.f, 1.f);
	//m_pTransform->SetWorldPos(_RESOLUTION.iWidth * (1.f - (1 - 560.f / 1280.f)),
	//	_RESOLUTION.iHeight * (1.f - (1 - 80.f / 720.f)), 1.f);
	m_pTransform->SetWorldScale(500.f, 528.f, 1.f);
	m_pTransform->SetWorldPos(560.f, 80.f, 0.f);

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
	tFrame.vRB = Vector2(500.f, 528.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("KeyBig_KeyCenter", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"KeyCenter_KeyBig", TEXT("UI/Key/CenterKey_Detail.png"));

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
