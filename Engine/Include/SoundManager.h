#pragma once

static bool bDoNotLoadSndManager = false;

PUN_BEGIN

class PUN_DLL CSoundManager
{
	//단발 Sound Effect
	//루프 Sound EffectInstance

public:
	bool Init();
	void Update();
	void AllStop();
	void Restart();

	void CreateSoundEffect(const string& KeyName, const wstring& FileName, const string& PathKey = SOUND_PATH);
	void SoundPlay(const string& KeyName, SOUND_TYPE type, const Vector3& EmitterPos = Vector3::Zero);

	const AudioListener& GetMainListener() const;

	shared_ptr<SoundEffect> const& FindSoundEffect(const string& KeyName);

private:

	unique_ptr<AudioEngine> m_AudioEngine;
	unordered_map<string, shared_ptr<SoundEffect>> m_SoundEffectMap;
	unordered_map<string, shared_ptr<SoundEffectInstance>> m_SoundEffectInstanceMap;

	AudioListener m_Listener;
	AudioEmitter m_Emitter;
	SOUND_TYPE m_State;

	XMVECTOR m_vQuat;
	Vector3 m_vRot;

	static shared_ptr<SoundEffect> m_NULLPTR1;
	static shared_ptr<SoundEffectInstance> m_NULLPTR2;

public:

	Vector3 GetListenerPos() const;
	Vector3 GetListenerEulerRot() const;

	void SetListenerPos(const Vector3& pos);
	void SetListenerEulerRot(const Vector3& rot, const Vector3& upVec);
	void SetListenerPos(float x, float y, float z);

	void SetListenerQuat(const XMVECTOR& quat); //리스너의 사원수 회전(Quaternion)을 입력합니다
	void SetBgmVolume(const std::string& strKey, float volume);
	void SetBgmPitch(const std::string& strKey, float pitch);
	void SetBgmPan(const std::string& strKey, float panRatio); //Pan : 2차원 밋밋한 사운드의 양쪽 볼륨 비율을 조정

private:
	CLASS_IN_SINGLE(CSoundManager)
};

PUN_END