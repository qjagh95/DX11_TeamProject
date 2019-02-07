#include "EngineHeader.h"
#include "PathManager.h"

PUN_USING

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

bool CPathManager::Init()
{
	TCHAR	strPath[MAX_PATH] = {};

	GetModuleFileName(nullptr, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '\\' || strPath[i] == '/')
		{
			memset(strPath + (i + 1), 0, sizeof(TCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(DATA_PATH, TEXT("Data\\"));
	AddPath(MESH_PATH, L"Mesh\\");
	return true;
}

bool CPathManager::AddPath(const string & strKey, const wstring & strPath,
	const string & strBaseKey)
{
	if (FindPath(strKey))
		return false;

	const TCHAR* pPath = FindPath(strBaseKey);

	wstring	strFullPath;
	if (pPath)
		strFullPath = pPath;

	strFullPath += strPath;

	m_mapPath.insert(make_pair(strKey, strFullPath));

	return true;
}

const TCHAR * CPathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second.c_str();
}

const char * CPathManager::FindPathFromMultibyte(const string & strKey)
{
	const TCHAR* pPath = FindPath(strKey);

	if (!pPath)
		return nullptr;

	memset(m_strPath, 0, sizeof(char) * MAX_PATH);

	// 유니코드 문자열을 멀티바이트로 변환한다.
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pPath, -1, m_strPath, lstrlen(pPath) * 2,
		0, 0);
#else
	memcpy(m_strPath, pPath, sizeof(TCHAR) * lstrlen(pPath));
#endif	// UNICODE

	return m_strPath;
}