#include "EngineHeader.h"
#include "Sound.h"

#include "../SoundManager.h"

PUN_USING

CSound::CSound()	:
	m_bBGM(false),
	m_bPlay(false)
{
	m_eComType = CT_SOUND;
	SetTag("Sound");
}

CSound::CSound(const CSound & sound)	:
	CComponent(sound)
{
	*this = sound;
}

CSound::~CSound()
{
}

void CSound::SetSound(const string & strFileName, bool bLoop, 
	const string & strPathKey)
{
	m_strFileName = strFileName;

	m_bLoop = bLoop;

	GET_SINGLE(CSoundManager)->LoadSound(m_pScene, bLoop, strFileName.c_str(),
		strPathKey);
}

void CSound::EnableBGM()
{
	m_bBGM = true;
}

void CSound::Play()
{
	m_bPlay = true;
	GET_SINGLE(CSoundManager)->Play(m_strFileName, m_bBGM);
}

void CSound::Stop()
{
	m_bPlay = false;
	GET_SINGLE(CSoundManager)->Stop(m_strFileName, m_bBGM);
}

void CSound::Pause()
{
}

void CSound::Start()
{
	if (m_bPlay)
		GET_SINGLE(CSoundManager)->Play(m_strFileName, m_bBGM);
}

void CSound::AfterClone()
{
}

bool CSound::Init()
{
	return true;
}

int CSound::Input(float fTime)
{
	return 0;
}

int CSound::Update(float fTime)
{
	return 0;
}

int CSound::LateUpdate(float fTime)
{
	return 0;
}

void CSound::Collision(float fTime)
{
}

void CSound::Render(float fTime)
{
}

CSound * CSound::Clone()
{
	return new CSound(*this);
}
