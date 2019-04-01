#include "EngineHeader.h"
#include "SoundManager.h"
#include "Scene/Scene.h"
#include "Component/Transform.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"

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
	//Camera 찾아 업데이트 : 원래 Listener는 여러개일 수 있고,
	//프레임워크에서 제공하는 것이 맞는데 에디터랑 꼬이고 하는 등의 문제로 다른데서 언급 불가
	//어쩔수 없이 여기서 SceneManager를 불러서 MainCamera를 Listener로서 업데이트해준다

	PUN::CScene* pScene = PUN::CSceneManager::GetInst()->GetScene();

	if (!pScene)
		return;
	PUN::CCamera* pMainCam = pScene->GetMainCamera();

	if (pMainCam)
	{
		PUN::CTransform *pTrans = pMainCam->GetTransform();
		Vector3 vPos = pTrans->GetWorldPos();

		Vector3 vUp = pTrans->GetWorldAxis(PUN::AXIS_Y);
		Vector3 vFront = pTrans->GetWorldAxis(PUN::AXIS_Z);

		SAFE_RELEASE(pTrans);
		SAFE_RELEASE(pMainCam);

		DirectX::XMFLOAT3 mPos(vPos.x, vPos.y, vPos.z);
		m_Listener.SetPosition(mPos);

		m_Listener.SetOrientation(DirectX::XMFLOAT3(vFront.x, vFront.y, vFront.z),
			DirectX::XMFLOAT3(vUp.x, vUp.y, vUp.z));
	}

	SAFE_RELEASE(pScene);

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
		std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffectInstance>>::iterator itr;
			itr  = m_SoundEffectInstanceMap.find(KeyName);
		if (itr != m_SoundEffectInstanceMap.end())
		{
			itr->second->Play(true);
		}
		std::shared_ptr<DirectX::SoundEffectInstance> Sound = FindSoundEffect(KeyName)->CreateInstance();
		//std::unique_ptr을 반드시 std::shared_ptr로 바꿔줘야 map에 저장 가능
		m_SoundEffectInstanceMap.insert(std::make_pair(KeyName, Sound));
		Sound->Play(true);
	}
	break;

	}
}

const AudioListener & CSoundManager::GetMainListener() const
{
	// TODO: insert return statement here
	return m_Listener;
}

shared_ptr<SoundEffect> const & CSoundManager::FindSoundEffect(const string & KeyName)
{
	unordered_map<string, shared_ptr<SoundEffect>>::iterator FindIter = m_SoundEffectMap.find(KeyName);

	if (FindIter == m_SoundEffectMap.end())
		return m_NULLPTR1;

	return FindIter->second;
}

Vector3 CSoundManager::GetListenerPos() const
{
	// TODO: insert return statement here
	return Vector3(m_Listener.Position);
}

Vector3 CSoundManager::GetListenerEulerRot() const
{
	// TODO: insert return statement here
	return m_vRot;
}

void CSoundManager::SetListenerPos(const Vector3 & pos)
{
	DirectX::XMFLOAT3 xmFloat(pos.x, pos.y, pos.z);

	m_Listener.SetPosition(xmFloat);
}

void CSoundManager::SetListenerEulerRot(const Vector3 & rot, const Vector3& upVec)
{
	DirectX::XMFLOAT3 xmFloat(rot.x, rot.y, rot.z);
	DirectX::XMFLOAT3 xmUp(upVec.x, upVec.y, upVec.z);

	m_vRot = rot;
	m_Listener.SetOrientation(xmFloat, xmUp);

}

void CSoundManager::SetListenerPos(float x, float y, float z)
{
	DirectX::XMFLOAT3 xmFloat(x, y, z);
	m_Listener.SetPosition(xmFloat);
}

void CSoundManager::SetListenerQuat(const XMVECTOR & quat)
{
	m_vQuat = quat;
	m_Listener.SetOrientationFromQuaternion(quat);
	m_vRot = Vector3(m_Listener.OrientFront.x, m_Listener.OrientFront.y, m_Listener.OrientFront.z);
}




void CSoundManager::SetBgmVolume(const std::string & strKey, float volume)
{
	std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffectInstance>>::iterator itr;
	itr = m_SoundEffectInstanceMap.find(strKey);
	if (itr != m_SoundEffectInstanceMap.end())
	{
		itr->second->SetVolume(volume);
	}
	else
	{
		std::shared_ptr<DirectX::SoundEffectInstance> Sound = FindSoundEffect(strKey)->CreateInstance();
		m_SoundEffectInstanceMap.insert(std::make_pair(strKey, Sound));
		Sound->SetVolume(volume);
	}
}

void CSoundManager::SetBgmPitch(const std::string & strKey, float pitch)
{
	std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffectInstance>>::iterator itr;
	itr = m_SoundEffectInstanceMap.find(strKey);


	if (itr != m_SoundEffectInstanceMap.end())
	{
		itr->second->SetPitch(pitch);
	}
	else
	{
		std::shared_ptr<DirectX::SoundEffectInstance>  Sound = FindSoundEffect(strKey)->CreateInstance();
		m_SoundEffectInstanceMap.insert(std::make_pair(strKey, Sound));
		Sound->SetPitch(pitch);
	}
}

void CSoundManager::SetBgmPan(const std::string & strKey, float panRatio)
{
	std::unordered_map<std::string, std::shared_ptr<DirectX::SoundEffectInstance>>::iterator itr;
		itr = m_SoundEffectInstanceMap.find(strKey);
	if (itr != m_SoundEffectInstanceMap.end())
	{
		itr->second->SetPan(panRatio);
	}
	else
	{
		std::shared_ptr<DirectX::SoundEffectInstance> Sound = FindSoundEffect(strKey)->CreateInstance();
		m_SoundEffectInstanceMap.insert(std::make_pair(strKey, Sound));
		Sound->SetPan(panRatio);
	}
}


