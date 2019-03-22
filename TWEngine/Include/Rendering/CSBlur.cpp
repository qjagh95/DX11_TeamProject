#include "../EngineHeader.h"
#include "CSBlur.h"
#include "UAV.h"
#include "ViewManager.h"
#include "RenderTarget.h"
#include "ComputeShader.h"
#include "ShaderManager.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Component/Camera.h"


PUN_USING

CCSBlur::CCSBlur()
{
}


CCSBlur::~CCSBlur()
{
}

bool CCSBlur::Init()
{
	CUAV* pUAV = GET_SINGLE(CViewManager)->FindUAV("DownScale2x2");

	if (!pUAV)
		return false;

	m_pSrcSRV = pUAV->GetShaderResourceView();

	m_tBlurCBuffer.iSize = 9;
	m_tBlurCBuffer.iDepthOfField = 1;
	m_tBlurCBuffer.fCameraFar = 1000.0f;

	return true;
}

void CCSBlur::SetShaderResourceTo(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[BLUR_VERTICAL]->SetSRV(iRegister, eType);
}

void CCSBlur::ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType)
{
	m_vecUAV[BLUR_VERTICAL]->ResetSRV(iRegister, eType);
}

void CCSBlur::SetShaderResource(int iPass)
{
	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &m_pSrcSRV);
		m_vecUAV[BLUR_HORIZONTAL]->SetUAV(0);

		break;

	case 1:
		m_vecUAV[BLUR_HORIZONTAL]->SetSRV(0, ST_COMPUTE);
		m_vecUAV[BLUR_VERTICAL	]->SetUAV(0);

		break;
	}

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Blur", &m_tBlurCBuffer);
}

void CCSBlur::ResetShaderResource(int iPass)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	switch (iPass)
	{
	case 0:
		CONTEXT->CSSetShaderResources(0, 1, &pSRV);
		m_vecUAV[BLUR_HORIZONTAL]->ResetUAV(0);

		break;

	case 1:
		m_vecUAV[BLUR_HORIZONTAL]->ResetSRV(0, ST_COMPUTE);
		m_vecUAV[BLUR_VERTICAL]->ResetUAV(0);

		break;
	}
}
