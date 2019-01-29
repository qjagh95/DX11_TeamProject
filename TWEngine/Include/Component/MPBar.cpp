#include "stdafx.h"
#include "MPBar.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CMPBar::CMPBar() :
	m_pCollider(nullptr)
{
	m_eUIType = UT_BAR;
	SetTag("MPBar");
}

CMPBar::CMPBar(const CMPBar & mpbar)	:
	CUI(mpbar)
{
	m_eDir = mpbar.m_eDir;
	m_fValue = mpbar.m_fValue;
	m_fMinValue = mpbar.m_fMinValue;
	m_fMaxValue = mpbar.m_fMaxValue;
	m_tCBuffer = mpbar.m_tCBuffer;
	m_fValueLength = mpbar.m_fValueLength;
	m_vScale = mpbar.m_vScale;
}

CMPBar::~CMPBar()
{
	SAFE_RELEASE(m_pCollider);
}

void CMPBar::SetBarDir(BAR_DIR eDir)
{
	m_eDir = eDir;
	m_tCBuffer.iMoveDir = eDir;
	Vector3	vPos;

	switch (m_eDir)
	{
	case BD_LEFT:
		m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);
		break;
	case BD_RIGHT:
		m_pTransform->SetWorldPivot(1.f, 0.f, 0.f);
		vPos.x = m_pTransform->GetWorldScale().x;
		m_pTransform->Move(vPos);
		break;
	case BD_UP:
		m_pTransform->SetWorldPivot(0.f, 1.f, 0.f);
		vPos.y = m_pTransform->GetWorldScale().y;
		m_pTransform->Move(vPos);
		break;
	case BD_DOWN:
		m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);
		break;
	}
}

void CMPBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMPBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMPBar::SetMinMaxValue(float fMin, float fMax)
{
	m_fMinValue = fMin;
	m_fMaxValue = fMax;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_fValueLength = m_fMaxValue - m_fMinValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMPBar::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;

	m_pTransform->SetWorldScale(m_vScale);
}

void CMPBar::SetScale(float x, float y, float z)
{
	m_vScale = Vector3(x, y, z);

	m_pTransform->SetWorldScale(m_vScale);
}

void CMPBar::LightOn()
{
	m_tCBuffer.vLight = Vector4::White * 2.f;
}

void CMPBar::LightOff()
{
	m_tCBuffer.vLight = Vector4::White;
}

void CMPBar::AfterClone()
{
	m_pCollider = (CColliderRect*)FindComponentFromType<CCollider>(CT_COLLIDER);
}

bool CMPBar::Init()
{
	m_fMinValue = 0.f;
	m_fMaxValue = 100.f;
	m_fValue = 100.f;
	m_tCBuffer.fPercent = 1.f;
	m_tCBuffer.vLight = Vector4::White;

	m_fValueLength = 100.f;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("MPBarRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetShader(BAR_SHADER);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("MP_Bar", sizeof(BarCBuffer));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MP_Bar", TEXT("UI/HPMP/MP_Bar.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(80.f, 13.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	CColliderRect*	pRC = AddComponent<CColliderRect>("MPBarBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(95.f, 13.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CMPBar::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CMPBar::MouseOut);

	SAFE_RELEASE(pRC);

	SetBarDir(BD_LEFT);

	return true;
}

int CMPBar::Input(float fTime)
{
	return 0;
}

int CMPBar::Update(float fTime)
{
	Vector3	vScale = m_vScale;

	Vector3	vMin;

	switch (m_eDir)
	{
	case BD_LEFT:
		vScale.x *= m_tCBuffer.fPercent;
		m_pTransform->SetWorldScale(vScale);
		break;
	case BD_RIGHT:
		vScale.x *= m_tCBuffer.fPercent;
		vMin = vScale - m_vScale;
		m_pTransform->SetWorldScale(vScale);
		m_pCollider->SetInfo(vMin, vScale);
		break;
	case BD_UP:
		vScale.y *= m_tCBuffer.fPercent;
		vMin = vScale - m_vScale;
		m_pTransform->SetWorldScale(vScale);
		m_pCollider->SetInfo(vMin, vScale);
		break;
	case BD_DOWN:
		vScale.y *= m_tCBuffer.fPercent;
		m_pTransform->SetWorldScale(vScale);
		break;
	}

	return 0;
}

int CMPBar::LateUpdate(float fTime)
{
	return 0;
}

void CMPBar::Collision(float fTime)
{
}

void CMPBar::Render(float fTime)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Bar", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
}

CMPBar * CMPBar::Clone()
{
	return new CMPBar(*this);
}

void CMPBar::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CMPBar::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
