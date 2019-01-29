#include "stdafx.h"
#include "Transform.h"
#include "../GameObject.h"

PUN_USING

CTransform::CTransform()
{
	m_eComType = CT_TRANSFORM;
	m_bUpdate = true;
	m_bStatic = false;
	m_iParentFlag = 0;
	m_pParent = nullptr;
}

CTransform::CTransform(const CTransform & tr)
{
	*this = tr;
	m_bUpdate = true;
	m_pParent = nullptr;
}

CTransform::~CTransform()
{
	Safe_Release_VecList(m_ChildList);
}

void CTransform::Static(bool bStatic)
{
	m_bStatic = bStatic;
}

void CTransform::SetParentFlag(int iFlag)
{
	m_iParentFlag = iFlag;
}

void CTransform::AddParentFlag(TRANSFORM_PARENT_FLAG eFlag)
{
	m_iParentFlag |= eFlag;
}

void CTransform::DeleteParentFlag(TRANSFORM_PARENT_FLAG eFlag)
{
	if (m_iParentFlag & eFlag)
		m_iParentFlag ^= eFlag;
}

void CTransform::DeleteParentFlag()
{
	m_iParentFlag = 0;
}

void CTransform::SetLocalScale(const Vector3 & vScale)
{
	m_vLocalScale = vScale;

	m_matLocalScale.Scaling(m_vLocalScale);

	m_bUpdate = true;
}

void CTransform::SetLocalScale(float x, float y, float z)
{
	m_vLocalScale = Vector3(x, y, z);

	m_matLocalScale.Scaling(m_vLocalScale);

	m_bUpdate = true;
}

void CTransform::SetLocalRot(const Vector3 & vRot)
{
	m_vLocalRot = vRot;

	m_matLocalRot.Rotation(m_vLocalRot);

	m_bUpdate = true;

	ComputeLocalAxis();
}

void CTransform::SetLocalRot(float x, float y, float z)
{
	m_vLocalRot = Vector3(x, y, z);

	m_matLocalRot.Rotation(m_vLocalRot);

	m_bUpdate = true;

	ComputeLocalAxis();
}

void CTransform::SetLocalRotX(float x)
{
	m_vLocalRot.x = x;

	m_matLocalRot.Rotation(m_vLocalRot);

	m_bUpdate = true;

	ComputeLocalAxis();
}

void CTransform::SetLocalRotY(float y)
{
	m_vLocalRot.y = y;

	m_matLocalRot.Rotation(m_vLocalRot);

	m_bUpdate = true;

	ComputeLocalAxis();
}

void CTransform::SetLocalRotZ(float z)
{
	m_vLocalRot.z = z;

	m_matLocalRot.Rotation(m_vLocalRot);

	m_bUpdate = true;

	ComputeLocalAxis();
}

void CTransform::SetLocalPos(const Vector3 & vPos)
{
	m_vLocalPos = vPos;

	m_matLocalPos.Translation(m_vLocalPos);

	m_bUpdate = true;
}

void CTransform::SetLocalPos(float x, float y, float z)
{
	m_vLocalPos = Vector3(x, y, z);

	m_matLocalPos.Translation(m_vLocalPos);

	m_bUpdate = true;
}

void CTransform::ComputeLocalAxis()
{
	Matrix	matRot = m_matLocalRot;

	if (m_pParent)
		matRot *= m_pParent->GetLocalRotMatrix();

	for (int i = 0; i < 3; ++i)
	{
		m_vLocalAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vLocalAxis[i].Normalize();
	}
}

Vector3 CTransform::GetLocalScale() const
{
	return m_vLocalScale;
}

Vector3 CTransform::GetLocalRot() const
{
	return m_vLocalRot;
}

Vector3 CTransform::GetLocalPos() const
{
	return m_vLocalPos;
}

Vector3 CTransform::GetLocalAxis(AXIS eAxis) const
{
	return m_vLocalAxis[eAxis];
}

const Vector3 * CTransform::GetLocalAxis() const
{
	return m_vLocalAxis;
}

Matrix CTransform::GetLocalRotMatrix() const
{
	return m_matLocalRot;
}

Matrix CTransform::GetLocalMatrix() const
{
	return m_matLocal;
}

// =============== World ======================
void CTransform::SetWorldScale(const Vector3 & vScale)
{
	m_vWorldScale = vScale;

	ScaleParent();
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vWorldScale = Vector3(x, y, z);

	ScaleParent();
}

void CTransform::SetWorldRot(const Vector3 & vRot)
{
	m_vWorldRot = vRot;

	m_matWorldRot.Rotation(m_vWorldRot);

	m_bUpdate = true;

	ComputeWorldAxis();
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vWorldRot = Vector3(x, y, z);

	m_matWorldRot.Rotation(m_vWorldRot);

	m_bUpdate = true;

	ComputeWorldAxis();
}

void CTransform::SetWorldRotX(float x)
{
	m_vWorldRot.x = x;

	m_matWorldRot.Rotation(m_vWorldRot);

	m_bUpdate = true;

	ComputeWorldAxis();
}

void CTransform::SetWorldRotY(float y)
{
	m_vWorldRot.y = y;

	m_matWorldRot.Rotation(m_vWorldRot);

	m_bUpdate = true;

	ComputeWorldAxis();
}

void CTransform::SetWorldRotZ(float z)
{
	m_vWorldRot.z = z;

	m_matWorldRot.Rotation(m_vWorldRot);

	m_bUpdate = true;

	ComputeWorldAxis();
}

void CTransform::SetWorldPos(const Vector3 & vPos)
{
	m_vWorldMove = vPos - m_vWorldPos;
	m_vWorldRelativePos += m_vWorldMove;

	m_vWorldPos = vPos;

	PosParent();
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	Vector3 vPos(x, y, z);
	m_vWorldMove = vPos - m_vWorldPos;
	m_vWorldRelativePos += m_vWorldMove;

	m_vWorldPos = vPos;

	PosParent();
}

void CTransform::SetWorldRelativePos(const Vector3 & vPos)
{
	m_vWorldRelativePos = vPos;

	PosParent();
}

void CTransform::SetWorldRelativePos(float x, float y, float z)
{
	m_vWorldRelativePos = Vector3(x, y, z);

	PosParent();
}

void CTransform::SetWorldPivot(const Vector3 & vPivot)
{
	m_vPivot = vPivot;
}

void CTransform::SetWorldPivot(float x, float y, float z)
{
	m_vPivot = Vector3(x, y, z);
}

void CTransform::ComputeWorldAxis()
{
	Matrix matRot = m_matWorldRot;

	if (m_pParent)
		matRot *= m_pParent->GetWorldRotMatrix();

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}
}

Vector3 CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CTransform::GetPivot() const
{
	return m_vPivot;
}

Vector3 CTransform::GetWorldAxis(AXIS eAxis) const
{
	return m_vWorldAxis[eAxis];
}

const Vector3 * CTransform::GetWorldAxis() const
{
	return m_vWorldAxis;
}

Vector3 CTransform::GetWorldMove() const
{
	return m_vWorldMove;
}

Matrix CTransform::GetWorldRotMatrix() const
{
	return m_matWorldRot;
}

Matrix CTransform::GetParentMatrix() const
{
	return m_matParent;
}

Matrix CTransform::GetWorldMatrix() const
{
	return m_matWorld;
}

Matrix CTransform::GetParentMatrixFromNoScale() const
{
	if (!m_pParent)
		return m_matWorldRot * m_matWorldPos;

	return m_matWorldRot * m_matWorldPos * m_pParent->GetParentMatrixFromNoScale();
}

Matrix CTransform::GetParentRotMatrix() const
{
	if (!m_pParent)
		return m_matWorldRot;

	return m_matWorldRot * m_pParent->GetParentRotMatrix();
}

Matrix CTransform::GetParentPosMatrix() const
{
	if (!m_pParent)
		return m_matWorldPos;

	return m_matWorldPos * m_pParent->GetParentPosMatrix();
}

void CTransform::Move(AXIS eAxis, float fSpeed)
{
	Move(Vector3(m_vWorldAxis[eAxis] * fSpeed));
}

void CTransform::Move(AXIS eAxis, float fSpeed, float fTime)
{
	Move(Vector3(m_vWorldAxis[eAxis] * fSpeed * fTime));
}

void CTransform::Move(const Vector3 & vDir, float fSpeed)
{
	Move(Vector3(vDir * fSpeed));
}

void CTransform::Move(const Vector3 & vDir, float fSpeed, float fTime)
{
	Move(Vector3(vDir * fSpeed * fTime));
}

void CTransform::Move(const Vector3 & vMove)
{
	m_vWorldRelativePos += vMove;
	m_vWorldPos += vMove;

	m_vWorldMove = vMove;

	PosParent();
}

void CTransform::RotationX(float x)
{
	Rotation(Vector3(x, 0.f, 0.f));
}

void CTransform::RotationX(float x, float fTime)
{
	Rotation(Vector3(x * fTime, 0.f, 0.f));
}

void CTransform::RotationY(float y)
{
	Rotation(Vector3(0.f, y, 0.f));
}

void CTransform::RotationY(float y, float fTime)
{
	Rotation(Vector3(0.f, y * fTime, 0.f));
}

void CTransform::RotationZ(float z)
{
	Rotation(Vector3(0.f, 0.f, z));
}

void CTransform::RotationZ(float z, float fTime)
{
	Rotation(Vector3(0.f, 0.f, z * fTime));
}

void CTransform::Rotation(const Vector3 & vRot, float fTime)
{
	Rotation(vRot * fTime);
}

void CTransform::Rotation(const Vector3 & vRot)
{
	m_vWorldRot += vRot;

	//if (m_vWorldRot.z >= 360.f)
	//	m_vWorldRot.z -= 360.f;

	m_matWorldRot.Rotation(m_vWorldRot);

	ComputeWorldAxis();

	PosParent();
}

void CTransform::LookAt(CGameObject * pObj, AXIS eAxis)
{
	CTransform*	pTr = pObj->GetTransform();

	LookAt(pTr->GetWorldPos(), eAxis);
	SAFE_RELEASE(pTr);
}

void CTransform::LookAt(CComponent * pCom, AXIS eAxis)
{
	CTransform*	pTr = pCom->GetTransform();

	LookAt(pTr->GetWorldPos(), eAxis);
	SAFE_RELEASE(pTr);
}

void CTransform::LookAt(const Vector3 & vPos, AXIS eAxis)
{
	// 바라보는 방향을 구한다
	Vector3 vView = vPos - m_vWorldPos;

	vView.Normalize();

	Vector3 vAxis = Vector3::Axis[eAxis];

	// 각도를 구한다
	float	fAngle = vAxis.Angle(vView);

	Vector3 vRotAxis = vAxis.Cross(vView);
	vRotAxis.Normalize();

	if (vRotAxis != Vector3::Zero)
	{
		m_matWorldRot.RotationAxis(fAngle, vRotAxis);
	}

	ComputeWorldAxis();

	UpdateTransform();
}

void CTransform::LookAt2D(const Vector3& vPos)
{
	if (m_vWorldPos == vPos)
		return;

	Vector3 vView = vPos - m_vWorldPos;

	vView.Normalize();

	// 각도를 구한다
	float	fAngle = m_vWorldAxis[AXIS_Z].Angle(vView);

	if (isnan(fAngle) || fAngle == 0.f)
		return;

	Vector3 vAxis = m_vWorldAxis[AXIS_Z].Cross(vView);
	vAxis.Normalize();

	if (vAxis.z < 0.f)
		fAngle *= -1;

	RotationZ(fAngle);
}

void CTransform::UpdateTransform()
{
	m_bUpdate = true;

	if (!m_ChildList.empty())
	{
		list<CTransform*>::iterator	iter;
		list<CTransform*>::iterator	iterEnd = m_ChildList.end();

		for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->UpdateTransform();
		}
	}
}

void CTransform::ScaleParent()
{
	if (!m_pParent)
		m_vWorldRelativeScale = m_vWorldScale;

	else if (!(m_iParentFlag & TPF_SCALE))
		m_vWorldRelativeScale = m_vWorldScale / m_pParent->m_vWorldScale;

	else
		m_vWorldRelativeScale = m_vWorldScale / m_pParent->m_vWorldScale;

	m_matWorldScale.Scaling(m_vWorldScale);

	m_bUpdate = true;

	if (!m_ChildList.empty())
	{
		list<CTransform*>::iterator	iter;
		list<CTransform*>::iterator	iterEnd = m_ChildList.end();

		for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->ScaleParent();
		}
	}
}

void CTransform::PosParent()
{
	if (!m_pParent)
		m_vWorldRelativePos = m_vWorldPos;

	else if (!(m_iParentFlag & TPF_POS))
		m_vWorldRelativePos = m_vWorldPos;

	m_matWorldPos.Translation(m_vWorldRelativePos);

	if (m_pParent)
	{
		Matrix	matParent;
		if (m_iParentFlag & TPF_POS && m_iParentFlag & TPF_ROT)
		{
			matParent = m_pParent->GetParentMatrixFromNoScale();
		}

		else if (m_iParentFlag & TPF_POS)
			matParent = m_pParent->GetParentPosMatrix();

		else if (m_iParentFlag & TPF_ROT)
			matParent = m_pParent->GetParentRotMatrix();

		else
			matParent.Identity();

		Vector3 vPos = m_vWorldRelativePos.TransformCoord(matParent);

		m_vWorldMove = vPos - m_vWorldPos;
		m_vWorldPos = vPos;
	}

	m_bUpdate = true;

	if (!m_ChildList.empty())
	{
		list<CTransform*>::iterator	iter;
		list<CTransform*>::iterator	iterEnd = m_ChildList.end();

		for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->PosParent();
		}
	}
}

bool CTransform::Init()
{
	for (int i = 0; i < 3; ++i)
	{
		m_vLocalAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	return true;
}

int CTransform::Update(float fTime)
{
	if (m_bStatic)
		return 0;

	else if (!m_bUpdate)
		return 0;

	m_matLocal = m_matLocalScale * m_matLocalRot *
		m_matLocalPos;

	if (m_pParent)
	{
		if (m_iParentFlag & TPF_ROT && m_iParentFlag & TPF_POS)
			m_matParent = m_pParent->GetParentMatrixFromNoScale();

		else if (m_iParentFlag & TPF_ROT)
			m_matParent = m_pParent->GetParentRotMatrix();

		else if (m_iParentFlag & TPF_POS)
			m_matParent = m_pParent->GetParentPosMatrix();

		else
			m_matParent.Identity();
	}
	
	m_matWorld = m_matWorldScale * m_matWorldPos * m_matParent;
	//m_matWorld = m_matWorldScale * m_matWorldPos * m_matParent * m_matWorldRot;
	
	m_bUpdate = false;

	return 0;
}

int CTransform::LateUpdate(float fTime)
{
	if (m_bStatic)
		return 0;

	else if (!m_bUpdate)
		return 0;

	m_matLocal = m_matLocalScale * m_matLocalRot *
		m_matLocalPos;

	if (m_pParent)
	{
		if (m_iParentFlag & TPF_ROT && m_iParentFlag & TPF_POS)
			m_matParent = m_pParent->GetParentMatrixFromNoScale();

		else if (m_iParentFlag & TPF_ROT)
			m_matParent = m_pParent->GetParentRotMatrix();

		else if (m_iParentFlag & TPF_POS)
			m_matParent = m_pParent->GetParentPosMatrix();

		else
			m_matParent.Identity();
	}

	m_matWorld = m_matWorldScale * m_matWorldRot *
		m_matWorldPos * m_matParent;

	m_bUpdate = false;

	return 0;
}

void CTransform::Collision(float fTime)
{
}

void CTransform::Render(float fTime)
{
	m_vWorldMove = Vector3::Zero;
}

CTransform * CTransform::Clone()
{
	return new CTransform(*this);
}
