#include "EngineHeader.h"
#include "TimerManager.h"
#include "Timer.h"

PUN_USING

DEFINITION_SINGLE(CTimerManager)

CTimerManager::CTimerManager()
{
}

CTimerManager::~CTimerManager()
{
	unordered_map<string, CTimer*>::iterator	iter;
	unordered_map<string, CTimer*>::iterator	iterEnd = m_mapTimer.end();

	for (iter = m_mapTimer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapTimer.clear();
}

bool CTimerManager::Init()
{
	CreateTimer("MainTimer");

	return true;
}

bool CTimerManager::CreateTimer(const string & strName)
{
	CTimer*	pTimer = FindTimer(strName);

	if (pTimer)
		return false;

	pTimer = new CTimer;

	pTimer->m_strName = strName;

	pTimer->Init();

	m_mapTimer.insert(make_pair(strName, pTimer));

	return true;
}

CTimer * CTimerManager::FindTimer(const string & strKey)
{
	unordered_map<string, CTimer*>::iterator	iter = m_mapTimer.find(strKey);

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}
