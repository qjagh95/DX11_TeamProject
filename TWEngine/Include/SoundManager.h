#pragma once

/*
enum AUDIO_ENGINE_REVERB
	{
		Reverb_Off,
		Reverb_Default,
		Reverb_Generic,
		Reverb_Forest,
		Reverb_PaddedCell,
		Reverb_Room,
		Reverb_Bathroom,
		Reverb_LivingRoom,
		Reverb_StoneRoom,
		Reverb_Auditorium,
		Reverb_ConcertHall,
		Reverb_Cave,
		Reverb_Arena,
		Reverb_Hangar,
		Reverb_CarpetedHallway,
		Reverb_Hallway,
		Reverb_StoneCorridor,
		Reverb_Alley,
		Reverb_City,
		Reverb_Mountains,
		Reverb_Quarry,
		Reverb_Plain,
		Reverb_ParkingLot,
		Reverb_SewerPipe,
		Reverb_Underwater,
		Reverb_SmallRoom,
		Reverb_MediumRoom,
		Reverb_LargeRoom,
		Reverb_MediumHall,
		Reverb_LargeHall,
		Reverb_Plate,
		Reverb_MAX
	};
*/

PUN_BEGIN

enum FLUSH_BGM_BUFFER
{
	FBB_NO_BUF = 0,
	FBB_BUF_1 = 1,
	FBB_BUF_2 = 2
};

class PUN_DLL CSoundManager
{
	//단발 Sound Effect
	//루프 Sound EffectInstance
	
public:
	bool Init();
	void Update(float fTime);
	void AllStop();
	void Restart();

	void CreateSoundEffect(const string& KeyName, const wstring& FileName, const string& PathKey = SOUND_PATH);
	void SoundPlay(const string& KeyName, float fVolume = 1.f, float fPitch = 0.f, float fPan = 0.f); //Raw Sound Memory Play
	void SoundPlayAsUISound(const string& KeyName);
	const AudioListener& GetMainListener() const;

	shared_ptr<SoundEffect> const& FindSoundEffect(const string& KeyName);

private:
	float m_fAudioCoordSizeDiv;
	X3DAUDIO_CONE m_tSoundCone;

	std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffect>>::iterator m_itrCurrBgm;
	std::string m_strCurrBGMName;
	std::string m_strPrevBGMName;
	std::shared_ptr<DirectX::SoundEffect> m_NoKeyBgmBuf;
	std::shared_ptr<DirectX::SoundEffect> m_NoKeyBgmBuf2;
	class CTransform *m_pListenerTransform;

	unique_ptr<AudioEngine> m_AudioEngine;
	unordered_map<string, shared_ptr<SoundEffect>> m_SoundEffectMap;
	unordered_map<string, shared_ptr<SoundEffectInstance>> m_SoundEffectInstanceMap;

	AudioListener m_Listener;
	AudioEmitter m_Emitter;
	
	XMVECTOR m_vQuat;
	Vector3 m_vRot;

	static shared_ptr<SoundEffect> m_NULLPTR1;
	static shared_ptr<SoundEffectInstance> m_NULLPTR2;
	int m_iFlushFlag;

	shared_ptr<DirectX::SoundEffectInstance> m_sPtrCurrentBgmTrack;
	shared_ptr<DirectX::SoundEffectInstance> m_sPtrPrevBgmTrack;

	bool m_bOnTransition;

	float m_fBgmTransitionTime;
	float m_fCurrTransitionTimer;
	float m_fBgmVolume;
	float m_fBgmPitch;
	float m_fBgmPan;

	float m_fUIVolume;
	float m_fUIPitch;
	float m_fUIPan;

public:
	void SetListenerTransform(class CTransform* pTrans);
	float GetAudioCoordSize() const;
	void SetAudioCoordSize(float fSize);
	bool ForgetSound(const std::string strKey);
	bool GetIsPlayingBGM() const { return m_sPtrCurrentBgmTrack->GetState() == PLAYING; }
	
	void PlayBgm(const std::string& strKey, const wstring& wstrFileName,
		bool bEnableTransition = true,
		bool bLoop = true,
		bool bDeleteFormerBgmFromMem = false,
		const string& PathKey = SOUND_PATH); //현재 bgm이 m_SoundEffectMap에 없을 경우 올라감
	void PlayBgm(const std::string& strKey,
		bool bEnableTransition = true,
		bool bLoop = true,
		bool bDeleteFormerBgmFromMem = false); //이미 m_SoundEffectMap에 올라가 있는 사운드 이용
	void PlayBgm(const wstring& wstrFileName,
		bool bEnableTransition = true,
		bool bLoop = true,
		bool bDeleteFormerBgmFromMem = false, const string& PathKey = SOUND_PATH); //m_SoundEffectMap에 올리지 않음

	void PauseBgm();

	void ResumeBgm();

	void StopBgm(bool bEnbleTrasition = true);

	void SetBgmVolume(float volume);
	void SetBgmPitch(float pitch);
	void SetBgmPan(float panRatio); //Pan : 2차원 밋밋한 사운드의 양쪽 볼륨 비율을 조정

	float GetBgmVolume() const;
	float GetBgmPitch() const;
	float GetBgmPan() const;

	float GetUIVolume() const;
	float GetUIPitch() const;
	float GetUIPan() const;

	void SetUIVolume(float vol);
	void SetUIPitch(float pitch);
	void SetUIPan(float pan);

	void SetTransitionTime(float fTransitionTime);
	float GetTransitionTime() const;
	float GetCurrentTransitionRate() const;

private:
	CLASS_IN_SINGLE(CSoundManager)
};

PUN_END