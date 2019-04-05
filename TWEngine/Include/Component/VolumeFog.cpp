#include "../EngineHeader.h"
#include "VolumeFog.h"
#include "Camera.h"
#include "../Device.h"
#include "../GameObject.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ViewManager.h"
#include "../Rendering/RenderState.h"
#include "../Resource/Sampler.h"
#include "../Resource/ResourcesManager.h"

PUN_USING

CVolumeFog::CVolumeFog() :
	m_pMesh(nullptr),
	m_pLayOut(nullptr),
	m_iPass(0)
{
	m_eComType = CT_VOLUMEFOG;
}


CVolumeFog::CVolumeFog(const CVolumeFog & pCom)	:
	CComponent(pCom)
{
}

CVolumeFog::~CVolumeFog()
{
	SAFE_RELEASE(m_pMesh);
}

void CVolumeFog::SetMesh(const string & strMeshName)
{
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strMeshName);

	if (m_pMesh)
	{
		m_pLayOut = GET_SINGLE(CShaderManager)->FindInputLayout(m_pMesh->GetInputLayoutKey());
	}
}

void CVolumeFog::SetFogColor(const Vector4 & vColor)
{
	m_tFogCBuffer.vFogColor = vColor;
}

bool CVolumeFog::Init()
{
	m_pObject->SetRenderGroup(RG_FOG);

	SetMesh("Sky");

	return true;
}

void CVolumeFog::Render(float fTime)
{
	if (m_iPass == 0)
		UpdateData();

	m_pMesh->Render();

	if (++m_iPass == 3)
		m_iPass = 0;
}

CVolumeFog * CVolumeFog::Clone()
{
	return new CVolumeFog(*this);
}

void CVolumeFog::UpdateData()
{
	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	tCBuffer.matWorld = m_pTransform->GetLocalMatrix() * m_pTransform->GetWorldMatrix();
	tCBuffer.matView = pMainCamera->GetViewMatrix();
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.matInvProj = tCBuffer.matProj;
	tCBuffer.matInvProj.Inverse();
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
	tCBuffer.matInvProj.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);

	Vector3 vScale = m_pTransform->GetWorldScale();

	float fDensity = vScale.x;

	if (fDensity < vScale.y)
		fDensity = vScale.y;

	if (fDensity < vScale.z)
		fDensity = vScale.z;

	m_tFogCBuffer.fDensity = fDensity * 2.0f;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Fog", &m_tFogCBuffer);

	CONTEXT->IASetInputLayout(m_pLayOut);
}
