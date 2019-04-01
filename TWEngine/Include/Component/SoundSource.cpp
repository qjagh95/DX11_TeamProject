#include "../EngineHeader.h"
#include "../GameObject.h"
#include "SoundSource.h"
#include "../SoundManager.h"

std::shared_ptr<DirectX::SoundEffect> nullSptrSound;

PUN::CSoundSource::CSoundSource()
{
	m_tAudioEmitter = {};
	m_tAudioEmitter.ChannelCount = 5;
	
}

PUN::CSoundSource::CSoundSource(const CSoundSource & src) :
	PUN::CComponent(src)
{
	*this = src;
}

PUN::CSoundSource::~CSoundSource()
{
}

bool PUN::CSoundSource::Init()
{
	return true;
}

void PUN::CSoundSource::AfterClone()
{
	Vector3 vPos = m_pTransform->GetWorldPos();
	Vector3 vRotFront = m_pTransform->GetWorldAxis(PUN::AXIS_Z);
	Vector3 vRotUp = m_pTransform->GetWorldAxis(PUN::AXIS_Y);
	
	XMFLOAT3 xmPos(vPos.x, vPos.y, vPos.z);
	XMFLOAT3 xmRotFront(vRotFront.x, vRotFront.y, vRotFront.z);
	XMFLOAT3 xmRotUp(vRotUp.x, vRotUp.y, vRotUp.z);

	m_tAudioEmitter.SetPosition(xmPos);
	m_tAudioEmitter.SetOrientation(xmRotFront, xmRotUp);
}

int PUN::CSoundSource::Update(float fTime)
{
	Vector3 vPos = m_pTransform->GetWorldPos();
	Vector3 vRotFront = m_pTransform->GetWorldAxis(PUN::AXIS_Z);
	Vector3 vRotUp = m_pTransform->GetWorldAxis(PUN::AXIS_Y);

	XMFLOAT3 xmPos(vPos.x, vPos.y, vPos.z);
	XMFLOAT3 xmRotFront(vRotFront.x, vRotFront.y, vRotFront.z);
	XMFLOAT3 xmRotUp(vRotUp.x, vRotUp.y, vRotUp.z);

	m_tAudioEmitter.SetPosition(xmPos);
	//m_tAudioEmitter.InnerRadius = 50.f;
	m_tAudioEmitter.SetOrientation(xmRotFront, xmRotUp);

	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();

	for (int i = 0; i < m_vecsPtrSound.size(); ++i)
	{
		if(m_vecsPtrSound[i]->GetState() == PLAYING)
			m_vecsPtrSound[i]->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);
		
	}
	
	return 0;
}

int PUN::CSoundSource::LateUpdate(float fTime)
{
	return 0;
}

void PUN::CSoundSource::Collision(float fTime)
{
}

void PUN::CSoundSource::Render(float fTime)
{
}

PUN::CSoundSource * PUN::CSoundSource::Clone()
{
	return new PUN::CSoundSource(*this);
}

void PUN::CSoundSource::Play(int idxKey, bool bLoop)
{
	m_vecsPtrSound[idxKey]->Play(bLoop);
}

void PUN::CSoundSource::Play(std::string & strKey, bool bLoop)
{
	int iKey = m_mapSndInstKey.find(strKey)->second;
	m_vecsPtrSound[iKey]->Play(bLoop);
}

void PUN::CSoundSource::PauseClip(int idxKey)
{
	if (m_vecsPtrSound[idxKey]->GetState() == PAUSED)
	{
		m_vecsPtrSound[idxKey]->Resume();
	}
	else
		m_vecsPtrSound[idxKey]->Pause();
}

void PUN::CSoundSource::PauseClip(std::string & strKey)
{
	int idxKey = m_mapSndInstKey.find(strKey)->second;
	if (m_vecsPtrSound[idxKey]->GetState() == PAUSED)
	{
		m_vecsPtrSound[idxKey]->Resume();
	}
	else
		m_vecsPtrSound[idxKey]->Pause();
}

void PUN::CSoundSource::StopClip(int idxKey, bool bImmediate)
{
	m_vecsPtrSound[idxKey]->Stop(bImmediate);
}

void PUN::CSoundSource::StopClip(std::string & strKey, bool bImmediate )

{
	int idxKey = m_mapSndInstKey.find(strKey)->second;
	m_vecsPtrSound[idxKey]->Stop(bImmediate);
}

bool PUN::CSoundSource::LoadSounds(const vector<std::string>& vecStr)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;
	for (size_t i = 0; i < vecStr.size(); ++i)
	{
		std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(vecStr[i]);
		if (sPtrSound == sNullPTR)
			return false;
		std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst 
			= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

		sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

		m_vecsPtrSound.push_back(sPtrSoundInst);
		m_mapSndInstKey.insert(std::make_pair(vecStr[i], i));
		m_vecSndStr.push_back(vecStr[i]);
	}

	return true;
}

bool PUN::CSoundSource::LoadSounds(const char ** arrStr, int iCnt)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;
	for (int i = 0; i < iCnt; ++i)
	{
		std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(arrStr[i]);
		if (sPtrSound == sNullPTR)
			return false;
		std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst
			= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

		sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

		m_vecsPtrSound.push_back(sPtrSoundInst);
		m_mapSndInstKey.insert(std::make_pair(arrStr[i], i));
		m_vecSndStr.push_back(arrStr[i]);
	}

	return true;
}

bool PUN::CSoundSource::LoadSounds(const char ** arrStrName, const TCHAR ** arrFilePath, int iCnt, const string& PathKey)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;
	for (size_t i = 0; i < iCnt; ++i)
	{

		pMgr->CreateSoundEffect(arrStrName[i], arrFilePath[i], PathKey);
		std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(arrStrName[i]);
		if (sPtrSound == sNullPTR)
			return false;
		std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst
			= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

		sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

		m_vecsPtrSound.push_back(sPtrSoundInst);
		m_mapSndInstKey.insert(std::make_pair(arrStrName[i], i));
		m_vecSndStr.push_back(arrStrName[i]);
	}
	return true;
}

bool PUN::CSoundSource::LoadSounds(const vector<std::string>& vecStrName, const vector<wstring>& vecFilePath, const string & PathKey)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;
	for (size_t i = 0; i < vecStrName.size(); ++i)
	{
		
		pMgr->CreateSoundEffect(vecStrName[i], vecFilePath[i], PathKey);
		std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(vecStrName[i]);
		if (sPtrSound == sNullPTR)
			return false;
		std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst
			= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

		sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

		m_vecsPtrSound.push_back(sPtrSoundInst);
		m_mapSndInstKey.insert(std::make_pair(vecStrName[i], i));
		m_vecSndStr.push_back(vecStrName[i]);
	}

	return true;
}

float PUN::CSoundSource::GetRadius() const
{
	return m_tAudioEmitter.InnerRadius;
}

void PUN::CSoundSource::SetVolume(int iClipIdx, float vol)
{
	m_vecsPtrSound[iClipIdx]->SetVolume(vol);
}

void PUN::CSoundSource::SetPitch(int iClipIdx, float pitch)
{
	m_vecsPtrSound[iClipIdx]->SetPitch(pitch);
}

void PUN::CSoundSource::SetVolume(std::string strClip, float vol)
{
	int iClipIdx = m_mapSndInstKey.find(strClip)->second;
	m_vecsPtrSound[iClipIdx]->SetVolume(vol);
}

void PUN::CSoundSource::SetPitch(std::string strClip, float pitch)
{
	int iClipIdx = m_mapSndInstKey.find(strClip)->second;
	m_vecsPtrSound[iClipIdx]->SetPitch(pitch);
}

void PUN::CSoundSource::SetRadius(float rad)
{
	m_tAudioEmitter.InnerRadius = rad;
}

DirectX::AudioEmitter & PUN::CSoundSource::GetEmitterInfo()
{
	// TODO: insert return statement here
	return m_tAudioEmitter;
}

