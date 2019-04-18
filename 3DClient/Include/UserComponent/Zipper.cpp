#include "../ClientHeader.h"
#include "Zipper.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/Animation2D.h"

CZipper::CZipper()
{
}

CZipper::CZipper(const CZipper & zip)	:
	CUserComponent(zip)
{
}

CZipper::~CZipper()
{
	SAFE_RELEASE(m_pInvenObj);
}

void CZipper::AfterClone()
{
}

bool CZipper::Init()
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

	pMaterial->SetDiffuseTex(0, "Zipper2", TEXT("UI/Inven/ZipperAnimate.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(120.f, 510.f, 1.f);
	pTransform->SetWorldPos(255.f, 80.f, 1.f);

	SAFE_RELEASE(pTransform);

	CAnimation2D*	pAnimation = m_pObject->AddComponent<CAnimation2D>("ZipperAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	for (int i = 0; i < 10; ++i)
	{
		tFrame.vLT = Vector2(0.f + i * 215.f, 0.f);
		tFrame.vRB = Vector2(215.f + i * 215.f, 945.f);
		vecClipFrame.push_back(tFrame);
	}

	pAnimation->AddClip("Inven_Zipper", A2D_ATLAS, AO_ONCE_LAST, 0.5f, vecClipFrame,
		"Zipper_Inven", TEXT("UI/Inven/ZipperAnimate.png"));

	vecClipFrame.clear();

	SAFE_RELEASE(pAnimation);

	m_pInvenObj = CGameObject::FindObject("Inven");

	return true;
}

int CZipper::Input(float fTime)
{
	return 0;
}

int CZipper::Update(float fTime)
{
	if (m_pInvenObj->GetEnable() == false)
	{
		m_pObject->Die();
	}

	return 0;
}

int CZipper::LateUpdate(float fTime)
{
	return 0;
}

void CZipper::Collision(float fTime)
{
}

void CZipper::Render(float fTime)
{
}

CZipper * CZipper::Clone()
{
	return new CZipper(*this);
}

