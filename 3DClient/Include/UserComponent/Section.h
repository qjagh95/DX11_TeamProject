#pragma once
#include "Component/UserComponent.h"

PUN_USING

class CSection :
	public CUserComponent
{
public:
	virtual bool Init() override;
	void SectionOut(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
public:
	CSection();
	~CSection();
};

