#include "../EngineHeader.h"
#include "Arm.h"
#include "Transform.h"
#include "../Input.h"
#include "../Device.h"
#include "../GameObject.h"

PUN_USING

CArm::CArm() :
	m_bMouseAction(false),
	m_pTarget(nullptr)
{
	m_eComType = CT_ARM;
	m_fDistance = 5.f;
}

CArm::CArm(const CArm & arm) :
	CComponent(arm)
{
	m_fSpeed = arm.m_fSpeed;
	m_bMouseAction = arm.m_bMouseAction;
	m_pTarget = arm.m_pTarget;
	m_fDistance = arm.m_fDistance;

	if (m_pTarget)
		m_pTarget->AddRef();
}

CArm::~CArm()
{
	SAFE_RELEASE(m_pTarget);
}

void CArm::SetTarget(CGameObject * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget->GetTransform();

	m_pTransform->SetWorldRot(m_pTarget->GetWorldRot());
	m_pTransform->SetWorldPos(m_pTarget->GetWorldPos() - m_pTarget->GetWorldAxis(AXIS_Z) * m_fDistance);
}

void CArm::SetTarget(CComponent * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget->GetTransform();

	m_pTransform->SetWorldRot(m_pTarget->GetWorldRot());
	m_pTransform->SetWorldPos(m_pTarget->GetWorldPos() - m_pTarget->GetWorldAxis(AXIS_Z) * m_fDistance);
}

void CArm::EnableMouse()
{
	m_bMouseAction = true;
}

void CArm::MouseRotation(float fTime)
{
	if (!m_pTarget)
		return;

	if (m_bMouseAction == false)
		return;

	Vector2	vMove = GET_SINGLE(CInput)->GetMouseGap();

	if (CInput::GetInst()->KeyPush("RButton"))
	{
		if (vMove.x != 0.f)
		{
			float	fValueW = 180.f / _RESOLUTION.iWidth;
			float	fAngle = vMove.x * fValueW;

			m_pTransform->RotationY(fAngle);

			m_pTransform->SetWorldPos(m_pTarget->GetWorldPos() - m_pTransform->GetWorldAxis(AXIS_Z) * m_fDistance);
		}

		if (vMove.y != 0.f)
		{
			float	fValueH = 180.f / _RESOLUTION.iHeight;
			float	fAngle = vMove.y * fValueH * -1.f;

			m_pTransform->RotationX(fAngle);

		}
	}
}

bool CArm::Init()
{
	m_fSpeed = 500.f;

	return true;
}

int CArm::Input(float fTime)
{
	/*if (KEYPUSH("CameraFront"))
	{
	m_pTransform->Move(AXIS_Y, m_fSpeed, fTime);
	}

	if (KEYPUSH("CameraBack"))
	{
	m_pTransform->Move(AXIS_Y, -m_fSpeed, fTime);
	}

	if (KEYPUSH("CameraLeft"))
	{
	m_pTransform->Move(AXIS_X, -m_fSpeed, fTime);
	}

	if (KEYPUSH("CameraRight"))
	{
	m_pTransform->Move(AXIS_X, m_fSpeed, fTime);
	}*/

	return 0;
}

int CArm::Update(float fTime)
{
	if (m_bMouseAction && m_pTarget)
	{
		short	sWheel = GET_SINGLE(CInput)->GetWheelDir();

		if (sWheel != 0)
		{
			m_pTransform->Move(AXIS_Z, m_fSpeed * sWheel, fTime);
			m_fDistance += m_fSpeed * sWheel * fTime * -1;
		}
	}

	m_pTransform->SetWorldPos(m_pTarget->GetWorldPos() - m_pTransform->GetWorldAxis(AXIS_Z) * m_fDistance);

	MouseRotation(fTime);

	return 0;
}

int CArm::LateUpdate(float fTime)
{
	return 0;
}

void CArm::Collision(float fTime)
{
}

void CArm::Render(float fTime)
{
}

CArm * CArm::Clone()
{
	return new CArm(*this);
}
