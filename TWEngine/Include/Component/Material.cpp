#include "EngineHeader.h"
#include "Material.h"
#include "../Resource/Texture.h"
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
			if (mtrl.m_vecMaterial[i][j]->pDiffuse)
			{
				pMtrl->pDiffuse = new TextureSet;
				*pMtrl->pDiffuse = *mtrl.m_vecMaterial[i][j]->pDiffuse;
				pMtrl->pDiffuse->pTex->AddRef();
				pMtrl->pDiffuse->pSampler->AddRef();
			}

			if (mtrl.m_vecMaterial[i][j]->pNormal)
			{
				pMtrl->pNormal = new TextureSet;
				*pMtrl->pNormal = *mtrl.m_vecMaterial[i][j]->pNormal;
				pMtrl->pNormal->pTex->AddRef();
				pMtrl->pNormal->pSampler->AddRef();
			}

			if (mtrl.m_vecMaterial[i][j]->pSpecular)
			{
				pMtrl->pSpecular = new TextureSet;
				*pMtrl->pSpecular = *mtrl.m_vecMaterial[i][j]->pSpecular;
				pMtrl->pSpecular->pTex->AddRef();
				pMtrl->pSpecular->pSampler->AddRef();
			}

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

	m_iRefCount = 1;
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

			if (m_vecMaterial[i][j]->pDiffuse)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pDiffuse->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pDiffuse->pSampler);
			}

			if (m_vecMaterial[i][j]->pNormal)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pNormal->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pNormal->pSampler);
			}

			if (m_vecMaterial[i][j]->pSpecular)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pSpecular->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pSpecular->pSampler);
			}

			SAFE_DELETE(m_vecMaterial[i][j]->pDiffuse);
			SAFE_DELETE(m_vecMaterial[i][j]->pNormal);
			SAFE_DELETE(m_vecMaterial[i][j]->pSpecular);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}

		m_vecMaterial[i].clear();
	}

	m_vecMaterial.clear();
}

void CMaterial::SetMaterial(const Vector4 & vDiffuse, const Vector4& vAmbient,
	const Vector4& vSpecular, float fSpecularPower,
	const Vector4& vEmissive, int iContainer, int iSubset)
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
	m_vecMaterial[iContainer][iSubset]->tMaterial.vAmbient = vAmbient;
	m_vecMaterial[iContainer][iSubset]->tMaterial.vSpecular = vSpecular;
	m_vecMaterial[iContainer][iSubset]->tMaterial.vEmissive = vEmissive;
	m_vecMaterial[iContainer][iSubset]->tMaterial.vSpecular.w = fSpecularPower;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	pMaterial->pDiffuse->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, pFileName, strPathKey);
	pMaterial->pDiffuse->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	GET_SINGLE(CResourcesManager)->CreateTextureFromFullPath(strKey, pFullPath);
	pMaterial->pDiffuse->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, vecFileName, strPathKey);
	pMaterial->pDiffuse->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, vecFullPath);
	pMaterial->pDiffuse->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pTex);
	pTex->AddRef();
	pMaterial->pDiffuse->pTex = pTex;
	pMaterial->pDiffuse->iRegister = iRegister;
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

	if (!pMaterial->pDiffuse)
		pMaterial->pDiffuse = new TextureSet;

	SAFE_RELEASE(pMaterial->pDiffuse->pSampler);
	pMaterial->pDiffuse->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
	pMaterial->pDiffuse->iSamplerRegister = iRegister;
}

void CMaterial::SetNormalTex(int iRegister, const string & strKey, const TCHAR * pFileName, const string & strPathKey, int iContainer, int iSubset)
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

	if (!pMaterial->pNormal)
		pMaterial->pNormal = new TextureSet;

	SAFE_RELEASE(pMaterial->pNormal->pTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, pFileName, strPathKey);
	pMaterial->pNormal->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pNormal->iRegister = iRegister;
}

void CMaterial::SetNormalTexFromFullPath(int iRegister, const string & strKey, const TCHAR * pFullPath, int iContainer, int iSubset)
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

	if (!pMaterial->pNormal)
		pMaterial->pNormal = new TextureSet;

	SAFE_RELEASE(pMaterial->pNormal->pTex);
	GET_SINGLE(CResourcesManager)->CreateTextureFromFullPath(strKey, pFullPath);
	pMaterial->pNormal->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pNormal->iRegister = iRegister;
}

void CMaterial::SetNormalSampler(int iRegister, const string & strKey, int iContainer, int iSubset)
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

	if (!pMaterial->pNormal)
		pMaterial->pNormal = new TextureSet;

	SAFE_RELEASE(pMaterial->pNormal->pSampler);
	pMaterial->pNormal->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
	pMaterial->pNormal->iSamplerRegister = iRegister;
}

void CMaterial::SetSpecularTex(int iRegister, const string & strKey, const TCHAR * pFileName, const string & strPathKey, int iContainer, int iSubset)
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

	if (!pMaterial->pSpecular)
		pMaterial->pSpecular = new TextureSet;

	SAFE_RELEASE(pMaterial->pSpecular->pTex);
	GET_SINGLE(CResourcesManager)->CreateTexture(strKey, pFileName, strPathKey);
	pMaterial->pSpecular->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pSpecular->iRegister = iRegister;
}

void CMaterial::SetSpecularTexFromFullPath(int iRegister, const string & strKey, const TCHAR * pFullPath, int iContainer, int iSubset)
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

	if (!pMaterial->pSpecular)
		pMaterial->pSpecular = new TextureSet;

	SAFE_RELEASE(pMaterial->pSpecular->pTex);
	GET_SINGLE(CResourcesManager)->CreateTextureFromFullPath(strKey, pFullPath);
	pMaterial->pSpecular->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	pMaterial->pSpecular->iRegister = iRegister;
}

void CMaterial::SetSpecularSampler(int iRegister, const string & strKey, int iContainer, int iSubset)
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

	if (!pMaterial->pSpecular)
		pMaterial->pSpecular = new TextureSet;

	SAFE_RELEASE(pMaterial->pSpecular->pSampler);
	pMaterial->pSpecular->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
	pMaterial->pSpecular->iSamplerRegister = iRegister;
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

void CMaterial::ClearContainer()
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

			if (m_vecMaterial[i][j]->pDiffuse)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pDiffuse->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pDiffuse->pSampler);
			}

			if (m_vecMaterial[i][j]->pNormal)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pNormal->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pNormal->pSampler);
			}

			if (m_vecMaterial[i][j]->pSpecular)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->pSpecular->pTex);
				SAFE_RELEASE(m_vecMaterial[i][j]->pSpecular->pSampler);
			}

			SAFE_DELETE(m_vecMaterial[i][j]->pDiffuse);
			SAFE_DELETE(m_vecMaterial[i][j]->pNormal);
			SAFE_DELETE(m_vecMaterial[i][j]->pSpecular);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}

		m_vecMaterial[i].clear();
	}

	m_vecMaterial.clear();
}

bool CMaterial::Init()
{
	SetMaterial(Vector4::White, Vector4::White, Vector4::White,
		3.2f, Vector4::White);

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

void CMaterial::Save(FILE * pFile)
{
	size_t	iContainer = m_vecMaterial.size();
	fwrite(&iContainer, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iContainer; ++i)
	{
		size_t iSubset = m_vecMaterial[i].size();
		fwrite(&iSubset, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iSubset; ++j)
		{
			PSubsetMaterial	pSubset = m_vecMaterial[i][j];

			fwrite(&pSubset->tMaterial, sizeof(Material), 1, pFile);

			SaveTextureSet(pFile, pSubset->pDiffuse);
			SaveTextureSet(pFile, pSubset->pNormal);
			SaveTextureSet(pFile, pSubset->pSpecular);

			size_t	iMultiTexCount = pSubset->vecMultiTex.size();

			fwrite(&iMultiTexCount, sizeof(size_t), 1, pFile);

			for (size_t k = 0; k < iMultiTexCount; ++k)
			{
				SaveTextureSet(pFile, &pSubset->vecMultiTex[k]);
			}
		}
	}
}

void CMaterial::Load(FILE * pFile)
{
	ClearContainer();


	size_t	iContainer = 0;
	fread(&iContainer, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iContainer; ++i)
	{
		size_t iSubset = 0;
		fread(&iSubset, sizeof(size_t), 1, pFile);

		vector<PSubsetMaterial>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);

		for (size_t j = 0; j < iSubset; ++j)
		{
			PSubsetMaterial	pSubset = new SubsetMaterial;
			m_vecMaterial[i].push_back(pSubset);

			fread(&pSubset->tMaterial, sizeof(Material), 1, pFile);

			LoadTextureSet(pFile, &pSubset->pDiffuse);
			LoadTextureSet(pFile, &pSubset->pNormal);
			LoadTextureSet(pFile, &pSubset->pSpecular);

			size_t	iMultiTexCount = pSubset->vecMultiTex.size();

			fread(&iMultiTexCount, sizeof(size_t), 1, pFile);

			PTextureSet pTexSet = nullptr;
			for (size_t k = 0; k < iMultiTexCount; ++k)
			{
				pTexSet = &pSubset->vecMultiTex[k];
				LoadTextureSet(pFile, &pTexSet);
			}
		}
	}
}

void CMaterial::SaveFromPath(const char * pFileName, const string & strPathKey)
{
}

void CMaterial::LoadFromPath(const char * pFileName, const string & strPathKey)
{
}

void CMaterial::SetShader(int iContainer, int iSubset)
{
	PSubsetMaterial	pMaterial = m_vecMaterial[iContainer][iSubset];

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material",
		&pMaterial->tMaterial);

	if (pMaterial->pDiffuse)
	{
		pMaterial->pDiffuse->pTex->SetShader(pMaterial->pDiffuse->iRegister);
		pMaterial->pDiffuse->pSampler->SetShader(pMaterial->pDiffuse->iSamplerRegister);
	}

	if (pMaterial->pNormal)
	{
		pMaterial->pNormal->pTex->SetShader(pMaterial->pNormal->iRegister);
		pMaterial->pNormal->pSampler->SetShader(pMaterial->pNormal->iSamplerRegister);
		pMaterial->tMaterial.vDiffuse.w = 1.f; 
		// 재질의 Normal이 있을경우 vDiffuse의 w값은 1이 된다.(차후에 변수 변경해야됨)
	}
	else // Normal 이 없을경우
	{
		pMaterial->tMaterial.vDiffuse.w = 0.f; 
		// Diffuse의 w값이 0이된다.(차후에 변수 변경해야됨)
	}

	if (pMaterial->pSpecular)
	{
		pMaterial->pSpecular->pTex->SetShader(pMaterial->pSpecular->iRegister);
		pMaterial->pSpecular->pSampler->SetShader(pMaterial->pSpecular->iSamplerRegister);
		pMaterial->tMaterial.vAmbient.w = 1.f;
		//재질의 Specular값이 없을경우 vAmbient의 w값은 1이 된다.(차후에 변수 변경해야됨)
	}
	else
	{
		pMaterial->tMaterial.vAmbient.w = 0.f;
		//없을 경우 vAmbient의 w값은 0이된다.(차후에 변수 변경해야됨)
	}
	//현재 Diffuse나 Ambient의 w값이 사용처가 없어서 specular나 Normal의 유무 체크용
	//으로 사용하고 있음

	for (size_t i = 0; i < pMaterial->vecMultiTex.size(); ++i)
	{
		pMaterial->vecMultiTex[i].pTex->SetShader(pMaterial->vecMultiTex[i].iRegister);
		pMaterial->vecMultiTex[i].pSampler->SetShader(pMaterial->vecMultiTex[i].iSamplerRegister);
	}

	CShaderManager::GetInst()->UpdateCBuffer("Material", &pMaterial->tMaterial);
}

PSubsetMaterial CMaterial::CreateSubset()
{
	PSubsetMaterial pMtrl = new SubsetMaterial;

	return pMtrl;
}

void CMaterial::SaveTextureSet(FILE * pFile, PTextureSet pTexture)
{
	bool	bTexEnable = false;

	if (pTexture)
	{
		bTexEnable = true;
		fwrite(&bTexEnable, sizeof(bool), 1, pFile);

		string	strTexKey = pTexture->pTex->GetTag();
		size_t	iKeyLength = strTexKey.length();
		fwrite(&iKeyLength, sizeof(size_t), 1, pFile);
		fwrite(strTexKey.c_str(), 1, iKeyLength, pFile);

		// Texture 경로를 얻어온다.
		const vector<TCHAR*>* pPathList =
			pTexture->pTex->GetFullPath();

		size_t iPathCount = pPathList->size();
		fwrite(&iPathCount, sizeof(size_t), 1, pFile);

		vector<TCHAR*>::const_iterator	iter;
		vector<TCHAR*>::const_iterator	iterEnd = pPathList->end();

		for (iter = pPathList->begin(); iter != iterEnd; ++iter)
		{
			int	iPathLength = lstrlen(*iter);
			char	strPath[MAX_PATH] = {};

#ifdef UNICODE
			WideCharToMultiByte(CP_ACP, 0, *iter, -1, strPath, iPathLength, 0, 0);
#else
			strcpy_s(strPath, *iter);
#endif // UNICODE
			_strupr_s(strPath);

			for (int k = iPathLength - 1; k >= 0; --k)
			{
				if (strPath[k] == '\\' || strPath[k] == '/')
				{
					char	strBin[3];
					strBin[0] = 'N';
					strBin[1] = 'I';
					strBin[2] = 'B';
					bool	bEnable = true;
					for (int l = 1; l < 4; ++l)
					{
						if (strPath[k - l] != strBin[l - 1])
						{
							bEnable = false;
							break;
						}
					}

					if (bEnable)
					{
						char	strSavePath[MAX_PATH] = {};
						int	iSaveCount = iPathLength - (k + 1);
						memcpy(strSavePath, &strPath[k + 1], sizeof(char) * iSaveCount);
						fwrite(&iSaveCount, sizeof(int), 1, pFile);
						fwrite(strSavePath, sizeof(char), iSaveCount, pFile);
						break;
					}
				}
			}
		}

		// Sampler Key
		size_t	iSmpKeyLength = pTexture->pSampler->GetTag().length();

		fwrite(&iSmpKeyLength, sizeof(size_t), 1, pFile);
		fwrite(pTexture->pSampler->GetTag().c_str(), sizeof(char),
			iSmpKeyLength, pFile);

		fwrite(&pTexture->iRegister, sizeof(int), 1, pFile);
		fwrite(&pTexture->iSamplerRegister, sizeof(int), 1, pFile);
	}

	else
		fwrite(&bTexEnable, sizeof(bool), 1, pFile);
}

void CMaterial::LoadTextureSet(FILE * pFile, PTextureSet * ppTexture)
{
	bool	bTexEnable = false;
	fread(&bTexEnable, sizeof(bool), 1, pFile);

	if (bTexEnable)
	{
		char	strTexKey[256] = {};
		size_t	iKeyLength = 0;
		fread(&iKeyLength, sizeof(size_t), 1, pFile);
		fread(strTexKey, 1, iKeyLength, pFile);

		size_t iPathCount = 0;
		fread(&iPathCount, sizeof(size_t), 1, pFile);

		if (iPathCount == 1)
		{
			char	strPath[MAX_PATH] = {};
			int	iSaveCount = 0;
			fread(&iSaveCount, sizeof(int), 1, pFile);
			fread(strPath, sizeof(char), iSaveCount, pFile);

			TCHAR	strLoadPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strPath, -1, strLoadPath, (int)strlen(strPath) * 2);
#else
			strcpy_s(strLoadPath, strPath);
#endif // UNICODE

			*ppTexture = new TextureSet;

			GET_SINGLE(CResourcesManager)->CreateTexture(strTexKey, strLoadPath, ROOT_PATH);
			(*ppTexture)->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);
		}

		else
		{
			vector<const TCHAR*>	vecPath;
			for (size_t i = 0; i < iPathCount; ++i)
			{
				char	strPath[MAX_PATH] = {};
				int	iSaveCount = 0;
				fread(&iSaveCount, sizeof(int), 1, pFile);
				fread(strPath, sizeof(char), iSaveCount, pFile);

				TCHAR*	strLoadPath = new TCHAR[MAX_PATH];
				memset(strLoadPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strPath, -1, strLoadPath, (int)strlen(strPath) * 2);
#else
				strcpy_s(strLoadPath, strPath);
#endif // UNICODE
				vecPath.push_back(strLoadPath);
			}
			*ppTexture = new TextureSet;

			GET_SINGLE(CResourcesManager)->CreateTexture(strTexKey, vecPath, ROOT_PATH);
			(*ppTexture)->pTex = GET_SINGLE(CResourcesManager)->FindTexture(strTexKey);
		}

		// Sampler Key
		size_t	iSmpKeyLength = 0;

		fread(&iSmpKeyLength, sizeof(size_t), 1, pFile);
		char	strSmpKey[256] = {};
		fread(strSmpKey, sizeof(char), iSmpKeyLength, pFile);
		(*ppTexture)->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

		fread(&(*ppTexture)->iRegister, sizeof(int), 1, pFile);
		fread(&(*ppTexture)->iSamplerRegister, sizeof(int), 1, pFile);
	}
}
