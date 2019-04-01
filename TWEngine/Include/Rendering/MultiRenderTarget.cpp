#include "../EngineHeader.h"
#include "MultiRenderTarget.h"
#include "ViewManager.h"
#include "RenderTarget.h"
#include "RenderManager.h"
#include "../Device.h"

PUN_USING
CMultiRenderTarget::CMultiRenderTarget()
{
	m_DepthView = NULLPTR;
	m_OldDepthView = NULLPTR;
}

CMultiRenderTarget::~CMultiRenderTarget()
{
}

void CMultiRenderTarget::AddRenderTargetView(const string& TargetName)
{
	CRenderTarget* getTarget = CViewManager::GetInst()->FindRenderTarget(TargetName);

	if (getTarget == NULLPTR)
		return;

	m_vecTargetView.push_back(getTarget->GetRenderTargetView());
	m_vecOldTargetView.resize(m_vecTargetView.size());
	m_vecShaderResourceView.push_back(getTarget->GetShaderResourceView());

	AddDepthView(TargetName);
}

void CMultiRenderTarget::AddDepthView(const string & TargetName)
{
	CRenderTarget* getTarget = CViewManager::GetInst()->FindRenderTarget(TargetName);

	if (getTarget == NULLPTR)
		return;

	m_DepthView = getTarget->GetDepthView();
}

void CMultiRenderTarget::ReleaseOldTarget()
{
	for (UINT i = 0; i < m_vecOldTargetView.size(); i++)
		SAFE_RELEASE(m_vecOldTargetView[i]);
}

void CMultiRenderTarget::ClearRenderTarget(float ClearColor[4])
{
	for (size_t i = 0; i < m_vecTargetView.size(); i++)
		CDevice::GetInst()->GetContext()->ClearRenderTargetView(m_vecTargetView[i], ClearColor);

	if (m_DepthView != NULLPTR)
		CDevice::GetInst()->GetContext()->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CMultiRenderTarget::SetTarget()
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)m_vecOldTargetView.size(), &m_vecOldTargetView[0], &m_OldDepthView);

	ID3D11DepthStencilView*	pDepth = m_DepthView;

	if (pDepth == NULLPTR)
		pDepth = m_OldDepthView;

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)m_vecTargetView.size(), &m_vecTargetView[0], pDepth);
}

void CMultiRenderTarget::ResetTarget()
{
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)m_vecOldTargetView.size(), &m_vecOldTargetView[0], m_OldDepthView);

	ReleaseOldTarget();
	SAFE_RELEASE(m_OldDepthView);
}

void CMultiRenderTarget::SetShaderResource(int Register)
{
	CDevice::GetInst()->GetContext()->PSSetShaderResources((UINT)Register, (UINT)m_vecShaderResourceView.size(), &m_vecShaderResourceView[0]);
}

void CMultiRenderTarget::ResetShaderResource(int Register)
{
	static vector<ID3D11ShaderResourceView*> SRV;
	SRV.resize(m_vecShaderResourceView.size());

	CDevice::GetInst()->GetContext()->PSSetShaderResources((UINT)Register, (UINT)m_vecShaderResourceView.size(), &SRV[0]);
}
