#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CNumber :
	public CUI
{
	friend class CGameObject;

private:
	CNumber();
	CNumber(const CNumber& number);
	~CNumber();

private:
	vector<class CNumber*> m_vecNumber;
	vector<int> m_vecNumberValue;
	int		m_iNumber;
	float	m_fActiveTime;
	float	m_fPlayTime;
	float	m_fRotAngle;
	bool	m_bChangeNumber;
	float	m_fMove;
	int		m_iDigitCount;
	bool	m_bDotDamage;

public:
	void ClearNumber();
	void ClearVecNumber();
	void AddNumber(int iNumber);
	void SetNumber(int iNumber);
	void SetNumberPos(Vector3 vPos);
	void SetNumberPos(float x, float y, float z);
	void SetNumberPivot(Vector3 vPivot);
	void SetNumberPivot(float x, float y, float z);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CNumber* Clone();
};

PUN_END