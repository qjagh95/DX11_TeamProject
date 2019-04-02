#pragma once

PUN_BEGIN

class PUN_DLL CPathManager
{
private:
	unordered_map<string, wstring>	m_mapPath;
	char	m_strPath[MAX_PATH] = {};

public:
	bool Init();
	bool AddPath(const string& strKey, const wstring& strPath,
		const string& strBaseKey = ROOT_PATH);
	const TCHAR* FindPath(const string& strKey);
	const char*	FindPathFromMultibyte(const string& strKey);

	DECLARE_SINGLE(CPathManager)
};

PUN_END