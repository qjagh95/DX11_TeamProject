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


CSoundManager::CSoundManager() :
	m_fBgmPan(0.f),
	m_fBgmPitch(0.f),
	m_fBgmVolume(1.f),
	m_fBgmTransitionTime(1.2f),
	m_fCurrTransitionTimer(0.f),
	m_bOnTransition(false),
	m_iFlushFlag((int)FBB_NO_BUF),
	m_fUIVolume(1.f),
	m_fUIPan(0.f),
	m_fUIPitch(0.f),
	m_fAudioCoordSizeDiv(1.f),
	m_pListenerTransform(nullptr)
{
	m_sPtrCurrentBgmTrack = m_NULLPTR2;
	m_sPtrPrevBgmTrack = m_NULLPTR2;
	m_NoKeyBgmBuf = m_NULLPTR1;
	m_NoKeyBgmBuf2 = m_NULLPTR1;
}

CSoundManager::~CSoundManager()
{
	SAFE_RELEASE(m_pListenerTransform);
}

bool CSoundManager::Init()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS Flag = AudioEngine_Default;
#ifdef _DEBUG
	Flag = Flag | AudioEngine_Debug;
#endif
	m_AudioEngine = make_unique<AudioEngine>(Flag);
	m_AudioEngine->SetReverb(Reverb_Default);
	m_AudioEngine->GetChannelMask();
	/*
	m_tSoundCone.InnerAngle = X3DAUDIO_PI;
	m_tSoundCone.InnerReverb = 1.f;
	m_tSoundCone.InnerLPF = 0.75f;
	m_tSoundCone.InnerVolume = 2.f;
	m_tSoundCone.OuterAngle = X3DAUDIO_2PI;
	m_tSoundCone.OuterLPF = 0.25f;
	m_tSoundCone.OuterReverb = 0.5f;
	m_tSoundCone.OuterVolume = 1.f;
	m_Listener.pCone = &m_tSoundCone;
	*/
	

	//CreateSoundEffect("BGM", TEXT("bgm_Dnf.wav"));
	//CreateSoundEffect("Effect", TEXT("BugChange.wav"));

	return true;
}

void CSoundManager::Update(float fTime)
{
	//Camera 찾아 업데이트 : 원래 Listener는 여러개일 수 있고,
	//프레임워크에서 제공하는 것이 맞는데 에디터랑 꼬이고 하는 등의 문제로 다른데서 언급 불가
	//어쩔수 없이 여기서 SceneManager를 불러서 MainCamera를 Listener로서 업데이트해준다
		
	if (m_bOnTransition)
	{
		if (m_fBgmTransitionTime > 0.f)
		{
			if (m_fBgmTransitionTime >= m_fCurrTransitionTimer)
			{
				
				float fTransitVolInv = m_fCurrTransitionTimer / m_fBgmTransitionTime;
				if (m_sPtrPrevBgmTrack != m_NULLPTR2)
				{
					float fTransitVol = 1.f - fTransitVolInv;
					if (fTransitVol >= 0.f)
					{
						m_sPtrPrevBgmTrack->SetVolume(fTransitVol * m_fBgmVolume);
					}
					
				}

				if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
				{
					
					if (fTransitVolInv >= 0.f)
					{
						m_sPtrCurrentBgmTrack->SetVolume(fTransitVolInv * m_fBgmVolume);
					}
										
				}

				m_fCurrTransitionTimer += fTime;
			}
			else
			{
				if (m_sPtrPrevBgmTrack != m_NULLPTR2)
				{
					m_sPtrPrevBgmTrack->Stop();
					m_sPtrPrevBgmTrack = m_NULLPTR2;
					if(m_strPrevBGMName.size() > 0)
						ForgetSound(m_strPrevBGMName);
					m_strPrevBGMName.clear();
				}

				//Flush Buffer
				if (!(m_iFlushFlag &= 0x111))
				{
					m_NoKeyBgmBuf = m_NULLPTR1;
				}
				if (!(m_iFlushFlag &= 0x110))
				{
					m_NoKeyBgmBuf2 = m_NULLPTR1;
				}

				m_fCurrTransitionTimer = 0.f;
				m_sPtrCurrentBgmTrack->SetVolume(m_fBgmVolume);
				m_bOnTransition = false;
			}
		}
		else
		{
			if (m_sPtrPrevBgmTrack != m_NULLPTR2)
			{
				m_sPtrPrevBgmTrack->Stop();
				m_sPtrPrevBgmTrack = m_NULLPTR2;
				if (m_strPrevBGMName.size() > 0)
					ForgetSound(m_strPrevBGMName);
				
				m_strPrevBGMName.clear();
			}

			//Flush Buffer
			if (!(m_iFlushFlag &= 0x111))
			{
				m_NoKeyBgmBuf = m_NULLPTR1;
			}
			if (!(m_iFlushFlag &= 0x110))
			{
				m_NoKeyBgmBuf2 = m_NULLPTR1;
			}

			m_sPtrCurrentBgmTrack->SetVolume(m_fBgmVolume);
			m_bOnTransition = false;
		}
		
	}

	if (m_pListenerTransform)
	{
	
		Vector3 vPos = m_pListenerTransform->GetWorldPos();

		vPos /= m_fAudioCoordSizeDiv;

		Vector3 vUp = m_pListenerTransform->GetWorldAxis(PUN::AXIS_Y);
		Vector3 vFront = m_pListenerTransform->GetWorldAxis(PUN::AXIS_Z);

		DirectX::XMFLOAT3 mPos(vPos.x, vPos.y, vPos.z);
		m_Listener.SetPosition(mPos);

		m_Listener.SetOrientation(DirectX::XMFLOAT3(vFront.x, vFront.y, vFront.z),
			DirectX::XMFLOAT3(vUp.x, vUp.y, vUp.z));
	}

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

void CSoundManager::SoundPlay(const string & KeyName
,float fVolume , float fPitch , float fPan)
{
	/*
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
	*/
	auto Sound = FindSoundEffect(KeyName);
	Sound->Play(fVolume, fPitch, fPan);
}

void CSoundManager::SoundPlayAsUISound(const string & KeyName)
{
	auto Sound = FindSoundEffect(KeyName);
	Sound->Play(m_fUIVolume, m_fUIPitch, m_fUIPan);
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

void CSoundManager::SetListenerTransform(CTransform * pTrans)
{
	SAFE_RELEASE(m_pListenerTransform);
	m_pListenerTransform = pTrans;
	if(pTrans)
		pTrans->AddRef();
}

float CSoundManager::GetAudioCoordSize() const
{
	return m_fAudioCoordSizeDiv;
}

void CSoundManager::SetAudioCoordSize(float fSize)
{
	if (fSize == 0.f)
		return;

	if (isnan(fSize))
		return;

	if (isinf(fSize))
		return;

	m_fAudioCoordSizeDiv = fSize;
}

bool CSoundManager::ForgetSound(const std::string strKey)
{
	auto _snd = m_SoundEffectMap.find(strKey);

	if (_snd == m_SoundEffectMap.end())
		return false;

	m_SoundEffectMap.erase(_snd);

	return true;
}

void CSoundManager::PlayBgm(const std::string & strKey, const wstring & wstrFileName,
	bool bEnableTransition, bool bLoop,
	bool bDeleteFormerBgmFromMem,
	const string& PathKey)
{
	if (strKey == m_strCurrBGMName)
	{
		if(m_sPtrCurrentBgmTrack->GetState() == PLAYING)
			return;
		m_sPtrCurrentBgmTrack->Play(bLoop);
		return;
	}
		

	m_iFlushFlag = 0x000;
	auto _bgm = FindSoundEffect(strKey);
	if (_bgm == m_NULLPTR1)
	{
		const TCHAR* pPath = CPathManager::GetInst()->FindPath(PathKey);
		wstring	FullPath;

		if (pPath != NULLPTR)
			FullPath = pPath;

		FullPath += wstrFileName;

		std::unique_ptr<SoundEffect> _newSound = make_unique<SoundEffect>(m_AudioEngine.get(), FullPath.c_str());
		_bgm = move(_newSound);
		m_SoundEffectMap.insert(make_pair(strKey, _bgm));
	}
		
	m_sPtrPrevBgmTrack = m_sPtrCurrentBgmTrack;

	m_sPtrCurrentBgmTrack = _bgm->CreateInstance();
	

	if (bEnableTransition)
	{
		m_sPtrCurrentBgmTrack->SetVolume(0.f);
		m_strPrevBGMName = m_strCurrBGMName;
		m_bOnTransition = bEnableTransition;
	}
	else
	{
		m_sPtrCurrentBgmTrack->SetVolume(m_fBgmVolume);
		if (bDeleteFormerBgmFromMem)
		{
			if (m_strPrevBGMName.size() > 0)
				ForgetSound(m_strPrevBGMName);

		}
	}
	
	
	m_sPtrCurrentBgmTrack->SetPitch(m_fBgmPitch);
	m_sPtrCurrentBgmTrack->SetPan(m_fBgmPan);
	m_sPtrCurrentBgmTrack->Play(bLoop);
	m_strCurrBGMName = strKey;
	
}

void CSoundManager::PlayBgm(const std::string & strKey, bool bEnableTransition, bool bLoop,
	bool bDeleteFormerBgmFromMem)
{
	m_iFlushFlag = 0x000;
	if (strKey == m_strCurrBGMName)
		return;
	auto _bgm = FindSoundEffect(strKey);
	if (_bgm == m_NULLPTR1)
	{
		StopBgm(bEnableTransition);
	}


	m_sPtrPrevBgmTrack = m_sPtrCurrentBgmTrack;

	m_sPtrCurrentBgmTrack = _bgm->CreateInstance();
	

	if (bEnableTransition)
	{
		m_sPtrCurrentBgmTrack->SetVolume(0.f);
		if (m_sPtrPrevBgmTrack != m_NULLPTR2)
		{
			if(bDeleteFormerBgmFromMem)
				m_strPrevBGMName = m_strCurrBGMName;

		}
		m_bOnTransition = bEnableTransition;
	}
	else
	{
		m_sPtrCurrentBgmTrack->SetVolume(m_fBgmVolume);
		if (bDeleteFormerBgmFromMem)
		{
			if (m_strPrevBGMName.size() > 0)
				ForgetSound(m_strPrevBGMName);
		}
	}

	m_sPtrCurrentBgmTrack->SetPitch(m_fBgmPitch);
	m_sPtrCurrentBgmTrack->SetPan(m_fBgmPan);
	m_sPtrCurrentBgmTrack->Play(bLoop);

	m_strCurrBGMName = strKey;

}

void CSoundManager::PlayBgm(const wstring & wstrFileName, bool bEnableTransition,
	bool bLoop,
	bool bDeleteFormerBgmFromMem, const string& PathKey)
{
	const TCHAR* pPath = CPathManager::GetInst()->FindPath(PathKey);
	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;

	FullPath += wstrFileName;
	auto _bgm = make_unique<SoundEffect>(m_AudioEngine.get(), FullPath.c_str());
	//만약 m_NoKeyBgmBuf 가 이미 차 있으면 스마트 포인터는 원래 있던 버퍼의 음악을 지워버린다.
	//이것은 종료할 때 에러를 남기며 트랜지션이 적용될 경우 이전 음악이 삭제되는 듯한 느낌을 주게 하므로
	//m_NoKeyBuf2를 맨들어 해결했다.
	if (m_NoKeyBgmBuf != m_NULLPTR1)
	{
		m_NoKeyBgmBuf2 = m_NoKeyBgmBuf;
		m_iFlushFlag = (int)FBB_BUF_1 | FBB_BUF_2;
	}
	else
		m_iFlushFlag = (int)FBB_BUF_1;
	m_NoKeyBgmBuf = move(_bgm);
	m_sPtrPrevBgmTrack = m_sPtrCurrentBgmTrack;

	m_sPtrCurrentBgmTrack = m_NoKeyBgmBuf->CreateInstance();
	
	if (bEnableTransition)
	{
		m_sPtrCurrentBgmTrack->SetVolume(0.f);
		if (m_sPtrPrevBgmTrack != m_NULLPTR2)
		{
			m_strPrevBGMName = m_strCurrBGMName;

		}
		m_bOnTransition = bEnableTransition;
	}
	else
	{
		m_sPtrCurrentBgmTrack->SetVolume(m_fBgmVolume);
		if (bDeleteFormerBgmFromMem)
		{
			if (m_strPrevBGMName.size() > 0)
				ForgetSound(m_strPrevBGMName);
		}
	}
		
	m_sPtrCurrentBgmTrack->SetPitch(m_fBgmPitch);
	m_sPtrCurrentBgmTrack->SetPan(m_fBgmPan);

	m_sPtrCurrentBgmTrack->Play(bLoop);

	m_strCurrBGMName.clear();

}

void CSoundManager::PauseBgm()
{
	if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
	{
		m_sPtrCurrentBgmTrack->Pause();
	}

	if (m_sPtrPrevBgmTrack != m_NULLPTR2)
	{
		m_sPtrPrevBgmTrack->Pause();
	}
}

void CSoundManager::ResumeBgm()
{
	if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
	{
		m_sPtrCurrentBgmTrack->Resume();
	}

	if (m_sPtrPrevBgmTrack != m_NULLPTR2)
	{
		m_sPtrPrevBgmTrack->Resume();
	}
}

void CSoundManager::StopBgm(bool bEnbleTrasition)
{
	if (bEnbleTrasition)
	{
		m_sPtrPrevBgmTrack = m_sPtrCurrentBgmTrack;
		m_sPtrCurrentBgmTrack = m_NULLPTR2;
		m_strPrevBGMName = m_strCurrBGMName;
		m_strCurrBGMName.clear();
	}
	else
	{
		if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
		{
			m_sPtrCurrentBgmTrack->Stop(false);
		}

		if (m_sPtrPrevBgmTrack != m_NULLPTR2)
		{
			m_sPtrPrevBgmTrack->Stop(true);
			m_sPtrPrevBgmTrack = m_NULLPTR2;
		}
	}
	
}


void CSoundManager::SetBgmVolume(float volume)
{
	float fVal = volume;

	//clamp value
	if (fVal > 0.99999988f)
	{
		fVal = 1.f;
	}
	else if (fVal < 1.1754945E-38)
	{
		fVal = 0.f;
	}

	m_fBgmVolume = fVal;

	if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
	{
		m_sPtrCurrentBgmTrack->SetVolume(fVal);
	}

	if (m_sPtrPrevBgmTrack != m_NULLPTR2)
	{
		m_sPtrPrevBgmTrack->SetVolume(fVal);
	}
}

void CSoundManager::SetBgmPitch(float pitch)
{
	float fVal = pitch;
	//clamp value
	if (fVal > 0.99999988f)
	{
		fVal = 1.f;
	}
	else if (fVal < -0.99999988f)
	{
		fVal = -1.f;
	}

	m_fBgmPitch = fVal;

	if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
	{
		m_sPtrCurrentBgmTrack->SetPitch(pitch);
	}
}

void CSoundManager::SetBgmPan(float panRatio)
{
	float fVal = panRatio;
	//clamp value
	if (fVal > 0.99999988f)
	{
		fVal = 1.f;
	}
	else if (fVal < -0.99999988f)
	{
		fVal = -1.f;
	}

	m_fBgmPan = panRatio;

	if (m_sPtrCurrentBgmTrack != m_NULLPTR2)
	{
		m_sPtrCurrentBgmTrack->Pause();
		m_sPtrCurrentBgmTrack->SetPan(m_fBgmPan);
		m_sPtrCurrentBgmTrack->Resume();
	}
}

float CSoundManager::GetBgmVolume() const
{
	return  m_fBgmVolume;
}

float CSoundManager::GetBgmPitch() const
{
	return m_fBgmPitch;
}

float CSoundManager::GetBgmPan() const
{
	return m_fBgmPan;
}

float CSoundManager::GetUIVolume() const
{
	return m_fUIVolume;
}

float CSoundManager::GetUIPitch() const
{
	return m_fUIPitch;
}

float CSoundManager::GetUIPan() const
{
	return m_fUIPan;
}

void CSoundManager::SetUIVolume(float vol)
{
	m_fUIVolume = vol;
}

void CSoundManager::SetUIPitch(float pitch)
{
	m_fUIPitch = pitch;
}

void CSoundManager::SetUIPan(float pan)
{
	m_fUIPan = pan;
}

void CSoundManager::SetTransitionTime(float fTransitionTime)
{
	m_fBgmTransitionTime = fTransitionTime;
}

float CSoundManager::GetTransitionTime() const
{
	return m_fBgmTransitionTime;
}

float CSoundManager::GetCurrentTransitionRate() const
{
	if (m_fBgmTransitionTime > 0.f)
		return m_fCurrTransitionTimer / m_fBgmTransitionTime;
	return 1.f;
}


