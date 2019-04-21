#pragma once
#include "Component.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class PUN_DLL __declspec(align(16)) CTransform :
	public CComponent
{
	friend class CGameObject;

private:
	CTransform();
	CTransform(const CTransform& tr);
	virtual ~CTransform();

private:
	bool		m_bUpdate;
	bool		m_bStatic;
	int			m_iParentFlag;
	CTransform*	m_pParent;
	list<CTransform*>	m_ChildList;

public:
	void Static(bool bStatic);
	void SetParentFlag(int iFlag);
	void AddParentFlag(TRANSFORM_PARENT_FLAG eFlag);
	void DeleteParentFlag(TRANSFORM_PARENT_FLAG eFlag);
	void DeleteParentFlag();
	bool IsMoved();
	void RemoveChildList(CTransform* _pTr);
private:
	Vector3		m_vLocalScale;
	Vector3		m_vLocalRot;
	Vector3		m_vLocalPos;
	Vector3		m_vLocalAxis[AXIS_END];

	Matrix		m_matLocalScale;
	Matrix		m_matLocalRotX;
	Matrix		m_matLocalRotY;
	Matrix		m_matLocalRotZ;
	Matrix		m_matLocalRot;
	Matrix		m_matLocalPos;
	Matrix		m_matLocal;

public:
	void SetLocalScale(const Vector3& vScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalRot(const Vector3& vRot);
	void SetLocalRot(float x, float y, float z);
	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);
	void SetLocalPos(const Vector3& vPos);
	void SetLocalPos(float x, float y, float z);
	void ComputeLocalAxis();

public:
	Vector3 GetLocalScale()	const;
	Vector3 GetLocalRot()	const;
	Vector3 GetLocalPos()	const;
	Vector3 GetLocalAxis(AXIS eAxis)	const;
	const Vector3* GetLocalAxis()	const;
	Matrix GetLocalRotMatrix()	const;
	Matrix GetLocalMatrix()	const;
	Matrix GetBoneMatrix()	const;
	Matrix GetParentWorld() const;

private:
	Vector3		m_vWorldScale;
	Vector3		m_vWorldRot;
	Vector3		m_vWorldPos;
	Vector3		m_vWorldRelativeScale;
	Vector3		m_vWorldRelativeRot;
	Vector3		m_vWorldRelativePos;
	Vector3		m_vWorldAxis[AXIS_END];
	Vector3		m_vPivot;
	Vector3		m_vWorldMove;

	Matrix		m_matWorldScale;
	Matrix		m_matWorldRot;
	Matrix		m_matWorldPos;
	Matrix		m_matParent;
	Matrix		m_matWorld;	

	Vector3	m_vOffsetPos;
	Vector3	m_vOffsetRot;
	Matrix m_matBone;

	Matrix m_ParentScale;
	Matrix m_ParentPos;
	Matrix m_ParentRot;

	Matrix m_DeltaScale;
	Matrix m_DeltaRot;
	Matrix m_DeltaPos;
	Matrix m_ParentWorld;

public:
	void SetWorldScale(const Vector3& vScale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRot(const Vector3& vRot);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldPos(const Vector3& vPos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldRelativePos(const Vector3& vPos);
	void SetWorldRelativePos(float x, float y, float z);
	void SetWorldPivot(const Vector3& vPivot);
	void SetWorldPivot(float x, float y, float z);
	void ComputeWorldAxis();

public:
	Vector3 GetWorldScale()	const;
	Vector3 GetWorldRot()	const;
	Vector3 GetWorldPos()	const;
	Vector3 GetPivot()	const;
	Vector3 GetWorldAxis(AXIS eAxis)	const;
	const Vector3* GetWorldAxis()	const;
	Vector3 GetWorldMove()	const;
	Matrix GetWorldRotMatrix()	const;
	Matrix GetParentMatrix()	const;
	Matrix GetWorldMatrix()	const;
	Matrix GetWorldScaleMatrix() const;
	Matrix GetWorldPosMatrix() const;
	void SetBoneMatrix(const Matrix& matBone);
	void SetOffsetPos(const Vector3& vPos);
	void SetOffsetRot(const Vector3& vRot);

	void SetParentPos(const Matrix& parentPos);
	void SetParentRot(const Matrix& parentRot);
	void SetParentScale(const Matrix& parentScale);

	Matrix GetParentPos() const;
	Matrix GetParentRot() const;
	Matrix GetParentScale() const;
	Matrix GetRotDelta() const;
	Matrix GetPosDelta() const;
	Matrix GetScaleDelta() const;

	void SetUpdate(bool Value) { m_bUpdate = Value; }

public:
	void Move(AXIS eAxis, float fSpeed);
	void Move(AXIS eAxis, float fSpeed, float fTime);
	void Move(const Vector3& vDir, float fSpeed);
	void Move(const Vector3& vDir, float fSpeed, float fTime);
	void Move(const Vector3& vMove);
	void RotationX(float x);
	void RotationX(float x, float fTime);
	void RotationY(float y);
	void RotationY(float y, float fTime);
	void RotationZ(float z);
	void RotationZ(float z, float fTime);
	void Rotation(const Vector3& vRot, float fTime);
	void Rotation(const Vector3& vRot);
	void LookAt(class CGameObject* pObj, AXIS eAxis = AXIS_Z);
	void LookAt(class CComponent* pCom, AXIS eAxis = AXIS_Z);
	void LookAt(const Vector3& vPos, AXIS eAxis = AXIS_Z);
	void LookAtY(class CGameObject* pObj);
	void LookAtY(class CComponent* pCom);
	void LookAtY(const Vector3& vPos);

	void LookAt2D(const Vector3& vPos);
	void UpdateTransform();

private:
	void ScaleParent();
	void RotParent();

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CTransform* Clone();
};

PUN_END