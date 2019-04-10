#pragma once

#include "../EngineHeader.h"

PUN_BEGIN

class PUN_DLL CPostEffect
{
	friend class CRenderManager;
	friend class CCSHDRFilter;
	friend class CCSAdaptFilter;
	friend class CCSBloomFilter;

private:
	CPostEffect();
	~CPostEffect();

private:
	HDR1stPassCB		m_tDownScaleCBInfo;
	HDR2ndPassCB		m_tFinalPassCBInfo;
	AdaptationCB		m_tAdaptInfo;
	BloomThresholdCB	m_tBloomThresholdInfo;
	BloomCB				m_tBloomInfo;
	float				m_fMiddleGrey;
	float				m_fLumWhite;
	float				m_fHeight;
	float				m_fWidth;
	float				m_fGroups;

public:
	void	SetAdaptation(float fAdaptation);
	float SetMiddleGrey(float fMiddleGrey);
	float SetLumWhite(float fLumWhite);

public:
	float GetMiddleGrey()	const;
	float GetLumWhite()		const;

public:
	void SetDownScaleCB(int iWidth, int iHeight, float fTime);
	void SetFinalPassCB(float fMiddleGrey, float fLumWhite);
	void SetFinalPassCB(float fMiddleGrey, float fLumWhite, float fTime);
	void SetAdaptationCB(float fAdaptation, float fTime);
	void SetAdaptationCB(float fAdaptation, float fBloomThreshold, float fTime);
	void SetBloomThresholdCB(float fThreshold, float fTime);
	void SetBloomCB(float fBloomScale, float fTime);

	void UpdateCBuffer(int iPass);

public:
	bool Init();
	int  Update(float fTime);
};

PUN_END