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
	// 피라미드 인덱스
	//            
	//     1(5)       2(6)

	//            0

	//     4(8)       3(7) 

	// 각각의 Vertex 정점간의 연결되어있는 선이 바라보는 방향을 구한다.

	Vector3	PlaneNormal[4]; // 바라보는 방향 Vector를 구할 PlaneNormal
	Vector3	Edge1, Edge2; // 각각의 정점선의 Vector를 구할 Edge 변수

	// pyramid의 왼쪽면의 방향벡터를 구한다.
	Edge1 = PyramidPos[4] - PyramidPos[0]; // 4번 Vector와 0번 Vector를 뺀다.
	Edge2 = PyramidPos[1] - PyramidPos[0]; // 1번 Vector와 0번 Vector를 뺀다.
	Edge1.Normalize(); //빼서 방향을 구한 Vector를 정규화 한다.
	Edge2.Normalize(); //빼서 방향을 구한 Vector를 정규화 한다.
	PlaneNormal[0] = Edge1.Cross(Edge2); // 양쪽 선의 Vector를 외적하고
	PlaneNormal[0].Normalize(); // 정규화해서 면의 방향을 구한다.

	// pyramid의 뒤면의 방향벡터를 구한다.
	Edge1 = PyramidPos[1] - PyramidPos[0];
	Edge2 = PyramidPos[2] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[1] = Edge1.Cross(Edge2);
	PlaneNormal[1].Normalize();

	// pyramid의 오른쪽면의 방향벡터를 구한다.
	Edge1 = PyramidPos[2] - PyramidPos[0];
	Edge2 = PyramidPos[3] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[2] = Edge1.Cross(Edge2);
	PlaneNormal[2].Normalize();

	// pyramid의 앞쪽면의 방향벡터를 구한다.
	Edge1 = PyramidPos[3] - PyramidPos[0];
	Edge2 = PyramidPos[4] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[3] = Edge1.Cross(Edge2);
	PlaneNormal[3].Normalize();

	// 법선 평균 Vector를 구한다(각 4개의 Edge Line)
	Vector3	Normal[4];
	// 왼쪽면과 뒤면
	Normal[0] = (PlaneNormal[0] + PlaneNormal[1]);
	Normal[0].Normalize();
	// 뒤면과 오른쪽
	Normal[1] = (PlaneNormal[1] + PlaneNormal[2]);
	Normal[1].Normalize();
	// 오른쪽면과 앞면
	Normal[2] = (PlaneNormal[2] + PlaneNormal[3]);
	Normal[2].Normalize();
	// 앞면과 왼쪽면
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
	//{0, 4, 1} 왼쪽삼각형
	//{2, 1, 0} 윗면 삼각형(안보이는부분은 반시계방향)
	//{3, 0, 2} 오른쪽면 삼각형
	//{4, 0, 3} 앞면 삼각형
	//{8, 6, 5} 아래쪽면 사각형의 1번 삼각형
	//{7, 6, 8} 아래쪽면 사각형의 2번 삼각형
	CreateMesh("Pyramid", STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, Pyramid, 9, sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, PyramidIdx, 18, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
	// 선형필터링 하나 생성한다.
	CreateSampler(SAMPLER_LINEAR);
	CreateSampler(SAMPLER_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT);

	CreateSphereVolum(SPHERE_VOLUME, 0.5f, 64, 128);
	CreateSphereVolum("Sky", 1.0f, 32, 64, "Sky", POS_LAYOUT);


	CreateCylinderVolum(CYLINDER_VOLUME, 0.5f, 3, 32);
	CreateCylinderVolumColor("XGizmoCylinder", Vector4(Vector4::Red.x, Vector4::Red.y, Vector4::Red.z, 2.f), 0.5f, 3, 32);
	CreateCylinderVolumColor("YGizmoCylinder", Vector4(Vector4::Green.x, Vector4::Green.y, Vector4::Green.z, 2.f), 0.5f, 3, 32);
	CreateCylinderVolumColor("ZGizmoCylinder", Vector4(Vector4::Blue.x, Vector4::Blue.y, Vector4::Blue.z, 2.f), 0.5f, 3, 32);
	CreateCapsulVolum(CAPSUL_VOLUME, 0.5f, 3, 64, 128);
	CreateCornVolum(CORN_VOLUME, 0.5f, 0.5f, 64, 128);

	Vector3	vBox[8] =
	{
		Vector3(-1.f, 1.f, -1.f),
		Vector3(1.f, 1.f, -1.f),
		Vector3(-1.f, -1.f, -1.f),
		Vector3(1.f, -1.f, -1.f),
		Vector3(-1.f, 1.f, 1.f),
		Vector3(1.f, 1.f, 1.f),
		Vector3(-1.f, -1.f, 1.f),
		Vector3(1.f, -1.f, 1.f)
	};

	int	iBoxIdx[24] = { 0, 1, 0, 2, 1, 3, 2, 3, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 7, 7, 6, 6, 4 };

	CreateMesh("ColliderBox", COLLIDER_SHADER, POS_LAYOUT,
		vBox, 8, sizeof(Vector3),
		D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST, iBoxIdx,
		24, 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT);

	Vector3	vDecalBox[8] =
	{
		Vector3(-0.5f, 0.5f, -0.5f),
		Vector3(0.5f, 0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(-0.5f, 0.5f, 0.5f),
		Vector3(0.5f, 0.5f, 0.5f),
		Vector3(-0.5f, -0.5f, 0.5f),
		Vector3(0.5f, -0.5f, 0.5f)
	};

	int	iSSDBoxIdx[36] =
	{
		0, 1, 3, 0, 3, 2, 1, 5, 7, 1, 7, 3, 4, 0, 2, 4, 2, 6, 5, 4, 6, 5, 6, 7,
		2, 3, 7, 2, 7, 6, 4, 5, 1, 4, 1, 0
	};

	CreateMesh("VolumeBox", DECAL_SHADER, POS_LAYOUT,
		vDecalBox, 8, sizeof(Vector3),
		D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, iSSDBoxIdx,
		36, 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT);

	Vector3	vParticle;
	CreateMesh("Particle", PARTICLE_SHADER, POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	CreateTexture("RandomNormal", TEXT("RandomNormal.jpg"));

	return true;
}

bool CResourcesManager::SamplerInit()
{
	CreateSampler(SAMPLER_LINEAR);
	CreateSampler(SAMPLER_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT);

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

bool CResourcesManager::LoadMesh(const string & strName, const TCHAR * pFileName, const string & strPathKey)
{
	CMesh*	pMesh = FindMesh(strName);

	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	pMesh = new CMesh;

	if (!pMesh->LoadMesh(strName, pFileName, strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	m_mapMesh.insert(make_pair(strName, pMesh));

	return true;
}

bool CResourcesManager::LoadMeshFromFullPath(const string & strName, const TCHAR * pFullPath)
{
	CMesh*	pMesh = FindMesh(strName);
	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	pMesh = new CMesh;
	if (!pMesh->LoadMeshFromFullPath(strName, pFullPath))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}
	m_mapMesh.insert(make_pair(strName, pMesh));

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

vector<string>* CResourcesManager::GetMeshNameList()
{
	unordered_map<string, CMesh*>::iterator iter;
	unordered_map<string, CMesh*>::iterator iterEnd = m_mapMesh.end();
	for (iter = m_mapMesh.begin(); iter != iterEnd; ++iter)
	{
		m_vecMeshName.push_back(iter->first);
	}

	return &m_vecMeshName;
}

void CResourcesManager::GetMeshNameList(vector<string>* _pVec)
{
	unordered_map<string, CMesh*>::iterator iter;
	unordered_map<string, CMesh*>::iterator iterEnd = m_mapMesh.end();
	for (iter = m_mapMesh.begin(); iter != iterEnd; ++iter)
	{
		_pVec->push_back(iter->first);
	}
}

CMesh * CResourcesManager::FindMeshNonCount(const string & strName)
{
	unordered_map<string, class CMesh*>::iterator	iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

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

CTexture * CResourcesManager::FindTextureNonCount(const string & strName)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return nullptr;

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

CSampler * CResourcesManager::FindSamplerNonCount(const string & strName)
{
	unordered_map<string, CSampler*>::iterator	iter = m_mapSampler.find(strName);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

void CResourcesManager::CreateSphereVolum(const string& KeyName, float Radius, int StackSlice, int SliceCount, const string& ShaderKey, const string& LayoutKey)
{
	vector<VertexNormalColor> vecVertexData;

	//ㅡ자로 짜른다 (최대180도)
	float Phi = PUN_PI / StackSlice;
	//안쪽으로 원통형으로 자른다 (최대360도)
	float Theta = PUN_PI * 2.0f / SliceCount;

	for (int i = 0; i <= StackSlice; i++)
	{
		for (size_t j = 0; j < SliceCount; j++)
		{
			VertexNormalColor newVertex;

			if (SliceCount / 2 > j) //StackSlice 기준 위쪽
				newVertex.vPos = Vector3(Radius * sin(Phi * i) * cos(Theta * j), Radius * cos(Phi * i), Radius * sin(Phi * i) * sin(Theta * j));
			else //아래쪽
				newVertex.vPos = Vector3(Radius * sin(Phi * i) * -cos(Theta * j - PUN_PI), Radius * cos(Phi * i), Radius * sin(Theta * i) * -sin(Theta * j - PUN_PI));

			newVertex.vNormal = newVertex.vPos;
			newVertex.vNormal.Normalize();
			newVertex.vColor = Vector4((rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f);

			vecVertexData.push_back(newVertex);
		}
	}

	vector<unsigned int> vecIndex;

	for (int i = 0; i <= StackSlice; ++i)
	{
		for (int j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;


			//마지막에 인덱스가 맞지않기때문에 다시 처음으로 맞춰준다.
			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	CreateMesh(KeyName, ShaderKey, LayoutKey, &vecVertexData[0], (int)vecVertexData.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void CResourcesManager::CreateCapsulVolum(const string & KeyName, float Radius, float Height, int StackSlice, int SliceCount)
{
	vector<VertexNormalColor> vecVertex{};

	auto stack_count = StackSlice;

	//짝수라면 홀수로 만든다. (중앙점 찾기가 쉬움)
	if (stack_count % 2 == 0)
		stack_count -= 1;

	float phi = PUN_PI / stack_count;
	float theta = PUN_PI * 2.0f / SliceCount;

	for (auto i = 0; i <= stack_count; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex{};

			auto y = Radius * cos(phi * i);

			if (j < SliceCount / 2)
				vertex.vPos = Vector3{ Radius * sin(phi * i) * cos(theta * j), y, Radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.vPos = Vector3{ Radius * sin(phi * i) * -cos(theta * j - PUN_PI), y, Radius * sin(phi * i) * -sin(theta * j - PUN_PI) };

			vertex.vNormal = vertex.vPos;
			vertex.vNormal.Normalize();

			//중앙을 찾아서 위라면 높이고 아래면 내린다.
			if (i <= stack_count / 2)
				vertex.vPos.y += Height * 0.5f;
			else
				vertex.vPos.y += -Height * 0.5f;

			vertex.vColor = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vecVertex.push_back(vertex);
		}
	}

	vector<unsigned int> vecIndex;

	for (auto i = 0; i < stack_count; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;

			//마지막에 인덱스가 맞지않기때문에 다시 처음으로 맞춰준다.
			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void CResourcesManager::CreateCylinderVolum(const string & KeyName, float Radius, int Height, int SliceCount)
{
	vector<VertexNormalColor> vecVertex;

	//돌려야 되니 360도
	auto theta = PUN_PI * 2.0f / SliceCount;

	//윗면 아랫면 2개
	for (auto i = 0; i < 2; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex;

			//구면좌표계를 보면 x = Radius * cos(세타), z는 Radius * sin(세타)
			if (j < SliceCount / 2)
				vertex.vPos = Vector3{ Radius * cos(theta * j), (0.5f - i) * Height, Radius * sin(theta * j) };
			else
				vertex.vPos = Vector3{ Radius * -cos(theta * j - PUN_PI), (0.5f - i) * Height, Radius * -sin(theta * j - PUN_PI) };

			//X, Z의 Normal을 들고있는 정점
			vertex.vNormal = vertex.vPos;
			vertex.vNormal.y = 0.0f;
			vertex.vNormal.Normalize();
			vertex.vColor = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vecVertex.push_back(vertex);

			//윗, 아랫면의 Y의 Normal을 들고 있는 정점
			//이놈때문에 밑에 인덱스 코드 복잡함.
			if (0 == i)
				vertex.vNormal = Vector3(0.0f, 1.0f, 0.0f);
			else if (1 == i)
				vertex.vNormal = Vector3(0.0f, -1.0f, 0.0f);

			//정점을 2개추가한다.
			vecVertex.push_back(vertex);
		}
	}

	vector<unsigned int> vecIndex{};

	// 윗면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1;
		index[1] = 1 + (i + 2) * 2; //* 2 = 위 아래 두개.
		index[2] = 1 + (i + 1) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	// 옆면
	for (auto i = 0; i < SliceCount; ++i)
	{
		unsigned int index[6];

		index[0] = i * 2;
		index[1] = (i + SliceCount + 1) * 2;
		index[2] = (i + SliceCount) * 2;
		index[3] = i * 2;
		index[4] = (i + 1) * 2;
		index[5] = (i + SliceCount + 1) * 2;

		if (i == SliceCount - 1)
		{
			index[1] = (i + 1) * 2;
			index[4] = (i - SliceCount + 1) * 2;
			index[5] = (i + 1) * 2;
		}

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
		vecIndex.push_back(index[3]);
		vecIndex.push_back(index[4]);
		vecIndex.push_back(index[5]);
	}

	// 아랫면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1 + SliceCount * 2;
		index[1] = 1 + SliceCount * 2 + (i + 1) * 2;
		index[2] = 1 + SliceCount * 2 + (i + 2) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void CResourcesManager::CreateCornVolum(const string & KeyName, float Radius, float Height, int StackSlice, int SliceCount)
{
	vector<VertexNormalColor> vecVertex{};

	//구를 45도 잘랐다.
	float phi = (PUN_PI / 4.0f) / StackSlice;
	float theta = PUN_PI * 2.0f / SliceCount;

	for (auto i = 0; i <= StackSlice; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex{};

			if (j < SliceCount / 2)
				vertex.vPos = Vector3{ Radius * sin(phi * i) * cos(theta * j), Radius * cos(phi * i), Radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.vPos = Vector3{ Radius * sin(phi * i) * -cos(theta * j - PUN_PI), Radius * cos(phi * i), Radius * sin(phi * i) * -sin(theta * j - PUN_PI) };

			vertex.vNormal= vertex.vPos;
			vertex.vNormal.Normalize();
			vertex.vColor = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vertex.vPos.y += Height;

			vecVertex.push_back(vertex);
		}
	}

	vecVertex.push_back(VertexNormalColor{ Vector3::Zero, Vector3(0.0f, -1.0f, 0.0f), Vector4::White });

	vector<unsigned int> vecIndex;

	//원을 말아준다.
	for (auto i = 0; i < StackSlice; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;

			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	//밑에점과 삼각형으로 연결한다.
	for (auto i = 0; i < SliceCount; ++i)
	{
		unsigned int index[3]{};

		index[0] = SliceCount * StackSlice + i;
		index[1] = SliceCount * StackSlice + i + 1;
		index[2] = (unsigned int)vecVertex.size() - 1;

		if (i == SliceCount - 1)
			index[1] = SliceCount * (StackSlice - 1) + i + 1;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

//void CResourcesManager::CreateSphere(vector<VertexNormalTex>& vecVertex,
//	vector<int>& vecIndex, float fRadius, unsigned int iSubDivision)
//{
//	// Put a cap on the number of subdivisions.
//	iSubDivision = min(iSubDivision, 5);
//
//	// Approximate a sphere by tessellating an icosahedron.
//	const float X = 0.525731f;
//	const float Z = 0.850651f;
//
//	Vector3 pos[12] =
//	{
//		Vector3(-X, 0.0f, Z),  Vector3(X, 0.0f, Z),
//		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
//		Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
//		Vector3(0.0f, -Z, X),  Vector3(0.0f, -Z, -X),
//		Vector3(Z, X, 0.0f),   Vector3(-Z, X, 0.0f),
//		Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f)
//	};
//
//	DWORD k[60] =
//	{
//		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
//		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
//		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
//		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
//	};
//
//	vecVertex.resize(12);
//	vecIndex.resize(60);
//
//	for (UINT i = 0; i < 12; ++i)
//		vecVertex[i].vPos = pos[i];
//
//	for (UINT i = 0; i < 60; ++i)
//		vecIndex[i] = k[i];
//
//	for (UINT i = 0; i < iSubDivision; ++i)
//		Subdivide(vecVertex, vecIndex);
//
//	// Project vertices onto sphere and scale.
//	for (UINT i = 0; i < vecVertex.size(); ++i)
//	{
//		// Project onto unit sphere.
//		Vector3	vN = vecVertex[i].vPos;
//		vN.Normalize();
//
//		// Project onto sphere.
//		Vector3 p = vN * fRadius;
//
//		vecVertex[i].vPos = p;
//		// Normal이 있을 경우 따로 저장한다.
//
//		// Derive texture coordinates from spherical coordinates.
//		float theta = AngleFromXY(
//			vecVertex[i].vPos.x,
//			vecVertex[i].vPos.z);
//
//		float phi = acosf(vecVertex[i].vPos.y / fRadius);
//
//		vecVertex[i].vUV.x = theta / XM_2PI;
//		vecVertex[i].vUV.y = phi / XM_PI;
//
//		// Partial derivative of P with respect to theta
//		/*vecVertices[i].vTangent.x = -fRadius*sinf(phi)*sinf(theta);
//		vecVertices[i].vTangent.y = 0.0f;
//		vecVertices[i].vTangent.z = +fRadius*sinf(phi)*cosf(theta);*/
//
//		//XMVECTOR T = XMLoadFloat3(&vecVertices[i].TangentU);
//		//XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
//	}
//}

void CResourcesManager::CreateCylinderVolumColor(const string & KeyName, Vector4 _vColor, float Radius, int Height, int SliceCount)
{
	vector<VertexNormalColor> vecVertex;

	//돌려야 되니 360도
	auto theta = PUN_PI * 2.0f / SliceCount;

	//윗면 아랫면 2개
	for (auto i = 0; i < 2; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex;

			//구면좌표계를 보면 x = Radius * cos(세타), z는 Radius * sin(세타)
			if (j < SliceCount / 2)
				vertex.vPos = Vector3{ Radius * cos(theta * j), (0.5f - i) * Height, Radius * sin(theta * j) };
			else
				vertex.vPos = Vector3{ Radius * -cos(theta * j - PUN_PI), (0.5f - i) * Height, Radius * -sin(theta * j - PUN_PI) };

			//X, Z의 Normal을 들고있는 정점
			vertex.vNormal = vertex.vPos;
			vertex.vNormal.y = 0.0f;
			vertex.vNormal.Normalize();
			vertex.vColor = _vColor;

			vecVertex.push_back(vertex);

			//윗, 아랫면의 Y의 Normal을 들고 있는 정점
			//이놈때문에 밑에 인덱스 코드 복잡함.
			if (0 == i)
				vertex.vNormal = Vector3(0.0f, 1.0f, 0.0f);
			else if (1 == i)
				vertex.vNormal = Vector3(0.0f, -1.0f, 0.0f);

			//정점을 2개추가한다.
			vecVertex.push_back(vertex);
		}
	}

	vector<unsigned int> vecIndex{};

	// 윗면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1;
		index[1] = 1 + (i + 2) * 2; //* 2 = 위 아래 두개.
		index[2] = 1 + (i + 1) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	// 옆면
	for (auto i = 0; i < SliceCount; ++i)
	{
		unsigned int index[6];

		index[0] = i * 2;
		index[1] = (i + SliceCount + 1) * 2;
		index[2] = (i + SliceCount) * 2;
		index[3] = i * 2;
		index[4] = (i + 1) * 2;
		index[5] = (i + SliceCount + 1) * 2;

		if (i == SliceCount - 1)
		{
			index[1] = (i + 1) * 2;
			index[4] = (i - SliceCount + 1) * 2;
			index[5] = (i + 1) * 2;
		}

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
		vecIndex.push_back(index[3]);
		vecIndex.push_back(index[4]);
		vecIndex.push_back(index[5]);
	}

	// 아랫면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1 + SliceCount * 2;
		index[1] = 1 + SliceCount * 2 + (i + 1) * 2;
		index[2] = 1 + SliceCount * 2 + (i + 2) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

}