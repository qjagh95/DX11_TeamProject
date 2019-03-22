#pragma once
#include "CSFilter.h"

PUN_BEGIN

enum HDR_PASS
{
	HDR_FIRST,
	HDR_SECOND
};

class CPostEffect;

class PUN_DLL CCSHDRFilter :
	public CCSFilter
{
	friend class CViewManager;

private:
	CCSHDRFilter();
	virtual ~CCSHDRFilter();

private:
	CPostEffect* m_pPostEffect;


public:
	virtual bool Init();
	virtual void SetShaderResourceTo(int iRegister, SHADER_TYPE eType);
	virtual void ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType);

private:
	virtual void SetShaderResource(int iPass);
	virtual void ResetShaderResource(int iPass);
};

PUN_END