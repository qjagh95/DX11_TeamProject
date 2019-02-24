#include "EngineHeader.h"
#include "SoundManager.h"
#include "Scene/Scene.h"
#include "Component/Transform.h"

PUN_USING
SINGLETON_VAR_INIT(CSoundManager)

shared_ptr<SoundEffect> CSoundManager::m_NULLPTR1;
shared_ptr<SoundEffectInstance> CSoundManager::m_NULLPTR2;

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{

}

bool CSoundManager::Init()
{
	AUDIO_ENGINE_FLAGS Flag = AudioEngine_Default;
#ifdef _DEBUG
	Flag = Flag | AudioEngine_Debug;
#endif
	m_AudioEngine = make_unique<AudioEngine>(Flag);

	//CreateSoundEffect("BGM", TEXT("bgm_Dnf.wav"));
	//CreateSoundEffect("Effect", TEXT("BugChange.wav"));

	return true;
}

void CSoundManager::Update()
{
	m_AudioEngine->Update();
}

void CSoundManager::AllStop()
{
	m_AudioEngine->Suspend();
}

void CSoundManager::Restart()
{
	m_AudioEngine->Resume();
}

void CSoundManager::CreateSoundEffect(const string & KeyName, const wstring & FileName, const string & PathKey)
{
	if (FindSoundEffect(KeyName) != m_NULLPTR1)
		return;

	const TCHAR* pPath = CPathManager::GetInst()->FindPath(PathKey);
	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;

	FullPath += FileName;

	unique_ptr<SoundEffect> newSoundEffect = make_unique<SoundEffect>(m_AudioEngine.get(), FullPath.c_str());
	m_SoundEffectMap.insert(make_pair(KeyName, move(newSoundEffect)));
}

void CSoundManager::CreateBGMList(const string & KeyName, unique_ptr<SoundEffectInstance> instance)
{
	m_SoundEffectInstanceMap.insert(make_pair(KeyName, move(instance)));
}

void CSoundManager::RemoveBGMList(const string & KeyName)
{
	unordered_map<string, shared_ptr<SoundEffectInstance>>::iterator FindIter = m_SoundEffectInstanceMap.find(KeyName);

	if (FindIter == m_SoundEffectInstanceMap.end())
		return;

	m_SoundEffectInstanceMap.erase(FindIter);
}

void CSoundManager::SoundPlay(const string & KeyName, SOUND_TYPE type, const Vector3& EmitterPos)
{
	switch (type)
	{
	case ST_EFFECT:
	{
		auto Sound = FindSoundEffect(KeyName);
		Sound->Play();
	}
	break;
	case ST_BGM:
	{
		auto Sound = FindSoundEffect(KeyName)->CreateInstance();
		Sound->Play(true);
		CreateBGMList(KeyName, move(Sound));
	}
	break;
	case ST_3D:
	{
		Vector3 cPos = CSceneManager::GetInst()->GetSceneNonCount()->GetMainCameraTransformNonCount()->GetWorldPos();
		XMFLOAT3 Convert1;
		Convert1.x = cPos.x;
		Convert1.y = cPos.y;
		Convert1.z = cPos.z;

		m_Listener.SetPosition(Convert1);

		XMFLOAT3 Convert2;
		Convert2.x = EmitterPos.x;
		Convert2.y = EmitterPos.y;
		Convert2.z = EmitterPos.z;

		m_Emitter.SetPosition(Convert2);

		auto Sound = FindSoundEffect(KeyName)->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);
		Sound->Play(false);
		Sound->Apply3D(m_Listener, m_Emitter);

		RemoveBGMList(KeyName);
		CreateBGMList(KeyName, move(Sound));
	}
	break;
	}
}

shared_ptr<SoundEffect> const & CSoundManager::FindSoundEffect(const string & KeyName)
{
	unordered_map<string, shared_ptr<SoundEffect>>::iterator FindIter = m_SoundEffectMap.find(KeyName);

	if (FindIter == m_SoundEffectMap.end())
		return m_NULLPTR1;

	return FindIter->second;
}

shared_ptr<SoundEffectInstance> const & CSoundManager::FindSoundEffectInstance(const string & KeyName)
{
	unordered_map<string, shared_ptr<SoundEffectInstance>>::iterator FindIter = m_SoundEffectInstanceMap.find(KeyName);

	if (FindIter == m_SoundEffectInstanceMap.end())
		return m_NULLPTR2;

	return FindIter->second;
}
