#include "../ClientHeader.h"
#include "Bed.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"

CBed::CBed()
{
	m_eComType = (COMPONENT_TYPE)UT_HIDABLE;
	m_eType = HT_BED;
}

CBed::CBed(const CBed & battery)
{
}


CBed::~CBed()
{
}

void CBed::AfterClone()
{
}

bool CBed::Init()
{
	CRenderer* pRD = m_pObject->AddComponent<CRenderer>("BedRenderer");
	pRD->SetMesh("bed", TEXT("bed.msh"));

	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);

	SAFE_RELEASE(pRD);

	return true;
}

int CBed::Input(float fTime)
{
	return 0;
}

int CBed::Update(float fTime)
{
	return 0;
}

int CBed::LateUpdate(float fTime)
{
	return 0;
}

void CBed::Collision(float fTime)
{
}

void CBed::Render(float fTime)
{
}

CBed * CBed::Clone()
{
	return nullptr;
}
