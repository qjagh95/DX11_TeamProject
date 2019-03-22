#pragma once

#define MAX_SAMPLE 7

PUN_BEGIN

class PUN_DLL CMBTexture
{
public:
	CMBTexture();
	~CMBTexture();

private:
	int									m_iRegister;
	int									m_iCapacity;
	int									m_iMaxIndex;
	int									m_iSize;
	int									m_iCurIndex;

	BlurCBuffer							m_tTestCBuffer;
	DXGI_FORMAT							m_eFormat;

	vector<ID3D11Texture2D*>			m_vecTex;
	vector<ID3D11ShaderResourceView*>	m_vecSRV;

	ID3D11Texture2D*					m_pSrcTex;

public:
	bool CreateMBTexture(int iCount, int iRegister, ID3D11Texture2D* pSrcTex, DXGI_FORMAT eFormat = DXGI_FORMAT_R32G32B32A32_FLOAT);
	bool Resize(int iCapacity);
	void SetShaderResourceToCS(int iRegister);
	void SetShaderResourceToCS();
	void ResetShaderResourceFromCS();
	void CopyResource();
	void Stop();

	float ComputeWeight(int iOffset, float fRho);
};

PUN_END