#pragma once

PUN_BEGIN

class PUN_DLL CTimer
{
	friend class CTimerManager;

private:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tTick;
	float			m_fTime;
	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iTick;
	float			m_fTimeScale;
	string			m_strName;

public:
	float	GetTime()	const;
	float	GetFPS()	const;

public:
	void SetTimeScale(float fTimeScale);

public:
	bool Init();
	void Update();
};

PUN_END