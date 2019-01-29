#include "stdafx.h"
#include "DamageNumber.h"
#include "Renderer.h"
#include "Material.h"
#include "../GameObject.h"
#include "Transform.h"

PUN_USING

CDamageNumber::CDamageNumber()
{
	m_eUIType = UT_NUMBER;
	m_fActiveTime = 0.f;
	m_fPlayTime = 0.f;
	m_iDigitCount = 0;
	m_fRotAngle = 0.f;
	m_fMove = 0.f;
	m_bDotDamage = false;
}

CDamageNumber::CDamageNumber(const CDamageNumber & dnumber)	:
	CUI(dnumber)
{
}

CDamageNumber::~CDamageNumber()
{
	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		SAFE_RELEASE(m_vecNumber[i]);
	}

	m_vecNumber.clear();
}

void CDamageNumber::SetDotDamage(bool bDotDamage)
{
	m_bDotDamage = bDotDamage;
}

void CDamageNumber::ClearNumber()
{
	m_bChangeNumber = true;
	m_iNumber = 0;
}

void CDamageNumber::ClearVecNumber()
{
	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		SAFE_RELEASE(m_vecNumber[i]);
	}

	m_vecNumber.clear();
}

void CDamageNumber::AddNumber(int iNumber)
{
	m_bChangeNumber = true;
	m_iNumber += iNumber;
}

void CDamageNumber::SetNumber(int iNumber)
{
	m_iNumber = iNumber;
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	char* pNumberTex = new char[MAX_PATH];
	sprintf(pNumberTex, "%s%d", "DamageNumber", iNumber);

	TCHAR* pFileName = new TCHAR[MAX_PATH];
	memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
	wsprintf(pFileName, L"Number/D%d.png", iNumber);
	pMaterial->SetDiffuseTex(0, pNumberTex, pFileName);
	delete[] pNumberTex;
	delete[] pFileName;
	SAFE_RELEASE(pMaterial);

}

void CDamageNumber::SetNumberPos(Vector3 vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CDamageNumber::SetNumberPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(Vector3(x, y, z));
}

void CDamageNumber::SetNumberPivot(Vector3 vPivot)
{
	m_pTransform->SetWorldPos(vPivot);
}

void CDamageNumber::SetNumberPivot(float x, float y, float z)
{
	m_pTransform->SetWorldPos(Vector3(x, y, z));
}

void CDamageNumber::AfterClone()
{
}

bool CDamageNumber::Init()
{
	m_iNumber = 1;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DNumberRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetMaterial(Vector4::White);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(23.f, 29.f, 0.f);
	
	return true;
}

int CDamageNumber::Input(float fTime)
{
	return 0;
}

int CDamageNumber::Update(float fTime)
{
	m_fActiveTime += fTime;
	Vector3 vPos = m_pTransform->GetWorldPos();	
	
	if (m_bDotDamage)
	{
		vPos.y += 50.f * fTime;
	}
	else
	{
		m_fMove += 1;

		vPos.x += m_fMove * fTime;

		if (m_fActiveTime >= 0.7f)
			vPos.y -= m_fMove * fTime;
		else
			vPos.y += m_fMove * fTime;
	}

	m_pTransform->SetWorldPos(vPos);

	if (m_fActiveTime >= 1.f)
	{
		m_pObject->Die();
	}

	return 0;
}

int CDamageNumber::LateUpdate(float fTime)
{
	return 0;
}

void CDamageNumber::Collision(float fTime)
{
}

void CDamageNumber::Render(float fTime)
{
}

CDamageNumber * CDamageNumber::Clone()
{
	return new CDamageNumber(*this);
}
