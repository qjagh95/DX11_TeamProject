#include "../EngineHeader.h"
#include "UAV.h"
#include "ShaderManager.h"
#include "ComputeShader.h"
#include "../Device.h"

PUN_USING

CUAV::CUAV()	:
	m_pUAV(nullptr),
	m_pResource(nullptr),
	m_pSRV(nullptr),
	m_pOldUAV(nullptr)
{
}

CUAV::~CUAV()
{
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pResource);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pOldUAV);
}


bool CUAV::CreateView(const string& strName, const string& strShaderName, 
	GPU_RESOURCE_TYPE eType, int iNumX, int iNumY, int iNumZ,
	int iWidth, int iHeight, DXGI_FORMAT eFormat)
{
	m_strName = strName;
	m_eType = eType;

	m_pCShader = GET_SINGLE(CShaderManager)->FindComputeShaderNonCount(strShaderName);

	m_iNumThreadGroupX = iNumX;
	m_iNumThreadGroupY = iNumY;
	m_iNumThreadGroupZ = iNumZ;

	D3D11_BUFFER_DESC		tDesc = {};
	D3D11_TEXTURE2D_DESC	tTexDesc = {};

	switch (eType)
	{
	case GRT_BUFFER:
		tDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		tDesc.StructureByteStride = sizeof(float);
		tDesc.ByteWidth = iWidth * tDesc.StructureByteStride;
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, (ID3D11Buffer**)&m_pResource)))
			return false;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_pResource, NULL,
			&m_pUAV)))
			return false;

		if (FAILED(DEVICE->CreateShaderResourceView(m_pResource, NULL, &m_pSRV)))
			return false;
		break;

	case GRT_TEXTURE2D:
		tTexDesc.Width = iWidth;
		tTexDesc.Height = iHeight;
		tTexDesc.MipLevels = 1;
		tTexDesc.ArraySize = 1;
		tTexDesc.SampleDesc.Count = 1;
		tTexDesc.SampleDesc.Quality = 0;
		tTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		tTexDesc.Format = eFormat;
		tTexDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(DEVICE->CreateTexture2D(&tTexDesc, nullptr, (ID3D11Texture2D**)&m_pResource)))
			return false;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_pResource, NULL, &m_pUAV)))
			return false;

		if (FAILED(DEVICE->CreateShaderResourceView(m_pResource, NULL, &m_pSRV)))
			return false;

		break;
	}

	return true;
}

void CUAV::SetUAV(int iRegister)
{
	CONTEXT->CSGetUnorderedAccessViews(iRegister, 1, &m_pOldUAV);
	CONTEXT->CSSetUnorderedAccessViews(iRegister, 1, &m_pUAV, 0);
}

void CUAV::ResetUAV(int iRegister)
{
	
	CONTEXT->CSSetUnorderedAccessViews(iRegister, 1, &m_pOldUAV, 0);
	SAFE_RELEASE(m_pOldUAV);
}

void CUAV::SetSRV(int iRegister, SHADER_TYPE eType)
{
	switch (eType)
	{
	case ST_PIXEL:
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
		break;
	case ST_VERTEX:
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_pSRV);
		break;
	case ST_COMPUTE:
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
		break;
	}
}

void CUAV::ResetSRV(int iRegister, SHADER_TYPE eType)
{
	ID3D11ShaderResourceView*	pSRV = nullptr;

	switch (eType)
	{
	case ST_PIXEL:
		CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
		break;
	case ST_VERTEX:
		CONTEXT->VSSetShaderResources(iRegister, 1, &pSRV);
		break;
	case ST_COMPUTE:
		CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
		break;
	}
}

void CUAV::Dispatch()
{
	m_pCShader->SetComputeShader();
	CONTEXT->Dispatch(m_iNumThreadGroupX, m_iNumThreadGroupY, m_iNumThreadGroupZ);
}

void CUAV::Dispatch(int iNumThreadGroupX, int iNumThreadGroupY, int iNumThreadGroupZ)
{
	m_pCShader->SetComputeShader();
	CONTEXT->Dispatch(iNumThreadGroupX, iNumThreadGroupY, iNumThreadGroupZ);
}
