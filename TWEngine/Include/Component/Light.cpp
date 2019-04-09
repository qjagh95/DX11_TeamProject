#include "../EngineHeader.h"
#include "Light.h"
#include "Core.h"

PUN_USING

CLight::CLight()
{
	m_tInfo.vDif = Vector4::White;
	m_tInfo.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.vSpc = Vector4::White;
	m_tInfo.fFallOff = 0.0f;
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
}

void CLight::SetLightType(LIGHT_TYPE eType)
{
	m_tInfo.iLightType = eType;
}

void CLight::SetLightRange(float fRange)
{
	m_tInfo.fRange = fRange;
}

void CLight::SetAngle(float fInAngle, float fOutAngle)
{
	m_fInAngle = fInAngle;
	m_fOutAngle = fOutAngle;

	m_tInfo.fInAngle = cosf(DegreeToRadian(fInAngle / 2.f));
	m_tInfo.fOutAngle = cosf(DegreeToRadian(fOutAngle / 2.f));
}

void CLight::SetLightColor(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc)
{
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;
}

void CLight::SetLightAmbient(const Vector4 & vAmb)
{
	m_tInfo.vAmb = vAmb;
}

void CLight::SetLightDiffuse(const Vector4 & vDif)
{
	m_tInfo.vDif = vDif;
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

void CLight::Start()
{
}

void CLight::AfterClone()
{
}

bool CLight::Init()
{
	m_pTransform->SetLocalRotX(90.0f);

	return true;
}

int CLight::Input(float fTime)
{
	return 0;
}

int CLight::Update(float fTime)
{
	return 0;
}

int CLight::LateUpdate(float fTime)
{
	if (m_tInfo.iLightType != LT_DIR)
		m_tInfo.vPos = m_pTransform->GetWorldPos();

	if (m_tInfo.iLightType == LT_SPOT)
		m_pTransform->SetWorldRot(RadianToDegree(m_tInfo.vDir.y), RadianToDegree(m_tInfo.vDir.x), 0.0f);

	SetRimColor(1.f, 1.f, 1.f);
	SetRimPower(3.f);

	static bool bPush = false;
	if (GetAsyncKeyState('R') & 0x8000)
	{
		bPush = true;
	}

	else if (bPush)
	{
		bPush = false;
		m_iRim *= -1;

		SetRimLight(m_iRim);
	}

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
