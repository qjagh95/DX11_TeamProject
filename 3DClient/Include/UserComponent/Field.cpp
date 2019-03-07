#include "../ClientHeader.h"
#include "Field.h"
#include "Input.h"
#include "Component/Transform.h"

CField::CField()
{
}

CField::CField(const CField & com)
	: CUserComponent(com)
{
}


CField::~CField()
{
}

void CField::Start()
{
}

void CField::AfterClone()
{
}

bool CField::Init()
{
	m_pTransform->SetWorldRotY(-180.f);
	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldPos(0.f, -2.f, 100.f);

	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");

	pRenderer->SetMesh("floor", TEXT("ground_test_1.msh"));

	SAFE_RELEASE(pRenderer);

	/*m_pAnimation = m_pObject->AddComponent<CAnimation>("Animation");

	m_pAnimation->Load("Monster4.anm");*/

	return true;
}

int CField::Input(float fTime)
{
	return 0;
}

int CField::Update(float fTime)
{
	return 0;
}

int CField::LateUpdate(float fTime)
{
	return 0;
}

void CField::Collision(float fTime)
{
}

void CField::Render(float fTime)
{
}

CField * CField::Clone()
{
	return new CField(*this);
}