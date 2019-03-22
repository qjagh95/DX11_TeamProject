#pragma once
#include "RenderState.h"

PUN_BEGIN

class PUN_DLL CBlendState :
	public CRenderState
{
	friend class CViewManager;
	friend class CRenderManager;

private:
	CBlendState();

public:
	~CBlendState();

private:
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecTargetDesc;
	float		m_fBlendFactor[4];
	UINT		m_iSampleMask;
	float		m_fOldBlendFactor[4];
	UINT		m_iOldSampleMask;

public:
	void SetSampleMask(UINT iMask);
	void SetBlendFactor(float fFactor[4]);
	void AddTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphaBlend = D3D11_BLEND_ONE,
		D3D11_BLEND destAlphaBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateState(BOOL bAlphaCoverage, BOOL bIndependent);

public:
	virtual void SetState();
	virtual void ResetState();
};

PUN_END