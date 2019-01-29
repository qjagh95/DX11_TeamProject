#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CNumber :
	public CComponent
{
	friend class CGameObject;

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CNumber* Clone();

private:
	int m_iNumber;
	float m_fActiveTime;

public:
	void SetNumber(int _iNumber);
	void SetNumberPos(Vector3 _vPos);
	void SetNumberPivot(Vector3 _vPivot);
private:
	CNumber();
	CNumber(const CNumber& number);
	~CNumber();
};

PUN_END