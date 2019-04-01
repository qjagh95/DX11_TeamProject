#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CDecal :
	public CComponent
{
	friend class CGameObject;

protected:
	CDecal();
	CDecal(const CDecal& decal);
	~CDecal();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CDecal* Clone();
};

PUN_END