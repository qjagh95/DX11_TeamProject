#pragma once

static bool bDoNotLoadSndManager = false;

PUN_BEGIN

enum FLUSH_BGM_BUFFER
{
	FBB_NO_BUF = 0,
	FBB_BUF_1 = 1,
	FBB_BUF_2 = 2
};

class PUN_DLL CSoundManager
{
	//�ܹ� Sound Effect
	//���� Sound EffectInstance

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

	std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffect>>::iterator m_itrCurrBgm;
	std::string m_strCurrBGMName;
	std::string m_strPrevBGMName;
	std::shared_ptr<DirectX::SoundEffect> m_NoKeyBgmBuf;
	std::shared_ptr<DirectX::SoundEffect> m_NoKeyBgmBuf2;

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

	bool ForgetSound(const std::string strKey);
	
	Vector3 GetListenerPos() const;
	Vector3 GetListenerEulerRot() const;

	void SetListenerPos(const Vector3& pos);
	void SetListenerEulerRot(const Vector3& rot, const Vector3& upVec);
	void SetListenerPos(float x, float y, float z);

	void SetListenerQuat(const XMVECTOR& quat); //�������� ����� ȸ��(Quaternion)�� �Է��մϴ�
	void PlayBgm(const std::string& strKey, const wstring& wstrFileName,
		bool bEnableTransition = true,
		bool bDeleteFormerBgmFromMem = false,
		bool bLoop = true,
		const string& PathKey = SOUND_PATH); //���� bgm�� m_SoundEffectMap�� ���� ��� �ö�
	void PlayBgm(const std::string& strKey,
		bool bEnableTransition = true,
		bool bLoop = true,
		bool bDeleteFormerBgmFromMem = false); //�̹� m_SoundEffectMap�� �ö� �ִ� ���� �̿�
	void PlayBgm(const wstring& wstrFileName,
		bool bEnableTransition = true,
		bool bLoop = true,
		bool bDeleteFormerBgmFromMem = false, const string& PathKey = SOUND_PATH); //m_SoundEffectMap�� �ø��� ����

	void PauseBgm();

	void ResumeBgm();

	void StopBgm(bool bEnbleTrasition = true);

	void SetBgmVolume(float volume);
	void SetBgmPitch(float pitch);
	void SetBgmPan(float panRatio); //Pan : 2���� �Թ��� ������ ���� ���� ������ ����

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