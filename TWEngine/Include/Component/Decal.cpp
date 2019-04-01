#include "../EngineHeader.h"
#include "Decal.h"
#include "Renderer.h"
#include "Material.h"
#include "../GameObject.h"

PUN_USING

CDecal::CDecal()
{
	SetTag("Decal");
	m_eComType = CT_DECAL;
}

CDecal::CDecal(const CDecal & decal) :
	CComponent(decal)
{
}

CDecal::~CDecal()
{
}

void CDecal::Start()
{
}

void CDecal::AfterClone()
{
}

bool CDecal::Init()
{
	m_pObject->SetRenderGroup(RG_DECAL);

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DecalRenderer");

	pRenderer->SetMesh("VolumeBox");
	pRenderer->SetRenderState(CULL_NONE);

	SAFE_RELEASE(pRenderer);

	return true;
}

int CDecal::Input(float fTime)
{
	return 0;
}

int CDecal::Update(float fTime)
{
	return 0;
}

int CDecal::LateUpdate(float fTime)
{
	return 0;
}

void CDecal::Collision(float fTime)
{
}

void CDecal::Render(float fTime)
{
}

CDecal * CDecal::Clone()
{
	return new CDecal(*this);
}
