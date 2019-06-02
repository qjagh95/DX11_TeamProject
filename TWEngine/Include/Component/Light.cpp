#include "../EngineHeader.h"
#include "Light.h"
#include "../Core.h"
#include "Camera.h"
#include "../Device.h"

PUN_USING

CLight::CLight() :
	m_fAccTime(0.0f),
	m_fLimitTime(0.0f),
	m_fCheckTime(0.0f),
	m_fDeltaTime(0.0f),
	m_bBlink(false),
	m_bBlinkFinalTurnOn(false),
	m_bTurnOn(true)
{
	m_tInfo.vDif = Vector4::White;
	m_tInfo.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.vSpc = Vector4::White;
	m_tInfo.fFallOff = 0.0f;
	m_vOriginColor = Vector4::White;
	m_vChangeColor = Vector4::Black;

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
	_pInstBW->WriteData(m_tInfo.fInAngle);
	_pInstBW->WriteData(m_tInfo.fOutAngle);
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

	// 로드된 데이터를 이용하여 조명 정보를 세팅한다.
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;
	m_tInfo.iLightType = iLightType;
	m_tInfo.vPos = vPos;
	m_tInfo.vDir = vDir;
	m_tInfo.fRange = fRange;
	m_tInfo.fInAngle = fInAngle;
	m_tInfo.fOutAngle = fOutAngle;
	m_tInfo.fFallOff = fFallOff;
	m_tInfo.iRimLight = iRimLight;

	m_vOriginColor = vDif;
}

void CLight::SetLightType(LIGHT_TYPE eType)
{
	m_eLightType = eType;
	m_tInfo.iLightType = eType;

	if (eType == LT_SPOT)
		m_pTransform->SetLocalRotX(90.0f);
}

void CLight::SetLightRange(float fRange)
{
	m_fRange = fRange;
	m_tInfo.fRange = fRange;

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle), m_fWidth / m_fHeight, 0.03f, m_fRange);
}

void CLight::SetAngle(float fInAngle, float fOutAngle)
{
	m_fInAngle = fInAngle;
	m_fOutAngle = fOutAngle;

	m_tInfo.fInAngle = cosf(DegreeToRadian(fInAngle * 0.5f));
	m_tInfo.fOutAngle = cosf(DegreeToRadian(fOutAngle * 0.5f));

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle), m_fWidth / m_fHeight, 0.03f, m_fRange);
}

void CLight::SetLightColor(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc)
{
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;

	m_vOriginColor = vDif;
}

void CLight::SetLightAmbient(const Vector4 & vAmb)
{
	m_tInfo.vAmb = vAmb;
}

void CLight::SetLightDiffuse(const Vector4 & vDif)
{
	m_tInfo.vDif = vDif;
	m_vOriginColor = vDif;
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
	m_fLimitTime = fLimitTime;
	m_fDeltaTime = fDeltaTime;
	m_fCheckTime = 0.0f;
	m_fAccTime = 0.0f;
	m_vChangeColor = vColor;
	m_bBlink = true;
	m_bBlinkFinalTurnOn = bFinalTurnOn;
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
					SetLightColor(m_vOriginColor, m_vOriginColor, m_vOriginColor);
					m_bTurnOn = true;
				}
				else
				{
					SetLightColor(m_vChangeColor, m_vChangeColor, m_vChangeColor);
					m_bTurnOn = false;
				}
			}
		}
		else
		{
			m_bBlink = false;
			if (m_bBlinkFinalTurnOn)
			{
				SetLightColor(m_vOriginColor, m_vOriginColor, m_vOriginColor);
				m_bTurnOn = true;
			}
			else
			{
				SetLightColor(m_vChangeColor, m_vChangeColor, m_vChangeColor);
				m_bTurnOn = false;
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
	m_fRange = pCamera->GetCameraFar();
	m_fOutAngle = pCamera->GetCameraViewAngle();

	m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fOutAngle), m_fWidth / m_fHeight, m_fNear, m_fRange);

	return true;
}

int CLight::Input(float fTime)
{
	if (m_eLightType != LT_DIR)
		m_tInfo.vPos = m_pTransform->GetWorldPos();

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
