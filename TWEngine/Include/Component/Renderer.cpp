#include "stdafx.h"
#include "Renderer.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Mesh.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "../Device.h"
#include "Transform.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "Material.h"
#include "../GameObject.h"
#include "../Rendering/RenderState.h"
#include "../Rendering/RenderManager.h"
#include "Animation2D.h"

PUN_USING

CRenderer::CRenderer() :
	m_pMesh(nullptr),
	m_pShader(nullptr),
	m_pLayout(nullptr),
	m_pMaterial(nullptr),
	m_b2DRenderer(false)
{
	m_eComType = CT_RENDERER;
	memset(m_pRenderState, 0, sizeof(CRenderState*) * RS_END);
	if (GET_SINGLE(CRenderManager)->GetGameMode() == GM_2D)
		SetRenderState(DEPTH_DISABLE);

	memset(&m_tComponentCBuffer, 0, sizeof(m_tComponentCBuffer));
}


CRenderer::CRenderer(const CRenderer & renderer) :
	CComponent(renderer)
{
	*this = renderer;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pShader)
		m_pShader->AddRef();

	m_pMaterial = nullptr;

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->AddRef();
	}

	m_mapCBuffer.clear();

	unordered_map<string, PRendererCBuffer>::const_iterator	iter;
	unordered_map<string, PRendererCBuffer>::const_iterator	iterEnd = renderer.m_mapCBuffer.end();

	for (iter = renderer.m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		PRendererCBuffer pBuffer = new RendererCBuffer;
		pBuffer->strName = iter->second->strName;
		pBuffer->pBuffer = new char[iter->second->iSize];
		pBuffer->iSize = iter->second->iSize;

		m_mapCBuffer.insert(make_pair(iter->first, pBuffer));
	}
}

CRenderer::~CRenderer()
{
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);

	for (int i = 0; i < RS_END; ++i)
	{
		SAFE_RELEASE(m_pRenderState[i]);
	}

	unordered_map<string, PRendererCBuffer>::iterator	iter;
	unordered_map<string, PRendererCBuffer>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}

	m_mapCBuffer.clear();
}

void CRenderer::Enable2DRenderer()
{
	m_b2DRenderer = true;
}

void CRenderer::SetMesh(CMesh * pMesh)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = pMesh;

	if (pMesh)
	{
		pMesh->AddRef();
		SetShader(pMesh->GetShaderKey());
		SetInputLayout(pMesh->GetInputLayoutKey());
	}
}

void CRenderer::SetMesh(const string & strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strKey);

	if (m_pMesh)
	{
		SetShader(m_pMesh->GetShaderKey());
		SetInputLayout(m_pMesh->GetInputLayoutKey());
	}
}

void CRenderer::SetShader(CShader * pShader)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = pShader;

	if (pShader)
		pShader->AddRef();
}

void CRenderer::SetShader(const string & strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
}

void CRenderer::SetInputLayout(const string & strKey)
{
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(strKey);
}

void CRenderer::SetRenderState(const string & strName)
{
	CRenderState*	pState = GET_SINGLE(CRenderManager)->FindRenderState(strName);

	if (!pState)
		return;

	m_pRenderState[pState->GetRenderState()] = pState;
}

bool CRenderer::CreateRendererCBuffer(const string & strName, int iSize)
{
	PRendererCBuffer	pBuffer = FindRendererCBuffer(strName);

	if (pBuffer)
		return false;

	pBuffer = new RendererCBuffer;
	pBuffer->strName = strName;
	pBuffer->pBuffer = new char[iSize];
	pBuffer->iSize = iSize;

	m_mapCBuffer.insert(make_pair(strName, pBuffer));

	return true;
}

void CRenderer::UpdateRendererCBuffer(const string & strName,
	void * pData, int iSize)
{
	PRendererCBuffer	pBuffer = FindRendererCBuffer(strName);

	if (!pBuffer)
	{
		pBuffer = new RendererCBuffer;
		pBuffer->strName = strName;
		pBuffer->pBuffer = new char[iSize];
		pBuffer->iSize = iSize;

		m_mapCBuffer.insert(make_pair(strName, pBuffer));
	}

	memcpy(pBuffer->pBuffer, pData, iSize);
}

void CRenderer::CheckComponent()
{
	const list<CComponent*>*	pList = m_pObject->GetComponentList();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = pList->end();

	for (iter = pList->begin(); iter != iterEnd; ++iter)
	{
		switch ((*iter)->GetComponentType())
		{
		case CT_ANIMATION2D:
			m_tComponentCBuffer.iTextureAnimation2D = 1;
			break;
		}
	}
}

void CRenderer::DeleteComponentCBuffer(CComponent * pComponent)
{
	switch (pComponent->GetComponentType())
	{
	case CT_ANIMATION2D:
		m_tComponentCBuffer.iTextureAnimation2D = 0;
		break;
	}
}

void CRenderer::UpdateAnimationType(int iType)
{
	m_tComponentCBuffer.iAnimationType = iType;
}

PRendererCBuffer CRenderer::FindRendererCBuffer(const string & strName)
{
	unordered_map<string, PRendererCBuffer>::iterator	iter = m_mapCBuffer.find(strName);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}

void CRenderer::AfterClone()
{
	m_pMaterial = FindComponentFromType<CMaterial>(CT_MATERIAL);
}

bool CRenderer::Init()
{
	// 재질정보는 반드시 같이 생성되게 한다.
	m_pMaterial = AddComponent<CMaterial>("Material");

	CheckComponent();

	return true;
}

int CRenderer::Update(float fTime)
{

	return 0;
}

int CRenderer::LateUpdate(float fTime)
{
	return 0;
}

void CRenderer::Collision(float fTime)
{
}

void CRenderer::Render(float fTime)
{
	if (!m_pMaterial)
	{
		m_pMaterial = FindComponentFromType<CMaterial>(CT_MATERIAL);
	}

	UpdateTransform();

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->SetState();
	}

	unordered_map<string, PRendererCBuffer>::iterator	iter;
	unordered_map<string, PRendererCBuffer>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->first, iter->second->pBuffer);
	}

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Component",
		&m_tComponentCBuffer);

	CONTEXT->IASetInputLayout(m_pLayout);
	m_pShader->SetShader();

	for (size_t i = 0; i < m_pMesh->GetContainCount(); ++i)
	{
		for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
		{
			m_pMaterial->SetShader(i, j);

			m_pMesh->Render(i, j);
		}
	}

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->ResetState();
	}
}

CRenderer * CRenderer::Clone()
{
	return new CRenderer(*this);
}

void CRenderer::UpdateTransform()
{
	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = nullptr;

	if (m_b2DRenderer)
		pMainCamera = m_pScene->GetUICamera();

	else
		pMainCamera = m_pScene->GetMainCamera();

	tCBuffer.matWorld = m_pTransform->GetWorldMatrix();
	tCBuffer.matView = pMainCamera->GetViewMatrix();
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = m_pTransform->GetPivot();
	tCBuffer.vLength = m_pMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);
}
