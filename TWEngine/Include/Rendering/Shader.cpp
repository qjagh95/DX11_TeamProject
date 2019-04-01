#include "EngineHeader.h"
#include "Shader.h"
#include "../Device.h"

PUN_USING

CShader::CShader() :
	m_pVSBlob(nullptr),
	m_pPSBlob(nullptr),
	m_pGSBlob(nullptr),
	m_pVS(nullptr),
	m_pPS(nullptr),
	m_pGS(nullptr)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pGSBlob)
	SAFE_RELEASE(m_pGS);
}

void * CShader::GetVSCode() const
{
	return m_pVSBlob->GetBufferPointer();
}

SIZE_T CShader::GetVSCodeSize() const
{
	return m_pVSBlob->GetBufferSize();
}

bool CShader::LoadShader(const string & strName,
	const TCHAR * pFileName, char * pEntry[SHADER_TYPE::ST_END],
	const string & strPathKey)
{
	m_strName = strName;
	SetTag(strName);

	if (!LoadVertexShader(pFileName, pEntry[ST_VERTEX],
		strPathKey))
		return false;

	if (!LoadPixelShader(pFileName, pEntry[ST_PIXEL],
		strPathKey))
		return false;

	if (pEntry[ST_GEOMETRY])
	{
		if (!LoadGeometryShader(pFileName, pEntry[ST_GEOMETRY],
			strPathKey))
			return false;
	}

	return true;
}

void CShader::SetShader()
{
	CONTEXT->VSSetShader(m_pVS, nullptr, 0);
	CONTEXT->PSSetShader(m_pPS, nullptr, 0);
	CONTEXT->GSSetShader(m_pGS, nullptr, 0);
}

bool CShader::LoadVertexShader(const TCHAR * pFileName, char * pEntry, const string & strPathKey)
{
	const TCHAR*	pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	DWORD	dwFlag = 0;

#ifdef _DEBUG
	dwFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	// HLSL 파일을 컴파일한다.
	ID3DBlob*	pError = nullptr;
	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "vs_5_0", dwFlag,
		0, &m_pVSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	// Blob에 컴파일된 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), nullptr, &m_pVS)))
		return false;

	return true;
}

bool CShader::LoadPixelShader(const TCHAR * pFileName, char * pEntry, const string & strPathKey)
{
	const TCHAR*	pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	DWORD	dwFlag = 0;

#ifdef _DEBUG
	dwFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	// HLSL 파일을 컴파일한다.
	ID3DBlob*	pError = nullptr;
	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "ps_5_0", dwFlag,
		0, &m_pPSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	// Blob에 컴파일된 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), nullptr, &m_pPS)))
		return false;

	return true;
}

bool CShader::LoadGeometryShader(const TCHAR * pFileName, char * pEntry, const string & strPathKey)
{
	const TCHAR*	pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	DWORD	dwFlag = 0;

#ifdef _DEBUG
	dwFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	// HLSL 파일을 컴파일한다.
	ID3DBlob*	pError = nullptr;
	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "gs_5_0", dwFlag,
		0, &m_pGSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	// Blob에 컴파일된 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(),
		m_pGSBlob->GetBufferSize(), nullptr, &m_pGS)))
		return false;

	return true;
}
