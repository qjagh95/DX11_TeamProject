#include "../EngineHeader.h"
#include "MotionBlurFilter.h"
#include "UAV.h"
#include "MBTexture.h"
#include "ComputeShader.h"
#include "ShaderManager.h"
#include "../Device.h"
#include "RenderTarget.h"
#include "ViewManager.h"


PUN_USING

CMotionBlurFilter::CMotionBlurFilter()	:
	m_pMBTex(nullptr)
{
}


CMotionBlurFilter::~CMotionBlurFilter()
{
	SAFE_DELETE(m_pMBTex);
}

void CMotionBlurFilter::Disable()
{
	m_bFilterOn = false;
	m_pMBTex->Stop();
}

bool CMotionBlurFilter::Init()
{
	m_pMBTex = new CMBTexture;

	CRenderTarget* pTarget = GET_SINGLE(CViewManager)->FindRenderTarget("LightBlend");

	if (!pTarget)
		return false;

	if (!m_pMBTex->CreateMBTexture(7, 0, pTarget->GetTexture()))
	{
		SAFE_DELETE(m_pMBTex);
		return false;
	}

	return true;
}

void CMotionBlurFilter::SetShaderResourceTo(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[0]->SetSRV(iRegister, eType);
}

void CMotionBlurFilter::ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[0]->ResetSRV(iRegister, eType);
}

void CMotionBlurFilter::SetShaderResource(int iPass)
{
	m_pMBTex->SetShaderResourceToCS();
	m_vecUAV[iPass]->SetUAV(0);
}

void CMotionBlurFilter::ResetShaderResource(int iPass)
{
	m_pMBTex->ResetShaderResourceFromCS();
	m_vecUAV[iPass]->ResetUAV(0);
}


void CMotionBlurFilter::Resize(int iCapacity)
{
	m_pMBTex->Resize(iCapacity);
}
