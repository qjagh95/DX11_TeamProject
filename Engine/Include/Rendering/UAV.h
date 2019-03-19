#pragma once

PUN_BEGIN

class PUN_DLL CUAV
{
	friend class CViewManager;

private:
	CUAV();
public:
	~CUAV();

private:
	ID3D11UnorderedAccessView*	m_pUAV;
	ID3D11UnorderedAccessView*	m_pOldUAV;
	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11Buffer*				m_pBuffer;
	ID3D11Texture2D*			m_pTex;	
	BUFFER_TYPE					m_eType;
	string						m_strName;
	int							m_iNumThreadGroupX;
	int							m_iNumThreadGroupY;
	int							m_iNumThreadGroupZ;

private:
	class CComputeShader*	m_pCShader;

public:
	bool CreateView(BUFFER_TYPE eType, const string& strName, 
		int iWidth, int iHeight, const string& strShaderName, DXGI_FORMAT eFormat = DXGI_FORMAT_R32G32B32A32_FLOAT);
	bool CreateView(BUFFER_TYPE eType, const string& strName, int iNumX, int iNumY, int iNumZ,
		int iWidth, int iHeight, const string& strShaderName, DXGI_FORMAT eFormat = DXGI_FORMAT_R32G32B32A32_FLOAT);

public:
	void SetUAV(int iRegister);
	void ResetUAV(int iRegister);
	void SetSRV(int iRegister, SHADER_TYPE eType);
	void ResetSRV(int iRegister, SHADER_TYPE eType);

public:
	void Dispatch();
	void Dispatch(int iNumThreadGroupX, int iNumThreadGroupY, int iNumThreadGroupZ);
};

PUN_END