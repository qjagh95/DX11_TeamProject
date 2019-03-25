#pragma once

#include "../EngineHeader.h"

PUN_BEGIN

class PUN_DLL CPostEffect
{
	friend class CCSHDRFilter;

private:
	CPostEffect();
	~CPostEffect();

private:
	HDR1stPassCB		m_tDownScaleCBInfo;
	HDR2ndPassCB		m_tFinalPassCBInfo;
	AdaptationCB		m_tAdaptInfo;
	float				m_fMiddleGrey;
	float				m_fLumWhite;
	float				m_fHeight;
	float				m_fWidth;
	float				m_fGroups;

public:
	void SetAdaptation(Vector4 vAdaptation);
	void SetAdaptation(float x, float y, float z, float w);
	float SetMiddleGrey(float fMiddleGrey);
	float SetLumWhite(float fLumWhite);

public:
	void SetDownScaleCB(int iWidth, int iHeight, float fTime);
	void SetFinalPassCB(float fMiddleGrey, float fLumWhite, float fTime);
	void SetAdaptationCB(Vector4 vAdaptation, float fTime);

	void UpdateCBuffer(int iPass);

public:
	const Vector4& GetAdaptation()	const;

public:
	bool Init();
	int  Update(float fTime);
};

PUN_END