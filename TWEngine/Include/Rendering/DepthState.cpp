#include "stdafx.h"
#include "DepthState.h"
#include "../Device.h"

PUN_USING

CDepthState::CDepthState()
{
	m_iStencilRef = 0xffffffff;
	m_eState = RS_DEPTHSTENCIL;
}

CDepthState::~CDepthState()
{
}

bool CDepthState::CreateState(BOOL bDepthEnable,
	D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eDepthFunc,
	BOOL bStencilEnable, UINT8 iStencilReadMask,
	UINT8 iStencilWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontFace,
	D3D11_DEPTH_STENCILOP_DESC tBackFace)
{
	D3D11_DEPTH_STENCIL_DESC	tDesc = {};
	tDesc.DepthEnable = bDepthEnable;
	tDesc.DepthWriteMask = eMask;
	tDesc.DepthFunc = eDepthFunc;
	tDesc.StencilEnable = bStencilEnable;
	tDesc.StencilReadMask = iStencilReadMask;
	tDesc.StencilWriteMask = iStencilWriteMask;
	tDesc.FrontFace = tFrontFace;
	tDesc.BackFace = tBackFace;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc,
		(ID3D11DepthStencilState**)&m_pState)))
		return false;

	return true;
}

void CDepthState::SetState()
{
	// Get함수를 이용해서 state를 얻어오게 되면 레퍼런스 카운트가
	// 증가한다.
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pOldState,
		&m_iOldStencilRef);

	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState,
		m_iStencilRef);
}

void CDepthState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pOldState,
		m_iOldStencilRef);
	SAFE_RELEASE(m_pOldState);
}
