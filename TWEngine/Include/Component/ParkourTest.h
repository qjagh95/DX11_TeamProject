#pragma once
#include "Component.h"
PUN_BEGIN

class PUN_DLL CParkourTest :
	public CComponent
{
public:
	void Start();
	void AfterClone();
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	CParkourTest* Clone();


public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);

public:
	CParkourTest();
	CParkourTest(const CParkourTest& ParkourTest);
	~CParkourTest();
};

PUN_END