#include "../EngineHeader.h"
#include "ColliderSphere.h"
#include "Transform.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Rendering/ShaderManager.h"
#include "ColliderPoint.h"
#include "ColliderOBB.h"
#include "ColliderPixel.h"
#include "ColliderRay.h"

PUN_USING

CColliderSphere::CColliderSphere()
{
	m_eCollType = CT_SPHERE;
#ifdef _DEBUG
	m_pWireFrame = GET_SINGLE(CRenderManager)->FindRenderState(WIRE_FRAME);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Sky");
#endif // _DEBUG
}

CColliderSphere::CColliderSphere(const CColliderSphere & com) :
	CCollider(com)
{
	m_tRelativeInfo = com.m_tRelativeInfo;
}

CColliderSphere::~CColliderSphere()
{
}

SphereInfo CColliderSphere::GetInfo() const
{
	return m_tInfo;
}

void CColliderSphere::SetInfo(const Vector3& vCenter, float fRadius)
{
	m_tRelativeInfo.vCenter = vCenter;
	m_tRelativeInfo.fRadius = fRadius;

	m_tInfo.fRadius = fRadius;
}

bool CColliderSphere::Init()
{
	return true;
}

int CColliderSphere::Input(float fTime)
{
	return 0;
}

int CColliderSphere::Update(float fTime)
{
	return 0;
}

int CColliderSphere::LateUpdate(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() *
		m_pTransform->GetWorldScale();

	m_tInfo.vCenter = vPos + m_tRelativeInfo.vCenter;

	// 이 정보 자체가 사각형의 영역을 판단할 데이터로 사용된다.
	m_vSectionMin = m_tInfo.vCenter - Vector3(m_tInfo.fRadius, m_tInfo.fRadius, m_tInfo.fRadius);
	m_vSectionMax = m_tInfo.vCenter + Vector3(m_tInfo.fRadius, m_tInfo.fRadius, m_tInfo.fRadius);

	return 0;
}

void CColliderSphere::Collision(float fTime)
{
}

void CColliderSphere::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale;
	matPos.Translation(m_tInfo.vCenter);
	matScale.Scaling(m_tInfo.fRadius);

	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	Matrix	matView;

	if (m_strCollisionGroup != "UI")
		matView = pMainCamera->GetViewMatrix();

	tCBuffer.matWorld = matScale * matPos;
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = m_vPivot;
	tCBuffer.vLength = m_pMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);

	CCollider::Render(fTime);
#endif // _DEBUG

}

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_SPHERE:
		return CollisionSphereToSphere(m_tInfo, ((CColliderSphere*)pDest)->GetInfo());
	case CT_RAY:
		return CollisionRayToSphere(((CColliderRay*)pDest)->GetInfo(), m_tInfo);
	}

	return false;
}
