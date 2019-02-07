#include "Player.h"
#include "Input.h"
#include "Component/Transform.h"

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(const CPlayer & com)
	:CUserComponent(com)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
}

void CPlayer::AfterClone()
{
}

bool CPlayer::Init()
{
	m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);

	GET_SINGLE(CInput)->BindAxis("Move", this, &CPlayer::Move);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_S, -1.f);

	GET_SINGLE(CInput)->BindAxis("Rotation", this, &CPlayer::Rotation);
	GET_SINGLE(CInput)->AddKeyScale("Rotation", DIK_D, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("Rotation", DIK_A, -1.f);

	GET_SINGLE(CInput)->BindAction("Fire", KEY_PRESS, this, &CPlayer::Fire);
	GET_SINGLE(CInput)->AddKeyAction("Fire", DIK_SPACE);

	bool	bSKey[SKEY_END] = { false, true, true };
	GET_SINGLE(CInput)->BindAction("Fire1", KEY_PRESS, this, &CPlayer::Fire1);
	GET_SINGLE(CInput)->BindAction("Fire1", KEY_RELEASE, this, &CPlayer::Fire1Release);
	GET_SINGLE(CInput)->AddKeyAction("Fire1", DIK_SPACE, bSKey);

	return true;
}

int CPlayer::Input(float fTime)
{
	return 0;
}

int CPlayer::Update(float fTime)
{
	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

void CPlayer::Collision(float fTime)
{
}

void CPlayer::Render(float fTime)
{
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CPlayer::Fire(float fTime)
{
	OutputDebugString(TEXT("Fire\n"));
}

void CPlayer::Fire1(float fTime)
{
	OutputDebugString(TEXT("Fire1\n"));
}

void CPlayer::Fire1Release(float fTime)
{
	OutputDebugString(TEXT("Fire1Release\n"));
}

void CPlayer::Rotation(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTransform->RotationY(180.f * fScale, fTime);
}
