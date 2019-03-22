#pragma once
#include "CSFilter.h"

PUN_BEGIN

class CMBTexture;

class PUN_DLL CMotionBlurFilter	:
	public CCSFilter
{
	friend class CViewManager;

private:
	CMotionBlurFilter();

public:
	virtual ~CMotionBlurFilter();

private:
	CMBTexture*			m_pMBTex;

public:
	virtual void Disable();

public:
	virtual bool Init();
	virtual void SetShaderResourceTo(int iRegister, SHADER_TYPE eType);
	virtual void ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType);

private:	
	virtual void SetShaderResource(int iPass);
	virtual void ResetShaderResource(int iPass);
	void Resize(int iCapacity);
};

PUN_END