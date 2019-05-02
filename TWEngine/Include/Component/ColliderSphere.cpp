#include "../EngineHeader.h"
#include "ColliderSphere.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderOBB.h"
#include "ColliderRay.h"
#include "ColliderPoint.h"
#include "ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Rendering/ShaderManager.h"

PUN_USING

CColliderSphere::CColliderSphere()
{
	m_eCollType = CT_SPHERE;
	m_pWireFrame = GET_SINGLE(CRenderManager)->FindRenderState(WIRE_FRAME);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Sky");
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
	if (CCollisionManager::GetInst()->GetIsShow() == false)
		return;

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
}

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

void CColliderSphere::Save(BinaryWrite * _pInstBW)
{
	//// Save
//Tag 저장
	_pInstBW->WriteData(m_strTag);
	//Collider Info 저장
	_pInstBW->WriteData(m_tInfo.vCenter);
	_pInstBW->WriteData(m_tInfo.fRadius);
	_pInstBW->WriteData(m_tRelativeInfo.vCenter);
	_pInstBW->WriteData(m_tRelativeInfo.fRadius);
	_pInstBW->WriteData(m_strCollisionGroup);
	_pInstBW->WriteData(m_MyTypeName);

	int iVecExceptTypeNameSize = m_vecContinueName.size();
	_pInstBW->WriteData(iVecExceptTypeNameSize);

	for (size_t i = 0; i < iVecExceptTypeNameSize; ++i)
	{
		_pInstBW->WriteData(m_vecContinueName[i]);
	}

	_pInstBW->WriteData((int)m_eColliderID);
	_pInstBW->WriteData((int)m_eCollType);
}

void CColliderSphere::Load(BinaryRead * _pInstBR)
{
	//Tag 저장
	_pInstBR->ReadData(m_strTag);
	//Collider Info 저장
	_pInstBR->ReadData(m_tInfo.vCenter);
	_pInstBR->ReadData(m_tInfo.fRadius);
	_pInstBR->ReadData(m_tRelativeInfo.vCenter);
	_pInstBR->ReadData(m_tRelativeInfo.fRadius);
	_pInstBR->ReadData(m_strCollisionGroup);
	_pInstBR->ReadData(m_MyTypeName);

	int iVecExceptTypeNameSize = 0;
	_pInstBR->ReadData(iVecExceptTypeNameSize);

	for (size_t i = 0; i < iVecExceptTypeNameSize; ++i)
	{
		string strTemp;
		_pInstBR->ReadData(strTemp);
		m_vecContinueName.push_back(strTemp);
	}
	int iColliderID = 0;
	_pInstBR->ReadData(iColliderID);
	m_eColliderID = ((COLLIDER_ID)iColliderID);
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
