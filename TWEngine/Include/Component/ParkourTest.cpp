#include "../EngineHeader.h"
#include "ParkourTest.h"
#include "Renderer.h"
#include "ColliderOBB3D.h"

PUN_USING

CParkourTest::CParkourTest()
{
}

CParkourTest::CParkourTest(const CParkourTest & ParkourTest)
	:CComponent(ParkourTest)
{
}

CParkourTest::~CParkourTest()
{
}

void CParkourTest::Start()
{
}

void CParkourTest::AfterClone()
{
}

bool CParkourTest::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("ParkourTestRenderer");

	pRenderer->SetMesh("ParkourTestRender", L"bed.msh");
	
	CMesh* pMesh = pRenderer->GetMesh();

	SAFE_RELEASE(pRenderer);
	m_pTransform->SetWorldScale(0.2f, 0.2f, 0.2f);
	m_pTransform->SetWorldPos(0.f, 0.f, 10.f);
	Vector3 vLength = pMesh->GetLength();

	vLength.x *= m_pTransform->GetWorldScale().x / 2.f;
	vLength.y *= m_pTransform->GetWorldScale().y / 2.f;
	vLength.z *= m_pTransform->GetWorldScale().z;

	CColliderOBB3D* pObb = m_pObject->AddComponent<CColliderOBB3D>("ParkourOBB");

	pObb->SetInfo(Vector3::Zero, Vector3::Axis, vLength);

	SAFE_RELEASE(pMesh);

	SAFE_RELEASE(pObb);
	return true;
}

int CParkourTest::Input(float fTime)
{
	return 0;
}

int CParkourTest::Update(float fTime)
{
	return 0;
}

int CParkourTest::LateUpdate(float fTime)
{
	return 0;
}

void CParkourTest::Collision(float fTime)
{
}

void CParkourTest::Render(float fTime)
{
}

CParkourTest * CParkourTest::Clone()
{
	return new CParkourTest(*this);
}