#include "Human_Player.h"
#include "Player_Interact_Value.hpp"
#include "Player_Item_Value.hpp"

void CHuman_Player::InteractCallBackEnter(CCollider* pSrc,
	CCollider* pDest, float fTime)
{

}

void CHuman_Player::InteractCallBackStay(CCollider * pSrc,
	CCollider * pDest, float fTime)
{

}

void CHuman_Player::InteractCallBackLeave(CCollider * pSrc,
	CCollider * pDest, float fTime)
{

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
