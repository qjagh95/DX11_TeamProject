#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Inventory.h"
#include "Component/ColliderOBB3D.h"
#include "Resource/Mesh.h"

bool CHuman_Player::Init_Interact()
{
	CRenderer* pRD = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	CColliderOBB3D* pOBB = m_pObject->AddComponent<CColliderOBB3D>("Player_Interact");

	pOBB->SetColliderID(UCI_PLAYER_INTERACT);

	CMesh* pMesh = pRD->GetMesh();

	Vector3 vScale = pMesh->GetLength();

	Vector3 vMax = pMesh->GetMax();
	Vector3 vMin = pMesh->GetMin();

	Vector3 vLength;

	vLength.x = 0.5f;
	vLength.y = fabs(vMax.y - vMin.y);
	vLength.z = fabs(vMax.z - vMin.z) * 0.5f;

	pOBB->SetInfo(Vector3::Zero, Vector3::Axis, vLength);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pOBB);
	SAFE_RELEASE(pRD);

	return true;
}


void CHuman_Player::OnDestroyInteract()
{
}

void CHuman_Player::Interact(float fTime) 
{
	
}
void CHuman_Player::Open_Door_Normal(float fTime)
{
	
}
void CHuman_Player::Close_Door_Normal(float fTime)
{

}
void CHuman_Player::Open_Door_Fast(float fTime)
{

}
void CHuman_Player::Close_Door_Fast(float fTime)
{

}
void CHuman_Player::Hide_Locker(float fTime)
{

}
void CHuman_Player::Hiding_Locker(float fTime)
{

}
void CHuman_Player::Exit_Locker(float fTime)
{

}
void CHuman_Player::Hide_Bed(float fTime)
{

}
void CHuman_Player::Hiding_Bed(float fTime)
{

}
void CHuman_Player::Exit_Bed(float fTime) 
{

}

int CHuman_Player::Input_Interact(float fTime)
{

	return 0;
}

int CHuman_Player::InteractUpdate(float fTime)
{
	return 0;
}
int CHuman_Player::InteractLateUpdate(float fTime)
{
	return 0;
}