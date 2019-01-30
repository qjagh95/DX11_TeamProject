
#include "EngineHeader.h"
#include "Sampler.h"
#include "../Device.h"

PUN_USING

CSampler::CSampler() :
	m_pSampler(nullptr)
{
}

CSampler::~CSampler()
{
	SAFE_RELEASE(m_pSampler);
}

bool CSampler::CreateSampler(const string & strName, D3D11_FILTER eFilter,
	D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV,
	D3D11_TEXTURE_ADDRESS_MODE eW)
{
	SetTag(strName);

	D3D11_SAMPLER_DESC	tDesc = {};

	tDesc.Filter = eFilter;
	tDesc.AddressU = eU;
	tDesc.AddressV = eV;
	tDesc.AddressW = eW;
	tDesc.MinLOD = -FLT_MAX;
	tDesc.MaxLOD = FLT_MAX;
	tDesc.MipLODBias = 0.f;
	tDesc.MaxAnisotropy = 1;
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	float	f = 1.f;
	memcpy(tDesc.BorderColor, &f, sizeof(float) * 4);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&tDesc, &m_pSampler)))
		return false;

	return true;
}

void CSampler::SetShader(int iRegister)
{
	CONTEXT->PSSetSamplers(iRegister, 1, &m_pSampler);
}