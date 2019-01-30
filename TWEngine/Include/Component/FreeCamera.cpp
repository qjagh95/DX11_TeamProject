#include "EngineHeader.h"
#include "FreeCamera.h"
#include "../Input.h"
#include "Transform.h"

PUN_USING

CFreeCamera::CFreeCamera()
{
	m_eComType = CT_FREECAMERA;
}

CFreeCamera::CFreeCamera(const CFreeCamera & camera)	:
	CComponent(camera)
{
	m_fSpeed = camera.m_fSpeed;
}

CFreeCamera::~CFreeCamera()
{
}

bool CFreeCamera::Init()
{
	GET_SINGLE(CInput)->AddKey("CameraFront", VK_UP);
	GET_SINGLE(CInput)->AddKey("CameraBack", VK_DOWN);
	GET_SINGLE(CInput)->AddKey("CameraLeft", VK_LEFT);
	GET_SINGLE(CInput)->AddKey("CameraRight", VK_RIGHT);

	m_fSpeed = 500.f;

	return true;
}

int CFreeCamera::Input(float fTime)
{
	if (KEYPUSH("CameraFront"))
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
	}

	return 0;
}

int CFreeCamera::Update(float fTime)
{
	return 0;
}

int CFreeCamera::LateUpdate(float fTime)
{
	return 0;
}

void CFreeCamera::Collision(float fTime)
{
}

void CFreeCamera::Render(float fTime)
{
}

CFreeCamera * CFreeCamera::Clone()
{
	return new CFreeCamera(*this);
}
