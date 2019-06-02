#include "EngineHeader.h"
#include "StrUtility.h"

PUN_USING

DEFINITION_SINGLE(CStrUtility)

CStrUtility::CStrUtility()
{
}


CStrUtility::~CStrUtility()
{
}

const string CStrUtility::TCHARToString(const TCHAR* _ptsz)
{
	// TCHAR -> string º¯È¯
	int len = (int)wcslen((wchar_t*)_ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)_ptsz, 2 * len + 1);
	string cvtStr = psz;
	delete[] psz;
	return cvtStr;
}

const TCHAR* CStrUtility::StringToTCHAR(const string& _str)
{
	const char* all = _str.c_str();
	int len = (int)(1 + strlen(all));
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

vector<wstring> CStrUtility::GetFilesInFolder(wstring _wstrPath, wstring _wstrType)
{
	vector<wstring> vecNames;
	wchar_t searchPath[200];
	wsprintf(searchPath, L"%s%s", _wstrPath.c_str(), _wstrType.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				vecNames.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return vecNames;
}