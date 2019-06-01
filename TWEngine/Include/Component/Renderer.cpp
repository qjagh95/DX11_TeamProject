#include "../EngineHeader.h"
#include "Renderer.h"
#include "../Core.h"
#include "Camera.h"
#include "Transform.h"
#include "Animation.h"
#include "Animation2D.h"
#include "../Device.h"
#include "../Rendering/Shader.h"
#include "../Rendering/RenderState.h"

void to_Euler_Angle123(const Vector4 & q, Vector3 & vEuler)
{
	float fSqrQx = q.x * q.x;
	float fSqrQy = q.y * q.y;
	float fSqrQz = q.z * q.z;
	float fSqrQw = q.w * q.w;

	float fSingularityTest = -2.f * ((q.x * q.z) - (q.w * q.y));

	if (fSingularityTest > 0.999998f || fSingularityTest < -0.999998f)
	{
		vEuler.y = copysignf(PUN_PI * 0.5f, fSingularityTest);
	}
	else
	{
		vEuler.y = asinf(fSingularityTest);
	}


	vEuler.z = atan2f(2.f *(q.x * q.y + q.w * q.z), fSqrQw + fSqrQx - fSqrQy - fSqrQz);

	vEuler.x = atan2f(2.f*((q.y*q.z) + (q.w*q.x)), fSqrQw - fSqrQx - fSqrQy + fSqrQz);
}

PUN_USING

CRenderer::CRenderer() :
	m_pMesh(nullptr),
	m_pShader(nullptr),
	m_pLayout(nullptr),
	m_pMaterial(nullptr),
	m_pBoneTex(nullptr),
	m_b2DRenderer(false),
	m_bDecalEnable(false),
	m_bDontRenderMat(false)
{
	m_eComType = CT_RENDERER;
	memset(m_pRenderState, 0, sizeof(CRenderState*) * RS_END);
	if (GET_SINGLE(CRenderManager)->GetGameMode() == GM_2D)
		SetRenderState(DEPTH_DISABLE);

	memset(&m_tComponentCBuffer, 0, sizeof(m_tComponentCBuffer));
	m_tComponentCBuffer.iDecalEnable = 0;
	m_tComponentCBuffer.i3DAnimation = 0;

	m_pAnimation = NULLPTR;
	m_BoneRot = Vector3::One;

	m_isRotFirstCheck = false;
	m_isPosFirstCheck = false;
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

bool CRenderer::IsShadowRender() const
{
	return m_bDontRenderMat;
}

void CRenderer::DontRenderMat(bool bSwitch)
{
	m_bDontRenderMat = bSwitch;
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

Vector3 CRenderer::GetDiffrentRot() const
{
	Vector3 ConvertFirst = m_RotFirstCheck;
	Vector3 ConvertLast = m_RotLastCheck;
	
	ConvertFirst = Vector3(fabs(ConvertFirst.x), fabs(ConvertFirst.y), fabs(ConvertFirst.z));
	ConvertLast = Vector3(fabs(ConvertLast.x), fabs(ConvertLast.y), fabs(ConvertLast.z));

	return Vector3(ConvertFirst + ConvertLast);
}

float CRenderer::GetDiffrentPosX() const
{
	Vector3 Result = m_PosLastCheck - m_PosFirstCheck;

	return Result.x;
}

float CRenderer::GetDiffrentPosY() const
{
	Vector3 Result = m_PosLastCheck - m_PosFirstCheck;
	return Result.y;
}

float CRenderer::GetDiffrentPosZ() const
{
	Vector3 Result = m_PosLastCheck - m_PosFirstCheck;
	return Result.z;
}

Vector3 CRenderer::GetDiffrentPos() const
{
	return m_PosLastCheck - m_PosFirstCheck;
}

CMesh * CRenderer::GetMesh() const
{
	if (m_pMesh == nullptr)
	{
		return nullptr;
	}

	m_pMesh->AddRef();
	return m_pMesh;
}

Vector3 CRenderer::GetMeshLength() const
{
	return m_pMesh->GetLength();
}

Vector3 CRenderer::GetModelMoveDist()
{
	Vector3 Min = m_pMesh->GetMin();
	Vector3 Max = m_pMesh->GetMax();

	return Vector3();
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
		case CT_UI:
			m_b2DRenderer = true;
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
	// Save Data
	// Exception List
	// - Renderstate Pass
	// - C-Buffer
	// - Bone Texture Name
	string strMeshTag = "";
	string strMaterialFileName = "";
	string strShaderTag = "";
	string strLayoutTag = "";
	if (m_pMesh != nullptr)
	{
		strMeshTag = m_pMesh->GetTag();
		strMaterialFileName = strMeshTag + ".msh";
		strShaderTag = m_pShader->GetTag();
		strLayoutTag = m_pMesh->GetInputLayoutKey();
	}
	bool is2DRenderer = m_b2DRenderer;
	bool isDecal = m_bDecalEnable;

	// Save
	_pInstBW->WriteData(strMeshTag);
	_pInstBW->WriteData(strMaterialFileName);
	_pInstBW->WriteData(strShaderTag);
	_pInstBW->WriteData(strLayoutTag);
	_pInstBW->WriteData(is2DRenderer);
	_pInstBW->WriteData(isDecal);
}

void CRenderer::Load(BinaryRead* _pInstBR)
{
	// Renderer 컴포넌트 추가시 Material 컴포넌트는 자동 등록된다.

	// Load Data
	string strMeshTag = _pInstBR->ReadString();
	string strMaterialFileName = _pInstBR->ReadString();
	string strShaderTag = _pInstBR->ReadString();
	string strLayoutKey = _pInstBR->ReadString();
	bool   is2DRenderer = _pInstBR->ReadBool();
	bool   isDecal = _pInstBR->ReadBool();

	// Set
	// Mesh
	const TCHAR* filePath = CPathManager::GetInst()->FindPath(MESH_DATA_PATH);
	string strFullFilePath = CW2A(filePath);
	strFullFilePath += strMaterialFileName;

	ifstream tempIfs;
	tempIfs.open(strFullFilePath, ios::in);
	if (tempIfs.is_open() == true)
	{
		// .msh File Set
		std::basic_string<TCHAR> converted(strMaterialFileName.begin(), strMaterialFileName.end());
		const TCHAR* strMshFileName = converted.c_str();
		SetMesh(strMeshTag, strMshFileName);
	}
	else
	{
		// Tag Set
		SetMesh(strMeshTag);
	}
	if (m_pMesh == nullptr)
	{
		return;
	}
	m_pMesh->SetTag(strMeshTag);

	// Shader
	if (!m_pShader)
	{
		SetShader(strShaderTag);
		m_pShader->SetTag(strShaderTag);
	}

	// Layout
	if (!m_pLayout)
		SetInputLayout(strLayoutKey);

	// 2D Renderer
	m_b2DRenderer = is2DRenderer;
	if (m_b2DRenderer == true)
	{
		Enable2DRenderer();
	}

	// Decal 
	m_bDecalEnable = isDecal;
	SetDecalEnable(m_bDecalEnable);

	tempIfs.close();
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

	m_tComponentCBuffer.iDecalEnable = 1;

	CheckComponent();

	if (CCore::GetInst()->m_bEditorMode)
		SetRenderState(CULL_NONE);

	return true;
}

int CRenderer::Update(float fTime)
{
	if (m_pAnimation == NULLPTR)
		return 0;

	if (m_BoneName.empty() == true)
		return 0;

	Vector3 Temp = Vector3(1.0f, 1.0f, 1.0f);

	m_matBone = *m_pAnimation->GetBone(m_BoneName);

	XMVECTOR xmScale; XMVECTOR xmPos; XMVECTOR xmRot;
	XMMatrixDecompose(&xmScale, &xmRot, &xmPos, m_matBone.matrix);

	Vector4 Qurt = Vector4(xmRot);
	Vector4 ModelFuckingPos = Vector4(xmPos);
	ModelFuckingPos.x *= m_pTransform->GetWorldScale().x;
	ModelFuckingPos.y *= m_pTransform->GetWorldScale().y;
	ModelFuckingPos.z *= m_pTransform->GetWorldScale().z;

	Vector3 Convert;
	to_Euler_Angle123(Qurt, Convert);

	m_BoneRot = Vector3(RadianToDegree(Convert.x), RadianToDegree(Convert.y), RadianToDegree(Convert.z));

	if (m_isRotFirstCheck == true)
	{
		m_RotFirstCheck = m_BoneRot;
		m_isRotFirstCheck = false;
	}

	if (m_isPosFirstCheck == true)
	{
		m_PosFirstCheck = Vector3(ModelFuckingPos.x, ModelFuckingPos.y, ModelFuckingPos.z);
		m_isPosFirstCheck = false;
	}

	if (m_pAnimation->IsCurAnimEnd() == true)
	{
		m_RotLastCheck = m_BoneRot;
		m_PosLastCheck = Vector3(ModelFuckingPos.x, ModelFuckingPos.y, ModelFuckingPos.z);
	}

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
	if (m_bDontRenderMat)
		return;
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

	if (m_pMesh)
	{
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
}

CRenderer * CRenderer::Clone()
{
	return new CRenderer(*this);
}

void CRenderer::RenderNaviEditorMode(float fTime)
{
	UpdateTransform();

	//if (m_pBoneTex)
	//	return;

	unordered_map<string, PRendererCBuffer>::iterator	iter;
	unordered_map<string, PRendererCBuffer>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->first, iter->second->pBuffer);
	}

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Component",
		&m_tComponentCBuffer);

	CONTEXT->IASetInputLayout(m_pLayout);

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
	tCBuffer.matLP = pMainCamera->GetShadowVP();
	tCBuffer.matWLP = tCBuffer.matWorld * tCBuffer.matLP;
	tCBuffer.matInvWVP = tCBuffer.matWVP;
	tCBuffer.matInvWVP.Inverse();
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
	tCBuffer.matVP = pMainCamera->GetVP();
	tCBuffer.matInvVP = tCBuffer.matVP;
	tCBuffer.matInvVP.Inverse();

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
	tCBuffer.matLP.Transpose();
	tCBuffer.matInvVP.Transpose();

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
	tCBuffer.matLP = pMainCamera->GetShadowVP();
	tCBuffer.matWLP = tCBuffer.matWorld * tCBuffer.matLP;
	tCBuffer.matInvWVP = tCBuffer.matWVP;
	tCBuffer.matInvWVP.Inverse();
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
	tCBuffer.matVP = pMainCamera->GetVP();
	tCBuffer.matInvVP = tCBuffer.matVP;
	tCBuffer.matInvVP.Inverse();


	tCBuffer.vPivot = m_pTransform->GetPivot();
	if(m_pMesh != nullptr)
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
	tCBuffer.matLP.Transpose();
	tCBuffer.matInvVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &tCBuffer);

	SAFE_RELEASE(pMainCamera);
}
