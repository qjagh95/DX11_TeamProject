#include "stdafx.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../Rendering/RenderManager.h"
#include "../Resource/ResourcesManager.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Rendering/ShaderManager.h"
#include "ColliderPoint.h"
#include "ColliderOBB.h"
#include "ColliderPixel.h"

PUN_USING

CColliderRect::CColliderRect()
{
	m_eCollType = CT_RECT;
#ifdef _DEBUG
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderRect");
#endif // _DEBUG
}

CColliderRect::CColliderRect(const CColliderRect & com) :
	CCollider(com)
{
	m_tRelativeInfo = com.m_tRelativeInfo;
}

CColliderRect::~CColliderRect()
{
}

BoxInfo CColliderRect::GetInfo() const
{
	return m_tInfo;
}

void CColliderRect::SetInfo(const Vector3 & vMin, const Vector3 & vMax)
{
	m_tRelativeInfo.vMin = vMin;
	m_tRelativeInfo.vMax = vMax;
	m_tRelativeInfo.vLength = vMax - vMin;

	m_tRelativeInfo.vMin.z = 0.f;
	m_tRelativeInfo.vMax.z = 0.f;
	m_tRelativeInfo.vLength.z = 1.f;
}

bool CColliderRect::Init()
{
	return true;
}

int CColliderRect::Input(float fTime)
{
	return 0;
}

int CColliderRect::Update(float fTime)
{
	return 0;
}

int CColliderRect::LateUpdate(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() *
		m_pTransform->GetWorldScale();

	m_tInfo.vMin = vPos + m_tRelativeInfo.vMin +
		m_vPivot * m_tRelativeInfo.vLength;
	m_tInfo.vMax = vPos + m_tRelativeInfo.vMax +
		m_vPivot * m_tRelativeInfo.vLength;
	m_tInfo.vLength = m_tRelativeInfo.vLength;

	// 이 정보 자체가 사각형의 영역을 판단할 데이터로 사용된다.
	m_vSectionMin = m_tInfo.vMin;
	m_vSectionMax = m_tInfo.vMax;

	return 0;
}

void CColliderRect::Collision(float fTime)
{
}

void CColliderRect::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale;
	matPos.Translation(m_tInfo.vMin);
	matScale.Scaling(m_tInfo.vLength);

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

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tInfo, ((CColliderRect*)pDest)->m_tInfo);
	case CT_POINT:
		return CollisionRectToPoint(m_tInfo, ((CColliderPoint*)pDest)->GetInfo());
	case CT_OBB:
		return CollisionOBB2DToRect(((CColliderOBB*)pDest)->GetInfo(), m_tInfo);
	case CT_PIXEL:
		return CollisionRectToPixel(m_tInfo, ((CColliderPixel*)pDest)->GetInfo());
	}

	return false;
}

