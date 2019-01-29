#include "stdafx.h"
#include "RenderTarget.h"
#include "../Device.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Mesh.h"
#include "DepthState.h"
#include "RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Component/Camera.h"
#include "../Resource/Sampler.h"

PUN_USING

CRenderTarget::CRenderTarget() :
	m_pTargetTex(nullptr),
	m_pTargetSRV(nullptr),
	m_pTargetView(nullptr),
	m_pDepthTex(nullptr),
	m_pDepthView(nullptr),
	m_pShader(nullptr),
	m_pMesh(nullptr),
	m_pLayout(nullptr),
	m_bDrawDebug(false),
	m_pOldTargetView(nullptr),
	m_pOldDepthView(nullptr),
	m_pDepthState(nullptr),
	m_pFullScreenShader(nullptr),
	m_pSampler(nullptr)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pFullScreenShader);
	SAFE_RELEASE(m_pDepthState);
	SAFE_RELEASE(m_pOldDepthView);
	SAFE_RELEASE(m_pOldTargetView);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetSRV);
	SAFE_RELEASE(m_pTargetTex);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDepthTex);
}

ID3D11RenderTargetView * CRenderTarget::GetRenderTargetView() const
{
	return m_pTargetView;
}

ID3D11DepthStencilView * CRenderTarget::GetDepthView() const
{
	return m_pDepthView;
}

bool CRenderTarget::CreateRenderTarget(DXGI_FORMAT eTargetFmt, const Vector3 & vPos,
	const Vector3 & vScale, DXGI_FORMAT eDepthFmt)
{
	m_vPos = vPos;
	m_vScale = vScale;
	Resolution	tRS = _RESOLUTION;

	// 타겟용 텍스처 생성
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.Width = tRS.iWidth;
	tDesc.Height = tRS.iHeight;
	tDesc.ArraySize = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tDesc.Format = eTargetFmt;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pTargetTex)))
		return false;

	// 만들어진 텍스처로 ShaderResourceView와 TargetView를 생성한다.
	if (FAILED(DEVICE->CreateShaderResourceView(m_pTargetTex, nullptr, &m_pTargetSRV)))
		return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTargetTex, nullptr, &m_pTargetView)))
		return false;

	if (eDepthFmt != DXGI_FORMAT_UNKNOWN)
	{
		tDesc.Width = tRS.iWidth;
		tDesc.Height = tRS.iHeight;
		tDesc.ArraySize = 1;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tDesc.Format = eDepthFmt;
		tDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pDepthTex)))
			return false;

		if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, nullptr, &m_pDepthView)))
			return false;
	}

	m_pSampler = GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_LINEAR);

	return true;
}

void CRenderTarget::SetClearColor(float fClearColor[4])
{
	memcpy(m_fClearColor, fClearColor, sizeof(float) * 4);
}

void CRenderTarget::SetClearColor(const Vector4 & vColor)
{
	memcpy(m_fClearColor, &vColor, sizeof(Vector4));
}

void CRenderTarget::ONDrawDebug(bool bDraw)
{
	m_bDrawDebug = bDraw;

	if (m_bDrawDebug)
	{
		m_pDepthState = (CDepthState*)GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
		m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_TEX_STATIC_SHADER);
		m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("TexRect");
		m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_UV_LAYOUT);
	}
}

void CRenderTarget::ClearTarget()
{
	CONTEXT->ClearRenderTargetView(m_pTargetView, m_fClearColor);

	if (m_pDepthView)
		CONTEXT->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CRenderTarget::SetTarget()
{
	// 기존 디바이스에 지정되어 있는 타겟뷰와 깊이뷰를 얻어온다.
	CONTEXT->OMGetRenderTargets(1, &m_pOldTargetView, &m_pOldDepthView);

	// 만약 깊이버퍼가 생성 안되어 있을 경우 기존에 지정되어 있던
	// 깊이 버퍼를 그대로 지정한다.
	ID3D11DepthStencilView*	pDepth = m_pOldDepthView;
	if (m_pDepthView)
		pDepth = m_pDepthView;

	CONTEXT->OMSetRenderTargets(1, &m_pTargetView, pDepth);
}

void CRenderTarget::ResetTarget()
{
	// 원래 타겟들로 교체한다.
	CONTEXT->OMSetRenderTargets(1, &m_pOldTargetView, m_pOldDepthView);
	SAFE_RELEASE(m_pOldDepthView);
	SAFE_RELEASE(m_pOldTargetView);
}

void CRenderTarget::Render(float fTime)
{
	if (!m_bDrawDebug)
		return;

	TransformCBuffer	tTransform = {};
	Matrix	matPos, matScale;
	matScale.Scaling(m_vScale);
	matPos.Translation(m_vPos);

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();

	CCamera*	pCamera = pScene->GetUICamera();

	Matrix	matView, matProj;
	matView = pCamera->GetViewMatrix();
	matProj = pCamera->GetProjMatrix();

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pScene);

	tTransform.matWorld = matScale * matPos;
	tTransform.matView = matView;
	tTransform.matProj = matProj;
	tTransform.matWV = tTransform.matWorld * matView;
	tTransform.matWVP = tTransform.matWV * matProj;
	tTransform.vLength = m_pMesh->GetLength();

	tTransform.matWorld.Transpose();
	tTransform.matView.Transpose();
	tTransform.matProj.Transpose();
	tTransform.matWV.Transpose();
	tTransform.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tTransform);

	if (m_pSampler)
		m_pSampler->SetShader(0);

	m_pDepthState->SetState();

	CONTEXT->IASetInputLayout(m_pLayout);

	CONTEXT->PSSetShaderResources(0, 1, &m_pTargetSRV);

	m_pShader->SetShader();

	m_pMesh->Render();

	ID3D11ShaderResourceView*	pSRV = nullptr;
	CONTEXT->PSSetShaderResources(0, 1, &pSRV);

	m_pDepthState->ResetState();
}

void CRenderTarget::RenderFullScreen()
{
	if (!m_pFullScreenShader)
		m_pFullScreenShader = GET_SINGLE(CShaderManager)->FindShader(FULLSCREEN_SHADER);

	m_pDepthState->SetState();

	if (m_pSampler)
		m_pSampler->SetShader(0);
	CONTEXT->PSSetShaderResources(0, 1, &m_pTargetSRV);

	m_pFullScreenShader->SetShader();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	CONTEXT->IASetInputLayout(nullptr);

	UINT iOffset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	m_pDepthState->ResetState();

	ID3D11ShaderResourceView*	pSRV = nullptr;
	CONTEXT->PSSetShaderResources(0, 1, &pSRV);
}

void CRenderTarget::SetShader(int iRegister)
{
	CONTEXT->PSSetShaderResources(iRegister, 1, &m_pTargetSRV);
}
