#include "EngineHeader.h"
#include "Timer.h"
#include "Core.h"


PUN_USING

CTimer::CTimer()
{
	m_iPlayTime = 0;
	m_fTimeVar = 0.0f;
	m_iPlaySecond = 0;
}

CTimer::~CTimer()
{
}

float CTimer::GetTime() const
{
	return m_fTime * m_fTimeScale;
}

float CTimer::GetFPS() const
{
	return m_fFPS;
}

void CTimer::SetTimeScale(float fTimeScale)
{
	m_fTimeScale = fTimeScale;
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);

	QueryPerformanceCounter(&m_tTick);

	m_fTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iTick = 0;
	m_fTimeScale = 1.f;

	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER	tTick;
	QueryPerformanceCounter(&tTick);

	m_fTime = (tTick.QuadPart - m_tTick.QuadPart) / (float)m_tSecond.QuadPart;
	m_tTick = tTick;

	m_iTick++;
	m_fFPSTime += m_fTime;
	m_fTimeVar += m_fTime;

	m_iPlayTime++;

	if (m_fTimeVar > 0.3f)
	{
		m_iPlaySecond++;
		m_fTimeVar = 0.0f;
	}

	if (m_fFPSTime >= 1.0f)
	{
		m_fFPS = m_iTick / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iTick = 0;
	}

}
