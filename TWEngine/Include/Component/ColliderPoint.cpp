#include "EngineHeader.h"
#include "ColliderPoint.h"
#include "Transform.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "ColliderRect.h"
#include "ColliderOBB.h"
#include "ColliderPixel.h"

PUN_USING

CColliderPoint::CColliderPoint()
{
	m_eCollType = CT_POINT;
#ifdef _DEBUG
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderRect");
#endif // _DEBUG
}

CColliderPoint::CColliderPoint(const CColliderPoint & com) :
	CCollider(com)
{
	*this = com;
}


CColliderPoint::~CColliderPoint()
{
}

Vector3 CColliderPoint::GetInfo() const
{
	return m_vPos;
}

void CColliderPoint::SetInfo(const Vector3 & vRelativePos)
{
	m_vRelativePos = vRelativePos;
}

bool CColliderPoint::Init()
{
	return true;
}

int CColliderPoint::Input(float fTime)
{
	return 0;
}

int CColliderPoint::Update(float fTime)
{
	return 0;
}

int CColliderPoint::LateUpdate(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos();

	m_vPos = vPos + m_vRelativePos;

	// 이 정보 자체가 사각형의 영역을 판단할 데이터로 사용된다.
	m_vSectionMin = m_vPos;
	m_vSectionMax = m_vPos;

	return 0;
}

void CColliderPoint::Collision(float fTime)
{
}

void CColliderPoint::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale;
	matPos.Translation(m_vPos - Vector3(1.5f, 1.5f, 0.f));
	matScale.Scaling(3.f, 3.f, 1.f);

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
	tCBuffer.vPivot = Vector3(0.f, 0.f, 0.f);
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

CColliderPoint * CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPoint(((CColliderRect*)pDest)->GetInfo(), m_vPos);
	case CT_POINT:
		return m_vPos == ((CColliderPoint*)pDest)->m_vPos;
	case CT_OBB:
		return CollisionOBB2DToPoint(((CColliderOBB*)pDest)->GetInfo(), m_vPos);
	case CT_PIXEL:
		return false;
	}

	return false;
}

