#include "stdafx.h"
#include "SPBar.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CSPBar::CSPBar() :
	m_pCollider(nullptr)
{
	m_eUIType = UT_BAR;
	SetTag("SPBar");
}


CSPBar::CSPBar(const CSPBar & spbar)	:
	CUI(spbar)
{
	m_eDir = spbar.m_eDir;
	m_fValue = spbar.m_fValue;
	m_fMinValue = spbar.m_fMinValue;
	m_fMaxValue = spbar.m_fMaxValue;
	m_tCBuffer = spbar.m_tCBuffer;
	m_fValueLength = spbar.m_fValueLength;
	m_vScale = spbar.m_vScale;
}

CSPBar::~CSPBar()
{
	SAFE_RELEASE(m_pCollider);
}

void CSPBar::SetBarDir(BAR_DIR eDir)
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

void CSPBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CSPBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CSPBar::SetMinMaxValue(float fMin, float fMax)
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

void CSPBar::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;

	m_pTransform->SetWorldScale(m_vScale);
}

void CSPBar::SetScale(float x, float y, float z)
{
	m_vScale = Vector3(x, y, z);

	m_pTransform->SetWorldScale(m_vScale);
}

void CSPBar::LightOn()
{
	m_tCBuffer.vLight = Vector4::White * 2.f;
}

void CSPBar::LightOff()
{
	m_tCBuffer.vLight = Vector4::White;
}

void CSPBar::AfterClone()
{
	m_pCollider = (CColliderRect*)FindComponentFromType<CCollider>(CT_COLLIDER);
}

bool CSPBar::Init()
{
	m_fMinValue = 0.f;
	m_fMaxValue = 100.f;
	m_fValue = 100.f;
	m_tCBuffer.fPercent = 1.f;
	m_tCBuffer.vLight = Vector4::White;

	m_fValueLength = 100.f;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("SPBarRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetShader(BAR_SHADER);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("SP_Bar", sizeof(BarCBuffer));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "SP_Bar", TEXT("UI/HPMP/SP_Bar.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(80.f, 13.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	CColliderRect*	pRC = AddComponent<CColliderRect>("SPBarBody");

	m_pCollider = pRC;

	pRC->SetCollisionGroup("UI");
	pRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(95.f, 13.f, 0.f));
	pRC->SetCollisionCallback(CCT_ENTER, this, &CSPBar::Hit);
	pRC->SetCollisionCallback(CCT_LEAVE, this, &CSPBar::MouseOut);

	SAFE_RELEASE(pRC);

	SetBarDir(BD_LEFT);

	return true;
}

int CSPBar::Input(float fTime)
{
	return 0;
}

int CSPBar::Update(float fTime)
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

int CSPBar::LateUpdate(float fTime)
{
	return 0;
}

void CSPBar::Collision(float fTime)
{
}

void CSPBar::Render(float fTime)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Bar", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
}

CSPBar * CSPBar::Clone()
{
	return new CSPBar(*this);
}

void CSPBar::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CSPBar::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
