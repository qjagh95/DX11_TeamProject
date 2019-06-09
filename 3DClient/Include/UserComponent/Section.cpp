#include "../Client.h"
#include "Section.h"
#include "Component/ColliderOBB3D.h"


CSection::CSection()
{
}


CSection::~CSection()
{
}

bool CSection::Init()
{
	CColliderOBB3D* pOBB3D = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

	pOBB3D->SetCollisionCallback(CCT_LEAVE, this, &CSection::SectionOut);

	SAFE_RELEASE(pOBB3D);

	return true;
}

void CSection::SectionOut(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerGeom")
	{
		pDest->GetGameObjectNonCount()->SetStageSection(m_pObject->GetStageSection());
	}
}