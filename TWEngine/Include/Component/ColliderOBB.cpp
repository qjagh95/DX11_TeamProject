#include "EngineHeader.h"
#include "ColliderOBB.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Scene/Scene.h"

PUN_USING

CColliderOBB::CColliderOBB()
{
	m_eCollType = CT_OBB;

	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderRect");
}

CColliderOBB::CColliderOBB(const CColliderOBB & com)	:
	CCollider(com)
{
	*this = com;
}

CColliderOBB::~CColliderOBB()
{
}

OBB2DInfo CColliderOBB::GetInfo() const
{
	return m_tInfo;
}

void CColliderOBB::SetInfo(const Vector3 & vCenter, Vector3 vAxis[2], float fLength[2])
{
	m_tRelativeInfo.vCenter = vCenter;
	for (int i = 0; i < 2; ++i)
	{
		m_tRelativeInfo.vAxis[i] = vAxis[i];
		m_tRelativeInfo.fLength[i] = fLength[i];
		m_tInfo.fLength[i] = fLength[i];
	}
}

bool CColliderOBB::Init()
{
	return true;
}

int CColliderOBB::Input(float fTime)
{
	return 0;
}

int CColliderOBB::Update(float fTime)
{
	return 0;
}

int CColliderOBB::LateUpdate(float fTime)
{
	Vector3 vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() *
		m_pTransform->GetWorldScale();

	Matrix	matWorld = m_pTransform->GetRotDelta();
	memcpy(&matWorld._41, &m_pTransform->GetWorldPos(), sizeof(Vector3));

	m_tInfo.vCenter = m_tRelativeInfo.vCenter.TransformCoord(matWorld);

	Vector3 vAxis[2];
	for (int i = 0; i < 2; ++i)
	{
		m_tInfo.vAxis[i] = m_tRelativeInfo.vAxis[i].TransformNormal(matWorld);
		m_tInfo.vAxis[i].Normalize();
		vAxis[i] = m_tInfo.vAxis[i] * m_tInfo.fLength[i];

		// 이부분 수정하기
		//XMVectorMin()
		m_vSectionMax.x = (m_tInfo.vCenter + vAxis[0] + vAxis[1]).x;
		m_vSectionMax.y = (m_tInfo.vCenter - vAxis[0] + vAxis[1]).y;

		m_vSectionMin.x = (m_tInfo.vCenter - vAxis[0] - vAxis[1]).x;
		m_vSectionMin.y = (m_tInfo.vCenter + vAxis[0] - vAxis[1]).y;
	}

	return 0;
}

void CColliderOBB::Collision(float fTime)
{
}

void CColliderOBB::Render(float fTime)
{
	if (CCollisionManager::GetInst()->GetIsShow() == false)
		return;

	Matrix	matPos, matScale, matRot;

	// 크기를 구한다
	Vector3 vLength = Vector3(m_tInfo.fLength[0] * 2.f,
		m_tInfo.fLength[1] * 2.f, 1.f);

	// 회전 각도를 구한다. 회전은 2D이기 때문에 Z축 회전을 한다
	float	fAngle = m_tInfo.vAxis[0].Angle(Vector3(1.f, 0.f, 0.f));

	if (m_tInfo.vAxis[0].y < 0.f)
		fAngle = 360.f - fAngle;

	matRot.RotationZ(fAngle);
	matPos.Translation(m_tInfo.vCenter);
	matScale.Scaling(vLength);

	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	Matrix	matView;

	if (m_strCollisionGroup != "UI")
		matView = pMainCamera->GetViewMatrix();

	tCBuffer.matWorld = matScale * matRot * matPos;
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = Vector3(0.5f, 0.5f, 0.f);
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
}

CColliderOBB * CColliderOBB::Clone()
{
	return new CColliderOBB(*this);
}

bool CColliderOBB::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionOBB2DToRect(m_tInfo, ((CColliderRect*)pDest)->GetInfo());
	case CT_POINT:
		return CollisionOBB2DToPoint(m_tInfo, ((CColliderPoint*)pDest)->GetInfo());
	case CT_OBB:
		return CollisionOBB2DToOBB2D(m_tInfo, ((CColliderOBB*)pDest)->GetInfo());
	case CT_PIXEL:
		return false;
	}

	return false;
}
