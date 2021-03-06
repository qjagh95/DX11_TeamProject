#include "../EngineHeader.h"
#include "EditTest.h"
#include "../Input.h"
#include "Camera.h"

/*

 Tab 누르면 Scale, Position전환
 방향키 or wasd로 Rotation(X,Y Axis)
 X,Y Move
*/

PUN_USING

CEditTest::CEditTest()
{
	m_eComType = CT_END;
}


CEditTest::CEditTest(const CEditTest & _EditTest)
	:CComponent(_EditTest)
{
}

CEditTest::~CEditTest()
{
}

bool CEditTest::Init()
{
	return true;
}

int CEditTest::Input(float fTime)
{
	return 0;
}

int CEditTest::Update(float fTime)
{
	return 0;
}

int CEditTest::LateUpdate(float fTime)
{
	return 0;
}

void CEditTest::Collision(float fTime)
{
}

void CEditTest::Render(float fTime)
{
}

CEditTest * CEditTest::Clone()
{
	return new CEditTest(*this);
}

void CEditTest::XMove(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	Vector3 CameraAxis = m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_X];

	m_pTransform->Move(Vector3(1.f, 0.f, 0.f), fScale, fTime);
}

void CEditTest::YMove(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->Move(Vector3(0.f, 1.f, 0.f), fScale, fTime);
}


void CEditTest::ZMove(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	Vector3 CameraAxis = m_pScene->GetMainCameraTransformNonCount()->GetWorldAxis()[AXIS_Z];
	m_pTransform->Move(Vector3(0.f, 0.f, 1.f), fScale, fTime);
}


void CEditTest::XRotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;


	m_pTransform->RotationX(fScale, fTime);
}

void CEditTest::YRotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationY(fScale, fTime);
}


void CEditTest::ZRotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationZ(fScale, fTime);
}