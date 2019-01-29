#include "stdafx.h"
#include "Material.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourcesManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Resource/Sampler.h"
#include "Animation2D.h"
#include "../GameObject.h"

PUN_USING

CMaterial::CMaterial()
{
	m_eComType = CT_MATERIAL;
}

CMaterial::CMaterial(const CMaterial & mtrl) :
	CComponent(mtrl)
{
	m_vecMaterial.clear();

	for (size_t i = 0; i < mtrl.m_vecMaterial.size(); ++i)
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
		for (size_t j = 0; j < mtrl.m_vecMaterial[i].size(); ++j)
		{
			PSubsetMaterial	pMtrl = new SubsetMaterial;

			pMtrl->tMaterial = mtrl.m_vecMaterial[i][j]->tMaterial;
			pMtrl->pDiffuseTex = mtrl.m_vecMaterial[i][j]->pDiffuseTex;
			pMtrl->iDiffuseRegister = mtrl.m_vecMaterial[i][j]->iDiffuseRegister;
			pMtrl->pDifSampler = mtrl.m_vecMaterial[i][j]->pDifSampler;
			pMtrl->iDifSamplerRegister = mtrl.m_vecMaterial[i][j]->iDifSamplerRegister;

			if (pMtrl->pDiffuseTex)
				pMtrl->pDiffuseTex->AddRef();

			if (pMtrl->pDifSampler)
				pMtrl->pDifSampler->AddRef();

			pMtrl->vecMultiTex = mtrl.m_vecMaterial[i][j]->vecMultiTex;

			for (size_t k = 0; k < pMtrl->vecMultiTex.size(); ++k)
			{
				if (pMtrl->vecMultiTex[k].pTex)
					pMtrl->vecMultiTex[k].pTex->AddRef();

				if (pMtrl->vecMultiTex[k].pSampler)
					pMtrl->vecMultiTex[k].pSampler->AddRef();
			}

			m_vecMaterial[i].push_back(pMtrl);
		}
	}
}

CMaterial::~CMaterial()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			for (size_t k = 0; k < m_vecMaterial[i][j]->vecMultiTex.size(); ++k)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTex[k].pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTex[k].pSampler);
			}

			SAFE_RELEASE(m_vecMaterial[i][j]->pDiffuseTex);
			SAFE_RELEASE(m_vecMaterial[i][j]->pDifSampler);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}

		m_vecMaterial[i].clear();
	}

	m_vecMaterial.clear();
}

void CMaterial::SetMaterial(const Vector4 & vDiffuse, int iContainer,
	int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	m_vecMaterial[iContainer][iSubset]->tMaterial.vDiffuse = vDiffuse;
}

void CMaterial::SetDiffuseTex(int iRegister, const string & strKey,
	int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	pMaterial->pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetDiffuseTex(int iRegister, const string & strKey,
	const TCHAR * pFileName, const string & strPathKey,
	int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, pFileName, strPathKey);
	pMaterial->pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetDiffuseTexFromFullPath(int iRegister, const string & strKey,
	const TCHAR * pFullPath, int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	GET_SINGLE(CResourcesManager)->CreateTextureFromFullPath(strKey, pFullPath);
	pMaterial->pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetDiffuseTex(int iRegister, const string & strKey,
	const vector<const TCHAR*>& vecFileName, const string & strPathKey,
	int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, vecFileName, strPathKey);
	pMaterial->pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetDiffuseTexFromFullPath(int iRegister, const string & strKey,
	const vector<const TCHAR*>& vecFullPath, int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	GET_SINGLE(CResourcesManager)->CreateTextureFromFullPath(strKey, vecFullPath);
	pMaterial->pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetDiffuseTex(int iRegister, CTexture * pTex,
	int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDiffuseTex);
	pTex->AddRef();
	pMaterial->pDiffuseTex = pTex;
	pMaterial->iDiffuseRegister = iRegister;
}

void CMaterial::SetSampler(int iRegister, const string & strKey,
	int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	SAFE_RELEASE(pMaterial->pDifSampler);
	pMaterial->pDifSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
	pMaterial->iDifSamplerRegister = iRegister;
}

void CMaterial::AddMultiTex(int iSmpRegister, const string& strSmpKey, int iRegister,
	const string & strKey, const TCHAR * pFileName,
	const string & strPathKey, int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<PSubsetMaterial>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
	{
		m_vecMaterial[iContainer].push_back(CreateSubset());
	}

	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	if (pMaterial->vecMultiTex.size() == 4)
		return;

	TextureSet	tSet = {};

	tSet.iRegister = iRegister;
	tSet.iSamplerRegister = iSmpRegister;

	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, pFileName, strPathKey);
	tSet.pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	tSet.pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);

	pMaterial->vecMultiTex.push_back(tSet);
}

bool CMaterial::Init()
{
	SetMaterial(Vector4::White);

	return true;
}

int CMaterial::Input(float fTime)
{
	return 0;
}

int CMaterial::Update(float fTime)
{
	return 0;
}

int CMaterial::LateUpdate(float fTime)
{
	return 0;
}

void CMaterial::Collision(float fTime)
{
}

void CMaterial::Render(float fTime)
{
}

CMaterial * CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::SetShader(int iContainer, int iSubset)
{
	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material",
		&pMaterial->tMaterial);

	if (pMaterial->pDiffuseTex)
		pMaterial->pDiffuseTex->SetShader(pMaterial->iDiffuseRegister);

	if (pMaterial->pDifSampler)
		pMaterial->pDifSampler->SetShader(pMaterial->iDifSamplerRegister);

	for (size_t i = 0; i < pMaterial->vecMultiTex.size(); ++i)
	{
		pMaterial->vecMultiTex[i].pTex->SetShader(pMaterial->vecMultiTex[i].iRegister);
		pMaterial->vecMultiTex[i].pSampler->SetShader(pMaterial->vecMultiTex[i].iSamplerRegister);
	}
}

PSubsetMaterial CMaterial::CreateSubset()
{
	PSubsetMaterial pMtrl = new SubsetMaterial;

	pMtrl->pDifSampler = GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_LINEAR);

	return pMtrl;
}
