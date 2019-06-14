#include "../ClientHeader.h"
#include "ElevatorController.h"
#include "Component/ColliderOBB3D.h"
#include "../GameManager.h"



CElevatorController::CElevatorController()
{
}

CElevatorController::CElevatorController(const CElevatorController & _Controller)
	:CUserComponent(_Controller)
{
}


CElevatorController::~CElevatorController()
{
}

bool CElevatorController::Init()
{
	CColliderOBB3D* pOBB = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

	pOBB->SetCollisionCallback(CCT_STAY, this, &CElevatorController::ControllerHit);

	SAFE_RELEASE(pOBB);
	return true;
}

void CElevatorController::ControllerHit(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "Player_Interact")
	{
		if (CInput::GetInst()->KeyPress("E") == true)
		{
			if (strstr(m_pObject->GetTag().c_str(), "Down") != nullptr)
			{
				CGameManager::GetInst()->SetElevatorDown(true);
			}
			else if(strstr(m_pObject->GetTag().c_str(), "Up") != nullptr)
			{
				CGameManager::GetInst()->SetElevatorUp(true);
			}
		}
	}
}

CElevatorController * CElevatorController::Clone()
{
	return new CElevatorController(*this);
}
