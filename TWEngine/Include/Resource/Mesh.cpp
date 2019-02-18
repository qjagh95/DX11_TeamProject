#include "Mesh.h"
#include "../Device.h"
#include "../PathManager.h"
#include "FbxLoader.h"
#include "../Component/Material.h"
#include "../Component/Animation.h"

PUN_USING

CMesh::CMesh() :
	m_pMaterial(nullptr),
	m_pAnimation(nullptr)
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pMaterial);

	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		SAFE_RELEASE(m_vecMeshContainer[i]->tVB.pBuffer);
		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVB.pData);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIB[j].pBuffer);
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIB[j].pData);
		}

		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();
}

Vector3 CMesh::GetCenter() const
{
	return m_vCenter;
}

Vector3 CMesh::GetMin() const
{
	return m_vMin;
}

Vector3 CMesh::GetMax() const
{
	return m_vMax;
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

float CMesh::GetRadius() const
{
	return m_fRadius;
}

string CMesh::GetShaderKey() const
{
	return m_strShaderKey;
}

string CMesh::GetInputLayoutKey() const
{
	return m_strInputLayoutKey;
}

size_t CMesh::GetContainCount() const
{
	return m_vecMeshContainer.size();
}

size_t CMesh::GetSubsetCount(int iContainer) const
{
	return m_vecMeshContainer[iContainer]->vecIB.size();
}

bool CMesh::CreateMesh(const string & strName, const string & strShaderKey, const string & strLayoutKey,
	void * pVtx, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pIdx, int iIdxCount, int iIdxSize,
	D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	SetTag(strName);
	m_strShaderKey = strShaderKey;
	m_strInputLayoutKey = strLayoutKey;

	PMeshContainer	pContainer = new MeshContainer;

	pContainer->ePrimitive = ePrimitive;

	m_vecMeshContainer.push_back(pContainer);

	if (!CreateVertexBuffer(pVtx, iVtxCount, iVtxSize, eVtxUsage))
		return false;

	if (pIdx)
	{
		if (!CreateIndexBuffer(pIdx, iIdxCount, iIdxSize, eIdxUsage, eFmt))
			return false;
	}

	return true;
}

bool CMesh::LoadMesh(const string & strName, const TCHAR * pFileName,
	const string & strPathKey)
{
	TCHAR	strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);
	lstrcat(strFullPath, pFileName);

	return LoadMeshFromFullPath(strName, strFullPath);
}

bool CMesh::LoadMeshFromFullPath(const string & strName,
	const TCHAR * pFullPath)
{
	m_strTag = strName;

	char	strFullPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_UTF8, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), 0, 0);

	char	strExt[_MAX_EXT] = {};

	_splitpath_s(strFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
	{
		CFbxLoader	loader;

		if (!loader.LoadFbx(strFullPath))
			return false;

		return ConvertFbx(&loader, strFullPath);
	}

	return LoadFromFullPath(strFullPath);
}

void CMesh::Render()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->ePrimitive);

		UINT	iStride = m_vecMeshContainer[i]->tVB.iSize;
		UINT	iOffset = 0;
		CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->tVB.pBuffer, &iStride, &iOffset);

		if (m_vecMeshContainer[i]->vecIB.empty())
		{
			CONTEXT->Draw(m_vecMeshContainer[i]->tVB.iCount, 0);
		}

		else
		{
			for (size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
			{
				CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j].pBuffer,
					m_vecMeshContainer[i]->vecIB[j].eFmt, 0);
				CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j].iCount, 0, 0);
			}
		}

	}
}

void CMesh::Render(int iContainer, int iSubset)
{
	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->ePrimitive);

	UINT	iStride = m_vecMeshContainer[iContainer]->tVB.iSize;
	UINT	iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[iContainer]->tVB.pBuffer, &iStride, &iOffset);

	if (m_vecMeshContainer[iContainer]->vecIB.empty())
	{
		CONTEXT->Draw(m_vecMeshContainer[iContainer]->tVB.iCount, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIB[iSubset].pBuffer,
			m_vecMeshContainer[iContainer]->vecIB[iSubset].eFmt, 0);
		CONTEXT->DrawIndexed(m_vecMeshContainer[iContainer]->vecIB[iSubset].iCount, 0, 0);
	}
}

void CMesh::UpdateVertexBuffer(void * pData, int iContainer)
{
	if (iContainer < 0 || iContainer >= m_vecMeshContainer.size())
		assert(false);

	PMeshContainer	pContainer = m_vecMeshContainer[iContainer];

	memcpy(pContainer->tVB.pData, pData,
		pContainer->tVB.iSize * pContainer->tVB.iCount);

	switch (pContainer->tVB.eUsage)
	{
		case D3D11_USAGE_DEFAULT:
			CONTEXT->UpdateSubresource(pContainer->tVB.pBuffer,
				0, nullptr, pData, 0, 0);
			break;
		case D3D11_USAGE_DYNAMIC:
		{
			D3D11_MAPPED_SUBRESOURCE	tMap;
			CONTEXT->Map(pContainer->tVB.pBuffer, 0, D3D11_MAP_WRITE_DISCARD,
				0, &tMap);

			memcpy(tMap.pData, pData, pContainer->tVB.iSize * pContainer->tVB.iCount);

			CONTEXT->Unmap(pContainer->tVB.pBuffer, 0);
		}
		break;
	}
}

CMaterial * CMesh::CloneMaterial()
{
	if (!m_pMaterial)
		return nullptr;

	return m_pMaterial->Clone();
}

CAnimation * CMesh::CloneAnimation()
{
	if (!m_pAnimation)
		return nullptr;

	return m_pAnimation->Clone();
}

bool CMesh::CreateVertexBuffer(void * pData, int iCount, int iSize, D3D11_USAGE eUsage)
{
	PMeshContainer	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	pContainer->tVB.iCount = iCount;
	pContainer->tVB.iSize = iSize;
	pContainer->tVB.eUsage = eUsage;
	pContainer->tVB.pData = new char[iCount * iSize];

	memcpy(pContainer->tVB.pData, pData, iCount * iSize);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iCount * iSize;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = eUsage;

	if (eUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tResource = {};
	tResource.pSysMem = pContainer->tVB.pData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tResource, &pContainer->tVB.pBuffer)))
		return false;

	char*	pVertices = (char*)pData;
	Vector3	vPos;
	memcpy(&vPos, pVertices, sizeof(Vector3));

	m_vMin = vPos;
	m_vMax = vPos;

	for (int i = 1; i < iCount; ++i)
	{
		memcpy(&vPos, pVertices + iSize * i, sizeof(Vector3));
		if (m_vMin.x > vPos.x)
			m_vMin.x = vPos.x;

		if (m_vMin.y > vPos.y)
			m_vMin.y = vPos.y;

		if (m_vMin.z > vPos.z)
			m_vMin.z = vPos.z;

		if (m_vMax.x < vPos.x)
			m_vMax.x = vPos.x;

		if (m_vMax.y < vPos.y)
			m_vMax.y = vPos.y;

		if (m_vMax.z < vPos.z)
			m_vMax.z = vPos.z;
	}

	m_vCenter = (m_vMin + m_vMax) / 2.f;
	m_vLength = m_vMax - m_vMin;
	m_fRadius = m_vLength.Length() / 2.f;

	return true;
}

bool CMesh::CreateIndexBuffer(void * pData, int iCount, int iSize, D3D11_USAGE eUsage, DXGI_FORMAT eFmt)
{
	PMeshContainer	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	IndexBuffer	tIB;

	tIB.iCount = iCount;
	tIB.iSize = iSize;
	tIB.eUsage = eUsage;
	tIB.pData = new char[iCount * iSize];
	tIB.eFmt = eFmt;

	memcpy(tIB.pData, pData, iCount * iSize);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iCount * iSize;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.Usage = eUsage;

	if (eUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tResource = {};
	tResource.pSysMem = tIB.pData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tResource, &tIB.pBuffer)))
		return false;

	pContainer->vecIB.push_back(tIB);

	return true;
}

bool CMesh::ConvertFbx(CFbxLoader * pLoader, const char* pFullPath)
{
	const vector<PFBXMESHCONTAINER>*	pvecContainer = pLoader->GetMeshContainers();
	const vector<vector<PFBXMATERIAL>>*	pvecMaterials = pLoader->GetMaterials();

	vector<PFBXMESHCONTAINER>::const_iterator	iter;
	vector<PFBXMESHCONTAINER>::const_iterator	iterEnd = pvecContainer->end();

	vector<bool>	vecEmptyIndex;

	for (iter = pvecContainer->begin(); iter != iterEnd; ++iter)
	{
		PMeshContainer	pContainer = new MeshContainer;

		m_strInputLayoutKey = VERTEX3D_LAYOUT;

		m_vecMeshContainer.push_back(pContainer);

		int	iVtxSize = 0;

		// 범프가 있을 경우
		if ((*iter)->bBump)
		{
			if ((*iter)->bAnimation)
			{
				m_strShaderKey = STANDARD_BUMP_ANIM_SHADER;
			}

			else
			{
				m_strShaderKey = STANDARD_BUMP_SHADER;
			}
		}

		// 범프가 없을 경우
		else
		{
			if ((*iter)->bAnimation)
			{
				m_strShaderKey = STANDARD_TEX_NORMAL_ANIM_SHADER;
			}

			else
			{
				m_strShaderKey = STANDARD_TEX_NORMAL_SHADER;
			}
		}

		vector<Vertex3D>	vecVtx;
		iVtxSize = sizeof(Vertex3D);

		for (size_t i = 0; i < (*iter)->vecPos.size(); ++i)
		{
			Vertex3D	tVtx = {};

			tVtx.vPos = (*iter)->vecPos[i];
			tVtx.vNormal = (*iter)->vecNormal[i];
			tVtx.vUV = (*iter)->vecUV[i];

			if (!(*iter)->vecTangent.empty())
				tVtx.vTangent = (*iter)->vecTangent[i];

			if (!(*iter)->vecBinormal.empty())
				tVtx.vBinormal = (*iter)->vecBinormal[i];

			if (!(*iter)->vecBlendWeight.empty())
			{
				tVtx.vWeight = (*iter)->vecBlendWeight[i];
				tVtx.vIndex = (*iter)->vecBlendIndex[i];
			}

			vecVtx.push_back(tVtx);
		}

		pContainer->ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		if (!CreateVertexBuffer(&vecVtx[0], vecVtx.size(),
			iVtxSize, D3D11_USAGE_DEFAULT))
			return false;

		// 인덱스버퍼 생성
		for (size_t i = 0; i < (*iter)->vecIndices.size(); ++i)
		{
			if ((*iter)->vecIndices[i].empty())
			{
				vecEmptyIndex.push_back(false);
				continue;
			}

			vecEmptyIndex.push_back(true);

			if (!CreateIndexBuffer(&(*iter)->vecIndices[i][0],
				(*iter)->vecIndices[i].size(), 4,
				D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT))
				return false;
		}
	}

	// 재질 정보를 읽어온다.
	const vector<vector<PFBXMATERIAL>>*	pMaterials = pLoader->GetMaterials();

	vector<vector<PFBXMATERIAL>>::const_iterator	iterM;
	vector<vector<PFBXMATERIAL>>::const_iterator	iterMEnd = pMaterials->end();

	if (!pMaterials->empty())
	{
		// 실제 사용할 재질 클래스를 생성한다.
		m_pMaterial = new CMaterial;

		if (!m_pMaterial->Init())
		{
			SAFE_RELEASE(m_pMaterial);
			return NULL;
		}

		m_pMaterial->ClearContainer();
	}

	int	iContainer = 0;
	for (iterM = pMaterials->begin(); iterM != iterMEnd; ++iterM, ++iContainer)
	{
		for (size_t i = 0; i < (*iterM).size(); ++i)
		{
			// 인덱스 버퍼가 비어있을 경우에는 재질을 추가하지 않는다.
			if (!vecEmptyIndex[i])
				continue;

			// 재질 정보를 얻어온다.
			PFBXMATERIAL	pMtrl = (*iterM)[i];

			m_pMaterial->SetMaterial(pMtrl->vDif, pMtrl->vAmb,
				pMtrl->vSpc, pMtrl->fShininess, pMtrl->vEmv, iContainer, i);

			// 이름을 불러온다.
			char	strName[MAX_PATH] = {};
			_splitpath_s(pMtrl->strDifTex.c_str(), NULL, 0, NULL, 0,
				strName, MAX_PATH, NULL, 0);

			TCHAR	strPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pMtrl->strDifTex.c_str(),
				-1, strPath, pMtrl->strDifTex.length());
#endif // UNICODE

			m_pMaterial->SetSampler(0, SAMPLER_LINEAR, iContainer, i);
			m_pMaterial->SetDiffuseTexFromFullPath(0, strName,
				strPath, iContainer, i);

			if (!pMtrl->strBumpTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strBumpTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(wchar_t) * MAX_PATH);

				MultiByteToWideChar(CP_ACP, 0, pMtrl->strBumpTex.c_str(),
					-1, strPath, pMtrl->strBumpTex.length());

				m_pMaterial->SetNormalSampler(0, SAMPLER_LINEAR, iContainer, i);
				m_pMaterial->SetNormalTexFromFullPath(1, strName, strPath, iContainer, i);
			}

			if (!pMtrl->strSpcTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strSpcTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(wchar_t) * MAX_PATH);

				MultiByteToWideChar(CP_ACP, 0, pMtrl->strSpcTex.c_str(),
					-1, strPath, pMtrl->strSpcTex.length());

				m_pMaterial->SetSpecularSampler(0, SAMPLER_LINEAR, iContainer, i);
				m_pMaterial->SetSpecularTexFromFullPath(2, strName, strPath, iContainer, i);
			}
		}
	}

	// 텍스쳐가 저장된 폴더명을 키로 변경한다.
	//char	strFullName[MAX_PATH] = {};
	//iterM = pMaterials->begin();
	//strcpy_s(strFullName, (*iterM)[0]->strDifTex.c_str());

	//int	iLength = strlen(strFullName);
	//for (int i = iLength - 1; i >= 0; --i)
	//{
	//	if (strFullName[i] == '\\' || strFullName[i] == '/')
	//	{
	//		memset(strFullName + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
	//		strFullName[i] = '\\';
	//		//strFullName[i] = 0;
	//		break;
	//	}
	//}

	//char	strChange[MAX_PATH] = {};
	//strcpy_s(strChange, strFullName);
	//iLength = strlen(strChange);
	//for (int i = iLength - 2; i >= 0; --i)
	//{
	//	if (strChange[i] == '\\' || strChange[i] == '/')
	//	{
	//		memset(strChange + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
	//		break;
	//	}
	//}

	//strcat_s(strChange, m_strTag.c_str());
	//strcat_s(strChange, "\\");

	//MoveFileA(strFullName, strChange);

	// Mesh\\ 까지의 경로를 제거한다.
	/*iLength = strlen(strChange);
	for (int i = iLength - 2; i >= 0; --i)
	{
		char	cText[5] = {};
		memcpy(cText, &strChange[i - 4], 4);
		_strupr_s(cText);

		if (strcmp(cText, "MESH") == 0)
		{
			memset(strChange, 0, sizeof(char) * (i + 1));
			memcpy(strChange, &strChange[i + 1], sizeof(char) * (iLength - (i + 1)));
			memset(strChange + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
			break;
		}
	}*/

	/*for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
	PMESHCONTAINER	pContainer = m_vecMeshContainer[i];

	for (size_t j = 0; j < pContainer->vecMaterial.size(); ++j)
	{
	pContainer->vecMaterial[j]->SetTexturePathKey(MESH_PATH);
	pContainer->vecMaterial[j]->ChangeTexturePath(strChange);
	}
	}*/

	m_vLength = m_vMax - m_vMin;

	m_vCenter = (m_vMax + m_vMin) / 2.f;
	m_fRadius = m_vLength.Length() / 2.f;

	// 애니메이션 처리
	const vector<PFBXBONE>*	pvecBone = pLoader->GetBones();

	if (pvecBone->empty())
		return true;

	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation;

	if (!m_pAnimation->Init())
	{
		SAFE_RELEASE(m_pAnimation);
		return false;
	}

	//// 본 수만큼 반복한다.
	vector<PFBXBONE>::const_iterator	iterB;
	vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		PBONE	pBone = new BONE;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		float	fMat[4][4];

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				fMat[i][j] = (*iterB)->matOffset.mData[i].mData[j];
			}
		}

		pBone->matOffset = new Matrix;
		*pBone->matOffset = fMat;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				fMat[i][j] = (*iterB)->matBone.mData[i].mData[j];
			}
		}

		pBone->matBone = new Matrix;
		*pBone->matBone = fMat;

		m_pAnimation->AddBone(pBone);
	}

	m_pAnimation->CreateBoneTexture();

	// 애니메이션 클립을 추가한다.
	const vector<PFBXANIMATIONCLIP>* pvecClip = pLoader->GetClips();

	// 클립을 읽어온다.
	vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		m_pAnimation->AddClip(AO_LOOP, *iterC);
	}

	char	strFullPath[MAX_PATH] = {};
	strcpy_s(strFullPath, pFullPath);
	int	iPathLength = strlen(strFullPath);
	memcpy(&strFullPath[iPathLength - 3], "msh", 3);

	SaveFromFullPath(strFullPath);

	if (m_pAnimation)
	{
		memcpy(&strFullPath[iPathLength - 3], "bne", 3);
		m_pAnimation->SaveBoneFromFullPath(strFullPath);

		memcpy(&strFullPath[iPathLength - 3], "anm", 3);
		m_pAnimation->SaveFromFullPath(strFullPath);
	}

	return true;
}

bool CMesh::Save(const char * pFileName, const string & strPathKey)
{
	char	strFullPath[MAX_PATH] = {};
	const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);
	strcat_s(strFullPath, pFileName);

	return SaveFromFullPath(strFullPath);
}

bool CMesh::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;

	int	iLength = m_strTag.length();

	// Tag 길이를 저장한다.
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);

	// ShaderName 길이를 저장한다.
	iLength = m_strShaderKey.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strShaderKey.c_str(), 1, iLength, pFile);

	// 입력레이아웃 이름 길이를 저장한다.
	iLength = m_strInputLayoutKey.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strInputLayoutKey.c_str(), 1, iLength, pFile);

	fwrite(&m_vCenter, sizeof(Vector3), 1, pFile);
	fwrite(&m_fRadius, sizeof(float), 1, pFile);
	fwrite(&m_vMin, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMax, sizeof(Vector3), 1, pFile);
	fwrite(&m_vLength, sizeof(Vector3), 1, pFile);

	size_t	iContainer = m_vecMeshContainer.size();

	fwrite(&iContainer, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iContainer; ++i)
	{
		PMeshContainer	pContainer = m_vecMeshContainer[i];

		fwrite(&pContainer->ePrimitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY),
			1, pFile);

		fwrite(&pContainer->tVB.iSize, sizeof(int), 1, pFile);
		fwrite(&pContainer->tVB.iCount, sizeof(int), 1, pFile);
		fwrite(&pContainer->tVB.eUsage, sizeof(D3D11_USAGE), 1, pFile);
		fwrite(pContainer->tVB.pData, pContainer->tVB.iSize,
			pContainer->tVB.iCount, pFile);

		size_t	iIdxCount = pContainer->vecIB.size();

		fwrite(&iIdxCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iIdxCount; ++j)
		{
			fwrite(&pContainer->vecIB[j].eFmt, sizeof(DXGI_FORMAT),
				1, pFile);
			fwrite(&pContainer->vecIB[j].iSize, sizeof(int), 1, pFile);
			fwrite(&pContainer->vecIB[j].iCount, sizeof(int), 1, pFile);
			fwrite(&pContainer->vecIB[j].eUsage, sizeof(D3D11_USAGE), 1, pFile);
			fwrite(pContainer->vecIB[j].pData, pContainer->vecIB[j].iSize,
				pContainer->vecIB[j].iCount, pFile);
		}
	}

	bool	bMaterial = false;

	if (m_pMaterial)
	{
		bMaterial = true;

		fwrite(&bMaterial, sizeof(bool), 1, pFile);

		m_pMaterial->Save(pFile);
	}

	else
		fwrite(&bMaterial, sizeof(bool), 1, pFile);

	fclose(pFile);

	return true;
}

bool CMesh::Load(const char * pFileName, const string & strPathKey)
{
	char	strFullPath[MAX_PATH] = {};
	const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);
	strcat_s(strFullPath, pFileName);

	return LoadFromFullPath(strFullPath);
}

bool CMesh::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	int	iLength = 0;

	// Tag 길이를 저장한다.
	fread(&iLength, 4, 1, pFile);
	char	strTag[256] = {};
	fread(strTag, 1, iLength, pFile);
	m_strTag = strTag;

	// ShaderName 길이를 저장한다.
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	char	strShaderKey[256] = {};
	fread(strShaderKey, 1, iLength, pFile);
	m_strShaderKey = strShaderKey;

	// 입력레이아웃 이름 길이를 저장한다.
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	char	strLayoutKey[256] = {};
	fread(strLayoutKey, 1, iLength, pFile);
	m_strInputLayoutKey = strLayoutKey;

	fread(&m_vCenter, sizeof(Vector3), 1, pFile);
	fread(&m_fRadius, sizeof(float), 1, pFile);
	fread(&m_vMin, sizeof(Vector3), 1, pFile);
	fread(&m_vMax, sizeof(Vector3), 1, pFile);
	fread(&m_vLength, sizeof(Vector3), 1, pFile);

	size_t	iContainer = 0;

	fread(&iContainer, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iContainer; ++i)
	{
		PMeshContainer	pContainer = new MeshContainer;
		m_vecMeshContainer.push_back(pContainer);

		int	iVtxSize = 0;
		int	iVtxCount = 0;
		D3D11_USAGE	eUsage;

		fread(&pContainer->ePrimitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, pFile);
		fread(&iVtxSize, sizeof(int), 1, pFile);
		fread(&iVtxCount, sizeof(int), 1, pFile);
		fread(&eUsage, sizeof(D3D11_USAGE), 1, pFile);
		char*	pData = new char[iVtxSize * iVtxCount];
		fread(pData, iVtxSize, iVtxCount, pFile);

		CreateVertexBuffer(pData, iVtxCount, iVtxSize, eUsage);

		SAFE_DELETE_ARRAY(pData);

		size_t	iIdxCount = 0;

		fread(&iIdxCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iIdxCount; ++j)
		{
			DXGI_FORMAT	eFmt;
			int	iIdxSize = 0;
			int	iIdxCount = 0;
			fread(&eFmt, sizeof(DXGI_FORMAT), 1, pFile);
			fread(&iIdxSize, sizeof(int), 1, pFile);
			fread(&iIdxCount, sizeof(int), 1, pFile);
			fread(&eUsage, sizeof(D3D11_USAGE), 1, pFile);

			pData = new char[iIdxSize * iIdxCount];

			fread(pData, iIdxSize, iIdxCount, pFile);

			CreateIndexBuffer(pData, iIdxCount, iIdxSize, eUsage, eFmt);

			SAFE_DELETE_ARRAY(pData);
		}
	}

	bool	bMaterial = false;
	fread(&bMaterial, sizeof(bool), 1, pFile);

	if (bMaterial)
	{
		SAFE_RELEASE(m_pMaterial);
		bMaterial = true;

		m_pMaterial = new CMaterial;

		m_pMaterial->Init();

		m_pMaterial->Load(pFile);
	}

	fclose(pFile);

	return true;
}
