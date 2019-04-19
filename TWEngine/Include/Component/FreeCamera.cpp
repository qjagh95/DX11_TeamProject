#include "EngineHeader.h"
#include "Transform.h"
#include "FreeCamera.h"
#include "../Input.h"

PUN_USING

CFreeCamera::CFreeCamera() :
	m_fSpeed(80.0f),
	m_fRotSpeed(180.0f)
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
	SAFE_RELEASE(m_pMouseTr);
}

bool CFreeCamera::Init()
{
	m_pMouseTr = GET_SINGLE(CInput)->GetMouseTr();

	if (!m_pMouseTr)
		return false;

	CInput::GetInst()->AddKey("ZeroRotation", 0x37);

	CInput::GetInst()->AddKey("CAM_FRONT", 'W');
	CInput::GetInst()->AddKey("CAM_BACK", 'S');
	CInput::GetInst()->AddKey("CAM_RIGHT", 'D');
	CInput::GetInst()->AddKey("CAM_LEFT", 'A');
	CInput::GetInst()->AddKey("CAM_ROT_RIGHT", 'E');
	CInput::GetInst()->AddKey("CAM_ROT_LEFT", 'Q');	
	CInput::GetInst()->AddKey("CAM_ROT_X_RETURN", VK_SPACE);

	return true;
}

int CFreeCamera::Input(float fTime)
{
	return 0;
}

int CFreeCamera::Update(float fTime)
{
	if (CInput::GetInst()->KeyPush("CAM_RIGHT"))
		XMove(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("CAM_LEFT"))
		XMove(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("CAM_FRONT"))
		ZMove(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("CAM_BACK"))
		ZMove(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("CAM_ROT_RIGHT"))
		m_pTransform->RotationY(m_fRotSpeed, fTime);
	else if (CInput::GetInst()->KeyPush("CAM_ROT_LEFT"))
		m_pTransform->RotationY(-m_fRotSpeed, fTime);
	
	if (CInput::GetInst()->KeyPress("CAM_ROT_X_RETURN"))
		m_pTransform->SetWorldRotX(0.0f);

	if (CInput::GetInst()->KeyPress("ZeroRotation"))
		RotationZero(fTime);

	if (KEYHOLD("RButton"))
		RotationByMouse(fTime);

	short	sWheel = GET_SINGLE(CInput)->GetWheelDir();

	if (sWheel != 0)
		m_pTransform->Move(Vector3::Axis[AXIS_Y], 8.0f * m_fSpeed * sWheel, fTime);

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

void CFreeCamera::SetMoveSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CFreeCamera::SetRotSpeed(float fSpeed)
{
	m_fRotSpeed = fSpeed;
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
	m_pTransform->Move(AXIS_X, m_fSpeed * fScale, fTime);
}

void CFreeCamera::YMove(float fScale, float fTime)
{
	m_pTransform->Move(Vector3(0.f, 1.f, 0.f), 40.f * fScale, fTime);
}

void CFreeCamera::ZMove(float fScale, float fTime)
{
	m_pTransform->Move(AXIS_Z, m_fSpeed * fScale, fTime);
}

void CFreeCamera::RotationZero(float fTime)
{
	m_pTransform->SetWorldRot(0.f, 0.f, 0.f);
}

void CFreeCamera::PositionZero(float fTime)
{
	m_pTransform->SetWorldPos(0.f, 0.f, 0.f);
}

void CFreeCamera::RotationByMouse(float fTime)
{
	if (m_pMouseTr->IsMoved())
	{
		Vector3 vMove = m_pMouseTr->GetWorldMove();
		int iX = 0;
		int	iY = 0;

		if (vMove.x < 0.0f)
			iX = -1;
		else if (vMove.x > 0.0f)
			iX = 1;
		if (vMove.y < 0.0f)
			iY = 1;
		else if (vMove.y > 0.0f)
			iY = -1;

		m_pTransform->Rotation(Vector3(m_fRotSpeed * iY, m_fRotSpeed * iX, 0.0f), fTime);
	}
}

