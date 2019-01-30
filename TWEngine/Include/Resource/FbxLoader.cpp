#include "FbxLoader.h"

PUN_USING

CFbxLoader::CFbxLoader() :
	m_pManager(NULL),
	m_pScene(NULL)
{
}


CFbxLoader::~CFbxLoader()
{
	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterials[i].size(); ++j)
		{
			SAFE_DELETE(m_vecMaterials[i][j]);
		}

		m_vecMaterials[i].clear();
	}

	m_vecMaterials.clear();

	Safe_Delete_VecList(m_vecMeshContainer);

	m_pScene->Clear();
	m_pScene->Destroy();
	m_pManager->Destroy();
}

bool CFbxLoader::LoadFbx(const char * pFullPath)
{
	// FbxManager ��ü�� �����Ѵ�.
	m_pManager = FbxManager::Create();

	// IOSetting�� �����Ѵ�.
	FbxIOSettings*	pIos = FbxIOSettings::Create(m_pManager, IOSROOT);

	// FbxManager�� �����Ѵ�.
	m_pManager->SetIOSettings(pIos);

	// FbxScene�� �����Ѵ�.
	m_pScene = FbxScene::Create(m_pManager, "");

	// FbxImporter �� �̿��ؼ� fbx ���Ϸκ��� �޽� ������ �о�´�.
	FbxImporter*	pImporter = FbxImporter::Create(m_pManager, "");

	// �ش� ��ο� �ִ� fbx ������ �о���� ������ ������.
	pImporter->Initialize(pFullPath, -1, m_pManager->GetIOSettings());

	// ������ ���� ������ FbxScene�� ��带 �����Ѵ�.
	pImporter->Import(m_pScene);

	if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	Triangulate(m_pScene->GetRootNode());

	LoadMesh(m_pScene->GetRootNode());

	pImporter->Destroy();

	return true;
}

void CFbxLoader::Triangulate(FbxNode * pNode)
{
	// FbxNodeAttribute : FbxNode �Ӽ��� ��Ÿ���� �������̽��̴�.
	// ���κ��� ���� �� �ִ�.
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_pManager);

		converter.Triangulate(pAttr, true);
	}

	// ���������� �д´�.
	int	iMtrlCount = pNode->GetMaterialCount();

	if (iMtrlCount > 0)
	{
		vector<PFBXMATERIAL>	vecMtrl;

		m_vecMaterials.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : Fbx �� ������ �ִ� ���� ������
			// ����ִ�.
			FbxSurfaceMaterial*	pMtrl = pNode->GetMaterial(i);

			if (!pMtrl)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	// ���� ����� �ڽĳ�� ���� ���´�.
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(pNode->GetChild(i));
	}
}

void CFbxLoader::LoadMaterial(FbxSurfaceMaterial * pMtrl)
{
	// ���� ������ ������ ����ü�� �����Ѵ�.
	PFBXMATERIAL	pMtrlInfo = new FBXMATERIAL;

	// ���� ������ �����̳ʿ� ���� ������ �߰��Ѵ�.
	m_vecMaterials[m_vecMaterials.size() - 1].push_back(pMtrlInfo);

	// Diffuse ������ �о�´�.
	pMtrlInfo->vDif = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse,
		FbxSurfaceMaterial::sDiffuseFactor);

	pMtrlInfo->vAmb = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient,
		FbxSurfaceMaterial::sAmbientFactor);

	pMtrlInfo->vSpc = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular,
		FbxSurfaceMaterial::sSpecularFactor);

	pMtrlInfo->vEmv = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive,
		FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->fSpecularPower = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->fShininess = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->fTransparencyFactor = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	pMtrlInfo->strDifTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);
	pMtrlInfo->strBumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);

	if (pMtrlInfo->strBumpTex.empty())
		pMtrlInfo->strBumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);

	pMtrlInfo->strSpcTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 CFbxLoader::GetMaterialColor(FbxSurfaceMaterial * pMtrl,
	const char * pPropertyName, const char * pPropertyFactorName)
{
	FbxDouble3	vResult(0, 0, 0);
	double		dFactor = 0;

	// ���ڷ� ���� �������� �ش� �̸��� ���� ������Ƽ�� �ִ����� �Ǵ��Ѵ���
	// ���´�. ���������� �ش� �̸��� ������Ƽ ���Ͱ� �ִ����� �Ǵ��Ѵ���
	// ���´�.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// ��ȿ���� üũ�Ѵ�.
	if (tProperty.IsValid() && tPropertyFactor.IsValid())
	{
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor�� 1�� �ƴ� ��� ������ ���������� ����Ͽ�
		// ���� vResult �� ��� �����ش�.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}

double CFbxLoader::GetMaterialFactor(FbxSurfaceMaterial * pMtrl,
	const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	double	dFactor = 0.0;

	if (tProperty.IsValid())
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

string CFbxLoader::GetMaterialTexture(FbxSurfaceMaterial * pMtrl,
	const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	string	str = "";

	if (tProperty.IsValid())
	{
		// FbxFileTexture �� Ÿ���� ��� �ִ����� ������ �Լ��̴�.
		int	iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture*	pFileTex = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (pFileTex)
				str = pFileTex->GetFileName();
		}
	}

	return str;
}

void CFbxLoader::LoadMesh(FbxNode * pNode)
{
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh*	pMesh = pNode->GetMesh();

		if (pMesh)
			LoadMesh(pMesh);
	}

	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadMesh(pNode->GetChild(i));
	}
}

void CFbxLoader::LoadMesh(FbxMesh * pMesh)
{
	// MeshContainer�� ����� �����̳ʺ� ������ �����̳�&����� ��
	// �ε��� ������ ���ͼ� �޽��� �����ؾ� �Ѵ�.
	PFBXMESHCONTAINER	pContainer = new FBXMESHCONTAINER;

	pContainer->bBump = false;

	m_vecMeshContainer.push_back(pContainer);

	// ControlPoint �� ��ġ������ ��� �ִ� �迭�̴�.
	// �� �迭�� ������ �� ������ ������ �ȴ�.
	int	iVtxCount = pMesh->GetControlPointsCount();

	// ���������� FbxVector4Ÿ���� �迭�� �����ϰ� �ֱ� ������ �迭�� 
	// �����ּҸ� ���´�.
	FbxVector4*	pVtxPos = pMesh->GetControlPoints();

	// �����̳ʰ� ������ �ִ� ���� �������� ��������ŭ resize ���ش�.
	pContainer->vecPos.resize(iVtxCount);
	pContainer->vecNormal.resize(iVtxCount);
	pContainer->vecUV.resize(iVtxCount);
	pContainer->vecTangent.resize(iVtxCount);
	pContainer->vecBinormal.resize(iVtxCount);

	for (int i = 0; i < iVtxCount; ++i)
	{
		// y�� z���� �ٲ�� �ֱ� ������ �������ش�.
		pContainer->vecPos[i].x = (float)pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = (float)pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = (float)pVtxPos[i].mData[1];
	}

	// ������ ���� ���´�.
	int	iPolygonCount = pMesh->GetPolygonCount();

	UINT	iVtxID = 0;

	// ���� ���� ���´�.
	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();

	// ���� ���� �� ����� ���̱� ������ ���� ����ŭ resize �Ѵ�.
	pContainer->vecIndices.resize(iMtrlCount);

	// ���� ������ ���´�.
	FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial();
	int iCount = pMesh->GetElementMaterialCount();
	// �ﰢ�� ����ŭ �ݺ��Ѵ�.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// �� �������� �����ϴ� ������ ���� ���´�.
		// �ﰢ���� ��� 3���� ���´�.
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		int	iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// ���� �ﰢ���� �����ϰ� �ִ� ���ؽ����� �������� �ε�����
			// ���Ѵ�.
			int	iControlIndex = pMesh->GetPolygonVertex(i, j);

			iIdx[j] = iControlIndex;

			LoadNormal(pMesh, pContainer, iVtxID, iControlIndex);

			LoadUV(pMesh, pContainer, pMesh->GetTextureUVIndex(i, j),
				iControlIndex);

			LoadTangent(pMesh, pContainer, iVtxID, iControlIndex);

			LoadBinormal(pMesh, pContainer, iVtxID, iControlIndex);

			++iVtxID;
		}

		int	iMtrlID = pMaterial->GetIndexArray().GetAt(i);

		pContainer->vecIndices[iMtrlID].push_back(iIdx[0]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[2]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[1]);
	}
}

void CFbxLoader::LoadNormal(FbxMesh * pMesh,
	PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	// �޽��κ��� ElementNormal ������ ���´�.
	FbxGeometryElementNormal*	pNormal = pMesh->GetElementNormal();

	int	iNormalIndex = iVtxID;

	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iNormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	pContainer->vecNormal[iControlIndex].x = (float)vNormal.mData[0];
	pContainer->vecNormal[iControlIndex].y = (float)vNormal.mData[2];
	pContainer->vecNormal[iControlIndex].z = (float)vNormal.mData[1];
}

void CFbxLoader::LoadUV(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer,
	int iUVID, int iControlIndex)
{
	int iCount = pMesh->GetElementUVCount();
	FbxGeometryElementUV*	pUV = pMesh->GetElementUV(0);

	if (!pUV)
		return;

	int	iUVIndex = iUVID;

	if (pUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	else if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			//iUVIndex = pUV->GetIndexArray().GetAt(iUVIndex);
		}
		break;
		default:
			break; // other reference modes not shown here!
		}
	}

	FbxVector2	vUV = pUV->GetDirectArray().GetAt(iUVIndex);

	pContainer->vecUV[iControlIndex].x = (float)(vUV.mData[0] - (int)vUV.mData[0]);
	pContainer->vecUV[iControlIndex].y = (float)(1.f - (vUV.mData[1] - (int)vUV.mData[1]));
}

void CFbxLoader::LoadTangent(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementTangent*	pTangent = pMesh->GetElementTangent();

	if (!pTangent)
		return;

	pContainer->bBump = true;

	int	iTangentIndex = iVtxID;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	pContainer->vecTangent[iControlIndex].x = (float)vTangent.mData[0];
	pContainer->vecTangent[iControlIndex].y = (float)vTangent.mData[2];
	pContainer->vecTangent[iControlIndex].z = (float)vTangent.mData[1];
}

void CFbxLoader::LoadBinormal(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementBinormal*	pBinormal = pMesh->GetElementBinormal();

	if (!pBinormal)
		return;

	pContainer->bBump = true;

	int	iBinormalIndex = iVtxID;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	pContainer->vecBinormal[iControlIndex].x = (float)vBinormal.mData[0];
	pContainer->vecBinormal[iControlIndex].y = (float)vBinormal.mData[2];
	pContainer->vecBinormal[iControlIndex].z = (float)vBinormal.mData[1];
}
