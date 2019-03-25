#include "EngineHeader.h"
#include "Texture.h"
#include "../Device.h"

PUN_USING

CTexture::CTexture() :
	m_pSRV(nullptr)
{
	memset(m_strFullPath, 0, sizeof(TCHAR) * MAX_PATH);
}

CTexture::~CTexture()
{
	for (size_t i = 0; i < m_vecFullPath.size(); ++i)
	{
		SAFE_DELETE_ARRAY(m_vecFullPath[i]);
	}

	m_vecFullPath.clear();


	SAFE_RELEASE(m_pSRV);
	Safe_Delete_VecList(m_vecImage);
}

int CTexture::GetWidth() const
{
	return (int)m_vecImage[0]->GetImage(0, 0, 0)->width;
}

int CTexture::GetHeight() const
{
	return (int)m_vecImage[0]->GetImage(0, 0, 0)->height;
}

const uint8_t* CTexture::GetPixels() const
{
	return m_vecImage[0]->GetPixels();
}

size_t CTexture::GetPixelSize() const
{
	return m_vecImage[0]->GetPixelsSize();
}

bool CTexture::LoadTexture(const string & strName, const TCHAR * pFileName,
	const string & strPathKey)
{
	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	TCHAR	strFullPath[MAX_PATH] = {};

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	return LoadTextureFromFullPath(strName, strFullPath);
}

bool CTexture::LoadTextureFromFullPath(const string & strName,
	const TCHAR * pFullPath)
{
	SetTag(strName);

	TCHAR*	pSavePath = new TCHAR[MAX_PATH];
	memset(pSavePath, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pSavePath, pFullPath);

	m_vecFullPath.push_back(pSavePath);

	TCHAR	strExt[_MAX_EXT] = {};
	char	ext[_MAX_EXT] = {};

#ifdef UNICODE
	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt,
		_MAX_EXT);
	WideCharToMultiByte(CP_UTF8, 0, strExt, -1, ext, lstrlen(strExt), 0, 0);
#else
	_splitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, ext,
		_MAX_EXT);
#endif // UNICODE

	_strupr_s(ext);

	ScratchImage*	pImage = new ScratchImage;

	if (strcmp(ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
			*pImage)))
			return false;
	}

	else if (strcmp(ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			return false;
	}

	m_vecImage.push_back(pImage);

	return CreateShaderResource();
}

bool CTexture::LoadTexture(const string & strName,
	const vector<const TCHAR*>& vecFileName, const string & strPathKey)
{
	SetTag(strName);

	for (size_t i = 0; i < vecFileName.size(); ++i)
	{
		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

		TCHAR	strFullPath[MAX_PATH] = {};

		if (pPath)
			lstrcpy(strFullPath, pPath);

		lstrcat(strFullPath, vecFileName[i]);

		TCHAR*	pSavePath = new TCHAR[MAX_PATH];
		memset(pSavePath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pSavePath, strFullPath);

		m_vecFullPath.push_back(pSavePath);

		memcpy(m_strFullPath, strFullPath, sizeof(TCHAR) * MAX_PATH);

		TCHAR	strExt[_MAX_EXT] = {};
		char	ext[_MAX_EXT] = {};

#ifdef UNICODE
		_wsplitpath_s(vecFileName[i], nullptr, 0, nullptr, 0, nullptr, 0, strExt,
			_MAX_EXT);
		WideCharToMultiByte(CP_UTF8, 0, strExt, -1, ext, lstrlen(strExt), 0, 0);
#else
		_splitpath_s(vecFileName[i], nullptr, 0, nullptr, 0, nullptr, 0, ext,
			_MAX_EXT);
#endif // UNICODE

		_strupr_s(ext);

		ScratchImage*	pImage = new ScratchImage;

		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(m_strFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
				return false;
		}

		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(m_strFullPath, nullptr, *pImage)))
				return false;
		}

		else
		{
			if (FAILED(LoadFromWICFile(m_strFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
				return false;
		}

		m_vecImage.push_back(pImage);
	}

	return CreateShaderResourceArray();
}

bool CTexture::LoadTextureFromFullPath(const string & strName,
	const vector<const TCHAR*>& vecFullPath)
{
	SetTag(strName);

	for (size_t i = 0; i < vecFullPath.size(); ++i)
	{
		TCHAR*	pSavePath = new TCHAR[MAX_PATH];
		memset(pSavePath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pSavePath, vecFullPath[i]);

		m_vecFullPath.push_back(pSavePath);

		memcpy(m_strFullPath, vecFullPath[i], sizeof(TCHAR) * MAX_PATH);

		TCHAR	strExt[_MAX_EXT] = {};
		char	ext[_MAX_EXT] = {};

#ifdef UNICODE
		_wsplitpath_s(vecFullPath[i], nullptr, 0, nullptr, 0, nullptr, 0, strExt,
			_MAX_EXT);
		WideCharToMultiByte(CP_UTF8, 0, strExt, -1, ext, lstrlen(strExt), 0, 0);
#else
		_splitpath_s(vecFullPath[i], nullptr, 0, nullptr, 0, nullptr, 0, ext,
			_MAX_EXT);
#endif // UNICODE

		_strupr_s(ext);

		ScratchImage*	pImage = new ScratchImage;

		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(m_strFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
				return false;
		}

		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(m_strFullPath, nullptr, *pImage)))
				return false;
		}

		else
		{
			if (FAILED(LoadFromWICFile(m_strFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
				return false;
		}

		m_vecImage.push_back(pImage);
	}

	return CreateShaderResourceArray();
}

void CTexture::SetShader(int iRegister)
{
	CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
}

void CTexture::ResetShader(int iRegister)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
}

bool CTexture::CreateShaderResource()
{
	if (FAILED(CreateShaderResourceView(DEVICE, m_vecImage[0]->GetImages(),
		m_vecImage[0]->GetImageCount(),
		m_vecImage[0]->GetMetadata(), &m_pSRV)))
		return false;

	return true;
}

bool CTexture::CreateShaderResourceArray()
{
	vector<ID3D11Texture2D*>	vecTexture;

	for (size_t i = 0; i < m_vecImage.size(); ++i)
	{
		ID3D11Texture2D*	pTexture = NULL;

		if (FAILED(DirectX::CreateTextureEx(DEVICE, m_vecImage[i]->GetImages(),
			m_vecImage[i]->GetImageCount(),
			m_vecImage[i]->GetMetadata(),
			D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, FALSE,
			(ID3D11Resource**)&pTexture)))
		{
			assert(false);
			return false;
		}

		vecTexture.push_back(pTexture);
	}

	D3D11_TEXTURE2D_DESC	tTexDesc = {};
	vecTexture[0]->GetDesc(&tTexDesc);

	// Array Texture를 만들기 위한 Desc 구조체를 생성한다.
	D3D11_TEXTURE2D_DESC	tArrayDesc = {};
	tArrayDesc.Width = tTexDesc.Width;
	tArrayDesc.Height = tTexDesc.Height;
	tArrayDesc.MipLevels = tTexDesc.MipLevels;
	tArrayDesc.ArraySize = (UINT)vecTexture.size();
	tArrayDesc.Format = tTexDesc.Format;
	tArrayDesc.SampleDesc.Count = 1;
	tArrayDesc.SampleDesc.Quality = 0;
	tArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	tArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// 위에서 설정한 정보를 이용해서 비어있는 Array Texture를 생성한다.
	ID3D11Texture2D*	pArrayTexture = NULL;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&tArrayDesc, NULL, &pArrayTexture)))
	{
		assert(false);
		return false;
	}	

	// Array Texture의 픽셀정보를 불러온 텍스쳐의 픽셀정보로 채워준다.
	for (UINT i = 0; i < vecTexture.size(); ++i)
	{
		// 각각의 텍스쳐들을 밉맵 수준만큼 반복한다.
		for (UINT mip = 0; mip < tTexDesc.MipLevels; ++mip)
		{
			// 텍스처를 Map을 걸어준다.
			D3D11_MAPPED_SUBRESOURCE	tMap = {};
			CONTEXT->Map(vecTexture[i], mip, D3D11_MAP_READ, 0, &tMap);

			CONTEXT->UpdateSubresource(pArrayTexture,
				D3D11CalcSubresource(mip, i, tTexDesc.MipLevels),
				NULL, tMap.pData, tMap.RowPitch, tMap.DepthPitch);

			CONTEXT->Unmap(vecTexture[i], mip);
		}
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC	tViewDesc = {};
	tViewDesc.Format = tArrayDesc.Format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.MostDetailedMip = 0;
	tViewDesc.Texture2DArray.MipLevels = tArrayDesc.MipLevels;
	tViewDesc.Texture2DArray.FirstArraySlice = 0;
	tViewDesc.Texture2DArray.ArraySize = (UINT)vecTexture.size();

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(pArrayTexture, &tViewDesc, &m_pSRV)))
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pArrayTexture);

	Safe_Release_VecList(vecTexture);

	return true;
}
