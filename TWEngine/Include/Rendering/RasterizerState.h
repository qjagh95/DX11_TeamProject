#pragma once
#include "RenderState.h"

PUN_BEGIN

class PUN_DLL CRasterizerState : public CRenderState
{
	friend class CViewManager;
	friend class CRenderManager;

private:
	CRasterizerState();

public:
	~CRasterizerState();

public:
	// fDepthBiasClamp : Maximum depth bias of a pixel. 
	// For info about depth bias, see Depth Bias.
	bool CreateState(D3D11_FILL_MODE eFill = D3D11_FILL_SOLID, D3D11_CULL_MODE eCull = D3D11_CULL_BACK,
		BOOL bFrontCounterClockwise = FALSE, int iDepthBias = 0,
		float fDepthBiasClamp = 0.f, float fSlopeScaledDepthBias = 0.f,
		BOOL bDepthClipEnable = TRUE, BOOL bScissorEnable = FALSE,
		BOOL bMultisampleEnable = FALSE, BOOL bAntialiasedLineEnable = FALSE);

public:
	virtual void SetState();
	virtual void ResetState();
};

PUN_END
