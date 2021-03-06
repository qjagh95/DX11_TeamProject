#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL __declspec(align(16)) CCamera :
	public CComponent
{
	friend class CGameObject;

protected:
	CCamera();
	CCamera(const CCamera& camera);
	virtual ~CCamera();

private:
	Matrix		m_matView;
	Matrix		m_matProj;
	Matrix		m_matVP;
	Matrix		m_matShadowView;
	Matrix		m_matShadowProj;
	Matrix		m_matShadowVP;

	CAMERA_TYPE	m_eCameraType;
	float		m_fWidth;
	float		m_fHeight;
	float		m_fViewAngle;
	float		m_fNear;
	float		m_fFar;
	class CTransform*	m_pTarget;
	class CLayer*		m_pLightLayer;
	Vector3				m_vTargetPivot;
	Vector3				m_vWorldSize;
	Vector3				m_vTargetPos;
	class CFrustum*		m_pFrustum;

	bool m_bShadow;
	bool m_bFrustum;
	class CTransform*	m_pShadowLight;

public:
	float GetCameraFar() const { return m_fFar; }
	float GetCameraNear() const { return m_fNear; }
	float GetCameraViewAngle() const { return m_fViewAngle; }
	void SetShadowLight(class CTransform* pShadowLight);
	void Shadow(bool bEnable);
	Matrix GetShadowViewMatrix() const;
	Matrix GetShadowProjMatrix() const;
	Matrix GetShadowVP() const;
	void SetShadowVP(const Matrix& matShadowVP);
	bool IsShadow()	const;
	void NoShadow();
	void SetFrustumCulling(bool Value) { m_bFrustum = Value; }
	bool GetFrustumCulling() const { return m_bFrustum; }

public:
	void SetTarget(class CGameObject* pTarget);
	void SetTarget(class CComponent* pTarget);
	void SetTargetPivot(Vector3 vTargetPivot);
	void SetWorldSize(Vector3 vWorldSize);
	void SetLightLayer(class CLayer* pLayer);

public:
	Matrix	GetViewMatrix()	const;
	Matrix	GetProjMatrix()	const;
	Matrix  GetVP() const;

public:
	void SetCameraType(CAMERA_TYPE eType);
	void SetCameraInfo(CAMERA_TYPE eType, float fWidth, float fHeight, float fViewAngle, float fNear, float fFar);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetViewAngle(float fAngle);
	void SetNear(float fNear);
	void SetFar(float fFar);
	bool FrustumInPoint(Vector3& vPos);
	bool FrustumInSphere(Vector3& vCenter, float fRadius);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCamera* Clone();
};

PUN_END