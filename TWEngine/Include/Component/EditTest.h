#pragma once
#include "Component.h"

PUN_BEGIN 

class PUN_DLL CEditTest :
	public CComponent
{
public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEditTest* Clone();
public:
	CEditTest();
	CEditTest(const CEditTest& _EditTest);
	~CEditTest();
};

PUN_END