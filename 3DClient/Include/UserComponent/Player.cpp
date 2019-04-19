#include "../ClientHeader.h"
#include "Player.h"
#include "Input.h"
#include "NavigationMesh.h"
#include "NavigationManager3D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ColliderSphere.h"
#include "Inventory.h"
#include "Handycam.h"

#include <BehaviorTree.h>
#include <SoundManager.h>

class ChairUp : public BehaviorTree::Action
{
public:
	ChairUp() {}
	~ChairUp() {}

	int Update(float DeltaTime) override
	{
		cout << "의자에서 일어난다" << endl;
		return ACTION_FALSE;
	}

	void Ending(float DeltaTime) override
	{
		cout << "ChairUp End함수 호출" << endl;
	}
};

class Action1 : public BehaviorTree::Action
{
public:
	Action1() {}
	~Action1() {}

	bool Check1(float DeltaTime)
	{
		cout << "Selector1 체크" << endl;
		return true;
	}
	bool Check2(float DeltaTime)
	{
		cout << "Selector2 체크" << endl;
		return true;
	}
	bool Check3(float DeltaTime)
	{
		cout << "Selector3 체크" << endl;
		return true;
	}

	void Init() override
	{
		cout << "Action1 Start" << endl;
	}

	int Update(float DeltaTime) override
	{
		cout << "보리차를 산다." << endl;

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			return ACTION_RUNNING;
		else
			return ACTION_FALSE;
	}
};

class Action2 : public BehaviorTree::Action
{
public:
	Action2() {}
	~Action2() {}

	int Update(float DeltaTime) override
	{
		cout << "물을 산다." << endl;
		return ACTION_FALSE;
	}
};

class MyHome : public BehaviorTree::Action
{
public:
	MyHome() {}
	~MyHome() {}

	int Update(float DeltaTime) override
	{
		cout << "내 자리로 간다." << endl;
		return ACTION_SUCCED;
	}
};

class SitDown : public BehaviorTree::Action
{
public:
	SitDown() {}
	~SitDown() {}

	int Update(float DeltaTime) override
	{
		cout << "의자에 앉는다" << endl;
		return ACTION_SUCCED;
	}
};

CPlayer::CPlayer()
	: m_pAnimation(nullptr)
	, m_pInvenObj(nullptr)
	, m_pInven(nullptr)
	, m_pInvenTr(nullptr)
	, m_pHandycamObj(nullptr)
	, m_pHandycam(nullptr)
{
	SetTag("Player");
	m_eComType = (COMPONENT_TYPE)UT_PLAYER;
}

CPlayer::CPlayer(const CPlayer & com)
	: CUserComponent(com)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pHandycam);
	SAFE_RELEASE(m_pHandycamObj);
	SAFE_RELEASE(m_pInvenTr);
	SAFE_RELEASE(m_pInven);
	SAFE_RELEASE(m_pInvenObj);
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

	GET_SINGLE(CInput)->AddKey("FrontMove", 'W');
	GET_SINGLE(CInput)->AddKey("BackMove", 'S');

	GET_SINGLE(CInput)->AddKey("YRotationP", 'D');
	GET_SINGLE(CInput)->AddKey("YRotationM", 'A');

	GET_SINGLE(CInput)->AddKey("Fire", VK_SPACE);
	GET_SINGLE(CInput)->AddKey("YODJPUMPTHISPARTY", 'J');

	GET_SINGLE(CInput)->AddKey("KingHasCome", 'K');

	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("BlackCow", TEXT("Monster4.msh"));

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	pMaterial->SetEmissive(1.f);
	SAFE_RELEASE(pMaterial);

	m_pAnimation = m_pObject->AddComponent<CAnimation>("Animation");
	m_pAnimation->LoadBone("Monster4.bne");
	m_pAnimation->Load("Monster4.anm");
	m_pAnimation->AddSocket("Bip01-Spine1", "Anjang");
	m_pAnimation->SetSocketOffset("Bip01-Spine1", "Anjang", Vector3(0.f, 2.5f, 0.f));

	CGameObject* pWeaponObj = CGameObject::CreateObject("Weapon", m_pLayer);
	CTransform*	pTr = pWeaponObj->GetTransform();
	pTr->SetWorldScale(0.2f, 0.2f, 0.2f);
	pTr->SetLocalRotY(-90.f);

	CRenderer*	pWeaponRenderer = pWeaponObj->AddComponent<CRenderer>("WeaponRenderer");
	pWeaponRenderer->SetMesh("chair", TEXT("chair.msh"));
	pWeaponRenderer->SetDecalEnable(false);

	m_pAnimation->SetSocketObject("Bip01-Spine1", "Anjang", pWeaponObj);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pWeaponRenderer);
	SAFE_RELEASE(pWeaponObj);
	SAFE_RELEASE(pRenderer);

	// Inventory
	m_pInvenObj = CGameObject::CreateObject("Inven", m_pLayer);

	m_pInven = m_pInvenObj->AddComponent<CInventory>("Inven");
	m_pInven->SetInvenMax(19);

	CTransform*	pInvenTr = m_pInvenObj->GetTransform();
	pInvenTr->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pInvenTr);

	pMaterial = m_pInvenObj->AddComponent<CMaterial>("Inven");
	pMaterial->SetDiffuseTex(0, "Inventory", TEXT("UI/Inven/InvenBack.png"));

	SAFE_RELEASE(pMaterial);


	// Handycam
	m_pHandycamObj = CGameObject::CreateObject("Handycam", m_pLayer);

	m_pHandycam = m_pHandycamObj->AddComponent<CHandycam>("Handycam");

	CTransform*	pHandycamTr = m_pHandycamObj->GetTransform();
	pHandycamTr->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pHandycamTr);

	return true;
}

int CPlayer::Input(float fTime)
{
	return 0;
}

int CPlayer::Update(float fTime)
{
	CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene,
		m_pTransform->GetWorldPos());

	if (pMesh)
	{
		float	fY = pMesh->GetY(m_pTransform->GetWorldPos());

		Vector3 vPos = m_pTransform->GetWorldPos();
		vPos.y = fY;
		m_pTransform->SetWorldPos(vPos);
	}

	if (CInput::GetInst()->KeyPush("FrontMove"))
		Move(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("BackMove"))
		Move(-1.0f, fTime);

	if (CInput::GetInst()->KeyPush("YRotationP"))
		Rotation(1.0f, fTime);
	else if (CInput::GetInst()->KeyPush("YRotationM"))
		Rotation(-1.0f, fTime);

	if (CInput::GetInst()->KeyPress("YODJPUMPTHISPARTY"))
		YoDjPumpThisParty(fTime);

	if (CInput::GetInst()->KeyPress("KingHasCome"))
		KingDdenGodTtack(fTime);

	static bool bPush = false;
	if (GetAsyncKeyState('I') & 0x8000)
	{
		bPush = true;
	}
	else if (bPush)
	{
		bPush = false;
		m_pInven->SetVisible();
	}


	static bool bCamPush = false;
	if (GetAsyncKeyState('G') & 0x8000)
	{
		bCamPush = true;
	}
	else if (bCamPush)
	{
		bCamPush = false;
		m_pHandycam->SetVisible();
	}

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
	m_pTransform->Move(AXIS_Z, 20.f * fScale, fTime);
}

void CPlayer::Rotation(float fScale, float fTime)
{
	m_pTransform->RotationY(180.f * fScale, fTime);
}

void CPlayer::YoDjPumpThisParty(float fTime)
{
	PUN::CSoundManager::GetInst()->PlayBgm("bgm2", TEXT("room-2010.wav"), true, true, false);
}

void CPlayer::KingDdenGodTtack(float fTime)
{
	PUN::CSoundManager::GetInst()->PlayBgm(TEXT("orig_sudden.wav"), true, true, true);
}
