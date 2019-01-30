#pragma once
#include "fmod.hpp"

using namespace FMOD;

#ifdef _WIN64
#pragma comment(lib, "fmod64_vc")
#else
#pragma comment(lib, "fmod_vc")
#endif

PUN_BEGIN

class PUN_DLL CSoundManager
{
private:
	typedef struct _tagSoundInfo
	{
		Sound*	pSound;
		bool	bLoop;
		class CScene*	pScene;
		int		iChannel;
	}SOUNDINFO, *PSOUNDINFO;

private:
	System *	m_pSystem;
	Channel*	m_pChannel[20];
	bool		m_bChannel[19];
	unordered_map<string, PSOUNDINFO>	m_mapSound;

public:
	bool Init();
	bool LoadSound(class CScene* pScene, bool bLoop, const char* pFileName,
		const string& strPathKey = SOUND_PATH);
	bool LoadSound(const string& strKey, bool bLoop, const TCHAR* pFileName,
		const string& strPathKey = SOUND_PATH);
	void Play(const string& strKey, bool bBGM = false);
	void Stop(const string& strKey, bool bBGM = false);
	void Pause(const string& strKey, bool bBGM = false);
	void Pause();
	void Resume();
	void SetMasterVolume(float fVolume);
	void SetVolume(float fVolume, bool bBGM = false);
	void DeleteSound();
	void DeleteSound(class CScene* pScene);
	void DeleteSound(const char* pFileName);
	void Update();

private:
	PSOUNDINFO FindSound(const string& strKey);

	DECLARE_SINGLE(CSoundManager)
};

PUN_END