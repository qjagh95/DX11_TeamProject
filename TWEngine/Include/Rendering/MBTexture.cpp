#include "../EngineHeader.h"
#include "MBTexture.h"
#include "../Device.h"
#include "../Rendering/ShaderManager.h"

PUN_USING

CMBTexture::CMBTexture() :
	m_iSize(0),
	m_iCapacity(0),
	m_iMaxIndex(0),
	m_iCurIndex(-1),
	m_iRegister(0),
	m_pSrcTex(nullptr)
{
	m_vecSRV.clear();
	m_vecTex.clear();
}


CMBTexture::~CMBTexture()
{
	vector<ID3D11Texture2D*>::iterator iter;
	vector<ID3D11Texture2D*>::iterator iterEnd = m_vecTex.end();

	for (iter = m_vecTex.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	vector<ID3D11ShaderResourceView*>::iterator iter1;
	vector<ID3D11ShaderResourceView*>::iterator iterEnd1 = m_vecSRV.end();

	for (iter1 = m_vecSRV.begin(); iter1 != iterEnd1; ++iter1)
	{
		SAFE_RELEASE((*iter1));
	}
}

bool CMBTexture::CreateMBTexture(int iCount, int iRegister, ID3D11Texture2D* pSrcTex, DXGI_FORMAT eFormat)
{
	if (iCount <= 0 || iCount > MAX_SAMPLE)
		return false;

	m_iCapacity = iCount;
	m_iMaxIndex = iCount - 1;
	m_eFormat = eFormat;

	m_pSrcTex = pSrcTex;


	Resolution	tRS = _RESOLUTION;

	// 타겟용 텍스처 생성
	D3D11_TEXTURE2D_DESC	tDesc = {};
	m_pSrcTex->GetDesc(&tDesc);
	//tDesc.Width = tRS.iWidth;
	//tDesc.Height = tRS.iHeight;
	//tDesc.ArraySize = 1;
	//tDesc.SampleDesc.Count = 1;
	//tDesc.SampleDesc.Quality = 0;
	//tDesc.MipLevels = 1;
	//tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//tDesc.Format = eFormat;
	//tDesc.Usage = D3D11_USAGE_DEFAULT;

	m_tTestCBuffer.iSize = 0;

	for (int i = 0; i < iCount; i++)
	{
		ID3D11Texture2D* pTex = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;

		if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &pTex)))
			return false;

		// 만들어진 텍스처로 ShaderResourceView와 TargetView를 생성한다.
		if (FAILED(DEVICE->CreateShaderResourceView(pTex, nullptr,
			&pSRV)))
			return false;

		m_vecTex.push_back(pTex);
		m_vecSRV.push_back(pSRV);
	}

	return true;
}

bool CMBTexture::Resize(int iCapacity)
{
	if (iCapacity > MAX_SAMPLE)
		return false;

	int AddedCount = iCapacity - m_iCapacity;

	m_iCapacity = iCapacity;
	m_iSize = 0;
	m_iMaxIndex = iCapacity - 1;
	m_iCurIndex = -1;
	m_tTestCBuffer.iSize = 0;

	if (AddedCount <= 0)
		return true;

	Resolution	tRS = _RESOLUTION;

	// 타겟용 텍스처 생성
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.Width = tRS.iWidth;
	tDesc.Height = tRS.iHeight;
	tDesc.ArraySize = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.MipLevels = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tDesc.Format = m_eFormat;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	for (int i = 0; i < AddedCount; i++)
	{
		ID3D11Texture2D* pTex = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;

		if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &pTex)))
			return false;

		// 만들어진 텍스처로 ShaderResourceView와 TargetView를 생성한다.
		if (FAILED(DEVICE->CreateShaderResourceView(pTex, nullptr,
			&pSRV)))
			return false;

		m_vecTex.push_back(pTex);
		m_vecSRV.push_back(pSRV);
	}

	return true;
}

void CMBTexture::SetShaderResourceToCS(int iRegister)
{
	CopyResource();

	ID3D11ShaderResourceView* pSRV[MAX_SAMPLE] = {};

	int j = m_iCurIndex;

	for (int i = 0; i < m_iSize; ++i)
	{		
		CONTEXT->CSSetShaderResources(i, 1, &m_vecSRV[j]);

		if (j == m_iMaxIndex)
			j = 0;
		else
			++j;
	}
	
	

	GET_SINGLE(CShaderManager)->UpdateCBuffer("MotionBlur", &m_tTestCBuffer);
}

void CMBTexture::SetShaderResourceToCS()
{
	CopyResource();

	ID3D11ShaderResourceView* pSRV[MAX_SAMPLE] = {};

	int j = m_iCurIndex;

	for (int i = 0; i < m_iSize; ++i)
	{
		pSRV[i] = m_vecSRV[j];

		if (j == m_iMaxIndex)
			j = 0;
		else
			++j;
	}

	CONTEXT->CSSetShaderResources(0, m_iSize, pSRV);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("MotionBlur", &m_tTestCBuffer);
}

void CMBTexture::ResetShaderResourceFromCS()
{
	ID3D11ShaderResourceView* pSRV[MAX_SAMPLE] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	CONTEXT->CSSetShaderResources(m_iRegister, m_iSize, pSRV);
}

void CMBTexture::CopyResource()
{
	if (m_iSize < m_iCapacity)
	{
		m_iCurIndex = m_iMaxIndex - m_iSize;
		++m_iSize;
		m_tTestCBuffer.iSize = m_iSize;
	}
	else if (m_iSize == m_iCapacity)
	{
		if (m_iCurIndex == 0)
			m_iCurIndex = m_iMaxIndex;
		else
			m_iCurIndex--;
	}	

	CONTEXT->CopyResource(m_vecTex[m_iCurIndex], m_pSrcTex);
}

void CMBTexture::Stop()
{
	m_iSize = 0;
	m_iCurIndex = -1;
	m_tTestCBuffer.iSize = 0;
}
