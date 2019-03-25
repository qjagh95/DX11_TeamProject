#pragma once
#include "CSFilter.h"

PUN_BEGIN

class CUAV;
class CComputeShader;

class PUN_DLL CDownScale	:
	public CCSFilter
{
	friend class CViewManager;

private:
	CDownScale();

public:
	~CDownScale();

private:
	ID3D11ShaderResourceView*	m_pDepthSRV;

public:
	

public:
	virtual bool Init();
	virtual void Dispatch();
	virtual void SetShaderResourceTo();
	virtual void ResetShaderResourceFrom();

private:
	virtual void SetShaderResource(int iPass);
	virtual void ResetShaderResource(int iPass);
};

PUN_END