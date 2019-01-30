#include "EngineHeader.h"
#include "SoundManager.h"
#include "Scene/Scene.h"

PUN_USING

DEFINITION_SINGLE(CSoundManager)

CSoundManager::CSoundManager()
{
	memset(m_pChannel, 0, sizeof(Channel*) * 2);
}


CSoundManager::~CSoundManager()
{
	unordered_map<string, PSOUNDINFO>::iterator	iter;
	unordered_map<string, PSOUNDINFO>::iterator	iterEnd = m_mapSound.end();

	for (iter = m_mapSound.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}

	m_pSystem->close();
	m_pSystem->release();
}

bool CSoundManager::Init()
{
	System_Create(&m_pSystem);

	m_pSystem->init(20, FMOD_INIT_NORMAL, nullptr);

	return true;
}

bool CSoundManager::LoadSound(CScene* pScene, bool bLoop,
	const char * pFileName, const string & strPathKey)
{
	PSOUNDINFO	pSound = FindSound(pFileName);

	if (pSound)
		return false;

	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	pSound = new SOUNDINFO;

	pSound->pScene = pScene;
	pSound->bLoop = bLoop;

	FMOD_MODE	eMode = FMOD_LOOP_NORMAL;

	if (!bLoop)
		eMode = FMOD_DEFAULT;

	m_pSystem->createSound(strFullPath.c_str(), eMode, NULL, &pSound->pSound);

	m_mapSound.insert(make_pair(pFileName, pSound));

	return true;
}

bool CSoundManager::LoadSound(const string & strKey, bool bLoop, const TCHAR * pFileName, const string & strPathKey)
{
	PSOUNDINFO pSound = FindSound(strKey);

	if (pSound)
		return false;

	const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string strFullPath;

	if (pPath)
		strFullPath = pPath;

	char strFileName[MAX_PATH] = {};

#ifdef _WIN64
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName), 0, 0);
#else
	strcpy_s(strFileName, pFileName);
#endif
	strFullPath += strFileName;

	pSound = new SOUNDINFO;

	pSound->bLoop = bLoop;

	FMOD_MODE eMode = FMOD_LOOP_NORMAL;

	if (!bLoop)
		eMode = FMOD_DEFAULT;

	m_pSystem->createSound(strFullPath.c_str(), eMode, NULL, &pSound->pSound);

	m_mapSound.insert(make_pair(strKey, pSound));

	return true;
}

void CSoundManager::Play(const string & strKey, bool bBGM)
{
	PSOUNDINFO pSound = FindSound(strKey);

	if (!pSound)
		return;

	/*if (bBGM)
	{
		m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[0]);
	}*/

	if (bBGM)
	{
		m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[0]);
		pSound->iChannel = 0;
	}


	else
	{
		int iChannel = -1;

		for (int i = 0; i < 20; ++i)
		{
			if (!m_bChannel[i])
			{
				iChannel = i + 1;
				m_bChannel[i] = true;
				break;
			}
		}

		if (iChannel == -1)
			iChannel = 1;

		m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[iChannel]);

		pSound->iChannel = iChannel;
	}
}

void CSoundManager::Stop(const string & strKey, bool bBGM)
{
	PSOUNDINFO pSound = FindSound(strKey);
	if (!pSound)
		return;

	m_pChannel[pSound->iChannel]->stop();

	m_bChannel[pSound->iChannel - 1] = false;

	m_pChannel[pSound->iChannel] = nullptr;
}

void CSoundManager::Pause(const string & strKey, bool bBGM)
{
	PSOUNDINFO	pSound = FindSound(strKey);

	if (!pSound)
		return;

	if (bBGM)
	{
		m_pChannel[0]->setPaused(false);
	}

	else
	{
		m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[1]);
	}
}

void CSoundManager::Pause()
{
	ChannelGroup*	pGroup;
	m_pSystem->getMasterChannelGroup(&pGroup);

	pGroup->setPaused(true);
}

void CSoundManager::Resume()
{
	ChannelGroup*	pGroup;
	m_pSystem->getMasterChannelGroup(&pGroup);

	pGroup->setPaused(false);
}

void CSoundManager::SetMasterVolume(float fVolume)
{
	ChannelGroup*	pGroup;
	m_pSystem->getMasterChannelGroup(&pGroup);

	pGroup->setVolume(fVolume);
}

void CSoundManager::SetVolume(float fVolume, bool bBGM)
{
	if (bBGM)
		m_pChannel[0]->setVolume(fVolume);

	else
		m_pChannel[1]->setVolume(fVolume);
}

void CSoundManager::DeleteSound()
{
	unordered_map<string, PSOUNDINFO>::iterator	iter;
	unordered_map<string, PSOUNDINFO>::iterator	iterEnd = m_mapSound.end();

	for (iter = m_mapSound.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}

	m_mapSound.clear();
}

void CSoundManager::DeleteSound(CScene * pScene)
{
	unordered_map<string, PSOUNDINFO>::iterator	iter;
	unordered_map<string, PSOUNDINFO>::iterator	iterEnd = m_mapSound.end();

	for (iter = m_mapSound.begin(); iter != iterEnd;)
	{
		if (iter->second->pScene == pScene)
		{
			iter->second->pSound->release();
			SAFE_DELETE(iter->second);
			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();
		}

		else
			++iter;
	}

	for (int i = 0; i < 2; ++i)
	{
		if (m_pChannel[i])
			m_pChannel[i]->stop();
	}
}

void CSoundManager::DeleteSound(const char * pFileName)
{
	unordered_map<string, PSOUNDINFO>::iterator	iter = m_mapSound.find(pFileName);

	if (iter == m_mapSound.end())
		return;

	iter->second->pSound->release();
	SAFE_DELETE(iter->second);
	m_mapSound.erase(iter);
}

void CSoundManager::Update()
{
	for (int i = 1; i < 20; ++i)
	{
		if (m_bChannel[i - 1])
		{
			bool bPlay = false;
			m_pChannel[i]->isPlaying(&bPlay);

			if (!bPlay)
			{
				m_pChannel[i] = nullptr;
				m_bChannel[i - 1] = false;
			}
		}
	}
}

CSoundManager::PSOUNDINFO CSoundManager::FindSound(const string & strKey)
{
	unordered_map<string, PSOUNDINFO>::iterator	iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}
