#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CItem :
	public CComponent
{
	friend class CGameObject;

public:
	CItem();
	CItem(const CItem& item);
	virtual ~CItem() = 0;
	
public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CItem* Clone() = 0;
};

PUN_END