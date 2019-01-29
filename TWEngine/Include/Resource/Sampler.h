#pragma once
#include "../Ref.h"

PUN_BEGIN

class PUN_DLL CSampler :
	public CRef
{
	friend class CResourcesManager;

private:
	CSampler();
	~CSampler();

private:
	ID3D11SamplerState*	m_pSampler;

public:
	bool CreateSampler(const string& strName,
		D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE eU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eW = D3D11_TEXTURE_ADDRESS_WRAP);
	void SetShader(int iRegister);
};

PUN_END