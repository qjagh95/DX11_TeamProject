#include "../EngineHeader.h"
#include "Light.h"

PUN_USING

CLight::CLight()
{
	m_tInfo.vDif = Vector4::White;
	m_tInfo.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.vSpc = Vector4::White;
	m_tInfo.fFallOff = 0.0f;
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

void CLight::SetLightDirection(const Vector3& Dir)
{
	m_tInfo.vDir = Dir;
}

void CLight::UpdateLightCBuffer()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Light", &m_tInfo);
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
		m_pTransform->SetWorldPos(m_tInfo.vPos);

	if (m_tInfo.iLightType == LT_SPOT)
		m_pTransform->SetWorldRot(RadianToDegree(m_tInfo.vDir.y), RadianToDegree(m_tInfo.vDir.x), 0.0f);

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
