#pragma once
#include "../EngineHeader.h"
#include "fbxsdk.h"

PUN_BEGIN
typedef struct PUN_DLL _tagFbxMaterial
{
	Vector4		vDif;
	Vector4		vAmb;
	Vector4		vSpc;
	Vector4		vEmv;
	float		fSpecularPower;
	float		fTransparencyFactor;
	float		fShininess;
	string		strDifTex;
	string		strBumpTex;
	string		strSpcTex;
}FBXMATERIAL , *PFBXMATERIAL;

typedef struct PUN_DLL _tagFBXMeshContainer
{
	vector<Vector3>	vecPos;
	vector<Vector3>	vecNormal;
	vector<Vector2>	vecUV;
	vector<Vector3>	vecTangent;
	vector<Vector3>	vecBinormal;
	//vector<Vector4>	vecBlendWeight;
	//vector<Vector4>	vecBlendIndex;
	vector<vector<UINT>>	vecIndices;
	//unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
	bool				bBump;
	//bool				bAnimation;

	_tagFBXMeshContainer()
	{
		bBump = false;
		//bAnimation = false;
	}
}FBXMESHCONTAINER, *PFBXMESHCONTAINER;

class PUN_DLL CFbxLoader
{
	friend class CMesh;

private:
	FbxManager*		m_pManager;
	FbxScene*		m_pScene;

private:
	vector<vector<PFBXMATERIAL>>	m_vecMaterials;
	vector<PFBXMESHCONTAINER>		m_vecMeshContainer;

public:
	const vector<vector<PFBXMATERIAL>>* GetMaterials()
	{
		return &m_vecMaterials;
	}

	const vector<PFBXMESHCONTAINER>* GetMeshContainers()
	{
		return &m_vecMeshContainer;
	}

public:
	bool LoadFbx(const char* _pFullPath);

private:
	void Triangulate(FbxNode* pNode);
	void LoadMaterial(FbxSurfaceMaterial * pMtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial * pMtrl,
		const char * pPropertyName, const char * pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial * pMtrl,
		const char * pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);

	void LoadMesh(FbxNode* pNode);
	void LoadMesh(FbxMesh* pMesh);

	void LoadNormal(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer, int iVtxID,
		int iControlIndex);
	void LoadUV(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer,
		int iUVID, int iControlIndex);
	void LoadTangent(FbxMesh * pMesh,
		PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex);
	void LoadBinormal(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer,
		int iVtxID, int iControlIndex);

private:
	CFbxLoader();
	~CFbxLoader();
};

PUN_END