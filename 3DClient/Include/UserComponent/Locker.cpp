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
	*this = battery;

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

	m_vRelativePos = Vector3(-31.f, 10.f, 24.5f);

	CRenderer* pRD = m_pObject->AddComponent<CRenderer>("LockerRenderer");
	CColliderOBB3D* pOBB = m_pObject->AddComponent<CColliderOBB3D>("LockerBody");
	pOBB->SetCollisionCallback(CCT_STAY, this, &CLocker::Interact);

	pRD->SetMesh("Large_Locker", TEXT("Large_Locker.msh"));

	m_pTransform->SetLocalRotY(90.0f);
	//m_pTransform->SetWorldPivot(0.0f, 0.0f, 0.0f);
	m_pTransform->SetWorldScale(0.055f, 0.034f, 0.05f);

	Vector3 vMeshLength = pRD->GetMeshLength();
	Vector3 vScale = vMeshLength * GetWorldScale();
	Vector3 vCenter;
	vCenter.x = vMeshLength.z * 0.0f;
	vCenter.y = vMeshLength.y * 0.5f;
	vCenter.z = vMeshLength.x * 0.0f;

	Vector3 vAxis[3];
	Matrix matLocalRot = m_pTransform->GetLocalRotMatrix();

	vScale = vScale * 0.5f;

	for (int i = 0; i < 3; ++i)
	{
		vAxis[i] = Vector3::Axis[(AXIS)i];
		vAxis[i] = vAxis[i].TransformNormal(matLocalRot);
	}

	pOBB->SetInfo(vCenter, vAxis, vScale);

	CGameObject* pObj = CGameObject::CreateObject("LockerDoorObj");
	CTransform* pTr = pObj->GetTransform();
	m_pDoor = pObj->AddComponent<CDoor>("LockerDoor");

	m_pDoor->SetDoorType(DOOR_LOCKER);
	m_pDoor->SetOpenTime(1.2f);
	m_pObject->AddChild(pObj);

	pTr->SetWorldPos(m_vRelativePos * pTr->GetWorldScale());
	pTr->SetWorldScale(0.055f, 0.0345f, 0.05f);
	//pTr->SetWorldRot(0.f, 90.f, 0.f);

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
	return new CLocker(*this);
}

void CLocker::Interact(CCollider * pSrc, CCollider * pDest, float fTime)
{
	PUN::CGameObject *pDoorObj = m_pDoor->GetGameObject();
	pDoorObj->SetFrustrumCullUse(false);
	SAFE_RELEASE(pDoorObj);
	int iID = pDest->GetColliderID();
	if (iID == UCI_PLAYER_INTERACT)
	{
		if (KEYDOWN("E"))
		{
			m_pDoor->Open(pDest->GetWorldAxis(AXIS_Z));


		}
			
	}
}
