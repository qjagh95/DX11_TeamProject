#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CSound :
	public CComponent
{
	friend class CGameObject;

protected:
	CSound();
	CSound(const CSound& sound);
	virtual ~CSound();

public:
	string	m_strFileName;
	bool	m_bBGM;
	bool	m_bLoop;
	bool	m_bPlay;

public:
	void SetSound(const string& strFileName, bool bLoop,
		const string& strPathKey = SOUND_PATH);
	void EnableBGM();
	void Play();
	void Stop();
	void Pause();

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CSound* Clone();
};

PUN_END