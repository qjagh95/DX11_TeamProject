#include "stdafx.h"
#include "MonsterBar.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CMonsterBar::CMonsterBar()
{
	SetTag("MonsterBar");
}

CMonsterBar::CMonsterBar(const CMonsterBar & monsbar)	:
	CComponent(monsbar)
{
	m_eDir = monsbar.m_eDir;
	m_fValue = monsbar.m_fValue;
	m_fMinValue = monsbar.m_fMinValue;
	m_fMaxValue = monsbar.m_fMaxValue;
	m_tCBuffer = monsbar.m_tCBuffer;
	m_fValueLength = monsbar.m_fValueLength;
	m_vScale = monsbar.m_vScale;
}

CMonsterBar::~CMonsterBar()
{
}

void CMonsterBar::SetBarDir(BAR_DIR eDir)
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

void CMonsterBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMonsterBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMonsterBar::SetMinMaxValue(float fMin, float fMax)
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

void CMonsterBar::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;

	m_pTransform->SetWorldScale(m_vScale);
}

void CMonsterBar::SetScale(float x, float y, float z)
{
	m_vScale = Vector3(x, y, z);

	m_pTransform->SetWorldScale(m_vScale);
}

void CMonsterBar::AfterClone()
{
}

bool CMonsterBar::Init()
{
	m_fMinValue = 0.f;
	m_fMaxValue = 100.f;
	m_fValue = 100.f;
	m_tCBuffer.fPercent = 1.f;
	m_tCBuffer.vLight = Vector4::White;

	m_fValueLength = 100.f;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("MonsterBarRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MonsterBar", TEXT("UI/HPMP/HP_Bar.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(80.f, 6.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	SetBarDir(BD_LEFT);

	return true;
}

int CMonsterBar::Input(float fTime)
{
	return 0;
}

int CMonsterBar::Update(float fTime)
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

int CMonsterBar::LateUpdate(float fTime)
{
	return 0;
}

void CMonsterBar::Collision(float fTime)
{
}

void CMonsterBar::Render(float fTime)
{
}

CMonsterBar * CMonsterBar::Clone()
{
	return new CMonsterBar(*this);
}

void CMonsterBar::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CMonsterBar::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
