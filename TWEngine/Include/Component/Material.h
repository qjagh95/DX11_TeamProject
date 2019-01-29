#pragma once
#include "Component.h"
#include "../Resource/ResourcesManager.h"

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
	class CTexture*	pDiffuseTex;
	int				iDiffuseRegister;
	class CSampler*	pDifSampler;
	int				iDifSamplerRegister;

	vector<TextureSet>	vecMultiTex;

	_tagSubsetMaterial() :
		pDiffuseTex(nullptr),
		iDiffuseRegister(-1),
		pDifSampler(nullptr),
		iDifSamplerRegister(0)
	{
	}
}SubsetMaterial, *PSubsetMaterial;

class PUN_DLL CMaterial :
	public CComponent
{
	friend class CGameObject;

private:
	CMaterial();
	CMaterial(const CMaterial& mtrl);
	~CMaterial();

private:
	vector<vector<PSubsetMaterial>>	m_vecMaterial;

public:
	void SetMaterial(const Vector4& vDiffuse, int iContainer = 0,
		int iSubset = 0);
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
	void AddMultiTex(int iSmpRegister, const string& strSmpKey, int iRegister,
		const string& strKey, const TCHAR* pFileName,
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

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMaterial* Clone();

public:
	void SetShader(int iContainer = 0, int iSubset = 0);

private:
	PSubsetMaterial CreateSubset();
};

PUN_END
