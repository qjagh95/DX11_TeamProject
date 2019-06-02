#include "Human_Player.h"
#include "Player_Interact_Value.hpp"
#include "Player_Item_Value.hpp"
#include <Component/Collider.h>
#include "Door.h"
#include "Locker.h"
#include "Bed.h"


void CHuman_Player::InteractCallBackEnter(CCollider* pSrc,
	CCollider* pDest, float fTime)
{

}

void CHuman_Player::InteractCallBackStay(CCollider * pSrc,
	CCollider * pDest, float fTime)
{
	PUN::CGameObject *pDestObj = pDest->GetGameObject();

	if (pDestObj)
	{
		CHidable *pHidable = pDestObj->FindComponentFromType<CLocker>((PUN::COMPONENT_TYPE)UT_HIDABLE);
		CBed *pBed = dynamic_cast<CBed*>(pHidable);
		if (pHidable)
		{
			CLocker *pLocker = dynamic_cast<CLocker*>(pHidable);
			if (pLocker)
			{
				pDestObj->SetFrustrumCullUse(false);
				Interact_With_Locker(pLocker, fTime);
			}
			else if (pBed)
			{
				pDestObj->SetFrustrumCullUse(false);
				Interact_With_Bed(pBed, fTime);
			}
			SAFE_RELEASE(pHidable);
		}

		CDoor *pDoor = pDestObj->FindComponentFromType<CDoor>((PUN::COMPONENT_TYPE)UT_DOOR);

		if (pDoor)
		{
			pDestObj->SetFrustrumCullUse(false);
			Interact_With_Door(pDoor, fTime);
			SAFE_RELEASE(pDoor);
		}
	}


	SAFE_RELEASE(pDestObj);
}

void CHuman_Player::InteractCallBackLeave(CCollider * pSrc,
	CCollider * pDest, float fTime)
{
	PUN::CGameObject *pDestObj = pDest->GetGameObject();

	if (pDestObj)
	{
		CHidable *pHidable = pDestObj->FindComponentFromType<CLocker>((PUN::COMPONENT_TYPE)UT_HIDABLE);

		if (pHidable)
		{
			CLocker *pLocker = dynamic_cast<CLocker*>(pHidable);
			CBed *pBed = dynamic_cast<CBed*>(pHidable);
			if (pLocker)
			{
				pDestObj->SetFrustrumCullUse(true);
				Interact_Exit_Locker(pLocker, fTime);
				//Interact_With_Locker(pLocker, fTime);
			}

			else if(pBed)
			{
				pDestObj->SetFrustrumCullUse(true);
				Interact_Exit_Bed(pBed, fTime);
			}

			SAFE_RELEASE(pHidable);
		}

		CDoor *pDoor = pDestObj->FindComponentFromType<CDoor>((PUN::COMPONENT_TYPE)UT_DOOR);

		if (pDoor)
		{
			pDestObj->SetFrustrumCullUse(false);
			Interact_Exit_Door(pDoor, fTime);
			SAFE_RELEASE(pDoor);
		}
	}


	SAFE_RELEASE(pDestObj);
}

void CHuman_Player::HitCallBackEnter(CCollider * pSrc,
	CCollider * pDest, float fTime)
{

}

void CHuman_Player::HitCallBackStay(CCollider * pSrc,
	CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_DOOR)
	{
		//여기서 못 통과하게 한다.
	}
}

void CHuman_Player::HitCallBackLeave(CCollider * pSrc,
	CCollider * pDest, float fTime)
{

}

void CHuman_Player::RayCallBackEnter(CCollider * pSrc,
	CCollider * pDest, float fTime)
{
}

void CHuman_Player::RayCallBackStay(CCollider * pSrc,
	CCollider * pDest, float fTime)
{

}

void CHuman_Player::RayCallBackLeave(CCollider * pSrc,
	CCollider * pDest, float fTime)
{
}
