#pragma once
#include "Ref.h"
#include "DirectXTex.h"

PUN_BEGIN

class PUN_DLL CTexture :
	public CRef
{
	friend class CResourcesManager;
	friend class CRenderTarget;
private:
	CTexture();
	~CTexture();

private:
	vector<ScratchImage*>		m_vecImage;
	ID3D11ShaderResourceView*	m_pSRV;
	std::vector<TCHAR*>			m_vecFullPath;
	TCHAR						m_strFullPath[MAX_PATH];

public:
	int GetWidth()	const;
	int GetHeight()	const;
	const uint8_t*	GetPixels()	const;
	size_t GetPixelSize()	const;
	const std::vector<TCHAR*>* GetFullPath() const
	{
		return &m_vecFullPath;
	}

public:
	bool LoadTexture(const string& strName, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const string& strName, const TCHAR* pFullPath);
	bool LoadTexture(const string& strName, const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const string& strName, const vector<const TCHAR*>& vecFullPath);
	void SetShader(int iRegister);
	void ResetShader(int iRegister);

private:
	bool CreateShaderResource();
	bool CreateShaderResourceArray();

};

PUN_END