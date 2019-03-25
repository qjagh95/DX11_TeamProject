#include "../EngineHeader.h"
#include "DownScale.h"
#include "ComputeShader.h"
#include "ShaderManager.h"
#include "UAV.h"
#include "../Device.h"
#include "RenderTarget.h"
#include "ViewManager.h"

PUN_USING

CDownScale::CDownScale()
{
	m_eFilterType = CFT_DOWNSCALE;
}

CDownScale::~CDownScale()
{
}

bool CDownScale::Init()
{
	CRenderTarget* pTarget = GET_SINGLE(CViewManager)->FindRenderTarget("Depth");

	if (!pTarget)
		return false;

	m_pDepthSRV = pTarget->GetShaderResourceView();

	pTarget = GET_SINGLE(CViewManager)->FindRenderTarget("SecondBackBuffer");

	if (!pTarget)
		return false;

	m_pSrcSRV = pTarget->GetShaderResourceView();

	return true;
}

void CDownScale::Dispatch()
{
	SetShaderResource(0);

	m_vecUAV[0]->Dispatch();

	ResetShaderResource(0);
}

void CDownScale::SetShaderResourceTo()
{
	m_vecUAV[0]->SetSRV(m_iFinalPassRegister);
}

void CDownScale::ResetShaderResourceFrom()
{
	m_vecUAV[0]->SetSRV(m_iFinalPassRegister);
}

void CDownScale::SetShaderResource(int iPass)
{
	CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
	CONTEXT->CSSetShaderResources(1, 1, &m_pDepthSRV);
	for (int i = 0; i < 3; ++i)
	{
		m_vecUAV[i]->SetUAV(i);
	}
}

void CDownScale::ResetShaderResource(int iPass)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(0, 1, &pSRV);
	CONTEXT->CSSetShaderResources(1, 1, &pSRV);

	for (int i = 0; i < 3; ++i)
	{
		m_vecUAV[i]->ResetUAV(i);
	}
}
