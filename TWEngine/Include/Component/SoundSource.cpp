#include "../EngineHeader.h"
#include "../GameObject.h"
#include "SoundSource.h"
#include "../SoundManager.h"

std::shared_ptr<DirectX::SoundEffect> nullSptrSound;


PUN::CSoundSource::CSoundSource()
{
	m_tAudioEmitter = {};
	m_tAudioEmitter.ChannelCount = 2;
	m_tAudioEmitter.ChannelRadius = 5.f;
	m_tAudioEmitter.InnerRadius = 5.f;
	m_tAudioEmitter.EmitterAzimuths[0] = X3DAUDIO_PI * 0.5f;
	m_tAudioEmitter.EmitterAzimuths[1] = X3DAUDIO_PI * 1.5f;
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

	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	float fDiv = pMgr->GetAudioCoordSize();
	vPos /= fDiv;
	const DirectX::AudioListener& _mainListener = pMgr->GetMainListener();
	
	XMFLOAT3 xmPos(vPos.x, vPos.y, vPos.z);
	
	Vector3 vOriFront = vPos - Vector3(_mainListener.Position);
	Vector3 vRotSide = m_pTransform->GetWorldAxis(PUN::AXIS_X);

	Vector3 vOriUp = vOriFront.Cross(vRotSide);
	/*
	XMFLOAT3 xmRotFront(vRotFront.x, vRotFront.y, vRotFront.z);
	XMFLOAT3 xmRotUp(vRotUp.x, vRotUp.y, vRotUp.z);
	*/
	
	m_tAudioEmitter.SetPosition(xmPos);
	
	//m_tAudioEmitter.InnerRadius = 50.f;

	XMFLOAT3 xmOriF(vOriFront.x, vOriFront.y, vOriFront.z);
	XMFLOAT3 xmOriU(vOriUp.x, vOriUp.y, vOriUp.z);

	m_tAudioEmitter.SetOrientation(xmOriF, xmOriU);

	

	for (int i = 0; i < m_vecsPtrSound.size(); ++i)
	{
		
		if (m_vecsPtrSound[i]->GetState() == PLAYING)
		{
			m_vecsPtrSound[i]->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter, false);
		}
		
		
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
	if (m_vecsPtrSound[idxKey]->GetState() == PLAYING)
		m_vecsPtrSound[idxKey]->Stop(true);
	m_vecsPtrSound[idxKey]->Play(bLoop);
}

void PUN::CSoundSource::Play(std::string & strKey, bool bLoop)
{
	size_t iKey = m_mapSndInstKey.find(strKey)->second;
	if (m_vecsPtrSound[iKey]->GetState() == PLAYING)
		m_vecsPtrSound[iKey]->Stop(true);
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
	size_t idxKey = m_mapSndInstKey.find(strKey)->second;
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
	size_t idxKey = m_mapSndInstKey.find(strKey)->second;
	m_vecsPtrSound[idxKey]->Stop(bImmediate);
}

int PUN::CSoundSource::GetClipState(std::string & strKey) const
{
	size_t idxKey = m_mapSndInstKey.find(strKey)->second;
	
	return m_vecsPtrSound[idxKey]->GetState();
}

int PUN::CSoundSource::GetClipState(int idxKey) const
{
	return m_vecsPtrSound[idxKey]->GetState();
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
		sPtrSoundInst->SetPan(0.f);
		sPtrSoundInst->SetVolume(1.f);
		sPtrSoundInst->SetPitch(0.f);
		m_vecsPtrSound.push_back(sPtrSoundInst);
		m_mapSndInstKey.insert(std::make_pair(vecStr[i], i));
		m_vecSndStr.push_back(vecStr[i]);
		m_vecsPtrSound_Data.push_back(sPtrSound);
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
		m_vecsPtrSound_Data.push_back(sPtrSound);
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
		m_vecsPtrSound_Data.push_back(sPtrSound);
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
		m_vecsPtrSound_Data.push_back(sPtrSound);
	}

	return true;
}

bool PUN::CSoundSource::LoadSound(const string & strName, const TCHAR * pFileName, const string & strPathKey)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;

	pMgr->CreateSoundEffect(strName, pFileName, strPathKey);
	std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(strName);
	if (sPtrSound == sNullPTR)
		return false;
	std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst
		= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

	sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

	m_vecsPtrSound.push_back(sPtrSoundInst);
	m_mapSndInstKey.insert(std::make_pair(strName, m_vecsPtrSound.size() - 1));
	m_vecSndStr.push_back(strName);
	m_vecsPtrSound_Data.push_back(sPtrSound);

	return true;
}

bool PUN::CSoundSource::LoadSound(int iIndex, const string & strName, const TCHAR * pFileName, const string & strPathKey)
{
	PUN::CSoundManager *pMgr = PUN::CSoundManager::GetInst();
	std::shared_ptr<DirectX::SoundEffect> sNullPTR;

	pMgr->CreateSoundEffect(strName, pFileName, strPathKey);
	std::shared_ptr<DirectX::SoundEffect> sPtrSound = pMgr->FindSoundEffect(strName);
	if (sPtrSound == sNullPTR)
		return false;
	std::shared_ptr<DirectX::SoundEffectInstance> sPtrSoundInst
		= sPtrSound->CreateInstance(SoundEffectInstance_Use3D | SoundEffectInstance_ReverbUseFilters);

	sPtrSoundInst->Apply3D(pMgr->GetMainListener(), m_tAudioEmitter);

	if (iIndex > m_vecsPtrSound.size() - 1)
	{
		m_vecsPtrSound.resize(iIndex + 1);
		m_vecSndStr.resize(iIndex + 1);
		m_vecsPtrSound_Data.resize(iIndex + 1);
	}

	m_vecsPtrSound[iIndex] = sPtrSoundInst;
	m_mapSndInstKey.insert(std::make_pair(strName, iIndex));
	m_vecSndStr[iIndex] = strName;
	m_vecsPtrSound_Data[iIndex] = sPtrSound;

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
	size_t iClipIdx = m_mapSndInstKey.find(strClip)->second;
	m_vecsPtrSound[iClipIdx]->SetVolume(vol);
}

void PUN::CSoundSource::SetPitch(std::string strClip, float pitch)
{
	size_t iClipIdx = m_mapSndInstKey.find(strClip)->second;
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

