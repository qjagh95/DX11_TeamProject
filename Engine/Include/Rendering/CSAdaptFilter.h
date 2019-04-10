#pragma once
#include "CSFilter.h"

PUN_BEGIN

enum ADAPT_PASS
{
	ADAPT_PASS_FIRST,
	ADAPT_OLD,
	ADAPT_PASS_SECOND
};

class PUN_DLL CCSAdaptFilter :
	public CCSFilter
{
	friend class CViewManager;

private:
	CCSAdaptFilter();
	virtual ~CCSAdaptFilter();

private:
	class	CPostEffect* m_pPostEffect;
	float	m_fElapsedTime;
	float	m_fAdaptation;
	float	m_fAdaptTime;
	float	m_fAdaptNorm;
	ID3D11UnorderedAccessView*	m_pAdaptAvgLumUAV;
	ID3D11ShaderResourceView*	m_pAdaptAvgLumSRV;
	ID3D11UnorderedAccessView*	m_pAdaptOldAvgLumUAV;
	ID3D11ShaderResourceView*	m_pAdaptOldAvgLumSRV;

public:
	void SetAdaptationTime(float fAdapt);

public:
	virtual bool Init();
	virtual void SetShaderResourceTo();
	virtual void ResetShaderResourceFrom();
	virtual void Dispatch(float fTime);

private:
	virtual void SetShaderResource(int iPass, float fTime);
	virtual void ResetShaderResource(int iPass, float fTime);
};

PUN_END