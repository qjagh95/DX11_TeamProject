#pragma once

#include "Component.h"

PUN_BEGIN

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
	float		m_fInAngle;
	float		m_fOutAngle;
public:
	LIGHT_TYPE  GetLightType()	const;
	LightInfo	GetLightInfo()	const;

public:
	void SetLightType(LIGHT_TYPE eType);
	void SetLightRange(float fRange);
	void SetAngle(float fInAngle, float fOutAngle);
	void SetLightColor(const Vector4& vDif, const Vector4& vAmb, const Vector4& vSpc);
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