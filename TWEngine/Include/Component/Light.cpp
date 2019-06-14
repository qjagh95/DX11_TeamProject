#include "../EngineHeader.h"
#include "Light.h"
#include "Camera.h"
#include "VolumeFog.h"
#include "../Core.h"
#include "../Device.h"

PUN_USING

CLight::CLight() :
	m_fAccTime(0.0f),
	m_fLimitTime(0.0f),
	m_fCheckTime(0.0f),
	m_fDeltaTime(0.0f),
	m_fInAngle(1.0f),
	m_fOutAngle(1.0f),
	m_fRange(5.0f),
	m_fAutoBlinkAccTime(0.0f),
	m_fAutoBlinkDeltaTime(0.0f),
	m_fAutoLimitTime(0.0f),
	m_fAutoDeltaTime(0.0f),
	m_bBlink(false),
	m_bBlinkFinalTurnOn(false),
	m_bTurnOn(true),
	m_bLightVolume(false),
	m_pFog(nullptr),
	m_bBlinkAuto(false)
{
	m_tInfo.vDif = Vector4::White;
	m_tInfo.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.vSpc = Vector4::White;
	m_tInfo.fFallOff = 0.0f;
	m_tInfo.fInAngle = 20.0f;
	m_tInfo.fOutAngle = 30.0f;
	m_tInfo.fRange = 50.0f;
	m_tInfo.iLightType = LT_DIR;
	m_vOriginColor = Vector4::White;
	m_vChangeColor = Vector4::Black;
	m_vAutoChangeColor = Vector4::Black;
	m_iRim = -1;
	m_eComType = CT_LIGHT;
}

CLight::CLight(const CLight & light) :
	CComponent(light)
{
	m_tInfo = light.m_tInfo;
}

CLight::~CLight()
{
	SAFE_RELEASE(m_pFog);
}

LIGHT_TYPE CLight::GetLightType() const
{
	return (LIGHT_TYPE)m_tInfo.iLightType;
}

LightInfo CLight::GetLightInfo() const
{
	return m_tInfo;
}

Vector4 CLight::GetLightColor() const
{	
	return m_tInfo.vDif;
}

Matrix CLight::GetShadowVP() const
{
	return m_matShadowVP;
}

float CLight::GetInAngle() const
{
	return m_fInAngle;
}

float CLight::GetOutAngle() const
{
	return m_fOutAngle;
}

bool CLight::IsTurnOn() const
{
	return m_bTurnOn;
}

void CLight::Save(BinaryWrite * _pInstBW)
{
	_pInstBW->WriteData(m_tInfo.vDif);
	_pInstBW->WriteData(m_tInfo.vAmb);
	_pInstBW->WriteData(m_tInfo.vSpc);
	_pInstBW->WriteData(m_tInfo.iLightType);
	_pInstBW->WriteData(m_tInfo.vPos);
	_pInstBW->WriteData(m_tInfo.vDir);
	_pInstBW->WriteData(m_tInfo.fRange);
	_pInstBW->WriteData(m_fInAngle);
	_pInstBW->WriteData(m_fOutAngle);
	_pInstBW->WriteData(m_tInfo.fFallOff);
	_pInstBW->WriteData(m_tInfo.iRimLight);
}

void CLight::Load(BinaryRead * _pInstBR)
{
	Vector4	vDif = _pInstBR->ReadVector4();
	Vector4	vAmb = _pInstBR->ReadVector4();
	Vector4	vSpc = _pInstBR->ReadVector4();
	int	iLightType = _pInstBR->ReadInt();
	Vector3	vPos = _pInstBR->ReadVector3();
	Vector3	vDir = _pInstBR->ReadVector3();
	float fRange = _pInstBR->ReadFloat();
	float fInAngle = _pInstBR->ReadFloat();
	float fOutAngle = _pInstBR->ReadFloat();
	float fFallOff = _pInstBR->ReadFloat();
	int iRimLight = _pInstBR->ReadInt();

	if (fRange < 0)
		fRange = 10.0f;
	
	if (fInAngle < 1.0f)
		fInAngle = 10.0f;
	
	if (fOutAngle < 1.0f)
		fOutAngle = 20.0f;

	// 로드된 데이터를 이용하여 조명 정보를 세팅한다.

	SetLightType((LIGHT_TYPE)iLightType);
	m_tInfo.vPos = vPos;
	m_tInfo.vDir = vDir;
	SetLightColor(vDif, vAmb, vSpc);

	if(m_eLightType != LT_DIR)
		SetLightRange(fRange);

	if(m_eLightType == LT_SPOT)
		SetAngle(fInAngle, fOutAngle);

	m_tInfo.fFallOff = fFallOff;
	m_tInfo.iRimLight = iRimLight;

	m_vOriginColor = vDif;

	if (strstr(m_pObject->GetTag().c_str(), "Lamp") != nullptr)
		EnableLightVolume();
}

void CLight::EnableLightVolume()
{
	m_bLightVolume = true;

	if (m_eLightType == LT_SPOT)
	{
		if (!m_pFog)
		{
			CGameObject* pObj = CGameObject::CreateObject("LightFog", nullptr);
			pObj->SetSave(false);
			CTransform* pTr = pObj->GetTransform();
			m_pObject->AddChild(pObj);

			m_pFog = pObj->AddComponent<CVolumeFog>("VolumeFog");

			m_pFog->SetFogColor(m_vOriginColor);
			m_pFog->SetMesh("Sky");
			m_pFog->SetDensity(0.1f);
			pTr->SetWorldPos(0.0f, 1.5f, 3.0f);
			pTr->SetWorldScale(3.0f);

			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pObj);
		}
		else
		{
			m_pFog->SetEnable(true);
			CTransform* pTr = m_pFog->GetTransform();

			m_pFog->SetFogColor(m_vOriginColor);
			m_pFog->SetMesh("Sky");

			pTr->SetWorldScale(3.0f);

			SAFE_RELEASE(pTr);
		}
	}
}

void CLight::SetLightType(LIGHT_TYPE eType)
{
	m_eLightType = eType;
	m_tInfo.iLightType = eType;

	if (m_bLightVolume)
	{
		if (m_eLightType == LT_DIR)
		{
			if (m_pFog)
				m_pFog->SetEnable(false);
		}
		else if (m_eLightType == LT_SPOT)
		{
			m_pTransform->SetLocalRotX(90.0f);

			if (m_pFog)
			{
				m_pFog->SetEnable(true);
				m_pFog->SetMesh(CORN_VOLUME);
			}
		}
		else if (m_eLightType == LT_POINT)
		{			
			if(m_pFog)
			{
				m_pFog->SetEnable(true);
				m_pFog->SetMesh("Sky");
			}
		}
	}
}

void CLight::SetLightRange(float fRange)
{
	m_fRange = fRange;
	m_tInfo.fRange = fRange;

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle), m_fWidth / m_fHeight, 0.03f, 500.0f);

	if (m_bLightVolume)
	{
		CTransform* pTr = m_pFog->GetTransform();

		if (m_eLightType == LT_SPOT)
		{
			float fRadius = 0.0f;
			fRadius = m_fRange * tanf(DegreeToRadian(m_fOutAngle * 0.5f));

			pTr->SetWorldScale(fRadius, m_fRange * 0.125f, fRadius);
		}
		else if (m_eLightType == LT_POINT)
		{
			float fRange = m_fRange * 0.125f;
			pTr->SetWorldScale(fRange, fRange, fRange);
		}

		SAFE_RELEASE(pTr);
	}
}

void CLight::SetAngle(float fInAngle, float fOutAngle)
{
	m_fInAngle = fInAngle;
	m_fOutAngle = fOutAngle;

	m_tInfo.fInAngle = cosf(DegreeToRadian(fInAngle * 0.5f));
	m_tInfo.fOutAngle = cosf(DegreeToRadian(fOutAngle * 0.5f));

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle * 0.5f), m_fWidth / m_fHeight, 0.03f, 500.0f);

	if (m_bLightVolume)
	{
		CTransform* pTr = m_pFog->GetTransform();

		float fRadius = 0.0f;
		fRadius = m_fRange * tanf(DegreeToRadian(m_fOutAngle * 0.5f));

		pTr->SetWorldScale(fRadius,	m_fRange * 0.125f, fRadius);

		SAFE_RELEASE(pTr);
	}
}

void CLight::SetLightColor(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc)
{
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;

	m_vOriginColor = vDif;

	if (m_bLightVolume)
		m_pFog->SetFogColor(vDif);
}

void CLight::SetLightAmbient(const Vector4 & vAmb)
{
	m_tInfo.vAmb = vAmb;
}

void CLight::SetLightDiffuse(const Vector4 & vDif)
{
	m_tInfo.vDif = vDif;
	m_vOriginColor = vDif;

	if(m_bLightVolume)
		m_pFog->SetFogColor(vDif);
}

void CLight::SetLightSpcular(const Vector4 & vSpc)
{
	m_tInfo.vSpc = vSpc;
}

void CLight::SetRimLight(int iRim)
{
	if (iRim == 1)
		m_tInfo.iRimLight = 1;
	else if (iRim == -1)
		m_tInfo.iRimLight = -1;
}

void CLight::SetRimColor(Vector3 vRimColor)
{
	m_tRimInfo.vRimColor = vRimColor;
}

void CLight::SetRimColor(float x, float y, float z)
{
	m_tRimInfo.vRimColor = Vector3(x, y, z);
}

void CLight::SetRimPower(float fRimPower)
{
	m_tRimInfo.fRimPower = fRimPower;
}

void CLight::SetLightDirection(const Vector3& Dir)
{
	m_tInfo.vDir = Dir;
}

void CLight::SetLightDirectionX(float x)
{
	m_tInfo.vDir.x = x;
}

void CLight::SetLightDirectionY(float y)
{
	m_tInfo.vDir.y = y;
}

void CLight::SetLightDirectionZ(float z)
{
	m_tInfo.vDir.z = z;
}

void CLight::UpdateLightCBuffer()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Light", &m_tInfo);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("RimLight", &m_tRimInfo);
}

void CLight::StartBlink(float fLimitTime, float fDeltaTime, const Vector4 & vColor, bool bFinalTurnOn)
{
	m_bBlink = true;
	m_fLimitTime = fLimitTime;
	m_fDeltaTime = fDeltaTime;
	m_fCheckTime = 0.0f;
	m_fAccTime = 0.0f;
	m_vChangeColor = vColor;
	m_bBlinkFinalTurnOn = bFinalTurnOn;
}

void CLight::SetAutoBlink(float fLimitTime, float fDeltaTime, const Vector4& vColor, float fAutoDeltaTime)
{
	m_bBlinkAuto = true;
	m_fAutoLimitTime = fLimitTime;
	m_fAutoDeltaTime = fDeltaTime;
	m_vAutoChangeColor = vColor;
	m_fDeltaTime = fAutoDeltaTime;
}

void CLight::Blink(float fTime)
{
	if (m_bBlink)
	{
		m_fAccTime += fTime;
		
		if (m_fAccTime < m_fLimitTime)
		{
			if (m_fAccTime - m_fCheckTime > m_fDeltaTime)
			{	
				if (m_bTurnOn)
				{
					m_tInfo.vDif = m_vChangeColor;
					m_bTurnOn = false;
				}
				else
				{
					m_tInfo.vDif = m_vOriginColor;
					m_bTurnOn = true;
				}
				m_fCheckTime = m_fAccTime;
			}
		}
		else
		{
			m_bBlink = false;

			if (m_bBlinkFinalTurnOn)
			{
				m_tInfo.vDif = m_vOriginColor;
				m_bTurnOn = true;
			}
			else
			{
				m_tInfo.vDif = m_vChangeColor;
				m_bTurnOn = false;
			}
			m_fAccTime = 0.0f;
			m_fCheckTime = 0.0f;;
		}
	}
	else
	{
		if (m_bBlinkAuto)
		{
			m_fAutoBlinkAccTime += fTime;
			if (m_fAutoBlinkAccTime >= m_fAutoBlinkDeltaTime)
			{
				StartBlink(m_fAutoLimitTime, m_fAutoDeltaTime, m_vAutoChangeColor, true);
				m_fAutoBlinkAccTime = 0.0f;
			}
		}
	}
}

float CLight::GetRange()
{
	return m_tInfo.fRange;
}

Vector4 CLight::GetLightSpecular()
{
	return m_tInfo.vSpc;
}

Matrix CLight::ComputeLightView()
{
	m_matShadowView.Identity();

	Vector3	vLightAxis[AXIS_END];

	for (int i = 0; i < AXIS_END; ++i)
	{
		vLightAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		vLightAxis[i].Normalize();
	}

	Vector3	vLightPos = m_pTransform->GetWorldPos();

	for (int i = 0; i < AXIS_END; ++i)
		memcpy(&m_matShadowView[i][0], &vLightAxis[i], sizeof(Vector3));

	m_matShadowView.Transpose();

	vLightPos *= -1.f;

	for (int i = 0; i < AXIS_END; ++i)
		m_matShadowView[3][i] = vLightPos.Dot(vLightAxis[i]);

	m_matShadowVP = m_matShadowView * m_matShadowProj;

	return m_matShadowVP;
}

void CLight::Start()
{
}

void CLight::AfterClone()
{
}

bool CLight::Init()
{
	CCamera* pCamera = m_pScene->GetMainCameraNonCount();

	m_fWidth = (float)_RESOLUTION.iWidth;
	m_fHeight = (float)_RESOLUTION.iHeight;

	m_fNear = pCamera->GetCameraNear();
	m_fRange = 100.0f;
	m_fOutAngle = pCamera->GetCameraViewAngle();

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle), m_fWidth / m_fHeight, m_fNear, 500.0f);

	return true;
}

int CLight::Input(float fTime)
{
	if (m_eLightType != LT_DIR)
		m_tInfo.vPos = Vector3(m_pTransform->GetPosDelta()._41, m_pTransform->GetPosDelta()._42, m_pTransform->GetPosDelta()._43);

	m_tInfo.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return 0;
}

int CLight::Update(float fTime)
{
	Blink(fTime);

	return 0;
}

int CLight::LateUpdate(float fTime)
{

	return 0;
}

void CLight::Collision(float fTime)
{
}

void CLight::Render(float fTime)
{
}

CLight * CLight::Clone()
{
	return new CLight(*this);
}
