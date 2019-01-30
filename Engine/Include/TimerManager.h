#pragma once

PUN_BEGIN

class PUN_DLL CTimerManager
{
private:
	unordered_map<string, class CTimer*>	m_mapTimer;

public:
	bool Init();
	bool CreateTimer(const string& strName);
	class CTimer*	FindTimer(const string& strKey);

	DECLARE_SINGLE(CTimerManager)
};

PUN_END