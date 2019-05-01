#include "Human_Player.h"
#include "Inventory.h"
#include "DocxInven.h"
#include "Component/Animation2D.h"
#include "Input.h"

bool CHuman_Player::Init_Items()
{

	PUN::CInput::GetInst()->AddKey("U", 'U');

	PUN::CGameObject *pDocxInvObj = PUN::CGameObject::CreateObject("DocxInven", this->m_pLayer);

	m_pDocxInven = pDocxInvObj->AddComponent<CDocxInven>("DocxInven");
	m_pDocxInven->SetDocxMax(19);

	SAFE_RELEASE(pDocxInvObj);


	return true;
}

void CHuman_Player::OnDestroyInven()
{
	SAFE_RELEASE(m_pDocxInven);
	SAFE_RELEASE(m_pAnimation2D);
}

int CHuman_Player::Input_Items(float fTime)
{

	if (PUN::CInput::GetInst()->KeyPress("U"))
	{
		m_pDocxInven->SetVisible();
	}

	return 0;
}

int CHuman_Player::ItemUpdate(float fTime)
{
	// 무적 상태
	if (m_invincibility == true)
	{
		if (m_invincibilityTime > 0.f)
		{
			m_invincibilityTime -= fTime;
		}
		else if (m_invincibilityTime <= 0.f)
		{
			m_invincibility = false;
			m_invincibilityTime = 0.f;
			OutputDebugString(L"Invincibility End\n");
		}
	}

	// 이속 증가 상태
	if (m_isAccel == true)
	{
		if (m_accelDuration > 0.f)
		{
			m_accelDuration -= fTime;
		}
		else if (m_accelDuration <= 0.f)
		{
			m_isAccel = false;
			m_accelSpeed = 1.f;
			m_accelDuration = 0.f;
			OutputDebugString(L"Accelation End\n");
		}
	}

	switch (m_playerHP)
	{
	case 5:
		//m_pAnimation2D->ChangeClip("Hit_Empty");
		break;
	case 4:
		//m_pAnimation2D->ChangeClip("Hit1");
		break;
	case 3:
		//m_pAnimation2D->ChangeClip("Hit2");
		break;
	case 2:
		//m_pAnimation2D->ChangeClip("Hit_Anim");
		break;
	case 1:
		break;
	}

	return 0;
}
int CHuman_Player::ItemLateUpdate(float fTime)
{
	return 0;
}

void CHuman_Player::Pickup_Item(float fTime)
{
};

void CHuman_Player::Open_Item(float fTime)
{
	m_pInven->SetVisible();
};
void CHuman_Player::Close_Item(float fTime)
{
	m_pInven->SetVisible();
}

// 무적 효과
void CHuman_Player::SetInvincibility(bool _isFlag)
{
	m_invincibility = _isFlag;
}

bool CHuman_Player::GetInvincibility()
{
	return m_invincibility;
}

void CHuman_Player::SetEnvincibilityTime(float _time)
{
	m_invincibilityTime = _time;
}

// 이속 증가 효과
void CHuman_Player::SetAccelState(bool _isFlag)
{
	m_isAccel = _isFlag;
}

bool CHuman_Player::GetAccelState()
{
	return m_isAccel;
}

void CHuman_Player::SetAccelSpeed(float _accSpeed)
{
	m_accelSpeed = _accSpeed;
}

float CHuman_Player::GetAccelSpeed()
{
	return m_accelSpeed;
}

void CHuman_Player::SetAccelDuration(float _time)
{
	m_accelDuration = _time;
}

float CHuman_Player::GetAccelDuration()
{
	return m_accelDuration;
}

// 회복 효과
void CHuman_Player::SetMaxHP()
{
	m_playerHP = m_playerMaxHP;
}

int CHuman_Player::GetMaxHP()
{
	return m_playerMaxHP;
}

void CHuman_Player::RecoveryHP(int _hp)
{
	if (m_playerHP < m_playerMaxHP)
	{
		m_playerHP += _hp;
		if (m_playerHP >= m_playerMaxHP)
		{
			m_playerHP = m_playerMaxHP;
		}
	}
}

int CHuman_Player::GetHP()
{
	return m_playerHP;
}