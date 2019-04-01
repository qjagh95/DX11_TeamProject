#pragma once
#include "../Ref.h"

PUN_BEGIN

class PUN_DLL CShader :
	public CRef
{
	friend class CShaderManager;

private:
	CShader();
	~CShader();

private:
	ID3DBlob*	m_pVSBlob;
	ID3D11VertexShader*	m_pVS;
	ID3DBlob*	m_pPSBlob;
	ID3D11PixelShader*	m_pPS;
	ID3DBlob*	m_pGSBlob;
	ID3D11GeometryShader*	m_pGS;
	string		m_strName;

public:
	void* GetVSCode()	const;
	SIZE_T GetVSCodeSize()	const;

public:
	bool LoadShader(const string& strName, const TCHAR* pFileName,
		char* pEntry[ST_END], const string& strPathKey = SHADER_PATH);
	void SetShader();

private:
	bool LoadVertexShader(const TCHAR* pFileName,
		char* pEntry, const string& strPathKey = SHADER_PATH);
	bool LoadPixelShader(const TCHAR* pFileName,
		char* pEntry, const string& strPathKey = SHADER_PATH);
	bool LoadGeometryShader(const TCHAR* pFileName,
		char* pEntry, const string& strPathKey = SHADER_PATH);
};

PUN_END
