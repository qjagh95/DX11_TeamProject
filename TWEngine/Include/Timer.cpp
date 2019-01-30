#include "EngineHeader.h"
#include "Timer.h"
#include "Core.h"


PUN_USING

CTimer::CTimer()
{
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

	if (m_fFPSTime >= 1.0f)
	{
#ifdef _DEBUG
		static char Buffer[255] = {};
		sprintf_s(Buffer, "Frame : %d", m_iTick);

		SetWindowTextA(CCore::GetInst()->GetWindowHandle(), Buffer);
#endif

		m_fFPS = m_iTick / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iTick = 0;

	}

}
