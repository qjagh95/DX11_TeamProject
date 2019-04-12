#include "EngineHeader.h"
#include "Transform.h"
#include "FreeCamera.h"
#include "../Input.h"

PUN_USING

CFreeCamera::CFreeCamera()
{
	m_eComType = CT_FREECAMERA;
}

CFreeCamera::CFreeCamera(const CFreeCamera & camera) :
	CComponent(camera)
{
	m_fSpeed = camera.m_fSpeed;
}

CFreeCamera::~CFreeCamera()
{
}

bool CFreeCamera::Init()
{
	CInput::GetInst()->AddKey("YRotationP", 0x34);
	CInput::GetInst()->AddKey("YRotationM", 0x33);

	CInput::GetInst()->AddKey("XRotationP", 0x32);
	CInput::GetInst()->AddKey("XRotationM", 0x31);

	CInput::GetInst()->AddKey("ZRotationP", 0x36);
	CInput::GetInst()->AddKey("ZRotationM", 0x35);

	CInput::GetInst()->AddKey("ZeroRotation", 0x37);
	m_pTransform->SetWorldRot(Vector3::Zero);

	CInput::GetInst()->AddKey("XMoveP", 'W');
	CInput::GetInst()->AddKey("XMoveM", 'Q');

	CInput::GetInst()->AddKey("YMoveP", 'R');
	CInput::GetInst()->AddKey("YMoveM", 'E');

	CInput::GetInst()->AddKey("ZMoveP", 'Y');
	CInput::GetInst()->AddKey("ZMoveM", 'T');

	return true;
}

int CFreeCamera::Input(float fTime)
{
	return 0;
}

int CFreeCamera::Update(float fTime)
{
	if (CInput::GetInst()->KeyPush("YRotationP"))
		YRotation(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("YRotationM"))
		YRotation(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("XRotationP"))
		XRotation(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("XRotationM"))
		XRotation(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("ZRotationP"))
		ZRotation(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("ZRotationM"))
		ZRotation(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("XMoveP"))
		XMove(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("XMoveM"))
		XMove(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("YMoveP"))
		YMove(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("YMoveM"))
		YMove(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("ZMoveP"))
		ZMove(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("ZMoveM"))
		ZMove(-1.0f, fTime);

	if (CInput::GetInst()->KeyPress("ZeroRotation"))
		RotationZero(fTime);

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

void CFreeCamera::YRotation(float fScale, float fTime)
{
	m_pScene->GetMainCameraTransformNonCount()->RotationY(50.f * fScale, fTime);
}

void CFreeCamera::XRotation(float fScale, float fTime)
{
	m_pScene->GetMainCameraTransformNonCount()->RotationX(50.f* fScale, fTime);
}

void CFreeCamera::ZRotation(float fScale, float fTime)
{
	m_pScene->GetMainCameraTransformNonCount()->RotationZ(50.f * fScale, fTime);
}

void CFreeCamera::XMove(float fScale, float fTime)
{
	m_pTransform->Move(Vector3(1.f, 0.f, 0.f), 40.f * fScale, fTime);
}

void CFreeCamera::YMove(float fScale, float fTime)
{
	m_pTransform->Move(Vector3(0.f, 1.f, 0.f), 40.f * fScale, fTime);
}

void CFreeCamera::ZMove(float fScale, float fTime)
{
	m_pTransform->Move(Vector3(0.f, 0.f, 1.f), 40.f * fScale, fTime);
}

void CFreeCamera::RotationZero(float fTime)
{
	m_pTransform->SetWorldRot(0.f, 0.f, 0.f);
}

void CFreeCamera::PositionZero(float fTime)
{
	m_pTransform->SetWorldPos(0.f, 0.f, 0.f);
}

