#include "EngineHeader.h"
#include "HPMP.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CHPMP::CHPMP()
{
	m_eComType = CT_UI;
	SetTag("HpMp");
}

CHPMP::CHPMP(const CHPMP & hpmp)	:
	CUI(hpmp)
{
}

CHPMP::~CHPMP()
{
}

void CHPMP::AfterClone()
{
}

bool CHPMP::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("HPMPRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "HPMP", TEXT("UI/HPMP/HPMPStatus2.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(210.f, 119.f, 1.f);
	m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);

	return true;
}

int CHPMP::Input(float fTime)
{
	return 0;
}

int CHPMP::Update(float fTime)
{
	return 0;
}

int CHPMP::LateUpdate(float fTime)
{
	return 0;
}

void CHPMP::Collision(float fTime)
{
}

void CHPMP::Render(float fTime)
{
}

CHPMP * CHPMP::Clone()
{
	return new CHPMP(*this);
}
