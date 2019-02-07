
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


	vector<VertexNormalTex>	vecSphereVtx;
	vector<int>		vecSphereIdx;

	CreateSphere(vecSphereVtx, vecSphereIdx, 1.f, 10);

	vector<Vector3>	vecSpherePos;
	vecSpherePos.resize(vecSphereVtx.size());
	for (size_t i = 0; i < vecSphereVtx.size(); ++i)
	{
		vecSpherePos[i] = vecSphereVtx[i].vPos;
	}

	CreateMesh("LightSphereVolume", STANDARD_NORMAL_COLOR_SHADER, POS_LAYOUT,
		&vecSpherePos[0], vecSpherePos.size(), sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecSphereIdx[0],
		(int)vecSphereIdx.size(), 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT);


	// 선형필터링 하나 생성한다.
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

	if (!pMesh->LoadMesh(strName, pFullPath))
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

void CResourcesManager::CreateSphere(vector<VertexNormalTex>& vecVertex, vector<int>& vecIndex, float fRadius, unsigned int iSubDivision)
{
	// Put a cap on the number of subdivisions.
	iSubDivision = min(iSubDivision, 5);

	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f;
	const float Z = 0.850651f;

	Vector3 pos[12] =
	{
		Vector3(-X, 0.0f, Z),  Vector3(X, 0.0f, Z),
		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
		Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
		Vector3(0.0f, -Z, X),  Vector3(0.0f, -Z, -X),
		Vector3(Z, X, 0.0f),   Vector3(-Z, X, 0.0f),
		Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vecVertex.resize(12);
	vecIndex.resize(60);

	for (UINT i = 0; i < 12; ++i)
		vecVertex[i].vPos = pos[i];

	for (UINT i = 0; i < 60; ++i)
		vecIndex[i] = k[i];

	for (UINT i = 0; i < iSubDivision; ++i)
		Subdivide(vecVertex, vecIndex);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vecVertex.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3	vN = vecVertex[i].vPos;
		vN.Normalize();

		// Project onto sphere.
		Vector3 p = vN * fRadius;

		vecVertex[i].vPos = p;
		// Normal이 있을 경우 따로 저장한다.

		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(
			vecVertex[i].vPos.x,
			vecVertex[i].vPos.z);

		float phi = acosf(vecVertex[i].vPos.y / fRadius);

		vecVertex[i].vUV.x = theta / XM_2PI;
		vecVertex[i].vUV.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		/*vecVertices[i].vTangent.x = -fRadius*sinf(phi)*sinf(theta);
		vecVertices[i].vTangent.y = 0.0f;
		vecVertices[i].vTangent.z = +fRadius*sinf(phi)*cosf(theta);*/

		//XMVECTOR T = XMLoadFloat3(&vecVertices[i].TangentU);
		//XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}
}

void CResourcesManager::Subdivide(vector<VertexNormalTex>& vecVertices, vector<int>& vecIndices)
{
	// Save a copy of the input geometry.
	vector<VertexNormalTex>	vecCopyVertex = vecVertices;
	vector<int>	vecCopyIndex = vecIndices;


	vecVertices.resize(0);
	vecIndices.resize(0);

	//       v1
	//       *
	//      / \
			//     /   \
	//  m0*-----*m1
//   / \   / \
	//  /   \ /   \
	// *-----*-----*
// v0    m2     v2

	UINT numTris = (UINT)vecCopyIndex.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		VertexNormalTex v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		VertexNormalTex v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		VertexNormalTex v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		VertexNormalTex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.vPos = Vector3(
			0.5f*(v0.vPos.x + v1.vPos.x),
			0.5f*(v0.vPos.y + v1.vPos.y),
			0.5f*(v0.vPos.z + v1.vPos.z));

		m1.vPos = Vector3(
			0.5f*(v1.vPos.x + v2.vPos.x),
			0.5f*(v1.vPos.y + v2.vPos.y),
			0.5f*(v1.vPos.z + v2.vPos.z));

		m2.vPos = Vector3(
			0.5f*(v0.vPos.x + v2.vPos.x),
			0.5f*(v0.vPos.y + v2.vPos.y),
			0.5f*(v0.vPos.z + v2.vPos.z));

		//
		// Add new geometry.
		//

		vecVertices.push_back(v0); // 0
		vecVertices.push_back(v1); // 1
		vecVertices.push_back(v2); // 2
		vecVertices.push_back(m0); // 3
		vecVertices.push_back(m1); // 4
		vecVertices.push_back(m2); // 5

		vecIndices.push_back(i * 6 + 0);
		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 5);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 2);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 1);
		vecIndices.push_back(i * 6 + 4);
	}
}

float CResourcesManager::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * PUN_PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + PUN_PI; // in [0, 2*pi).

	return theta;
}
