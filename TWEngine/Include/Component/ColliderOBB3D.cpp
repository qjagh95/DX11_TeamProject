#include "../EngineHeader.h"
#include "ColliderOBB3D.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "ColliderRay.h"

PUN_USING

CColliderOBB3D::CColliderOBB3D()
{
	m_eCollType = CT_OBB3D;
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderBox");
}

CColliderOBB3D::CColliderOBB3D(const CColliderOBB3D & com) :
	CCollider(com)
{
	*this = com;
}

CColliderOBB3D::~CColliderOBB3D()
{
}

OBB3DInfo CColliderOBB3D::GetInfo() const
{
	return m_tInfo;
}

void CColliderOBB3D::SetInfo(const Vector3 & vCenter,
	Vector3 vAxis[3], const Vector3& vLength)
{
	m_tRelativeInfo.vCenter = vCenter;
	m_tRelativeInfo.vLength = vLength;
	m_tInfo.vLength = vLength;
	for (int i = 0; i < 3; ++i)
	{
		m_tRelativeInfo.vAxis[i] = vAxis[i];
	}
}

bool CColliderOBB3D::Init()
{
	return true;
}

int CColliderOBB3D::Input(float fTime)
{
	return 0;
}

int CColliderOBB3D::Update(float fTime)
{
	return 0;
}

int CColliderOBB3D::LateUpdate(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() * m_pTransform->GetWorldScale();

	Matrix	matWorld;
	matWorld = m_pTransform->GetBoneMatrix() * m_pTransform->GetScaleDelta();
	matWorld *= m_pTransform->GetRotDelta();
	matWorld *= m_pTransform->GetPosDelta();

	m_tInfo.vCenter = m_tRelativeInfo.vCenter.TransformCoord(matWorld);

	for (int i = 0; i < 3; ++i)
	{
		m_tInfo.vAxis[i] = m_tRelativeInfo.vAxis[i].TransformNormal(matWorld);
		m_tInfo.vAxis[i].Normalize();
	}

	Vector3	vBoxPos[8] =
	{
		Vector3(-m_tInfo.vLength.x, m_tInfo.vLength.y, -m_tInfo.vLength.z),
		Vector3(m_tInfo.vLength.x, m_tInfo.vLength.y, -m_tInfo.vLength.z),
		Vector3(-m_tInfo.vLength.x, -m_tInfo.vLength.y, -m_tInfo.vLength.z),
		Vector3(m_tInfo.vLength.x, -m_tInfo.vLength.y, -m_tInfo.vLength.z),
		Vector3(-m_tInfo.vLength.x, m_tInfo.vLength.y, m_tInfo.vLength.z),
		Vector3(m_tInfo.vLength.x, m_tInfo.vLength.y, m_tInfo.vLength.z),
		Vector3(-m_tInfo.vLength.x, -m_tInfo.vLength.y, m_tInfo.vLength.z),
		Vector3(m_tInfo.vLength.x, -m_tInfo.vLength.y, m_tInfo.vLength.z)
	};

	for (int i = 0; i < 8; ++i)
	{
		vBoxPos[i] = vBoxPos[i].TransformCoord(m_pTransform->GetRotDelta());
		vBoxPos[i] += m_tInfo.vCenter;
	}

	m_vSectionMin = vBoxPos[0];
	m_vSectionMax = vBoxPos[0];

	for (int i = 1; i < 8; ++i)
	{
		if (m_vSectionMin.x > vBoxPos[i].x)
			m_vSectionMin.x = vBoxPos[i].x;

		if (m_vSectionMin.y > vBoxPos[i].y)
			m_vSectionMin.y = vBoxPos[i].y;

		if (m_vSectionMin.z > vBoxPos[i].z)
			m_vSectionMin.z = vBoxPos[i].z;

		if (m_vSectionMax.x < vBoxPos[i].x)
			m_vSectionMax.x = vBoxPos[i].x;

		if (m_vSectionMax.y < vBoxPos[i].y)
			m_vSectionMax.y = vBoxPos[i].y;

		if (m_vSectionMax.z < vBoxPos[i].z)
			m_vSectionMax.z = vBoxPos[i].z;
	}

	return 0;
}

void CColliderOBB3D::Collision(float fTime)
{
}

void CColliderOBB3D::Render(float fTime)
{
	if (CCollisionManager::GetInst()->GetIsShow() == false)
		return;

	Matrix	matPos, matScale, matRot;

	for (int i = 0; i < AXIS_END; ++i)
	{
		memcpy(&matRot[i][0], &m_tInfo.vAxis[i], sizeof(Vector3));
	}

	matPos.Translation(m_tInfo.vCenter);
	matScale.Scaling(m_tInfo.vLength);

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

	CCollider::Render(fTime);
}

CColliderOBB3D * CColliderOBB3D::Clone()
{
	return new CColliderOBB3D(*this);
}

void CColliderOBB3D::Save(BinaryWrite * _pInstBW)
{
	//// Save
	//Tag 历厘
	_pInstBW->WriteData(m_strTag);
	//Collider Info 历厘
	_pInstBW->WriteData(m_tInfo.vCenter);
	_pInstBW->WriteData(m_tInfo.vAxis[0]);
	_pInstBW->WriteData(m_tInfo.vAxis[1]);
	_pInstBW->WriteData(m_tInfo.vAxis[2]);
	_pInstBW->WriteData(m_tInfo.vLength);
	_pInstBW->WriteData(m_tRelativeInfo.vCenter);
	_pInstBW->WriteData(m_tRelativeInfo.vAxis[0]);
	_pInstBW->WriteData(m_tRelativeInfo.vAxis[1]);
	_pInstBW->WriteData(m_tRelativeInfo.vAxis[2]);
	_pInstBW->WriteData(m_tRelativeInfo.vLength);
	_pInstBW->WriteData(m_strCollisionGroup);
	_pInstBW->WriteData(m_MyTypeName);
	
	int iVecExceptTypeNameSize = (int)m_vecContinueName.size();
	_pInstBW->WriteData(iVecExceptTypeNameSize);

	for (size_t i = 0; i < iVecExceptTypeNameSize; ++i)
	{
		_pInstBW->WriteData(m_vecContinueName[i]);
	}

	_pInstBW->WriteData((int)m_eColliderID);

}

void CColliderOBB3D::Load(BinaryRead * _pInstBR)
{
	//Tag 历厘
	_pInstBR->ReadData(m_strTag);
	//Collider Info 历厘
	_pInstBR->ReadData(m_tInfo.vCenter);
	_pInstBR->ReadData(m_tInfo.vAxis[0]);
	_pInstBR->ReadData(m_tInfo.vAxis[1]);
	_pInstBR->ReadData(m_tInfo.vAxis[2]);
	_pInstBR->ReadData(m_tInfo.vLength);
	_pInstBR->ReadData(m_tRelativeInfo.vCenter);
	_pInstBR->ReadData(m_tRelativeInfo.vAxis[0]);
	_pInstBR->ReadData(m_tRelativeInfo.vAxis[1]);
	_pInstBR->ReadData(m_tRelativeInfo.vAxis[2]);
	_pInstBR->ReadData(m_tRelativeInfo.vLength);
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

bool CColliderOBB3D::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_OBB3D:
		return CollisionOBB3DToOBB3D(m_tInfo, ((CColliderOBB3D*)pDest)->GetInfo());
	case CT_RAY:
		return CollisionRayToOBB3D(m_tInfo, ((CColliderRay*)pDest)->GetInfo(), m_pTransform);
	}

	return false;
}
