#include "../EngineHeader.h"
#include "ComputeShader.h"
#include "../Device.h"
#include "../PathManager.h"

PUN_USING

CComputeShader::CComputeShader()	:
	m_pCSBlob(nullptr),
	m_pCS(nullptr)
{
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pCS);
}

bool CComputeShader::LoadCShader(const string & strName, 
	const TCHAR * pFileName, char * pEntry[ST_END], const string & strPathKey)
{
	if (pEntry[ST_COMPUTE])
	{
		if (!LoadComputeShader(pFileName, pEntry[ST_COMPUTE],
			strPathKey))
			return false;
	}

	return true;
}

void CComputeShader::SetComputeShader()
{
	CONTEXT->CSSetShader(m_pCS, nullptr, 0);
}

bool CComputeShader::LoadComputeShader(const TCHAR * pFileName, char * pEntry, 
	const string & strPathKey)
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
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "cs_5_0", dwFlag,
		0, &m_pCSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	// Blob에 컴파일된 코드를 이용해서 ComputeShader 객체를 만들어낸다.
	if (FAILED(DEVICE->CreateComputeShader(m_pCSBlob->GetBufferPointer(),
		m_pCSBlob->GetBufferSize(), nullptr, &m_pCS)))
		return false;

	return true;
}
