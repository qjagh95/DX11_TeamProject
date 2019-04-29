#include "../EngineHeader.h"
#include "LandScape.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../PathManager.h"
#include "../NavigationMesh.h"
#include "../NavigationManager3D.h"
#include "../Resource/ResourcesManager.h"
#include "../Core.h"
#include "../Device.h"
#include "../EditManager.h"
#include "../NavigationManager3D.h"

PUN_USING

CLandScape::CLandScape() :
	m_pNavMesh(nullptr)
{
	m_eComType = CT_LANDSCAPE;
	m_Mesh = NULLPTR;
	m_isMove = false;
	if (CCore::GetInst()->m_bEditorMode == true)
		CEditManager::GetInst()->SetNaviMove(m_isMove);

	m_tCBuffer = {};
	m_tNaviCBuffer.iBrushXRange = 5;
	m_tNaviCBuffer.iBrushZRange = 5;
	m_tNaviCBuffer.BrushColor = Vector4::Yellow;
	m_tNaviCBuffer.BrushPos = Vector3(-1.0f, -1.0f, -1.0f);

	m_tGridCBuffer.GridLineThickness = 0.5f;
	m_tGridCBuffer.isVisibleGrid = 1;
	m_tGridCBuffer.GridLineSize = 5;
	m_tGridCBuffer.GridColor = Vector4::White;

	m_isShow = true;
}

CLandScape::CLandScape(const CLandScape & landscape)
	:CComponent(landscape)
{
	*this = landscape;
	m_iRefCount = 1;
}

CLandScape::~CLandScape()
{
}

bool CLandScape::CreateLandScape(const string& strName, unsigned int iNumX, unsigned int iNumZ, const string& strTexKey, const TCHAR* pTexName, const TCHAR* pTexNormal, const TCHAR* pTexSpecular, const char * pFileName, const string & strPathKey)
{
	m_iNumX = iNumX;
	m_iNumZ = iNumZ;

	unsigned char* pHeight = nullptr;
	int	iPixelSize = 0;

	if (pFileName)
	{
		const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);
		char	strFullPath[MAX_PATH] = {};

		if (pPath)
			strcpy_s(strFullPath, pPath);

		strcat_s(strFullPath, pFileName);

		FILE*	pFile = nullptr;

		fopen_s(&pFile, strFullPath, "rb");

		if (pFile)
		{
			BITMAPFILEHEADER	fh;
			BITMAPINFOHEADER	ih;

			fread(&fh, sizeof(fh), 1, pFile);
			fread(&ih, sizeof(ih), 1, pFile);

			m_iNumX = ih.biWidth;
			m_iNumZ = ih.biHeight;

			iPixelSize = ih.biBitCount / 8;
			pHeight = new unsigned char[m_iNumX * m_iNumZ * iPixelSize];
			fread(pHeight, sizeof(unsigned char), m_iNumX * m_iNumZ * iPixelSize, pFile);

			unsigned char*	pLine = new unsigned char[m_iNumX * iPixelSize];

			for (int i = 0; i < (int)m_iNumZ / 2; ++i)
			{
				memcpy(pLine, &pHeight[i * m_iNumX * iPixelSize], sizeof(unsigned char) * (m_iNumX * iPixelSize));
				memcpy(&pHeight[i * m_iNumX * iPixelSize], &pHeight[(m_iNumZ - i - 1) * m_iNumX * iPixelSize], sizeof(unsigned char) * (m_iNumX * iPixelSize));
				memcpy(&pHeight[(m_iNumZ - i - 1) * m_iNumX * iPixelSize], pLine, sizeof(unsigned char) * (m_iNumX * iPixelSize));
			}

			SAFE_DELETE_ARRAY(pLine);

			fclose(pFile);
		}
	}

	// 정점정보를 만든다.
	for (unsigned int i = 0; i < m_iNumZ; ++i)
	{
		for (unsigned int j = 0; j < m_iNumX; ++j)
		{
			Vertex3DColor tVtx = {};

			float	y = 0.f;

			if (pHeight)
			{
				y = pHeight[i * m_iNumX * iPixelSize + j * iPixelSize] * 0.05f;
			}

			tVtx.vPos = Vector3((float)j, (float)y, (float)(m_iNumZ - i - 1));
			//tVtx.vNormal = Vector3(0.f, 0.f, 0.f);
			tVtx.vUV = Vector2((float)j / (float)(m_iNumX - 1), (float)i / (float)(m_iNumZ - 1));
			tVtx.vColor = Vector4(0.0f, 1.0f, 0.5f, 1.0f);

			//tVtx.vTangent = Vector3(1.f, 0.f, 0.f);
			//tVtx.vBinormal = Vector3(0.f, 0.f, -1.f);

			m_vecVtx.push_back(tVtx);
		}
	}

	m_vecFaceNormal.resize((m_iNumX - 1) * (m_iNumZ - 1) * 2);

	int	iTriIdx = 0;
	Vector3	vEdge1, vEdge2;
	// 인덱스 정보를 만든다.
	for (int i = 0; i < (int)m_iNumZ - 1; ++i)
	{
		for (int j = 0; j < (int)m_iNumX - 1; ++j)
		{
			int	iAddr = i * (int)m_iNumX + j;

			// 우상단 삼각형
			m_vecIdx.push_back(iAddr);
			m_vecIdx.push_back(iAddr + 1);
			m_vecIdx.push_back(iAddr + m_iNumX + 1);

			vEdge1 = m_vecVtx[iAddr + 1].vPos - m_vecVtx[iAddr].vPos;
			vEdge2 = m_vecVtx[iAddr + m_iNumX + 1].vPos - m_vecVtx[iAddr].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			m_vecFaceNormal[iTriIdx] = vEdge1.Cross(vEdge2);
			m_vecFaceNormal[iTriIdx].Normalize();
			++iTriIdx;

			// 좌하단 삼각형
			m_vecIdx.push_back(iAddr);
			m_vecIdx.push_back(iAddr + m_iNumX + 1);
			m_vecIdx.push_back(iAddr + m_iNumX);

			vEdge1 = m_vecVtx[iAddr + m_iNumX + 1].vPos - m_vecVtx[iAddr].vPos;
			vEdge2 = m_vecVtx[iAddr + m_iNumX].vPos - m_vecVtx[iAddr].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			m_vecFaceNormal[iTriIdx] = vEdge1.Cross(vEdge2);
			m_vecFaceNormal[iTriIdx].Normalize();
			++iTriIdx;
		}
	}

	ComputeNormal();
	ComputeTangent();

	SetTag(strName);

	GET_SINGLE(CResourcesManager)->CreateMesh(strName, LANDSCAPE_COLOR_SHADER, VERTEX3D_LAYOUT_COLOR,
		&m_vecVtx[0], (int)m_vecVtx.size(), sizeof(Vertex3DColor), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecIdx[0], (int)m_vecIdx.size(),
		4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	m_Mesh = CResourcesManager::GetInst()->FindMeshNonCount(strName);

	CRenderer*	pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		pRenderer = m_pObject->AddComponent<CRenderer>("LandScapeRenderer");

	pRenderer->SetMesh(strName);
	//pRenderer->SetRenderState(WIRE_FRAME);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	if (pTexName)
	{
		pMaterial->SetDiffuseTex(0, strTexKey, pTexName, strPathKey);
		pMaterial->SetSampler(0, SAMPLER_LINEAR);
	}

	if (pTexNormal)
	{
		string	strKey = strTexKey;
		strKey += "_N";
		pMaterial->SetNormalTex(1, strKey, pTexNormal, strPathKey);
		pMaterial->SetNormalSampler(0, SAMPLER_LINEAR);
	}

	if (pTexSpecular)
	{
		string	strKey = strTexKey;
		strKey += "_S";
		pMaterial->SetSpecularTex(2, strKey, pTexSpecular, strPathKey);
		pMaterial->SetSpecularSampler(0, SAMPLER_LINEAR);
	}

	SAFE_RELEASE(pMaterial);

	m_pNavMesh = GET_SINGLE(CNavigationManager3D)->CreateNavMesh(m_pScene);

	for (int i = 0; i < (int)m_iNumZ - 1; ++i)
	{
		for (int j = 0; j < (int)m_iNumX - 1; ++j)
		{
			int	iAddr = i * m_iNumX + j;

			m_pNavMesh->AddCell(&m_vecVtx, iAddr, iAddr + 1, iAddr + m_iNumX + 1);
			m_pNavMesh->AddCell(&m_vecVtx, iAddr, iAddr + m_iNumX + 1, iAddr + m_iNumX);
		}
	}
	m_pNavMesh->CreateGridMapAdj(m_iNumX - 1);
	SAFE_DELETE_ARRAY(pHeight);

	return true;
}

bool CLandScape::AddSplatDifTexture(const string & strName,
	const vector<const TCHAR*>& pFileName, const string & strPathName)
{
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	pMaterial->AddMultiTex(11, SAMPLER_LINEAR, 11, strName,
		pFileName, strPathName);

	SAFE_RELEASE(pMaterial);

	m_tCBuffer.iSplatCount = (int)pFileName.size();

	return true;
}

bool CLandScape::AddSplatNrmTexture(const string & strName,
	const vector<const TCHAR*>& pFileName, const string & strPathName)
{
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	pMaterial->AddMultiTex(11, SAMPLER_LINEAR, 12, strName,
		pFileName, strPathName);

	SAFE_RELEASE(pMaterial);

	return true;
}

bool CLandScape::AddSplatSpcTexture(const string & strName,
	const vector<const TCHAR*>& pFileName, const string & strPathName)
{
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	pMaterial->AddMultiTex(11, SAMPLER_LINEAR, 13, strName,
		pFileName, strPathName);

	SAFE_RELEASE(pMaterial);

	return true;
}

bool CLandScape::AddSplatAlphaTexture(const string & strName,
	const vector<const TCHAR*>& pFileName, const string & strPathName)
{
	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	pMaterial->AddMultiTex(11, SAMPLER_LINEAR, 14, strName,
		pFileName, strPathName);

	SAFE_RELEASE(pMaterial);

	return true;
}
void CLandScape::Start()
{
}

void CLandScape::AfterClone()
{
}

bool CLandScape::Init()
{
	m_tCBuffer.fDetailLevel = 20.f;
	m_tCBuffer.iSplatCount = 0;
	m_pObject->SetRenderGroup(RG_LANDSCAPE);

	CInput::GetInst()->AddKey("LandOnOff", VK_F4);
	CInput::GetInst()->AddKey("LandUp", VK_UP);
	CInput::GetInst()->AddKey("LandDown", VK_DOWN);

	return true;
}

int CLandScape::Input(float fTime)
{
	OnOff();



	if (CCore::GetInst()->m_bEditorMode == true)
	{
		if (CInput::GetInst()->KeyPress("TabButton") == true)
			ChangeFlag(fTime);
	}

	return 0;
}

int CLandScape::Update(float fTime)
{
	m_tNaviCBuffer.iBrushXRange = (int)CInput::GetInst()->GetXBrushSize();
	m_tNaviCBuffer.iBrushZRange = (int)CInput::GetInst()->GetZBrushSize();

	if (CCore::GetInst()->m_bEditorMode == true)
	{
		if (CEditManager::GetInst()->IsNaviEditorMode() == true)
		{
			CheckSelectIndex(fTime);
			if (CInput::GetInst()->KeyPush("LButton") == true && CInput::GetInst()->KeyPush("NaviDragMode") == false)
			{
				if (m_isMove)
					m_pNavMesh->Click(m_isMove, Vector4(0.0f, 1.0f, 0.5f, 1.0f));
				else
					m_pNavMesh->Click(m_isMove);
			}
			else if (CInput::GetInst()->KeyPress("LButton") == true && CInput::GetInst()->KeyPush("NaviDragMode") == true)
			{
				m_pNavMesh->DragStart();
			}
			else if (CInput::GetInst()->KeyRelease("LButton") == true && CInput::GetInst()->KeyPush("NaviDragMode") == true)
			{
				if (m_isMove)
					m_pNavMesh->DragEnd(m_isMove, Vector4(0.0f, 1.0f, 0.5f, 1.0f));
				else
					m_pNavMesh->DragEnd(m_isMove);
			}

			if (CInput::GetInst()->KeyPush("LandUp") == true)
			{
				m_pNavMesh->LandUp(fTime);
				m_Mesh->UpdateVertexBuffer(&m_vecVtx[0]);
			}
			else if (CInput::GetInst()->KeyPush("LandDown") == true)
			{
				m_pNavMesh->LandDown(fTime);
				m_Mesh->UpdateVertexBuffer(&m_vecVtx[0]);
			}
		}
	}

	return 0;
}

int CLandScape::LateUpdate(float fTime)
{
	// 내비메쉬에 이 지형의 트랜스폼에서 위치를 얻어와 지정해준다.
	//m_pNavMesh->SetOffset(m_pTransform->GetWorldPos());
	//m_pNavMesh->SetOffsetScale(m_pTransform->GetWorldScale());

	return 0;
}

void CLandScape::Collision(float fTime)
{
}

void CLandScape::Render(float fTime)
{
	CShaderManager::GetInst()->UpdateCBuffer("NaviLandCBuffer", &m_tNaviCBuffer);
	CShaderManager::GetInst()->UpdateCBuffer("GridCBuffer", &m_tGridCBuffer);

}

CLandScape * CLandScape::Clone()
{
	return new CLandScape(*this);
}

void CLandScape::ComputeNormal()
{
	//for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	//{
	//	int	idx0 = m_vecIdx[i * 3];
	//	int	idx1 = m_vecIdx[i * 3 + 1];
	//	int	idx2 = m_vecIdx[i * 3 + 2];

	//	m_vecVtx[idx0].vNormal += m_vecFaceNormal[i];
	//	m_vecVtx[idx1].vNormal += m_vecFaceNormal[i];
	//	m_vecVtx[idx2].vNormal += m_vecFaceNormal[i];
	//}

	//for (size_t i = 0; i < m_vecVtx.size(); ++i)
	//{
	//	m_vecVtx[i].vNormal.Normalize();
	//}
}

void CLandScape::ComputeTangent()
{
	// 탄젠트 벡터 구함.
	//for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	//{
	//	int	idx0 = m_vecIdx[i * 3];
	//	int	idx1 = m_vecIdx[i * 3 + 1];
	//	int	idx2 = m_vecIdx[i * 3 + 2];

	//	float	fVtx1[3], fVtx2[3];
	//	fVtx1[0] = m_vecVtx[idx1].vPos.x - m_vecVtx[idx0].vPos.x;
	//	fVtx1[1] = m_vecVtx[idx1].vPos.y - m_vecVtx[idx0].vPos.y;
	//	fVtx1[2] = m_vecVtx[idx1].vPos.z - m_vecVtx[idx0].vPos.z;

	//	fVtx2[0] = m_vecVtx[idx2].vPos.x - m_vecVtx[idx0].vPos.x;
	//	fVtx2[1] = m_vecVtx[idx2].vPos.y - m_vecVtx[idx0].vPos.y;
	//	fVtx2[2] = m_vecVtx[idx2].vPos.z - m_vecVtx[idx0].vPos.z;

	//	float	ftu[2], ftv[2];
	//	ftu[0] = m_vecVtx[idx1].vUV.x - m_vecVtx[idx0].vUV.x;
	//	ftv[0] = m_vecVtx[idx1].vUV.y - m_vecVtx[idx0].vUV.y;

	//	ftu[1] = m_vecVtx[idx2].vUV.x - m_vecVtx[idx0].vUV.x;
	//	ftv[1] = m_vecVtx[idx2].vUV.y - m_vecVtx[idx0].vUV.y;

	//	float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

	//	Vector3	vTangent;
	//	vTangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
	//	vTangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
	//	vTangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

	//	vTangent.Normalize();

	//	m_vecVtx[idx0].vTangent = vTangent;
	//	m_vecVtx[idx1].vTangent = vTangent;
	//	m_vecVtx[idx2].vTangent = vTangent;

	//	m_vecVtx[idx0].vBinormal = m_vecVtx[idx0].vNormal.Cross(vTangent);
	//	m_vecVtx[idx1].vBinormal = m_vecVtx[idx1].vNormal.Cross(vTangent);
	//	m_vecVtx[idx2].vBinormal = m_vecVtx[idx2].vNormal.Cross(vTangent);

	//	m_vecVtx[idx0].vBinormal.Normalize();
	//	m_vecVtx[idx1].vBinormal.Normalize();
	//	m_vecVtx[idx2].vBinormal.Normalize();
	//}
}

void CLandScape::ChangeFlag(float DeltaTime)
{
	m_isMove ^= true;

	CEditManager::GetInst()->SetNaviMove(m_isMove);
}

void CLandScape::CheckSelectIndex(float DeltaTime)
{
	Vector3 vRayInterSect;
	RayInfo vRayInfo;
	Vector3 vRayEnd;

	int iMinSection = 10000000;
	int iMaxSection = -10000000;
	vRayInfo = CInput::GetInst()->MouseRayInfo();

	vRayEnd.x = vRayInfo.vPos.x + (vRayInfo.vDir.x * 1000.0f);
	vRayEnd.z = vRayInfo.vPos.z + (vRayInfo.vDir.z * 1000.0f);

	if (vRayInfo.vPos.x < vRayEnd.x)
	{
		if (vRayInfo.vPos.z < vRayEnd.z)
		{
			for (float z = vRayInfo.vPos.z; z < vRayEnd.z; z += 10.0f)
			{
				for (float x = vRayInfo.vPos.x; x < vRayEnd.x; ++x)
				{
					int iSection = m_pNavMesh->GetSectionIndex(Vector3(x, 0.f, z));

					if (iSection < iMinSection)
					{
						iMinSection = iSection;
						break;
					}

					if (iSection > iMaxSection)
					{
						iMaxSection = iSection;
						break;
					}
				}
			}
		}
		else
		{
			for (float z = vRayInfo.vPos.z; z > vRayEnd.z; z -= 10.0f)
			{
				for (float x = vRayInfo.vPos.x; x < vRayEnd.x; ++x)
				{
					int iSection = m_pNavMesh->GetSectionIndex(Vector3(x, 0.f, z));

					if (iSection < iMinSection)
						iMinSection = iSection;

					if (iSection > iMaxSection)
						iMaxSection = iSection;
				}
			}
		}
	}
	else if (vRayInfo.vPos.x > vRayEnd.x)
	{
		if (vRayInfo.vPos.z < vRayEnd.z)
		{
			for (float z = vRayInfo.vPos.z; z < vRayEnd.z; z += 10.0f)
			{
				for (float x = vRayInfo.vPos.x; x > vRayEnd.x; --x)
				{
					int iSection = m_pNavMesh->GetSectionIndex(Vector3(x, 0.f, z));

					if (iSection < iMinSection)
						iMinSection = iSection;

					if (iSection > iMaxSection)
						iMaxSection = iSection;
				}
			}
		}
		else
		{
			for (float z = vRayInfo.vPos.z; z > vRayEnd.z; z -= 10.0f)
			{
				for (float x = vRayInfo.vPos.x; x > vRayEnd.x; --x)
				{
					int iSection = m_pNavMesh->GetSectionIndex(Vector3(x, 0.f, z));

					if (iSection < iMinSection)
						iMinSection = iSection;

					if (iSection > iMaxSection)
						iMaxSection = iSection;
				}
			}
		}
	}

	CInput::GetInst()->SetSelectNavIndex(m_pNavMesh->MousePickGetCellIndex(iMinSection, iMaxSection, vRayInfo.vPos, vRayInfo.vDir, m_tNaviCBuffer.BrushPos));
	m_Mesh->UpdateVertexBuffer(&m_vecVtx[0]);
}

void CLandScape::OnOff()
{
	if (CInput::GetInst()->KeyPress("LandOnOff"))
	{
		m_isShow ^= true;

		if (m_isShow == true)
		{
			CRenderer* getRenderer = m_pObject->FindComponentFromTypeNonCount<CRenderer>(CT_RENDERER);
			getRenderer->SetEnable(m_isShow);
		}
		else
		{
			CRenderer* getRenderer = m_pObject->FindComponentFromTypeNonCount<CRenderer>(CT_RENDERER);
			getRenderer->SetEnable(m_isShow);
		}
	}
}

void CLandScape::SaveLandScape(const string& FullPath)
{
	string Temp = FullPath;
	for (int i = 0; i < 3; ++i)
		Temp.pop_back();
	Temp += "Land";

	FILE* pFile = NULL;

	fopen_s(&pFile, Temp.c_str(), "wb");

	if (!pFile)
		return;

	fwrite(&m_iNumX, sizeof(int), 1, pFile);
	fwrite(&m_iNumZ, sizeof(int), 1, pFile);
	int iVecVtxSize = (int)m_vecVtx.size();
	fwrite(&iVecVtxSize, sizeof(int), 1, pFile);
	for (size_t i = 0; i < m_vecVtx.size(); ++i)
	{
		fwrite(&m_vecVtx[i], sizeof(Vertex3DColor), 1, pFile);
	}

	int iVecIndexSize = (int)m_vecIdx.size();
	fwrite(&iVecIndexSize, sizeof(int), 1, pFile);
	for (size_t i = 0; i < m_vecIdx.size(); ++i)
	{
		fwrite(&m_vecIdx[i], sizeof(int), 1, pFile);
	}

	int iVecFaceNormalSize = (int)m_vecFaceNormal.size();
	fwrite(&iVecFaceNormalSize, sizeof(int), 1, pFile);
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		fwrite(&m_vecFaceNormal[i], sizeof(Vector3), 1, pFile);
	}

	fclose(pFile);
}

void CLandScape::LoadLandScape(const string& FullPath)
{
	string Temp = FullPath;
	for (int i = 0; i < 3; ++i)
		Temp.pop_back();
	Temp += "Land";

	FILE* pFile = NULL;

	fopen_s(&pFile, Temp.c_str(), "rb");

	if (!pFile)
		return;

	fread(&m_iNumX, sizeof(int), 1, pFile);
	fread(&m_iNumZ, sizeof(int), 1, pFile);
	int iVecVtxSize = 0;
	fread(&iVecVtxSize, sizeof(int), 1, pFile);

	for (size_t i = 0; i < iVecVtxSize; ++i)
	{
		Vertex3DColor tColor = {};
		fread(&tColor, sizeof(Vertex3DColor), 1, pFile);
		m_vecVtx.push_back(tColor);
	}

	int iVecIndexSize = 0;
	fread(&iVecIndexSize, sizeof(int), 1, pFile);

	for (size_t i = 0; i < iVecIndexSize; ++i)
	{
		int iIdx = {};
		fread(&iIdx, sizeof(int), 1, pFile);
		m_vecIdx.push_back(iIdx);
	}

	int iVecFaceNormalSize = 0;
	fread(&iVecFaceNormalSize, sizeof(int), 1, pFile);

	for (size_t i = 0; i < iVecFaceNormalSize; ++i)
	{
		Vector3 vFaceNormal = {};
		fread(&vFaceNormal, sizeof(Vector3), 1, pFile);
		m_vecFaceNormal.push_back(vFaceNormal);
	}
	fclose(pFile);

	//CreateLandScape("TestLandScape", m_iNumX, m_iNumZ, "LandScapeDif", NULLPTR, NULLPTR, NULLPTR, NULLPTR);

	//m_pNavMesh = CNavigationManager3D::GetInst()->CreateNavMesh(m_pScene, FullPath);
	//for (int i = 0; i < m_vecVtx.size(); ++i)
	//{
	//	m_vecVtx[i].vColor = tempvec[i].vColor;
	//}
	//for (int i = 0; i < (int)m_iNumZ - 1; ++i)
	//{
	//	for (int j = 0; j < (int)m_iNumX - 1; ++j)
	//	{
	//		int	iAddr = i * m_iNumX + j;

	//		m_pNavMesh->SetCellCopyVertex(m_vecVtx[iAddr], m_vecVtx[iAddr + 1], m_vecVtx[iAddr + m_iNumX + 1] , iAddr * 2);
	//		m_pNavMesh->SetCellCopyVertex(m_vecVtx[iAddr], m_vecVtx[iAddr + m_iNumX + 1], m_vecVtx[iAddr + m_iNumX] , (iAddr * 2) + 1);
	//	}
	//}

	GET_SINGLE(CResourcesManager)->CreateMesh("TestLandScape", LANDSCAPE_COLOR_SHADER, VERTEX3D_LAYOUT_COLOR,
		&m_vecVtx[0], (int)m_vecVtx.size(), sizeof(Vertex3DColor), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecIdx[0], (int)m_vecIdx.size(),
		4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	m_Mesh = CResourcesManager::GetInst()->FindMeshNonCount("TestLandScape");

	CRenderer*	pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		pRenderer = m_pObject->AddComponent<CRenderer>("LandScapeRenderer");

	pRenderer->SetMesh("TestLandScape");
	//pRenderer->SetRenderState(WIRE_FRAME);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	if (!pMaterial)
		pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

	SAFE_RELEASE(pMaterial);

	m_pNavMesh = GET_SINGLE(CNavigationManager3D)->CreateNavMesh(m_pScene);

	m_pNavMesh->LoadFromFullPath(FullPath.c_str());
	m_pNavMesh->LoadCell(&m_vecVtx);
}