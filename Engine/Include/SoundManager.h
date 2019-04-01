#pragma once

static bool bDoNotLoadSndManager = false;

PUN_BEGIN

class PUN_DLL CSoundManager
{
	//�ܹ� Sound Effect
	//���� Sound EffectInstance

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

	void SetListenerQuat(const XMVECTOR& quat); //�������� ����� ȸ��(Quaternion)�� �Է��մϴ�
	void SetBgmVolume(const std::string& strKey, float volume);
	void SetBgmPitch(const std::string& strKey, float pitch);
	void SetBgmPan(const std::string& strKey, float panRatio); //Pan : 2���� �Թ��� ������ ���� ���� ������ ����

private:
	CLASS_IN_SINGLE(CSoundManager)
};

PUN_END