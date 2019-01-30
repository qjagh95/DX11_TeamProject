
#include "EngineHeader.h"
#include "ResourcesManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Sampler.h"

PUN_USING

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map(m_mapSampler);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapMesh);
}

bool CResourcesManager::Init()
{
	VertexColor	tColorTri[3] =
	{
		VertexColor(Vector3(0.f, 0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4::Green),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::Blue)
	};

	unsigned short	sIdx[3] = { 0, 1, 2 };

	CreateMesh("ColorTri", STANDARD_COLOR_SHADER, POS_COLOR_LAYOUT,
		tColorTri, 3, sizeof(VertexColor), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sIdx, 3, 2);

	VertexColor	tColorRect[4] =
	{
		VertexColor(Vector3(0.f, 1.f, 0.f), Vector4::Red),
		VertexColor(Vector3(1.f, 1.f, 0.f), Vector4::Green),
		VertexColor(Vector3(0.f, 0.f, 0.f), Vector4::Blue),
		VertexColor(Vector3(1.f, 0.f, 0.f), Vector4::Yellow)
	};

	unsigned short	sIdxRC[6] = { 0, 1, 3, 0, 3, 2 };

	CreateMesh("ColorRect", STANDARD_COLOR_SHADER, POS_COLOR_LAYOUT,
		tColorRect, 4, sizeof(VertexColor), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sIdxRC, 6, 2);

	Vector3	vPos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	CreateMesh("ColliderRect", COLLIDER_SHADER, POS_LAYOUT,
		vPos, 5, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	vPos[0] = Vector3(0.5f, 1.f, 0.f);
	vPos[1] = Vector3(1.f, 0.5f, 0.f);
	vPos[2] = Vector3(0.5f, 0.f, 0.f);
	vPos[3] = Vector3(0.f, 0.5f, 0.f);
	vPos[4] = Vector3(0.5f, 1.f, 0.f);

	CreateMesh("TileIso", COLLIDER_SHADER, POS_LAYOUT,
		vPos, 5, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	VertexTex	tTexRect[4] =
	{
		VertexTex(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexTex(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexTex(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexTex(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	CreateMesh("TexRect", STANDARD_TEX_SHADER, POS_UV_LAYOUT,
		tTexRect, 4, sizeof(VertexTex), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sIdxRC, 6, 2);

	//CreateTexture("yso", TEXT("yso.jpg"));

	Vector3	PyramidPos[5] =
	{
		Vector3(0.0f, 0.5f, 0.0f),  //0
		Vector3(-0.5f, -0.5f, 0.5f), //1
		Vector3(0.5f, -0.5f, 0.5f), //2
		Vector3(0.5f, -0.5f, -0.5f), //3
		Vector3(-0.5f, -0.5f, -0.5f) //4
	};
	// �Ƕ�̵� �ε���
	//            
	//     1(5)       2(6)

	//            0

	//     4(7)       3(8) 

	// ������ Vertex �������� ����Ǿ��ִ� ���� �ٶ󺸴� ������ ���Ѵ�.

	Vector3	PlaneNormal[4]; // �ٶ󺸴� ���� Vector�� ���� PlaneNormal
	Vector3	Edge1, Edge2; // ������ �������� Vector�� ���� Edge ����

	// pyramid�� ���ʸ��� ���⺤�͸� ���Ѵ�.
	Edge1 = PyramidPos[4] - PyramidPos[0]; // 4�� Vector�� 0�� Vector�� ����.
	Edge2 = PyramidPos[1] - PyramidPos[0]; // 1�� Vector�� 0�� Vector�� ����.
	Edge1.Normalize(); //���� ������ ���� Vector�� ����ȭ �Ѵ�.
	Edge2.Normalize(); //���� ������ ���� Vector�� ����ȭ �Ѵ�.
	PlaneNormal[0] = Edge1.Cross(Edge2); // ���� ���� Vector�� �����ϰ�
	PlaneNormal[0].Normalize(); // ����ȭ�ؼ� ���� ������ ���Ѵ�.

	// pyramid�� �ڸ��� ���⺤�͸� ���Ѵ�.
	Edge1 = PyramidPos[1] - PyramidPos[0];
	Edge2 = PyramidPos[2] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[1] = Edge1.Cross(Edge2);
	PlaneNormal[1].Normalize();

	// pyramid�� �����ʸ��� ���⺤�͸� ���Ѵ�.
	Edge1 = PyramidPos[2] - PyramidPos[0];
	Edge2 = PyramidPos[3] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[2] = Edge1.Cross(Edge2);
	PlaneNormal[2].Normalize();

	// pyramid�� ���ʸ��� ���⺤�͸� ���Ѵ�.
	Edge1 = PyramidPos[3] - PyramidPos[0];
	Edge2 = PyramidPos[4] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[3] = Edge1.Cross(Edge2);
	PlaneNormal[3].Normalize();

	// ���� ��� Vector�� ���Ѵ�(�� 4���� Edge Line)
	Vector3	Normal[4];
	// ���ʸ�� �ڸ�
	Normal[0] = (PlaneNormal[0] + PlaneNormal[1]);
	Normal[0].Normalize();
	// �ڸ�� ������
	Normal[1] = (PlaneNormal[1] + PlaneNormal[2]);
	Normal[1].Normalize();
	// �����ʸ�� �ո�
	Normal[2] = (PlaneNormal[2] + PlaneNormal[3]);
	Normal[2].Normalize();
	// �ո�� ���ʸ�
	Normal[3] = (PlaneNormal[3] + PlaneNormal[0]);
	Normal[3].Normalize();

	VertexNormalColor Pyramid[9] =
	{
		VertexNormalColor(PyramidPos[0], Vector3(0.0f, 1.0f, 0.0f),  Vector4::Red),
		VertexNormalColor(PyramidPos[1], Normal[0], Vector4::Green),
		VertexNormalColor(PyramidPos[2], Normal[1], Vector4::Blue),
		VertexNormalColor(PyramidPos[3], Normal[2], Vector4::Yellow),
		VertexNormalColor(PyramidPos[4], Normal[3], Vector4::Magenta),
		VertexNormalColor(PyramidPos[1], Vector3(0.0f, -1.0f, 0.0f), Vector4::Green),
		VertexNormalColor(PyramidPos[2], Vector3(0.0f, -1.0f, 0.0f), Vector4::Blue),
		VertexNormalColor(PyramidPos[3], Vector3(0.0f, -1.0f, 0.0f), Vector4::Yellow),
		VertexNormalColor(PyramidPos[4], Vector3(0.0f, -1.0f, 0.0f), Vector4::Magenta)
	};

	int	PyramidIdx[18] = { 0, 4, 1, 2, 0, 1, 3, 0, 2, 4, 0, 3, 8, 6, 5, 7, 6, 8 };
	//{0, 4, 1} ���ʻﰢ��
	//{2, 1, 0} ���� �ﰢ��(�Ⱥ��̴ºκ��� �ݽð����)
	//{3, 0, 2} �����ʸ� �ﰢ��
	//{4, 0, 3} �ո� �ﰢ��
	//{8, 6, 5} �Ʒ��ʸ� �簢���� 1�� �ﰢ��
	//{7, 6, 8} �Ʒ��ʸ� �簢���� 2�� �ﰢ��
	CreateMesh("Pyramid", STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, Pyramid, 9, sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, PyramidIdx, 18, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	// �������͸� �ϳ� �����Ѵ�.
	CreateSampler(SAMPLER_LINEAR);

	return true;
}

bool CResourcesManager::CreateMesh(const string & strName, const string & strShaderKey,
	const string & strLayoutKey, void * pVtx, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pIdx, int iIdxCount, int iIdxSize,
	D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	CMesh*	pMesh = FindMesh(strName);

	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	pMesh = new CMesh;

	if (!pMesh->CreateMesh(strName, strShaderKey, strLayoutKey, pVtx, iVtxCount, iVtxSize,
		eVtxUsage, ePrimitive, pIdx, iIdxCount, iIdxSize, eIdxUsage, eFmt))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	m_mapMesh.insert(make_pair(strName, pMesh));

	return true;
}

bool CResourcesManager::CreateTexture(const string & strName,
	const TCHAR * pFileName, const string & strPathKey)
{
	CTexture*	pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strName, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourcesManager::CreateTextureFromFullPath(const string & strName,
	const TCHAR * pFullPath)
{
	CTexture*	pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFromFullPath(strName, pFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourcesManager::CreateTexture(const string & strName,
	const vector<const TCHAR*>& vecFileName, const string & strPathKey)
{
	CTexture*	pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strName, vecFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourcesManager::CreateTextureFromFullPath(const string & strName,
	const vector<const TCHAR*>& vecFullPath)
{
	CTexture*	pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFromFullPath(strName, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourcesManager::CreateSampler(const string & strName,
	D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU,
	D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW)
{
	CSampler*	pSampler = FindSampler(strName);

	if (pSampler)
	{
		SAFE_RELEASE(pSampler);
		return false;
	}

	pSampler = new CSampler;

	if (!pSampler->CreateSampler(strName, eFilter, eU, eV, eW))
	{
		SAFE_RELEASE(pSampler);
		return false;
	}

	m_mapSampler.insert(make_pair(strName, pSampler));

	return true;
}

CMesh * CResourcesManager::FindMesh(const string & strName)
{
	unordered_map<string, class CMesh*>::iterator	iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CTexture * CResourcesManager::FindTexture(const string & strName)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CSampler * CResourcesManager::FindSampler(const string & strName)
{
	unordered_map<string, CSampler*>::iterator	iter = m_mapSampler.find(strName);

	if (iter == m_mapSampler.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}
