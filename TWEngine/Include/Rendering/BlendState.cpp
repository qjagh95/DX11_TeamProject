#include "EngineHeader.h"
#include "BlendState.h"
#include "../Device.h"

PUN_USING

CBlendState::CBlendState()
{
	m_iSampleMask = 0xffffffff;
	m_eState = RS_BLEND;
}

CBlendState::~CBlendState()
{
}

void CBlendState::SetSampleMask(UINT iMask)
{
	m_iSampleMask = iMask;
}

void CBlendState::SetBlendFactor(float fFactor[4])
{
	memcpy(m_fBlendFactor, fFactor, sizeof(float) * 4);
}

void CBlendState::AddTargetDesc(BOOL bEnable,
	D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
	D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend,
	D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp,
	UINT8 iWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};
	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = srcBlend;
	tDesc.DestBlend = destBlend;
	tDesc.BlendOp = blendOp;
	tDesc.SrcBlendAlpha = srcAlphaBlend;
	tDesc.DestBlendAlpha = destAlphaBlend;
	tDesc.BlendOpAlpha = blendAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecTargetDesc.push_back(tDesc);
}

bool CBlendState::CreateState(BOOL bAlphaCoverage,
	BOOL bIndependent)
{
	D3D11_BLEND_DESC	tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	tDesc.IndependentBlendEnable = bIndependent;

	for (size_t i = 0; i < m_vecTargetDesc.size(); ++i)
	{
		memcpy(&tDesc.RenderTarget[i],
			&m_vecTargetDesc[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	}

	m_vecTargetDesc.clear();

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	// Get함수를 이용해서 state를 얻어오게 되면 레퍼런스 카운트가
	// 증가한다.
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pOldState,
		m_fOldBlendFactor, &m_iOldSampleMask);

	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState,
		m_fBlendFactor, m_iSampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pOldState,
		m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pOldState);
}
