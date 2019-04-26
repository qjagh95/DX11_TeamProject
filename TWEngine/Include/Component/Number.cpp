#include "../EngineHeader.h"
#include "Number.h"

PUN_USING

CNumber::CNumber()
{
	m_eUIType = UT_NUMBER;
	m_fActiveTime = 0.f;
	m_fPlayTime = 0.f;
	m_iDigitCount = 0;
	m_fRotAngle = 0.f;
	m_fMove = 0.f;
	m_bDotDamage = false;
}

CNumber::CNumber(const CNumber & number)	:
	CUI(number)
{
}

CNumber::~CNumber()
{
	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		SAFE_RELEASE(m_vecNumber[i]);
	}

	m_vecNumber.clear();
}

void CNumber::ClearNumber()
{
	m_bChangeNumber = true;
	m_iNumber = 0;
}

void CNumber::ClearVecNumber()
{
	for (size_t i = 0; i < m_vecNumber.size(); ++i)
	{
		SAFE_RELEASE(m_vecNumber[i]);
	}

	m_vecNumber.clear();
}

void CNumber::AddNumber(int iNumber)
{
	m_bChangeNumber = true;
	m_iNumber += iNumber;
}

void CNumber::SetNumber(int iNumber)
{
	m_iNumber = iNumber;
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	char* pNumberTex = new char[MAX_PATH];
	sprintf(pNumberTex, "%s%d", "Number", iNumber);

	TCHAR* pFileName = new TCHAR[MAX_PATH];
	memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
	wsprintf(pFileName, L"UI/Number/Font0%d.png", iNumber);
	pMaterial->SetDiffuseTex(0, pNumberTex, pFileName);
	delete[] pNumberTex;
	delete[] pFileName;
	SAFE_RELEASE(pMaterial);
}

void CNumber::SetNumberPos(Vector3 vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CNumber::SetNumberPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(Vector3(x, y, z));
}

void CNumber::SetNumberPivot(Vector3 vPivot)
{
	m_pTransform->SetWorldPos(vPivot);
}

void CNumber::SetNumberPivot(float x, float y, float z)
{
	m_pTransform->SetWorldPos(Vector3(x, y, z));
}

void CNumber::AfterClone()
{
}

bool CNumber::Init()
{
	m_iNumber = 1;

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DNumberRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();
	SAFE_RELEASE(pRenderer);

	m_pObject->SetRenderGroup(RG_UI);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(15.f, 15.f, 0.f);

	return true;
}

int CNumber::Input(float fTime)
{
	return 0;
}

int CNumber::Update(float fTime)
{
	return 0;
}

int CNumber::LateUpdate(float fTime)
{
	return 0;
}

void CNumber::Collision(float fTime)
{
}

void CNumber::Render(float fTime)
{
}

CNumber * CNumber::Clone()
{
	return new CNumber(*this);
}
