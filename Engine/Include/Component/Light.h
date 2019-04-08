#pragma once

#include "Component.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class PUN_DLL CLight :
	public CComponent
{
	friend class CGameObject;
	friend class CScene;

private:
	CLight();
	CLight(const CLight& light);
	~CLight();

private:
	LightInfo	m_tInfo;
	RimLightInfo	m_tRimInfo;
	float		m_fInAngle;
	float		m_fOutAngle;
	int			m_iRim;

public:
	LIGHT_TYPE  GetLightType()	const;
	LightInfo	GetLightInfo()	const;

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

public:
	void SetLightType(LIGHT_TYPE eType);
	void SetLightRange(float fRange);
	void SetLightDirection(const Vector3& Dir);
	void SetAngle(float fInAngle, float fOutAngle);
	void SetLightColor(const Vector4& vDif, const Vector4& vAmb, const Vector4& vSpc);
	void SetRimLight(int iRim);
	void SetRimColor(Vector3 vRimColor);
	void SetRimColor(float x, float y, float z);
	void SetRimPower(float fRimPower);
	void UpdateLightCBuffer();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLight* Clone();
};

PUN_END