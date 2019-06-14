#pragma once

PUN_BEGIN

class PUN_DLL CStrUtility
{
public:
	const string TCHARToString(const TCHAR* _ptsz);
	const TCHAR* StringToTCHAR(const string& _str);
	vector<wstring> GetFilesInFolder(wstring _wstrPath, wstring _wstrType = L"*.*");

	DECLARE_SINGLE(CStrUtility)
};

PUN_END