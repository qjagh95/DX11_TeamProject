#pragma once

#include "../EngineHeader.h"

PUN_BEGIN

class PUN_DLL CPostEffect
{
	friend class CViewManager;

private:
	DownScaleCB			m_tDownScaleCBInfo;
	FinalPassCB			m_tFinalPassCBInfo;
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

public:
	const Vector4& GetAdaptation()	const;

public:
	virtual bool Init();
	virtual int Update(float fTime);

DECLARE_SINGLE(CPostEffect)
};

PUN_END