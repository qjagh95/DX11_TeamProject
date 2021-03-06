#include "EngineHeader.h"
#include "UIBar.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "Animation2D.h"
#include "ColliderRect.h"
#include "../GameObject.h"

PUN_USING

CUIBar::CUIBar()
{
	m_eUIType = UT_BAR;
	SetTag("UIBar");
	m_iFlag = -1;
}

CUIBar::CUIBar(const CUIBar & bar) :
	CUI(bar)
{
	m_eDir = bar.m_eDir;
	m_fValue = bar.m_fValue;
	m_fMinValue = bar.m_fMinValue;
	m_fMaxValue = bar.m_fMaxValue;
	m_tCBuffer = bar.m_tCBuffer;
	m_fValueLength = bar.m_fValueLength;
	m_vScale = bar.m_vScale;
}

CUIBar::~CUIBar()
{
}

void CUIBar::SetBarDir(BAR_DIR eDir)
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

void CUIBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CUIBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CUIBar::SetMinMaxValue(float fMin, float fMax)
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

void CUIBar::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;

	m_pTransform->SetWorldScale(m_vScale);
}

void CUIBar::SetScale(float x, float y, float z)
{
	m_vScale = Vector3(x, y, z);

	m_pTransform->SetWorldScale(m_vScale);
}

void CUIBar::LightOn()
{
	m_tCBuffer.vLight = Vector4::White * 2.f;
}

void CUIBar::LightOff()
{
	m_tCBuffer.vLight = Vector4::White;
}

void CUIBar::SetColor(Vector4 vColor)
{
	m_tCBuffer.vLight = vColor;
}

float CUIBar::GetValue() const
{
	return m_fValue;
}

void CUIBar::AfterClone()
{
}

void CUIBar::SetVisible(float fBatteryTime)
{
	if (fBatteryTime < 0.5f)
	{
		m_pObject->SetEnable(true);
		m_tCBuffer.vLight = Vector4::Red;
	}

	if (fBatteryTime >= 0.5f)
	{
		m_pObject->SetEnable(false);
	}
}

bool CUIBar::Init()
{
	m_fMinValue = 0.f;
	m_fMaxValue = 100.f;
	m_fValue = 100.f;
	m_tCBuffer.fPercent = 100.f;
	m_tCBuffer.vLight = Vector4::White;

	m_fValueLength = 100.f;

	m_pObject->SetRenderGroup(RG_UI);

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("BarRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetShader(BAR_SHADER);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	pRenderer->CreateRendererCBuffer("Bar", sizeof(BarCBuffer));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "HandycamBar", TEXT("UI/CameraBar.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(90.f, 12.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	SetBarDir(BD_LEFT);

	return true;
}

int CUIBar::Input(float fTime)
{
	return 0;
}

int CUIBar::Update(float fTime)
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
		break;
	case BD_UP:
		vScale.y *= m_tCBuffer.fPercent;
		vMin = vScale - m_vScale;
		m_pTransform->SetWorldScale(vScale);
		break;
	case BD_DOWN:
		vScale.y *= m_tCBuffer.fPercent;
		m_pTransform->SetWorldScale(vScale);
		break;
	}

	return 0;
}

int CUIBar::LateUpdate(float fTime)
{
	return 0;
}

void CUIBar::Collision(float fTime)
{
}

void CUIBar::Render(float fTime)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRenderer->UpdateRendererCBuffer("Bar", &m_tCBuffer, sizeof(m_tCBuffer));

	SAFE_RELEASE(pRenderer);
}

CUIBar * CUIBar::Clone()
{
	return new CUIBar(*this);
}

void CUIBar::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CUIBar::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
