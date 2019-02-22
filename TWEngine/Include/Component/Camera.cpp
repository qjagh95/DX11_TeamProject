#include "EngineHeader.h"
#include "Camera.h"
#include "Transform.h"
#include "../GameObject.h"
#include "Device.h"

PUN_USING

CCamera::CCamera()
{
	m_eComType = CT_CAMERA;
	m_eCameraType = CT_PERSPECTIVE;
	m_pTarget = nullptr;
}


CCamera::CCamera(const CCamera & camera) :
	CComponent(camera)
{
	m_eCameraType = camera.m_eCameraType;
	m_matView = camera.m_matView;
	m_matProj = camera.m_matProj;
	m_pTarget = nullptr;
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pTarget);
}

void CCamera::SetTarget(CGameObject * pTarget)
{
	m_pTarget = pTarget->GetTransform();
}

void CCamera::SetTarget(CComponent * pTarget)
{
	m_pTarget = pTarget->GetTransform();
}

Matrix CCamera::GetViewMatrix()	const
{
	return m_matView;
}

Matrix CCamera::GetProjMatrix() const
{
	return m_matProj;
}

void CCamera::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
	SetCameraType(m_eCameraType);
}

void CCamera::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
	SetCameraType(m_eCameraType);
}

void CCamera::SetViewAngle(float fAngle)
{
	m_fViewAngle = fAngle;
	SetCameraType(m_eCameraType);
}

void CCamera::SetNear(float fNear)
{
	m_fNear = fNear;
	SetCameraType(m_eCameraType);
}

void CCamera::SetFar(float fFar)
{
	m_fFar = fFar;
	SetCameraType(m_eCameraType);
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
	m_eCameraType = eType;

	switch (eType)
	{
	case CT_PERSPECTIVE:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fViewAngle),
			m_fWidth / m_fHeight, m_fNear, m_fFar);
		break;
	case CT_ORTHO:
		/*
		0, 0.5, 0, 1		100 0   0 0
		0.5, -0.5, 0, 1		0   100 0 0
		-0.5, -0.5, 0, 1	0   0   1 0
							100 100 0 1
		100, 150, 0, 1		1/400 0      0 0
		150, 50, 0, 1		0     1/-300 0 0
		50, 50, 0, 1		0     0      1 0
							-1    1      1 1
		-0.75, 0.5
		-0.625, 0.83
		2/(r-l)      0            0           0
		0            2/(t-b)      0           0
		0            0            1/(zf-zn)   0
		(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
		*/
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, m_fWidth,
			0.f, m_fHeight, m_fNear, m_fFar);
		break;
	}
}

void CCamera::SetCameraInfo(CAMERA_TYPE eType, float fWidth, float fHeight,
	float fViewAngle, float fNear, float fFar)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fViewAngle = fViewAngle;
	m_fNear = fNear;
	m_fFar = fFar;

	SetCameraType(eType);
}


bool CCamera::Init()
{
	return true;
}

int CCamera::Input(float fTime)
{
	return 0;
}

int CCamera::Update(float fTime)
{
	m_matView.Identity();

	if (m_pTarget)
	{
		m_pTransform->SetWorldPos(m_pTarget->GetWorldPos());
		Vector3	vMove = m_pTarget->GetWorldMove();

		m_pTransform->SetWorldRot(m_pTarget->GetWorldRot());

		if (vMove != Vector3::Zero)
		{
			m_pTransform->Move(vMove);
		}
	}

	Vector3	vPos = m_pTransform->GetWorldPos() + m_pTransform->GetLocalPos();
	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/
	for (int i = 0; i < AXIS_END; ++i)
	{
		memcpy(&m_matView[i][0], &m_pTransform->GetWorldAxis((AXIS)i),
			sizeof(Vector3));
	}

	/*
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/
	m_matView.Transpose();

	vPos *= -1.f;

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_matView[3][i] = vPos.Dot(m_pTransform->GetWorldAxis((AXIS)i));
	}


	return 0;
}

int CCamera::LateUpdate(float fTime)
{
	return 0;
}

void CCamera::Collision(float fTime)
{
}

void CCamera::Render(float fTime)
{
}

CCamera * CCamera::Clone()
{
	return new CCamera(*this);
}
