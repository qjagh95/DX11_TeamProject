#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL __declspec(align(16)) CEditorCamera :
	public CComponent
{
	friend class CGameObject;

protected:
	CEditorCamera();
	CEditorCamera(const CEditorCamera& editcamera);
	virtual ~CEditorCamera();

private:
	Matrix	m_matView;
	Matrix	m_matProj;
	CAMERA_TYPE	m_eCameraType;
	float	m_fWidth;
	float	m_fHeight;
	float	m_fViewAngle;
	float	m_fNear;
	float	m_fFar;
	class CTransform*	m_pTarget;

public:
	void SetTarget(class CGameObject* pTarget);
	void SetTarget(class CComponent* pTarget);

public:
	Matrix GetViewMatrix()	const;
	Matrix GetProjMatrix()	const;

public:
	void SetCameraType(CAMERA_TYPE eType);
	void SetCameraInfo(CAMERA_TYPE eType, float fWidth, float fHeight,
		float fViewAngle, float fNear, float fFar);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetViewAngle(float fAngle);
	void SetNear(float fNear);
	void SetFar(float fFar);

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEditorCamera* Clone();
};

PUN_END