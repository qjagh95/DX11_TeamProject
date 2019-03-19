#pragma once
#include "../Ref.h"

PUN_BEGIN

class PUN_DLL CComputeShader :
	public CRef
{
	friend class CShaderManager;

private:
	CComputeShader();
	~CComputeShader();

private:
	ID3DBlob*	m_pCSBlob;
	ID3D11ComputeShader*	m_pCS;

public:
	bool LoadCShader(const string& strName, const TCHAR* pFileName,
		char* pEntry[ST_END], const string& strPathKey = SHADER_PATH);

public:
	void SetComputeShader();

public:
	bool LoadComputeShader(const TCHAR* pFileName,
		char* pEntry, const string& strPathKey = SHADER_PATH);
};

PUN_END