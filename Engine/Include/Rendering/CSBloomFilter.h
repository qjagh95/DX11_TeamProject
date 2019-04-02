#pragma once
#include "CSFilter.h"

PUN_BEGIN

enum BLOOM_PASS
{
	BLOOM_FIRST,
	BLOOM_SECOND,
	BLOOM_PROCESSING, // Temp0
	BLOOM_BLUR, // Temp1
	BLOOM_FINAL,
	BLOOM_OLD
};

class PUN_DLL  CCSBloomFilter :
	public CCSFilter
{
	friend class CViewManager;

private:
	CCSBloomFilter();
	virtual ~CCSBloomFilter();

private:
	class	CPostEffect* m_pPostEffect;
	float	m_pBloomThreshold;
	float	m_pBloomScale;
	bool	m_bFinal;
	CUAV*						m_pDownScale;
	ID3D11UnorderedAccessView*	m_pDownScaleUAV;
	ID3D11ShaderResourceView*	m_pDownScaleSRV;
	ID3D11UnorderedAccessView*	m_pAdaptAvgLumUAV;
	ID3D11ShaderResourceView*	m_pAdaptAvgLumSRV;
	ID3D11UnorderedAccessView*	m_pAdaptOldAvgLumUAV;
	ID3D11ShaderResourceView*	m_pAdaptOldAvgLumSRV;

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