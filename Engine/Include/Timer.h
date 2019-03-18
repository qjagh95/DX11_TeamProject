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
	int				m_iPlayTime;
	float			m_fTimeVar;
	int				m_iPlaySecond;

public:
	float	GetTime()	const;
	float	GetFPS()	const;
	int   GetFrame() const { return m_iPlayTime; }
	int   GetSecond() const { return m_iPlaySecond - 1; }

public:
	void SetTimeScale(float fTimeScale);

public:
	bool Init();
	void Update();
};

PUN_END