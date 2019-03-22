#include "../EngineHeader.h"
#include "CSHDRFilter.h"
#include "UAV.h"
#include "PostEffect.h"
#include "ViewManager.h"
#include "RenderTarget.h"
#include "../Device.h"

PUN_USING

CCSHDRFilter::CCSHDRFilter()
{
}


CCSHDRFilter::~CCSHDRFilter()
{
	SAFE_DELETE(m_pPostEffect);
}

bool CCSHDRFilter::Init()
{
	m_pPostEffect = new CPostEffect;

	if (!m_pPostEffect->Init())
		return false;

	CUAV* pUAV = GET_SINGLE(CViewManager)->FindUAV("DownScale4x4");

	if (!pUAV)
		return false;

	m_pSrcSRV = pUAV->GetShaderResourceView();

	return true;
}

void CCSHDRFilter::SetShaderResourceTo(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[HDR_SECOND]->SetSRV(iRegister, eType);
}

void CCSHDRFilter::ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[HDR_SECOND]->ResetSRV(iRegister, eType);
}

void CCSHDRFilter::SetShaderResource(int iPass)
{
	m_vecUAV[iPass]->SetUAV(0);
	m_pPostEffect->UpdateCBuffer(iPass);

	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
		break;

	case 1:
		m_vecUAV[HDR_FIRST]->SetSRV(0, ST_COMPUTE);
		break;
	}
}

void CCSHDRFilter::ResetShaderResource(int iPass)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	m_vecUAV[iPass]->ResetUAV(0);

	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		break;

	case 1:
		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		break;
	}
}