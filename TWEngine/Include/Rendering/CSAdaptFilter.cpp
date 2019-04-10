#include "../EngineHeader.h"
#include "CSAdaptFilter.h"
#include "UAV.h"
#include "PostEffect.h"
#include "ViewManager.h"
#include "RenderTarget.h"
#include "../Device.h"

PUN_USING

CCSAdaptFilter::CCSAdaptFilter()
{
	m_eFilterType = CFT_ADAPTATION;
	m_fElapsedTime = 0.f;
	m_fAdaptation = 1.f;
	m_fAdaptTime = 3.f;
}

CCSAdaptFilter::~CCSAdaptFilter()
{
	SAFE_DELETE(m_pPostEffect);
}

void CCSAdaptFilter::SetAdaptationTime(float fAdapt)
{
	m_fAdaptTime = fAdapt;
}

bool CCSAdaptFilter::Init()
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

void CCSAdaptFilter::SetShaderResourceTo()
{
	m_vecUAV[ADAPT_PASS_SECOND]->SetSRV(m_iFinalPassRegister);
#ifdef _DEBUG
#else
	m_pPostEffect->UpdateCBuffer(1);
#endif
}

void CCSAdaptFilter::ResetShaderResourceFrom()
{
	m_vecUAV[ADAPT_PASS_SECOND]->ResetSRV(m_iFinalPassRegister);
}

void CCSAdaptFilter::Dispatch(float fTime)
{
	for (int i = 0; i < 2; ++i)
	{
		SetShaderResource(i, fTime);
		m_vecUAV[i]->Dispatch();
		ResetShaderResource(i, fTime);
	}
}

void CCSAdaptFilter::SetShaderResource(int iPass, float fTime)
{
	m_fElapsedTime += fTime / 2;

	static bool bFirstTime = true;

	float fAdaptNorm;

	if (bFirstTime)
	{
		fAdaptNorm = 0.f;
		bFirstTime = false;
		m_pAdaptAvgLumUAV = m_vecUAV[ADAPT_PASS_SECOND]->GetUnorderedAccessView();
		m_pAdaptAvgLumSRV = m_vecUAV[ADAPT_PASS_SECOND]->GetShaderResourceView();
		m_pAdaptOldAvgLumUAV = m_vecUAV[ADAPT_OLD]->GetUnorderedAccessView();
		m_pAdaptOldAvgLumSRV = m_vecUAV[ADAPT_OLD]->GetShaderResourceView();
	}

	else
	{
		float fResetTime = m_fAdaptTime;

#ifdef _DEBUG
		if (m_fElapsedTime > fResetTime)
		{
			m_fElapsedTime -= fResetTime;
		}		
#endif
		
		fAdaptNorm = min(m_fAdaptTime < 0.0001f ? m_fAdaptTime : m_fElapsedTime / m_fAdaptTime, m_fAdaptTime - 0.0001f);
	}

	m_fAdaptation = fAdaptNorm;
	m_pPostEffect->SetAdaptationCB(fAdaptNorm, fTime);

	m_pPostEffect->UpdateCBuffer(0);
	m_pPostEffect->UpdateCBuffer(2);

	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
		m_vecUAV[ADAPT_PASS_FIRST]->SetUAV(0);
		break;

	case 1:
		m_vecUAV[ADAPT_PASS_SECOND]->SetUAV(0);

		m_vecUAV[ADAPT_PASS_FIRST]->SetSRV(1, ST_COMPUTE);
		m_vecUAV[ADAPT_OLD]->SetSRV(2, ST_COMPUTE);
		break;

	}
}

void CCSAdaptFilter::ResetShaderResource(int iPass, float fTime)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	m_vecUAV[iPass]->ResetUAV(0);

	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		m_vecUAV[ADAPT_PASS_FIRST]->ResetUAV(0);
		break;

	case 1:
		m_vecUAV[ADAPT_PASS_SECOND]->ResetUAV(0);
		CONTEXT->CSSetShaderResources(1, 1, &pSRV);
		CONTEXT->CSSetShaderResources(2, 1, &pSRV);	

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
