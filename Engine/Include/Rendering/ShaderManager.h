#pragma once

PUN_BEGIN

class PUN_DLL CShaderManager
{
private:
	unordered_map<string, class CShader*>	m_mapShader;
	unordered_map<string, ID3D11InputLayout*>	m_mapInputLayout;
	vector<D3D11_INPUT_ELEMENT_DESC>			m_vecInputDesc;
	int		m_iInputSize;
	unordered_map<string, PCBuffer>	m_mapCBuffer;

public:
	bool Init();
	bool LoadShader(const string& strName, const TCHAR* pFileName,
		char* pEntry[ST_END], const string& strPathKey = SHADER_PATH);
	class CShader* FindShader(const string& strName);
	class CShader * FindShaderNonCount(const string & strName);
	void AddInputElement(char* pSemantic, int iIdx, DXGI_FORMAT eFmt,
		int iSize, int iInputSlot = 0, D3D11_INPUT_CLASSIFICATION eInputClass = D3D11_INPUT_PER_VERTEX_DATA,
		int iInstanceStepRate = 0);
	bool CreateInputLayout(const string& strName, const string& strShaderKey);
	ID3D11InputLayout* FindInputLayout(const string& strName);
	bool CreateCBuffer(const string& strName, int iRegister, int iSize,
		int iShaderType);
	bool UpdateCBuffer(const string& strName, void* pData);

private:
	PCBuffer FindCBuffer(const string& strName);

	DECLARE_SINGLE(CShaderManager)
};

PUN_END
