#include "../EngineHeader.h"
#include "Renderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Animation.h"
#include "Animation2D.h"
#include "../Device.h"
#include "../Rendering/Shader.h"
#include "../Rendering/RenderState.h"

PUN_USING

CRenderer::CRenderer() :
	m_pMesh(nullptr),
	m_pShader(nullptr),
	m_pLayout(nullptr),
	m_pMaterial(nullptr),
	m_pBoneTex(nullptr),
	m_b2DRenderer(false),
	m_bDecalEnable(false)
{
	m_eComType = CT_RENDERER;
	memset(m_pRenderState, 0, sizeof(CRenderState*) * RS_END);
	if (GET_SINGLE(CRenderManager)->GetGameMode() == GM_2D)
		SetRenderState(DEPTH_DISABLE);

	memset(&m_tComponentCBuffer, 0, sizeof(m_tComponentCBuffer));
	m_tComponentCBuffer.iDecalEnable = 0;
	m_tComponentCBuffer.i3DAnimation = 0;
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

	m_pBoneTex = nullptr;
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

void CRenderer::SetBoneTexture(ID3D11ShaderResourceView** pBoneTex)
{
	m_pBoneTex = pBoneTex;
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

void CRenderer::SetMesh(const string & strKey, const TCHAR * pFileName,
	const string & strPathKey)
{
	SAFE_RELEASE(m_pMesh);
	GET_SINGLE(CResourcesManager)->LoadMesh(strKey, pFileName, strPathKey);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strKey);

	if (m_pMesh)
	{
		SetShader(m_pMesh->GetShaderKey());
		SetInputLayout(m_pMesh->GetInputLayoutKey());


		CMaterial*	pMaterial = m_pMesh->CloneMaterial();

		if (pMaterial)
		{
			SAFE_RELEASE(m_pMaterial);
			m_pMaterial = pMaterial;
			m_pObject->RemoveComponentFromType(CT_MATERIAL);
			m_pObject->AddComponent(m_pMaterial);
		}

		CAnimation*	pAnimation = m_pMesh->CloneAnimation();

		if (pAnimation)
		{
			m_pObject->RemoveComponentFromType(CT_ANIMATION);
			m_pObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);
		}
	}
}

CMesh * CRenderer::GetMesh() const
{
	m_pMesh->AddRef();
	return m_pMesh;
}

void CRenderer::SetMeshFromFullPath(const string & strKey, const TCHAR * pFullPath)
{
	SAFE_RELEASE(m_pMesh);
	GET_SINGLE(CResourcesManager)->LoadMeshFromFullPath(strKey,
		pFullPath);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strKey);

	if (m_pMesh)
	{
		SetShader(m_pMesh->GetShaderKey());
		SetInputLayout(m_pMesh->GetInputLayoutKey());


		CMaterial*	pMaterial = m_pMesh->CloneMaterial();

		if (pMaterial)
		{
			SAFE_RELEASE(m_pMaterial);
			m_pMaterial = pMaterial;
			m_pObject->RemoveComponentFromType(CT_MATERIAL);
			m_pObject->AddComponent(m_pMaterial);
		}

		CAnimation*	pAnimation = m_pMesh->CloneAnimation();

		if (pAnimation)
		{
			m_pObject->RemoveComponentFromType(CT_ANIMATION);
			m_pObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);
		}
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

void CRenderer::SetDecalEnable(bool bEnable)
{
	m_bDecalEnable = bEnable;

	if (!bEnable)
		m_tComponentCBuffer.iDecalEnable = 0;
	else
		m_tComponentCBuffer.iDecalEnable = 1;
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

void CRenderer::Save(BinaryWrite* _pInstBW)
{
	_pInstBW->WriteData(m_pMesh->GetTag());
	_pInstBW->WriteData(m_pShader->GetTag());
	_pInstBW->WriteData(m_strLayoutKey);
}

void CRenderer::Load(BinaryRead* _pInstBR)
{
	string strMeshTag = _pInstBR->ReadString();
	string strShaderTag = _pInstBR->ReadString();
	m_strLayoutKey = _pInstBR->ReadString();
	m_pMesh->SetTag(strMeshTag);
	m_pShader->SetTag(strShaderTag);

	// .msh 파일이 있는 경우에 메시를 세팅한다.
	TCHAR* strMshFileName = CA2W((strMeshTag + ".msh").c_str());
	ifstream* pIfs = _pInstBR->GetIfStream();
	pIfs->open(strMeshTag + ".msh", ios::in);
	pIfs->close();
	if (pIfs->is_open() == true)
	{
		SetMesh(strMeshTag, strMshFileName);
		return;
	}

	// 파일이 없다면 메시 태그명으로 설정하는 함수를 호출한다.
	SetMesh(strMeshTag);
}
void CRenderer::AfterClone()
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = FindComponentFromType<CMaterial>(CT_MATERIAL);
}

bool CRenderer::Init()
{
	SAFE_RELEASE(m_pMaterial);
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

	if (m_pBoneTex)
		CONTEXT->VSSetShaderResources(3, 1, m_pBoneTex);

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
	if (m_pShader)
	{
		m_pShader->SetShader();
	}

	if (m_pMesh)
	{
		for (int i = 0; i < m_pMesh->GetContainCount(); ++i)
		{
			if (m_pMesh->IsSubset(i))
			{
				for (int j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
				{
					m_pMaterial->SetShader(i, j);

					m_pMesh->Render(i, j);

					m_pMaterial->ResetShader((int)i, (int)j);
				}
			}
			else
			{
				m_pMaterial->SetShader(i, 0);

				m_pMesh->Render(i, 0);

				m_pMaterial->ResetShader((int)i, 0);
			}
		}
	}

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->ResetState();
	}
}

void CRenderer::RenderShadow(float fTime)
{
	UpdateShadowTransform();

	if (m_pBoneTex)
	{
		m_tComponentCBuffer.i3DAnimation = 1;
		CONTEXT->VSSetShaderResources(3, 1, m_pBoneTex);
	}

	unordered_map<string, PRendererCBuffer>::iterator	iter;
	unordered_map<string, PRendererCBuffer>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
		GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->first, iter->second->pBuffer);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Component", &m_tComponentCBuffer);

	CONTEXT->IASetInputLayout(m_pLayout);
	//m_pShader->SetShader();

	for (int i = 0; i < m_pMesh->GetContainCount(); ++i)
	{
		if (m_pMesh->IsSubset(i))
		{
			for (int j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
				m_pMesh->Render(i, j);
		}
		else
			m_pMesh->Render(i, 0);
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

	tCBuffer.matWorld = m_pTransform->GetLocalMatrix() * m_pTransform->GetWorldMatrix();
	tCBuffer.matView = pMainCamera->GetViewMatrix();
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.matWLP = tCBuffer.matWorld * pMainCamera->GetShadowViewMatrix() * pMainCamera->GetShadowProjMatrix();
	tCBuffer.matInvWVP = tCBuffer.matWVP;
	tCBuffer.matInvWVP.Inverse();
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
	tCBuffer.matVP = tCBuffer.matView * tCBuffer.matProj;
	tCBuffer.vPivot = m_pTransform->GetPivot();

	if (m_pMesh)
	{
		tCBuffer.vLength = m_pMesh->GetLength();
	}

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();
	tCBuffer.matInvWVP.Transpose();
	tCBuffer.matWLP.Transpose();
	tCBuffer.matInvProj.Transpose();
	tCBuffer.matVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);
}

void CRenderer::UpdateShadowTransform()
{
	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	tCBuffer.matWorld = m_pTransform->GetLocalMatrix() * m_pTransform->GetWorldMatrix();
	tCBuffer.matView = pMainCamera->GetShadowViewMatrix();
	tCBuffer.matProj = pMainCamera->GetShadowProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.matWLP = tCBuffer.matWorld * pMainCamera->GetShadowViewMatrix() *	pMainCamera->GetShadowProjMatrix();
	tCBuffer.matInvWVP = tCBuffer.matWVP;
	tCBuffer.matInvWVP.Inverse();
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
	tCBuffer.matVP = tCBuffer.matView * tCBuffer.matProj;
	tCBuffer.vPivot = m_pTransform->GetPivot();
	tCBuffer.vLength = m_pMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();
	tCBuffer.matWLP.Transpose();
	tCBuffer.matInvWVP.Transpose();
	tCBuffer.matInvProj.Transpose();
	tCBuffer.matVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &tCBuffer);

	SAFE_RELEASE(pMainCamera);
}
