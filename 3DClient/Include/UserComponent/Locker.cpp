#include "../ClientHeader.h"
#include "Locker.h"
#include "Door.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"

CLocker::CLocker()
{
	m_eComType = (COMPONENT_TYPE)UT_HIDABLE;
	m_eType = HT_LOCKER;
}

CLocker::CLocker(const CLocker & battery)
{
}

CLocker::~CLocker()
{
}

void CLocker::AfterClone()
{
}

bool CLocker::Init()
{
	CRenderer* pRD = m_pObject->AddComponent<CRenderer>("BedRenderer");
	pRD->SetMesh("LockerLarge", TEXT("LockerLarge.msh"));

	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);

	CGameObject* pObj = CGameObject::CreateObject("LockerDoorObj");
	CDoor* pDoor = pObj->AddComponent<CDoor>("LockerDoor");
	CTransform* pTr = pObj->GetTransform();

	pDoor->SetDoorType(DOOR_LOCKER);

	m_pObject->AddChild(pObj);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pDoor);
	SAFE_RELEASE(pObj);

	SAFE_RELEASE(pRD);

	return true;
}

int CLocker::Input(float fTime)
{
	return 0;
}

int CLocker::Update(float fTime)
{
	return 0;
}

int CLocker::LateUpdate(float fTime)
{
	return 0;
}

void CLocker::Collision(float fTime)
{
}

void CLocker::Render(float fTime)
{
}

CLocker * CLocker::Clone()
{
	return nullptr;
}
