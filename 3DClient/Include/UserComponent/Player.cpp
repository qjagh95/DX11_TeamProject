#include "../ClientHeader.h"
#include "Player.h"
#include "Input.h"
#include "Input.h"
#include "NavigationMesh.h"
#include "NavigationManager3D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ColliderSphere.h"
#include "../SceneComponent/SecTestScene.h"
#include "../SceneComponent/TrdTestScene.h"

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
{
}

CPlayer::CPlayer(const CPlayer & com)
	: CUserComponent(com)
{
}


CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pAnimation);
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

	PUN::CInput* _input = GET_SINGLE(CInput);
	_input->BindAction("YODJPUMPTHISPARTY", KEY_PRESS, this, &CPlayer::YoDjPumpThisParty);
	_input->AddKeyAction("YODJPUMPTHISPARTY", DIK_J);

	_input->BindAction("KingHasCome", KEY_PRESS, this, &CPlayer::KingDdenGodTtack);
	_input->AddKeyAction("KingHasCome", DIK_K);

	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");

	pRenderer->SetMesh("BlackCow", TEXT("Monster4.msh"));

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetEmissive(1.f);

	SAFE_RELEASE(pMaterial);

	CAnimation* pAnimation = m_pObject->AddComponent<CAnimation>("Animation");

	pAnimation->LoadBone("Monster4.bne");
	pAnimation->Load("Monster4.anm");

	SAFE_RELEASE(pAnimation);
	SAFE_RELEASE(pRenderer);

	//CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("PlayerBody");

	//pBody->SetInfo(Vector3::Zero, 3.f);

	//SAFE_RELEASE(pBody);

	//CColliderOBB3D * pBody = m_pObject->AddComponent<CColliderOBB3D>("PlayerBody");

	//float	fLength[3] = { 3.f, 3.f, 3.f };
	//pBody->SetInfo(Vector3::Zero, Vector3::Axis, fLength);

	//SAFE_RELEASE(pBody);

	ChairUp* chairUp;
	Action1* ranAction1;
	Action2* ranAction2;

	BehaviorTree* myState = BTManager::Get()->CreateBehaviorTree("PlayerState1", BT_SELECTOR);
	myState->AddRootSelectorInSelector("Selector1");

	ranAction1 = myState->AddSelectorInAction<Action1>("Selector1", "Action1");
	ranAction2 = myState->AddSelectorInAction<Action2>("Selector1", "Action2");

	myState->AddSelectorInSelector("Selector1", "Selector2");
	myState->AddSelectorInSelector("Selector2", "Selector3");

	BehaviorTree* myState1 = BTManager::Get()->CreateBehaviorTree("PlayerState2", BT_SELECTOR);
	BehaviorTree* myState2 = BTManager::Get()->CreateBehaviorTree("PlayerState3", BT_SELECTOR);
	BehaviorTree* myState3 = BTManager::Get()->CreateBehaviorTree("PlayerState4", BT_SELECTOR);

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

	//float fRotationX = 0.f;
	//float fRotationY = 0.f;
	//fRotationX += -CInput::GetInst()->GetWorldMouseMove().y * 500.f * fTime;
	//fRotationY += CInput::GetInst()->GetWorldMouseMove().x * 500.f * fTime;

	//cout << fRotationX << " " << fRotationY << endl;

	//m_pTransform->RotationX(18.f * fRotationX, fTime);
	//m_pTransform->RotationY(18.f * fRotationY, fTime);

	//if (m_pTransform->GetWorldPos().z >= 0)
	//{
	//	if (m_pScene->GetTag() == "" || m_pScene->GetTag() == "TrdTestScene")
	//	{
	//		GET_SINGLE(CSceneManager)->CreateNextScene(true , "SecTestScene");
	//		GET_SINGLE(CSceneManager)->AddSceneComponent<CSecTestScene>("SecTestScene", false);
	//		m_pTransform->SetWorldPos(0.f, 0.f, -300.f);
	//	}
	//	else
	//	{
	//		GET_SINGLE(CSceneManager)->CreateNextScene(true, "TrdTestScene");
	//		GET_SINGLE(CSceneManager)->AddSceneComponent<CTrdTestScene>("TrdTestScene", false);
	//		m_pTransform->SetWorldPos(0.f, 0.f, -300.f);
	//	}
	//}

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

	m_pTransform->Move(AXIS_Z, 20.f * fScale, fTime);
}

void CPlayer::Fire(float fTime)
{
	OutputDebugString(TEXT("Fire\n"));
}

void CPlayer::Fire1(float fTime)
{
	m_pTransform->SetWorldRot(0.f, 0.f, 0.f);
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

void CPlayer::YoDjPumpThisParty(float fTime)
{
	PUN::CSoundManager::GetInst()->PlayBgm("bgm2", TEXT("room-2010.wav"),true, true, false);
}

void CPlayer::KingDdenGodTtack(float fTime)
{
	PUN::CSoundManager::GetInst()->PlayBgm(TEXT("orig_sudden.wav"), true, true, true);
}
