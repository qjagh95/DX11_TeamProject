#include "../EngineHeader.h"
#include "RasterizerState.h"
#include "../Device.h"

PUN_USING

CRasterizerState::CRasterizerState()
{
	m_eState = RS_RASTERIZER;
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::CreateState(D3D11_FILL_MODE eFill, D3D11_CULL_MODE eCull,
	BOOL bFrontCounterClockwise, int iDepthBias, float fDepthBiasClamp,
	float fSlopeScaledDepthBias, BOOL bDepthClipEnable, BOOL bScissorEnable,
	BOOL bMultisampleEnable, BOOL bAntialiasedLineEnable)
{
	D3D11_RASTERIZER_DESC	tDesc;

	tDesc.FillMode = eFill;
	tDesc.CullMode = eCull;
	tDesc.FrontCounterClockwise = bFrontCounterClockwise;
	tDesc.DepthBias = iDepthBias;
	tDesc.DepthBiasClamp = fDepthBiasClamp;
	tDesc.SlopeScaledDepthBias = fSlopeScaledDepthBias;
	tDesc.DepthClipEnable = bDepthClipEnable;
	tDesc.ScissorEnable = bScissorEnable;
	tDesc.MultisampleEnable = bMultisampleEnable;
	tDesc.AntialiasedLineEnable = bAntialiasedLineEnable;

	if (FAILED(DEVICE->CreateRasterizerState(&tDesc, (ID3D11RasterizerState**)&m_pState)))
		return false;

	return true;
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pOldState);
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pOldState);
	SAFE_RELEASE(m_pOldState);
}