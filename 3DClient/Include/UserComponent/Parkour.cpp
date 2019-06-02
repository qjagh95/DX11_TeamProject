#include "../ClientHeader.h"
#include "Parkour.h"
#include "Component/Renderer.h"
#include "GameObject.h"
#include "Component/ColliderOBB3D.h"
CParkour::CParkour()
{
	m_eComType = (PUN::COMPONENT_TYPE)UT_JUMPOVER;
}

CParkour::CParkour(const CParkour & _Parkour)
	:CUserComponent(_Parkour)
{
	m_eComType = (PUN::COMPONENT_TYPE)UT_JUMPOVER;
}

CParkour::~CParkour()
{
}

void CParkour::AfterClone()
{
}

bool CParkour::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("ParkourRenderer");

	pRenderer->SetMesh("CoffeeTable", L"Coffee_table.msh");

	Vector3 vLength = pRenderer->GetMeshLength();

	SAFE_RELEASE(pRenderer);

	m_pTransform->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	m_pTransform->SetWorldPos(Vector3(20.f, 0.f, 20.f));
	vLength.x *= m_pTransform->GetWorldScale().x;
	vLength.y *= m_pTransform->GetWorldScale().y;
	vLength.z *= m_pTransform->GetWorldScale().z;

	CColliderOBB3D* pObb = m_pObject->AddComponent<CColliderOBB3D>("ParkourObb");
	pObb->SetColliderID(UCI_PARKOUR);
	vLength.x /= 6.f;	
	vLength.y /= 2.f;
	vLength.z /= 2.f;


	pObb->SetInfo(Vector3(0.f , 40.f , 0.f), Vector3::Axis, vLength);

	SAFE_RELEASE(pObb);
	return true;
}

int CParkour::Input(float fTime)
{
	return 0;
}

int CParkour::Update(float fTime)
{
	return 0;
}

int CParkour::LateUpdate(float fTime)
{
	return 0;
}

void CParkour::Collision(float fTime)
{
}

void CParkour::Render(float fTime)
{
}

CParkour * CParkour::Clone()
{
	return new CParkour(*this);
}
