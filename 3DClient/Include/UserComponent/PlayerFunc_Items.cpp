#include "Human_Player.h"
#include "Inventory.h"
#include "DocxInven.h"
#include "KeyInven.h"
#include "Input.h"
#include "Component/ColliderRay.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"


bool CHuman_Player::Init_Items()
{
	PUN::CInput::GetInst()->AddKey("F", 'F');
	PUN::CInput::GetInst()->AddKey("U", 'U');
	PUN::CInput::GetInst()->AddKey("K", 'K');

	PUN::CGameObject *pDocxInvObj = PUN::CGameObject::CreateObject("DocxInven", this->m_pLayer);

	m_pDocxInven = pDocxInvObj->AddComponent<CDocxInven>("DocxInven");
	m_pDocxInven->SetDocxMax(19);

	SAFE_RELEASE(pDocxInvObj);

	PUN::CGameObject *pKeyInvObj = PUN::CGameObject::CreateObject("KeyInven", this->m_pLayer);

	m_pKeyInven = pKeyInvObj->AddComponent<CKeyInven>("KeyInven");
	m_pKeyInven->SetKeyMax(19);

	SAFE_RELEASE(pKeyInvObj);

	// 아이템 피킹용 충돌체 생성
	PUN::CGameObject*	pRayObj = PUN::CGameObject::CreateObject("Ray", this->m_pLayer);

	CRenderer*	pRenderer = pRayObj->AddComponent<CRenderer>("RayRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	pRayObj->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = pRayObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Ray", TEXT("UI/AimOff.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pRayTr = pRayObj->GetTransform();

	m_pRayTr->SetWorldScale(47.f, 47.f, 1.f);
	m_pRayTr->SetWorldPivot(0.5f, 0.5f, 0.f);
	m_pRayTr->SetWorldPos(640.f, 353.f, 0.f);	

	CColliderRay*	pRay = pRayObj->AddComponent<CColliderRay>("RayBody");

	Vector2 vMouse = PUN::CInput::GetInst()->GetMouse3DClient();

	Vector3 vMousePos = Vector3(vMouse.x, vMouse.y, 1.f);

	pRay->SetInfo(vMousePos, Vector3::Axis[AXIS_Z]);
	pRay->MouseEnable();
	pRay->SetCollisionCallback(CCT_ENTER, this, &CHuman_Player::RayCallBackEnter);
	pRay->SetCollisionCallback(CCT_LEAVE, this, &CHuman_Player::RayCallBackLeave);
	pRay->SetColliderID((COLLIDER_ID)UCI_PLAYER_RAY);

	SAFE_RELEASE(pRay);

	m_pRayAnimation = pRayObj->AddComponent<CAnimation2D>("RayAnimation2D");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(47.f, 47.f);
	vecClipFrame.push_back(tFrame);

	m_pRayAnimation->AddClip("AimEmpty", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"EmptyAim", TEXT("UI/AimEmpty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(47.f, 47.f);
	vecClipFrame.push_back(tFrame);

	m_pRayAnimation->AddClip("AimOff", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"OffAim", TEXT("UI/AimOff.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(47.f, 47.f);
	vecClipFrame.push_back(tFrame);

	m_pRayAnimation->AddClip("AimOn", A2D_ATLAS, AO_LOOP, 0.5f, vecClipFrame,
		"OnAim", TEXT("UI/ImgAimOn.png"));

	vecClipFrame.clear();

	SAFE_RELEASE(pRayObj);

	return true;
}

void CHuman_Player::ChangeRayAnim(const string& strName)
{
	m_pRayAnimation->ChangeClip(strName);
}

void CHuman_Player::OnDestroyInven()
{
	SAFE_RELEASE(m_pKeyInven);
	SAFE_RELEASE(m_pDocxInven);
	SAFE_RELEASE(m_pRayAnimation);
	SAFE_RELEASE(m_pRayTr);
}

int CHuman_Player::Input_Items(float fTime)
{
	if (PUN::CInput::GetInst()->KeyPress("U"))
	{
		if (m_iState & PSTATUS_DOCX)
		{
			m_iState ^= (PSTATUS_DOCX | PSTATUS_INACTIVE);
			Close_Docx(fTime);
		}
		else
		{
			m_iState |= (PSTATUS_DOCX | PSTATUS_INACTIVE);
			Open_Docx(fTime);
		}
	}

	if (PUN::CInput::GetInst()->KeyPress("K"))
	{
		if (m_iState & PSTATUS_KEY)
		{
			m_iState ^= (PSTATUS_KEY | PSTATUS_INACTIVE);
			Close_Key(fTime);
		}

		else
		{
			m_iState |= (PSTATUS_KEY | PSTATUS_INACTIVE);
			Open_Key(fTime);
		}
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

void CHuman_Player::Open_Docx(float fTime)
{
	m_pDocxInven->SetVisible();
}
void CHuman_Player::Close_Docx(float fTime)
{
	m_pDocxInven->SetVisible();
}

void CHuman_Player::Open_Key(float fTime)
{
	m_pKeyInven->SetVisible();
}
void CHuman_Player::Close_Key(float fTime)
{
	m_pKeyInven->SetVisible();
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