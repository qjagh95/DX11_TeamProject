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

	m_iFinalPassRegister = 10;

	return true;
}

void CCSHDRFilter::SetShaderResourceTo()
{
	m_vecUAV[HDR_SECOND]->SetSRV(m_iFinalPassRegister);
	m_pPostEffect->UpdateCBuffer(HDR_SECOND);
}

void CCSHDRFilter::ResetShaderResourceFrom()
{
	m_vecUAV[HDR_SECOND]->ResetSRV(m_iFinalPassRegister);
}

void CCSHDRFilter::SetShaderResource(int iPass)
{
	m_vecUAV[iPass]->SetUAV(0);
	m_pPostEffect->UpdateCBuffer(HDR_FIRST);

	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
		break;

	case 1:
		m_vecUAV[HDR_FIRST]->SetSRV(1, ST_COMPUTE);
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
		CONTEXT->CSSetShaderResources(1, 1, &pSRV);
		break;
	}
}