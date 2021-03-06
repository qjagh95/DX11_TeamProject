#include "../EngineHeader.h"
#include "Decal.h"
#include "Renderer.h"
#include "Material.h"
#include "../GameObject.h"

PUN_USING

CDecal::CDecal():
	m_pRenderer(nullptr)
{
	SetTag("Decal");
	m_eComType = CT_DECAL;
}

CDecal::CDecal(const CDecal & decal) :
	CComponent(decal)
{
	m_pRenderer = nullptr;
}

CDecal::~CDecal()
{
	SAFE_RELEASE(m_pRenderer);

}

void CDecal::Start()
{
}

void CDecal::AfterClone()
{
}

bool CDecal::Init()
{
	m_pObject->SetRenderGroup(RG_DECAL);

	m_pRenderer = m_pObject->AddComponent<CRenderer>("DecalRenderer");
	m_pRenderer->SetSave(false);
	m_pRenderer->SetMesh("VolumeBox");
	m_pRenderer->SetRenderState(ALPHA_BLEND);
	m_pRenderer->SetRenderState(CULL_NONE);
	
	return true;
}

int CDecal::Input(float fTime)
{
	return 0;
}

int CDecal::Update(float fTime)
{
	return 0;
}

int CDecal::LateUpdate(float fTime)
{
	return 0;
}

void CDecal::Collision(float fTime)
{
}

void CDecal::Render(float fTime)
{
}

CDecal * CDecal::Clone()
{
	return new CDecal(*this);
}

void CDecal::Save(BinaryWrite * _pInstBW)
{
	int iSize = 0;
	iSize = (int)m_strDiffuseTex.length();
	_pInstBW->WriteData(iSize);
	if (iSize > 0)
	{
		_pInstBW->WriteData(m_strDiffuseTex, iSize);
	}

	iSize = (int)m_strNormalTex.length();
	_pInstBW->WriteData(iSize);
	if (iSize > 0)
	{
		_pInstBW->WriteData(m_strNormalTex, iSize);
	}

	iSize = (int)m_strSpecTex.length();
	_pInstBW->WriteData(iSize);
	if (iSize > 0)
	{
		_pInstBW->WriteData(m_strSpecTex, iSize);
	}
}

void CDecal::Load(BinaryRead * _pInstBR)
{
	int iSize = 0;
	CMaterial* pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	_pInstBR->ReadData(iSize);
	if (iSize > 0)
	{
		string strTemp;
		wstring wstrTemp;
		_pInstBR->ReadData(m_strDiffuseTex, iSize);
		strTemp = m_strDiffuseTex;
		strTemp += ".png";
		wstrTemp.assign(strTemp.begin(), strTemp.end());
		pMaterial->SetDiffuseTex(0, m_strDiffuseTex, wstrTemp.c_str());
		pMaterial->SetSampler(0, SAMPLER_LINEAR);
	}

	_pInstBR->ReadData(iSize);
	if (iSize > 0)
	{
		string strTemp;
		wstring wstrTemp;
		_pInstBR->ReadData(m_strNormalTex, iSize);
		strTemp = m_strNormalTex;
		strTemp += ".png";
		wstrTemp.assign(strTemp.begin(), strTemp.end());
		pMaterial->SetNormalTex(1, m_strNormalTex, wstrTemp.c_str());
		pMaterial->SetNormalSampler(0, SAMPLER_LINEAR);
	}

	_pInstBR->ReadData(iSize);
	if (iSize > 0)
	{
		string strTemp;
		wstring wstrTemp;
		_pInstBR->ReadData(m_strSpecTex, iSize);
		strTemp = m_strSpecTex;
		strTemp += ".png";
		wstrTemp.assign(strTemp.begin(), strTemp.end());
		pMaterial->SetSpecularTex(2, m_strSpecTex, wstrTemp.c_str());
		pMaterial->SetSpecularSampler(0, SAMPLER_LINEAR);
	}
	SAFE_RELEASE(pMaterial);
}
