#include "stdafx.h"
#include "MonsterBackBar.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Animation2D.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CMonsterBackBar::CMonsterBackBar()
{
	SetTag("MonsterBackBar");
}

CMonsterBackBar::CMonsterBackBar(const CMonsterBackBar & monsbackbar)	:
	CComponent(monsbackbar)
{
	m_eDir = monsbackbar.m_eDir;
	m_fValue = monsbackbar.m_fValue;
	m_fMinValue = monsbackbar.m_fMinValue;
	m_fMaxValue = monsbackbar.m_fMaxValue;
	m_tCBuffer = monsbackbar.m_tCBuffer;
	m_fValueLength = monsbackbar.m_fValueLength;
	m_vScale = monsbackbar.m_vScale;
}

CMonsterBackBar::~CMonsterBackBar()
{
}

void CMonsterBackBar::SetBarDir(BAR_DIR eDir)
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

void CMonsterBackBar::SetValue(float fValue)
{
	m_fValue = fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMonsterBackBar::AddValue(float fValue)
{
	m_fValue += fValue;

	if (m_fValue < m_fMinValue)
		m_fValue = m_fMinValue;

	else if (m_fValue > m_fMaxValue)
		m_fValue = m_fMaxValue;

	m_tCBuffer.fPercent = (m_fValue - m_fMinValue) / m_fValueLength;
}

void CMonsterBackBar::SetMinMaxValue(float fMin, float fMax)
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

void CMonsterBackBar::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;

	m_pTransform->SetWorldScale(m_vScale);
}

void CMonsterBackBar::SetScale(float x, float y, float z)
{
	m_vScale = Vector3(x, y, z);

	m_pTransform->SetWorldScale(m_vScale);
}

void CMonsterBackBar::AfterClone()
{
}

bool CMonsterBackBar::Init()
{
	m_fMinValue = 0.f;
	m_fMaxValue = 100.f;
	m_fValue = 100.f;
	m_tCBuffer.fPercent = 1.f;
	m_tCBuffer.vLight = Vector4::White;

	m_fValueLength = 100.f;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("MonsterBackBarRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MonsterBackBar", TEXT("UI/HPMP/Back_Bar.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(80.f, 6.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	SetBarDir(BD_LEFT);

	return true;
}

int CMonsterBackBar::Input(float fTime)
{
	return 0;
}

int CMonsterBackBar::Update(float fTime)
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

int CMonsterBackBar::LateUpdate(float fTime)
{
	return 0;
}

void CMonsterBackBar::Collision(float fTime)
{
}

void CMonsterBackBar::Render(float fTime)
{
}

CMonsterBackBar * CMonsterBackBar::Clone()
{
	return new CMonsterBackBar(*this);
}

void CMonsterBackBar::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CMonsterBackBar::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
