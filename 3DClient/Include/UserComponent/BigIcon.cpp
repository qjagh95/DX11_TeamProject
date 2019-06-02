#include "../ClientHeader.h"
#include "BigIcon.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Input.h"
#include "Inventory.h"
#include "BatteryIcon.h"
#include "HealingPackIcon.h"
#include "CigaretteIcon.h"
#include "TabletIcon.h"

CBigIcon::CBigIcon()
{
	m_eComType = (COMPONENT_TYPE)IT_BIGICON;
	m_pAnimation = nullptr;
}

CBigIcon::CBigIcon(const CBigIcon & bigicon)	:
	CUserComponent(bigicon)
{
	m_pAnimation = nullptr;
}

CBigIcon::~CBigIcon()
{
	SAFE_RELEASE(m_pAnimation);
}

void CBigIcon::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CBigIcon::AfterClone()
{
}

bool CBigIcon::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("BigIconRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "BigIcon", TEXT("UI/Icon/Battery_Detail.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(800.f, 600.f, 1.f);
	pTransform->SetWorldPos(350.f, 80.f, 0.f);

	SAFE_RELEASE(pTransform);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("BigIconAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800.f, 600.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("BigIcon_Empty", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"Empty_BigIcon", TEXT("UI/Icon/BigIconEmpty.png"));

	vecClipFrame.clear();


	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800.f, 600.f);
	vecClipFrame.push_back(tFrame);	

	m_pAnimation->AddClip("Battery_Detail", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"DeTail_Battery", TEXT("UI/Icon/Battery_Detail.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Cola_Detail", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"DeTail_Cola", TEXT("UI/Icon/lunchbox_Detail.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("FoodCan_Detail", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"DeTail_FoodCan", TEXT("UI/Icon/MedicalKit_Detail.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800.f, 600.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Tablet_Detail", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"DeTail_Tablet", TEXT("UI/Icon/Tablet_Detail.png"));

	vecClipFrame.clear();
	
	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800.f, 600.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Dambae_Detail", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"DeTail_Dambae", TEXT("UI/Icon/Dambae_Detail.png"));

	vecClipFrame.clear();

	m_pObject->SetEnable(false);

	return true;
}

int CBigIcon::Input(float fTime)
{
	return 0;
}

int CBigIcon::Update(float fTime)
{
	return 0;
}

int CBigIcon::LateUpdate(float fTime)
{
	return 0;
}

void CBigIcon::Collision(float fTime)
{
}

void CBigIcon::Render(float fTime)
{
}

CBigIcon * CBigIcon::Clone()
{
	return new CBigIcon(*this);
}

