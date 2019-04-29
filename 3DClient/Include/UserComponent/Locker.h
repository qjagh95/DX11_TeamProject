#pragma once
#include "Hidable.h"

PUN_USING

class CLocker :
	public CHidable
{
	friend class CGameObject;

private:
	CLocker();
	CLocker(const CLocker& battery);
	virtual ~CLocker();


public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLocker* Clone();
};
