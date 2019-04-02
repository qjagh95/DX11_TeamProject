#include "../EngineHeader.h"
#include "CSBloomFilter.h"
#include "UAV.h"
#include "PostEffect.h"
#include "ViewManager.h"
#include "RenderTarget.h"
#include "../Device.h"

PUN_USING

CCSBloomFilter::CCSBloomFilter()
{
	m_eFilterType = CFT_BLOOM;
	m_pBloomThreshold = 0.f;
	m_pBloomScale = 0.f;
	m_bFinal = false;
}

CCSBloomFilter::~CCSBloomFilter()
{
	SAFE_DELETE(m_pPostEffect);
}

bool CCSBloomFilter::Init()
{
	m_pPostEffect = new CPostEffect;

	if (!m_pPostEffect->Init())
		return false;

	CUAV* pUAV = GET_SINGLE(CViewManager)->FindUAV("DownScale4x4");

	if (!pUAV)
		return false;

	m_pSrcSRV = pUAV->GetShaderResourceView();

	CUAV* m_pDownScale = GET_SINGLE(CViewManager)->FindUAV("DownScale4x4");

	if (!m_pDownScale)
		return false;

	m_pDownScaleSRV = m_pDownScale->GetShaderResourceView();
	m_pDownScaleUAV = m_pDownScale->GetUnorderedAccessView();

	m_iFinalPassRegister = 10;

	return true;
}

void CCSBloomFilter::SetShaderResourceTo()
{
	if (m_bFinal)
	{
		m_vecUAV[BLOOM_FINAL]->SetSRV(3);
	}
	m_vecUAV[BLOOM_SECOND]->SetSRV(m_iFinalPassRegister);
	m_pPostEffect->UpdateCBuffer(1);
}

void CCSBloomFilter::ResetShaderResourceFrom()
{
	if (m_bFinal)
	{
		m_vecUAV[BLOOM_FINAL]->ResetSRV(3);
	}
	m_vecUAV[BLOOM_SECOND]->ResetSRV(m_iFinalPassRegister);
}

void CCSBloomFilter::Dispatch(float fTime)
{
	for (int i = 0; i < 5; ++i)
	{
		SetShaderResource(i, fTime);
		m_vecUAV[i]->Dispatch();
		ResetShaderResource(i, fTime);
	}
}

void CCSBloomFilter::SetShaderResource(int iPass, float fTime)
{
	m_pPostEffect->SetBloomThresholdCB(1.1f, fTime);
	m_pPostEffect->SetBloomCB(0.74f, fTime);

	m_pPostEffect->UpdateCBuffer(0);
	m_pPostEffect->UpdateCBuffer(3);
	m_pPostEffect->UpdateCBuffer(4);

	switch (iPass)
	{
		// First Pass
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
		m_vecUAV[BLOOM_FIRST]->SetUAV(0);
		break;

		// Second Pass
	case 1:
		m_vecUAV[BLOOM_SECOND]->SetUAV(0);

		m_vecUAV[BLOOM_FIRST]->SetSRV(1, ST_COMPUTE);
		m_vecUAV[BLOOM_OLD]->SetSRV(2, ST_COMPUTE);
		break;

		// Bloom Processing
	case 2:
		m_vecUAV[BLOOM_PROCESSING]->SetUAV(1);

		CONTEXT->CSSetShaderResources(0, 1, &m_pDownScaleSRV);
		m_vecUAV[BLOOM_SECOND]->SetSRV(1, ST_COMPUTE);
		break;

		// Horiz Blur
	case 3:
		m_vecUAV[BLOOM_BLUR]->SetUAV(0);

		m_vecUAV[BLOOM_PROCESSING]->SetSRV(0, ST_COMPUTE);
		break;

		// Vertical Blur
	case 4:
		m_vecUAV[BLOOM_FINAL]->SetUAV(0);

		m_vecUAV[BLOOM_BLUR]->SetSRV(0, ST_COMPUTE);
		m_bFinal = true;
		break;
	}
}

void CCSBloomFilter::ResetShaderResource(int iPass, float fTime)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	m_vecUAV[iPass]->ResetUAV(0);

	switch (iPass)
	{
		// First Pass
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		m_vecUAV[BLOOM_FIRST]->ResetUAV(0);
		break;

		// Second Pass
	case 1:
		m_vecUAV[BLOOM_SECOND]->ResetUAV(0);

		CONTEXT->CSSetShaderResources(1, 1, &pSRV);
		CONTEXT->CSSetShaderResources(2, 1, &pSRV);
		break;

		// Bloom Processing
	case 2:
		m_vecUAV[BLOOM_PROCESSING]->ResetUAV(1);

		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		m_vecUAV[BLOOM_SECOND]->ResetSRV(1, ST_COMPUTE);
		break;

		// Horiz Blur
	case 3:
		m_vecUAV[BLOOM_BLUR]->ResetUAV(0);

		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		break;

		// Vertical Blur
	case 4:
		m_vecUAV[BLOOM_FINAL]->ResetUAV(0);

		CONTEXT->CSSetShaderResources(0, 1, &pSRV);

		static bool bFirstTime = true;

		if (bFirstTime)
		{
			bFirstTime = false;
			m_pAdaptAvgLumUAV = m_vecUAV[BLOOM_SECOND]->GetUnorderedAccessView();
			m_pAdaptAvgLumSRV = m_vecUAV[BLOOM_SECOND]->GetShaderResourceView();
			m_pAdaptOldAvgLumUAV = m_vecUAV[BLOOM_OLD]->GetUnorderedAccessView();
			m_pAdaptOldAvgLumSRV = m_vecUAV[BLOOM_OLD]->GetShaderResourceView();
		}

		//ID3D11Buffer*				pTempBuffer;
		ID3D11UnorderedAccessView*	pTempUAV;
		ID3D11ShaderResourceView*	pTempSRV;

		// Swap วุมุดู
		pTempUAV = m_pAdaptAvgLumUAV;
		pTempSRV = m_pAdaptAvgLumSRV;
		m_pAdaptAvgLumUAV = m_pAdaptOldAvgLumUAV;
		m_pAdaptAvgLumSRV = m_pAdaptOldAvgLumSRV;
		m_pAdaptOldAvgLumUAV = pTempUAV;
		m_pAdaptOldAvgLumSRV = pTempSRV;
		break;
	}
}
