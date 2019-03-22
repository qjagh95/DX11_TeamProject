#pragma once
#include "CSFilter.h"

PUN_BEGIN

enum BLUR_TYPE
{
	BLUR_HORIZONTAL,
	BLUR_VERTICAL
};

class CComputeShader;
class CUAView;

class PUN_DLL CCSBlur :
	public CCSFilter
{
	friend class CViewManager;

private:
	CCSBlur();
	
public:
	~CCSBlur();

private:
	BlurCBuffer	 m_tBlurCBuffer;

public:
	virtual bool Init();
	virtual void SetShaderResourceTo(int iRegister, SHADER_TYPE eType = ST_PIXEL);
	virtual void ResetShaderResourceFrom(int iRegister, SHADER_TYPE eType = ST_PIXEL);

private:
	virtual void SetShaderResource(int iPass);
	virtual void ResetShaderResource(int iPass);
};

PUN_END