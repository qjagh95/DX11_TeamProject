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
	void XMove(float fScale, float fTime);
	void YMove(float fScale, float fTime);
	void ZMove(float fScale, float fTime);
	void XRotation(float fScale, float fTime);
	void YRotation(float fScale, float fTime);
	void ZRotation(float fScale, float fTime);
public:
	CEditTest();
	CEditTest(const CEditTest& _EditTest);
	~CEditTest();
};

PUN_END