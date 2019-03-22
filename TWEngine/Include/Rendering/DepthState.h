#pragma once
#include "RenderState.h"

PUN_BEGIN

class PUN_DLL CDepthState :
	public CRenderState
{
	friend class CViewManager;
	friend class CRenderManager;

private:
	CDepthState();

public:
	~CDepthState();


private:
	UINT	m_iStencilRef;
	UINT	m_iOldStencilRef;

public:
	bool CreateState(BOOL bDepthEnable,
		D3D11_DEPTH_WRITE_MASK eMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		BOOL bStencilEnable = FALSE,
		UINT8 iStencilReadMask = 0, UINT8 iStencilWriteMask = 0,
		D3D11_DEPTH_STENCILOP_DESC tFrontFace = {},
		D3D11_DEPTH_STENCILOP_DESC tBackFace = {});
	
	void SetStencilRef(UINT Value) { m_iStencilRef = Value; }

public:
	virtual void SetState();
	virtual void ResetState();
};

PUN_END