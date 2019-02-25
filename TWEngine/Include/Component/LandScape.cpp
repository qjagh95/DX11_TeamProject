#include "../EngineHeader.h"
#include "LandScape.h"

PUN_USING

CLandScape::CLandScape()
{
}

CLandScape::CLandScape(const CLandScape & landscape) :
	CComponent(landscape)
{
	*this = landscape;
	m_iRefCount = 1;
}

CLandScape::~CLandScape()
{
}

bool CLandScape::CreateLandScape(const string& strName,
	unsigned int iNumX, unsigned int iNumZ, const string& strTexKey,
	const TCHAR* pTexName,
	const char * pFileName, const string & strPathKey)
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

			fread(pHeight, sizeof(unsigned char), m_iNumX * m_iNumZ * iPixelSize,
				pFile);

			unsigned char*	pLine = new unsigned char[m_iNumX * iPixelSize];

			for (unsigned int i = 0; i < m_iNumZ / 2; ++i)
			{
				memcpy(pLine, &pHeight[i * m_iNumX * iPixelSize],
					sizeof(unsigned char) * (m_iNumX * iPixelSize));
				memcpy(&pHeight[i * m_iNumX * iPixelSize],
					&pHeight[(m_iNumZ - i - 1) * m_iNumX * iPixelSize],
					sizeof(unsigned char) * (m_iNumX * iPixelSize));
				memcpy(&pHeight[(m_iNumZ - i - 1) * m_iNumX * iPixelSize], pLine,
					sizeof(unsigned char) * (m_iNumX * iPixelSize));
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
			Vertex3D	tVtx = {};

			float	y = 0.f;

			if (pHeight)
			{
				y = pHeight[i * m_iNumX * iPixelSize + j * iPixelSize] * 0.5f;
			}

			tVtx.vPos = Vector3((float)j, (float)y, (float)(m_iNumZ - i - 1));
			tVtx.vNormal = Vector3(0.f, -1.f, 0.f);
			tVtx.vUV = Vector2((float)j, (float)i);

			tVtx.vTangent = Vector3(1.f, 0.f, 0.f);
			tVtx.vBinormal = Vector3(0.f, 0.f, -1.f);

			m_vecVtx.push_back(tVtx);
		}
	}

	// 인덱스 정보를 만든다.
	for (unsigned int i = 0; i < m_iNumZ - 1; ++i)
	{
		for (unsigned int j = 0; j < m_iNumX - 1; ++j)
		{
			int	iAddr = i * m_iNumX + j;

			// 우상단 삼각형
			m_vecIdx.push_back(iAddr);
			m_vecIdx.push_back(iAddr + 1);
			m_vecIdx.push_back(iAddr + m_iNumX + 1);

			// 좌하단 삼각형
			m_vecIdx.push_back(iAddr);
			m_vecIdx.push_back(iAddr + m_iNumX + 1);
			m_vecIdx.push_back(iAddr + m_iNumX);
		}
	}

	SetTag(strName);

	GET_SINGLE(CResourcesManager)->CreateMesh(strName, LANDSCAPE_SHADER, VERTEX3D_LAYOUT,
		&m_vecVtx[0], (int)m_vecVtx.size(), sizeof(Vertex3D), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecIdx[0], (int)m_vecIdx.size(),
		4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	CRenderer*	pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		pRenderer = m_pObject->AddComponent<CRenderer>("LandScapeRenderer");

	pRenderer->SetMesh(strName);

	SAFE_RELEASE(pRenderer);

	if (pTexName)
	{
		CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

		if (!pMaterial)
			pMaterial = m_pObject->AddComponent<CMaterial>("LandScapeMaterial");

		pMaterial->SetDiffuseTex(0, strTexKey, pTexName, strPathKey);
		pMaterial->SetSampler(0, SAMPLER_LINEAR);

		SAFE_RELEASE(pMaterial);
	}

	SAFE_DELETE_ARRAY(pHeight);

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
	return true;
}

int CLandScape::Input(float fTime)
{
	return 0;
}

int CLandScape::Update(float fTime)
{
	return 0;
}

int CLandScape::LateUpdate(float fTime)
{
	return 0;
}

void CLandScape::Collision(float fTime)
{
}

void CLandScape::Render(float fTime)
{
}

CLandScape * CLandScape::Clone()
{
	return new CLandScape(*this);
}
