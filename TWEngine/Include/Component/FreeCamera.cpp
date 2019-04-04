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
	GET_SINGLE(CInput)->BindAxis("Move", this, &CFreeCamera::Move);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_S, -1.f);

	GET_SINGLE(CInput)->BindAxis("Rotation", this, &CFreeCamera::Rotation);
	GET_SINGLE(CInput)->AddKeyScale("Rotation", DIK_D, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("Rotation", DIK_A, -1.f);

	GET_SINGLE(CInput)->BindAxis("XRotation", this, &CFreeCamera::XRotation);
	GET_SINGLE(CInput)->AddKeyScale("XRotation", DIK_2, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("XRotation", DIK_1, -1.f);

	GET_SINGLE(CInput)->BindAxis("YRotation", this, &CFreeCamera::ZRotation);
	GET_SINGLE(CInput)->AddKeyScale("YRotation", DIK_4, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("YRotation", DIK_3, -1.f);

	GET_SINGLE(CInput)->BindAxis("ZeroRotation", this, &CFreeCamera::RotationZero);
	GET_SINGLE(CInput)->AddKeyScale("ZeroRotation", DIK_5, 1.f);

	GET_SINGLE(CInput)->BindAxis("ZeroPosition", this, &CFreeCamera::PositionZero);
	GET_SINGLE(CInput)->AddKeyScale("ZeroPosition", DIK_6, 1.f);

	m_pTransform->SetWorldRot(Vector3::Zero);

	return true;
}

int CFreeCamera::Input(float fTime)
{
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

void CFreeCamera::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->Move(AXIS_Z, 20.f * fScale, fTime);
}

void CFreeCamera::Rotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationY(180.f * fScale, fTime);
}

void CFreeCamera::XRotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationX(180.f * fScale, fTime);
}

void CFreeCamera::ZRotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationZ(180.f * fScale, fTime);
}

void CFreeCamera::RotationZero(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->SetWorldRot(0.f, 0.f, 0.f);
}

void CFreeCamera::PositionZero(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->SetWorldPos(0.f, 0.f, 0.f);
}

