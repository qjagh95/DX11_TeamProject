#pragma once
#include "Component/UserComponent.h"

PUN_USING

class CElevatorController
	: public CUserComponent
{
	friend class CGameObject;
public:
	virtual bool Init();
	void ControllerHit(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	virtual	CElevatorController* Clone();
private:
	CElevatorController();
	CElevatorController(const CElevatorController& _Controller);
	~CElevatorController();
};

