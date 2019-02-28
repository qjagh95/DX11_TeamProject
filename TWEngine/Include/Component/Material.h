#pragma once
#include "Component.h"

PUN_BEGIN

typedef struct PUN_DLL _tagTextureSet
{
	class CTexture*	pTex;
	int				iRegister;
	class CSampler*	pSampler;
	int				iSamplerRegister;

	_tagTextureSet() :
		pTex(nullptr),
		iRegister(-1),
		pSampler(nullptr),
		iSamplerRegister(0)
	{
	}
}TextureSet, *PTextureSet;

typedef struct PUN_DLL _tagSubsetMaterial
{
	Material	tMaterial;
	PTextureSet	pDiffuse;
	PTextureSet	pNormal;
	PTextureSet	pSpecular;
	vector<TextureSet>	vecMultiTex;

	_tagSubsetMaterial() :
		pDiffuse(nullptr),
		pNormal(nullptr),
		pSpecular(nullptr) 
	{
		tMaterial.vSpecular.w = 3.2f;
	}
}SubsetMaterial, *PSubsetMaterial;

class PUN_DLL CMaterial :
	public CComponent
{
	friend class CGameObject;
	friend class CMesh;

private:
	CMaterial();
	CMaterial(const CMaterial& mtrl);
	~CMaterial();

private:
	vector<vector<PSubsetMaterial>>	m_vecMaterial;

public:
	void SetMaterial(const Vector4& vDiffuse, const Vector4& vAmbient,
		const Vector4& vSpecular, float fSpecularPower,
		const Vector4& vEmissive, int iContainer = 0, int iSubset = 0);
	void SetDiffuseTex(int iRegister, const string& strKey, int iContainer = 0,
		int iSubset = 0);
	void SetDiffuseTex(int iRegister, const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void SetDiffuseTexFromFullPath(int iRegister, const string& strKey,
		const TCHAR* pFullPath, int iContainer = 0, int iSubset = 0);
	void SetDiffuseTex(int iRegister, const string& strKey, const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void SetDiffuseTexFromFullPath(int iRegister, const string& strKey,
		const vector<const TCHAR*>& vecFullPath, int iContainer = 0, int iSubset = 0);
	void SetDiffuseTex(int iRegister, class CTexture* pTex,
		int iContainer = 0, int iSubset = 0);
	void SetSampler(int iRegister, const string& strKey,
		int iContainer = 0, int iSubset = 0);

	void SetNormalTex(int iRegister, const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void SetNormalTexFromFullPath(int iRegister, const string& strKey,
		const TCHAR* pFullPath, int iContainer = 0, int iSubset = 0);
	void SetNormalSampler(int iRegister, const string& strKey,
		int iContainer = 0, int iSubset = 0);

	void SetSpecularTex(int iRegister, const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void SetSpecularTexFromFullPath(int iRegister, const string& strKey,
		const TCHAR* pFullPath, int iContainer = 0, int iSubset = 0);
	void SetSpecularSampler(int iRegister, const string& strKey,
		int iContainer = 0, int iSubset = 0);

	void AddMultiTex(int iSmpRegister, const string& strSmpKey, int iRegister,
		const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void AddMultiTex(int iSmpRegister, const string& strSmpKey,
		int iRegister, const string& strKey, const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	/*void AddMultiTex(int iRegister, const string& strKey, int iContainer = 0,
		int iSubset = 0);
	void AddMultiTex(int iRegister, const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void AddMultiTexFromFullPath(int iRegister, const string& strKey,
		const TCHAR* pFullPath, int iContainer = 0, int iSubset = 0);
	void AddMultiTex(int iRegister, const string& strKey, const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH,
		int iContainer = 0, int iSubset = 0);
	void AddMultiTexFromFullPath(int iRegister, const string& strKey,
		const vector<const TCHAR*>& vecFullPath, int iContainer = 0, int iSubset = 0);
	void AddMultiTex(int iRegister, class CTexture* pTex,
		int iContainer = 0, int iSubset = 0);
	void AddMultiSampler(int iRegister, const string& strKey,
		int iContainer = 0, int iSubset = 0);*/

	void ClearContainer();
public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMaterial* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	virtual void LoadFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
public:
	void SetShader(int iContainer = 0, int iSubset = 0);


private:
	PSubsetMaterial CreateSubset();
	void SaveTextureSet(FILE* pFile, PTextureSet pTexture);
	void LoadTextureSet(FILE* pFile, PTextureSet* ppTexture);
};

PUN_END
