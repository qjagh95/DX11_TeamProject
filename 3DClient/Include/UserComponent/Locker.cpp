#include "../ClientHeader.h"
#include "Locker.h"
#include "Door.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/ColliderOBB3D.h"

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
	SAFE_RELEASE(m_pDoor);
}

void CLocker::AfterClone()
{
}

bool CLocker::Init()
{
	// »ó´ë ÁÂÇ¥
	//-28 108 28.8 * scale

	m_vRelativePos = Vector3(-28.0f, 15.0f, 24.0f);

	CRenderer* pRD = m_pObject->AddComponent<CRenderer>("LockerRenderer");
	CCollider* pOBB = m_pObject->AddComponent<CColliderOBB3D>("LockerBody");
	pOBB->SetCollisionCallback(CCT_STAY, this, &CLocker::Interact);

	pRD->SetMesh("Large_Locker", TEXT("Large_Locker.msh"));

	m_pTransform->SetLocalRotY(90.0f);
	//m_pTransform->SetWorldPivot(0.0f, 0.0f, 0.0f);
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);

	CGameObject* pObj = CGameObject::CreateObject("LockerDoorObj");
	CTransform* pTr = pObj->GetTransform();
	m_pDoor = pObj->AddComponent<CDoor>("LockerDoor");

	m_pDoor->SetDoorType(DOOR_LOCKER);
	m_pObject->AddChild(pObj);

	pTr->SetWorldPos(m_vRelativePos * pTr->GetWorldScale());

	SAFE_RELEASE(pOBB);
	SAFE_RELEASE(pTr);
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

void CLocker::Interact(CCollider * pSrc, CCollider * pDest, float fTime)
{
	int iID = pDest->GetColliderID();
	if (iID == UCI_PLAYER_INTERACT)
	{
		if(KEYDOWN("E"))
			m_pDoor->Open(pDest->GetWorldAxis(AXIS_Z));
	}
}
