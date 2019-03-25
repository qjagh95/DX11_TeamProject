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
	GPU_RESOURCE_TYPE			m_eType;
	string						m_strName;
	int							m_iNumThreadGroupX;
	int							m_iNumThreadGroupY;
	int							m_iNumThreadGroupZ;

private:
	class CComputeShader*	m_pCShader;

public:
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pSRV; }

public:
	bool CreateView(const string& strName, const string& strShaderName, 
		GPU_RESOURCE_TYPE eType, int iNumX, int iNumY, int iNumZ,
		int iWidth, int iHeight, 
		DXGI_FORMAT eFormat = DXGI_FORMAT_R32G32B32A32_FLOAT);

	void SetUAV(int iRegister);
	void ResetUAV(int iRegister);
	void SetSRV(int iRegister, SHADER_TYPE eType = ST_PIXEL);
	void ResetSRV(int iRegister, SHADER_TYPE eType = ST_PIXEL);

public:
	void Dispatch();
	void Dispatch(int iNumThreadGroupX, int iNumThreadGroupY, int iNumThreadGroupZ);

	template <typename T>
	T* GetResource() const
	{
		return (T*)m_pResource;
	}
};

PUN_END