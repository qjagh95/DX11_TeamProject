#include "../ClientHeader.h"
#include "KaTok.h"

CKaTok::CKaTok()
{
}

CKaTok::CKaTok(const CKaTok & doc)	:
	CUserComponent(doc)
{
}

CKaTok::~CKaTok()
{
}

void CKaTok::AfterClone()
{
}

bool CKaTok::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("SMRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MobileMEssage", TEXT("UI/Document/MobileMessage.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(720.f, 720.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	return true;
}

int CKaTok::Input(float fTime)
{
	return 0;
}

int CKaTok::Update(float fTime)
{
	return 0;
}

int CKaTok::LateUpdate(float fTime)
{
	return 0;
}

void CKaTok::Collision(float fTime)
{
}

void CKaTok::Render(float fTime)
{
}

CKaTok * CKaTok::Clone()
{
	return new CKaTok(*this);
}
