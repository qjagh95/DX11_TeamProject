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
	LightInfo		m_tInfo;
	RimLightInfo	m_tRimInfo;
	LIGHT_TYPE		m_eLightType;
	float			m_fInAngle;
	float			m_fOutAngle;
	float			m_fNear;
	float			m_fRange;
	float			m_fWidth;
	float			m_fHeight;
	int				m_iRim;
	
	bool			m_bBlink;
	bool			m_bTurnOn;
	bool			m_bBlinkFinalTurnOn;
	bool			m_bLightVolume;
	float			m_fLimitTime;
	float			m_fAccTime;
	float			m_fCheckTime;
	float			m_fDeltaTime;
	Vector4			m_vChangeColor;
	Vector4			m_vOriginColor;

	Matrix			m_matShadowView;
	Matrix			m_matShadowProj;
	Matrix			m_matShadowVP;

	class CVolumeFog* m_pFog;

public:
	LIGHT_TYPE  GetLightType()	const;
	LightInfo	GetLightInfo()	const;
	Vector4		GetLightColor() const;
	Matrix		GetShadowVP()	const;
	bool		IsTurnOn()		const;
public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

public:
	void SetLightType(LIGHT_TYPE eType);
	void SetLightRange(float fRange);
	void SetLightDirection(const Vector3& Dir);
	void SetLightDirectionX(float x);
	void SetLightDirectionY(float y);
	void SetLightDirectionZ(float z);
	void SetAngle(float fInAngle, float fOutAngle);
	void SetLightColor(const Vector4& vDif, const Vector4& vAmb, const Vector4& vSpc);
	void SetLightAmbient(const Vector4& vAmb);
	void SetLightDiffuse(const Vector4& vDif);
	void SetLightSpcular(const Vector4& vSpc);
	void SetRimLight(int iRim);
	void SetRimColor(Vector3 vRimColor);
	void SetRimColor(float x, float y, float z);
	void SetRimPower(float fRimPower);
	void UpdateLightCBuffer();

	void StartBlink(float fLimitTime, float fDeltaTime, const Vector4& vColor, bool bFinalTurnOn = true);
	void Blink(float fTime);

	float GetRange();
	Vector4 GetLightSpecular();

	Matrix ComputeLightView();

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