#include "../ClientHeader.h"
#include "Handycam.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Rendering/RenderManager.h"

CHandycam::CHandycam()
{
	m_eComType = (COMPONENT_TYPE)UT_HANDYCAM;
	m_iFlag = -1;
	m_bVisible = false;
}

CHandycam::CHandycam(const CHandycam & handycam)	:
	CUserComponent(handycam)
{
}

CHandycam::~CHandycam()
{
}

void CHandycam::SetVisible(bool bVisible)
{
	m_pObject->SetEnable(bVisible);
}

void CHandycam::SetVisible()
{
	if (m_iFlag == -1)
	{
		m_bVisible = true;
		m_pObject->SetEnable(true);
		GET_SINGLE(CRenderManager)->SetStarLightScope(1);
	}

	else if (m_iFlag == 1)
	{
		m_bVisible = false;
		m_pObject->SetEnable(false);
		GET_SINGLE(CRenderManager)->SetStarLightScope(-1);
	}

	m_iFlag *= -1;
}

void CHandycam::AfterClone()
{
}

bool CHandycam::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("Icon1Renderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Handycam", TEXT("UI/CameraHud.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();

	pTransform->SetWorldScale(1280.f, 720.f, 1.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	m_pObject->SetEnable(false);

	return true;
}

int CHandycam::Input(float fTime)
{
	return 0;
}

int CHandycam::Update(float fTime)
{
	return 0;
}

int CHandycam::LateUpdate(float fTime)
{
	return 0;
}

void CHandycam::Collision(float fTime)
{
}

void CHandycam::Render(float fTime)
{
}

CHandycam * CHandycam::Clone()
{
	return new CHandycam(*this);
}
