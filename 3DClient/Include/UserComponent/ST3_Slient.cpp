#include "../ClientHeader.h"
#include "ST3_Slient.h"
#include "Human_Player.h"
#include "Door.h"

#include "../GameManager.h"
#include "../SceneComponent/JBH_Stage3.h"

#include <Component/SoundSource.h>

ST3_Slient::ST3_Slient()
{
	m_Attack = 1;
	m_TraceDist = 5.0f;
	m_BashCount = 3;
	m_MoveSpeed = 22.5f;
	m_WateTime = 2.0f; 
	m_WateTimeVar = 0.0f;
	m_PlayerState = 0;
	m_FindTime = 3.0f;
	m_FindTimeVar = 0.0f;
	m_BashCount = 0;
	m_HitMoveSpeed = 20.0f;
	m_isDoorColl = false;
	m_isParkourColl = false;

	m_isHit = false;
	m_SlowTime = 0.0f;
	m_SlowTimeVar = 0.0f;

	m_PlayerCollDoor = NULLPTR;
	m_3DSoundObject = NULLPTR;
	m_3DSound = NULLPTR;

	m_AttackJap = NULLPTR;
	m_AttackJapBox = NULLPTR;
	m_AttackHook = NULLPTR;
	m_AttackHookBox = NULLPTR;
	m_AttackHead = NULLPTR;
	m_AttackHeadBox = NULLPTR;
}

ST3_Slient::ST3_Slient(const ST3_Slient & CopyData)
	:NPCBase(CopyData)
{
}

ST3_Slient::~ST3_Slient()
{
	SAFE_RELEASE(m_3DSoundObject);
	SAFE_RELEASE(m_3DSound);

	SAFE_RELEASE(m_AttackJap);
	SAFE_RELEASE(m_AttackJapBox);
	SAFE_RELEASE(m_AttackHook);
	SAFE_RELEASE(m_AttackHookBox);
	SAFE_RELEASE(m_AttackHead);
	SAFE_RELEASE(m_AttackHeadBox);
}

bool ST3_Slient::Init()
{
	NPCBase::Init();

	/*
	1. 쫓아오는 몹.
	2. Mesh : Cannibal
	3. bne, anm : Cannibal
	*/

	m_AniName[STS_IDLE] = "idle_search_forward";
	m_AniName[STS_WALK] = "move_forward_slow";
	m_AniName[STS_DOOR_TRACE] = "move_forward_fast";
	m_AniName[STS_CAN_WATE] = "idle_search_forward";
	m_AniName[STS_WALK_IDLE] = "idle_search_forward";
	m_AniName[STS_SUPRISE_TRACE] = "move_forward_fast";
	m_AniName[STS_USER_TRACE] = "move_forward_fast";
	m_AniName[STS_CAN_TRACE] = "move_forward_fast";
	m_AniName[STS_BASH_DOOR] = "bash_door_loop";
	m_AniName[STS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[STS_HOOK] = "attack_right";
	m_AniName[STS_JAP] = "attack_left";
	m_AniName[STS_HEAD_ATTACK] = "attack_middle";

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Renderer->SetSelectBoneName("NPCMedium-Pelvis");

	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("enem_def_2.anm");

	m_AttackJap = CGameObject::CreateObject("JapBody", m_pLayer);
	m_AttackJapBox = m_AttackJap->AddComponent< CColliderOBB3D>("JapBody");
	m_AttackJapBox->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.5f, 0.5f, 0.5f));
	m_AttackJapBox->SetContinueTypeName("MonsterBodyOBB");
	m_AttackJapBox->SetContinueTypeName("HookBody");
	m_AttackJapBox->SetContinueTypeName("HeadBody");
	m_AttackJapBox->SetEnable(false);

	m_AttackHook = CGameObject::CreateObject("HookBody", m_pLayer);
	m_AttackHookBox = m_AttackHook->AddComponent< CColliderOBB3D>("HookBody");
	m_AttackHookBox->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.5f, 0.5f, 0.5f));
	m_AttackHookBox->SetContinueTypeName("MonsterBodyOBB");
	m_AttackHookBox->SetContinueTypeName("JapBody");
	m_AttackHookBox->SetContinueTypeName("HeadBody");
	m_AttackHookBox->SetEnable(false);

	m_AttackHead = CGameObject::CreateObject("HeadBody", m_pLayer);
	m_AttackHeadBox = m_AttackHead->AddComponent< CColliderOBB3D>("HeadBody");
	m_AttackHeadBox->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.5f, 0.5f, 0.5f));
	m_AttackHeadBox->SetContinueTypeName("MonsterBodyOBB");
	m_AttackHeadBox->SetContinueTypeName("HookBody");
	m_AttackHeadBox->SetContinueTypeName("JapBody");
	m_AttackHeadBox->SetEnable(false);

	m_Animation->AddSocket("NPCMedium-L-Finger0", "JapBody");
	m_Animation->AddSocket("NPCMedium-R-Finger0", "HookBody");
	m_Animation->AddSocket("NPCMedium-Head", "HeadBody");

	m_Animation->SetSocketObject("NPCMedium-L-Finger0", "JapBody", m_AttackJap);
	m_Animation->SetSocketObject("NPCMedium-R-Finger0", "HookBody", m_AttackHook);
	m_Animation->SetSocketObject("NPCMedium-Head", "HeadBody", m_AttackHead);
	
	//수정
	m_Animation->SetSocketOffset("NPCMedium-L-Finger0", "JapBody", Vector3(25.0f, 95.0f, -35.0f));
	m_Animation->SetSocketOffset("NPCMedium-R-Finger0", "HookBody", Vector3(-25.0f, 95.0f, -35.0f));
	m_Animation->SetSocketOffset("NPCMedium-Head", "HeadBody", Vector3(0.0f, 180.0f, 0.0f));

	ChangeState(STS_IDLE, m_AniName);

	m_3DSoundObject = CGameObject::CreateObject("3DCanSound", m_pLayer);
	m_3DSound = m_3DSoundObject->AddComponent<CSoundSource>("3DCanSound");
	m_3DSound->GetTransformNonCount()->SetWorldPos(Vector3(67.0f, 0.0f, 155.0f));

	m_3DSound->LoadSound("Engry1", L"enemies\\Enemies_def_angry1.wav");
	m_3DSound->LoadSound("Engry2", L"enemies\\Enemies_def_angry2.wav");
	m_3DSound->LoadSound("Engry3", L"enemies\\Enemies_def_angry3.wav");
	m_3DSound->LoadSound("Engry4", L"enemies\\Enemies_def_angry4.wav");
	m_3DSound->LoadSound("Engry5", L"enemies\\Enemies_def_angry5.wav");

	m_3DSound->LoadSound("Walk1", L"player\\FootStep\\FTS_CARPET_WALK_01.wav");
	m_3DSound->LoadSound("Walk2", L"player\\FootStep\\FTS_CARPET_WALK_02.wav");
	
	m_3DSound->LoadSound("Bash1", L"door\\SFX_WoodenDoor_Bash_01.wav");
	m_3DSound->LoadSound("Bash2", L"door\\SFX_WoodenDoor_Bash_02.wav");
	m_3DSound->LoadSound("Bash3", L"door\\SFX_WoodenDoor_Bash_03.wav");
	m_3DSound->LoadSound("Bash4", L"door\\SFX_WoodenDoor_Bash_04.wav");
	m_3DSound->LoadSound("Bash5", L"door\\SFX_WoodenDoor_Bash_05.wav");
	m_3DSound->LoadSound("Bash6", L"door\\SFX_WoodenDoor_Bash_06.wav");
	m_3DSound->LoadSound("Bash7", L"door\\SFX_WoodenDoor_Bash_07.wav");
	m_3DSound->LoadSound("Bash8", L"door\\SFX_WoodenDoor_Bash_08.wav");

	m_3DSound->LoadSound("BashMob1", L"enemies\\Enemies_type2_angry1.wav");
	
	m_3DSound->LoadSound("HookSound", L"player\\hurt\\Patient_whoosh_strong_1.wav");
	m_3DSound->LoadSound("JabSound", L"player\\hurt\\Patient_whoosh_strong_2.wav");
	m_3DSound->LoadSound("HeadAttackSound", L"player\\hurt\\Patient_whoosh_strong_1.wav");
	
	m_3DSound->LoadSound("Scream1", L"enemies\\Enemies_def_hit1.wav");
	m_3DSound->LoadSound("Scream2", L"enemies\\Enemies_def_hit2.wav");
	m_3DSound->LoadSound("Scream3", L"enemies\\Enemies_def_hit3.wav");

	m_3DSound->LoadSound("BashOpen", L"dist_wood_03.wav");

	m_Animation->SetCallback("move_forward_fast", this, 8, &ST3_Slient::Walk1Sound);
	m_Animation->SetCallback("move_forward_fast", this, 19, &ST3_Slient::Walk2Sound);
	m_Animation->SetCallback("bash_door_loop", this, 10, &ST3_Slient::BashSound);
	m_Animation->SetCallback("bash_door_loop", this, 1, &ST3_Slient::BashMobSound);
	m_Animation->SetCallback("attack_right", this, 6, &ST3_Slient::HookSound);
	m_Animation->SetCallback("attack_left", this, 6, &ST3_Slient::JabSound);
	m_Animation->SetCallback("attack_middle", this, 7, &ST3_Slient::HeadAttackSound);

	m_Animation->SetCallback("attack_right", this, 1, &ST3_Slient::AttackScream);
	m_Animation->SetCallback("attack_left", this, 1, &ST3_Slient::AttackScream);
	m_Animation->SetCallback("attack_middle", this, 1, &ST3_Slient::AttackScream);

	m_BodyOBB->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.8f, 7.0f, 0.8f));
	m_BodyOBB->SetCollisionCallback(CCT_ENTER, this, &ST3_Slient::DoorCollFirst);
	m_BodyOBB->SetCollisionCallback(CCT_LEAVE, this, &ST3_Slient::DoorCollEnd);
	m_BodyOBB->SetCollisionCallback(CCT_ENTER, this, &ST3_Slient::PlayerBulletHit);
	m_BodyOBB->SetCollisionCallback(CCT_STAY, this, &ST3_Slient::MonsterColl);

	return true;
}

int ST3_Slient::Input(float DeltaTime)
{
	NPCBase::Input(DeltaTime);

	return 0;
}

int ST3_Slient::Update(float DeltaTime)
{
	NPCBase::Update(DeltaTime);

	m_3DSoundObject->GetTransformNonCount()->SetWorldPos(m_pTransform->GetWorldPos());
	m_PlayerState = m_TargetPlayer->GetState();

	if (m_isHit == true)
		m_MoveSpeed = 10.0f;
	else
		m_MoveSpeed = 22.5f;

	if (m_isHit == true)
	{
		m_SlowTimeVar += DeltaTime;

		if (m_SlowTime <= m_SlowTimeVar)
		{
			m_SlowTimeVar = 0.0f;
			m_isHit = false;
		}
		m_MoveSpeed = m_HitMoveSpeed;
	}
	else
		m_MoveSpeed = 22.5f;

	switch (m_State)
	{
	case STS_IDLE:
		FS_IDLE(DeltaTime);
		break;
	case STS_DOOR_TRACE:
		FS_DOOR_TRACE(DeltaTime);
		break;
	case STS_WALK:
		FS_WALK(DeltaTime);
		break;
	case STS_WALK_IDLE:
		FS_WALK_IDLE(DeltaTime);
		break;
	case STS_CAN_WATE:
		FS_CAN_WATE(DeltaTime);
		break;
	case STS_USER_TRACE:
		FS_USER_TRACE(DeltaTime);
		break;
	case STS_CAN_TRACE:
		FS_CAN_TRACE(DeltaTime);
		break;
	case STS_SUPRISE_TRACE:
		FS_SUPRISE_TRACE(DeltaTime);
		break;
	case STS_HOOK:
		FS_HOOK(DeltaTime);
		break;
	case STS_JAP:
		FS_JAP(DeltaTime);
		break;
	case STS_HEAD_ATTACK:
		FS_HEAD_ATTACK(DeltaTime);
		break;
	case STS_BASH_DOOR:
		FS_BASH_DOOR(DeltaTime);
		break;
	case STS_BASH_DOOR_OPEN:
		FS_BASH_DOOR_OPEN(DeltaTime);
		break;
	}

	return 0;
}

int ST3_Slient::LateUpdate(float DeltaTime)
{
	return 0;
}

void ST3_Slient::Collision(float DeltaTime)
{
}

void ST3_Slient::Render(float DeltaTime)
{
}

ST3_Slient * ST3_Slient::Clone()
{
	return new ST3_Slient(*this);
}

void ST3_Slient::Walk1Sound(float DeltaTime)
{
	string temp = "Walk1";
	m_3DSound->Play(temp);
}

void ST3_Slient::Walk2Sound(float DeltaTime)
{
	string temp = "Walk2";
	m_3DSound->Play(temp);
}

void ST3_Slient::BashSound(float DeltaTime)
{
	if (m_TargetDistance > 60.0f)
		return;

	int RandNum = rand() % 8;
	RandNum += 1;
	
	char B[2] = "";
	char B2[11] = "Bash";

	_itoa_s(RandNum, B, 10);
	strcat_s(B2, 11, B);

	string SoundKey = B2;

	m_3DSound->SetVolume(SoundKey, 3.0f);
	m_3DSound->Play(SoundKey);
}

void ST3_Slient::BashMobSound(float DeltaTime)
{
	if (m_TargetDistance > 60.0f)
		return;

	string SoundKey = "BashMob1";
	m_3DSound->Play(SoundKey);
}

void ST3_Slient::HookSound(float DeltaTime)
{
	string SoundKey = "HookSound";
	m_3DSound->Play(SoundKey);
}

void ST3_Slient::JabSound(float DeltaTime)
{
	string SoundKey = "JabSound";
	m_3DSound->Play(SoundKey);
}

void ST3_Slient::HeadAttackSound(float DeltaTime)
{
	string SoundKey = "HeadAttackSound";
	m_3DSound->Play(SoundKey);
}

void ST3_Slient::AttackScream(float DeltaTime)
{
	int RandNum = rand() % 3;
	RandNum += 1;

	char B[2] = "";
	char B2[18] = "Scream";

	_itoa_s(RandNum, B, 10);
	strcat_s(B2, 18, B);
	string SoundKey = B2;

	bool isPlay = false;
	for (int i = 0; i < 3; i++)
	{
		char Buffer[2] = "";
		char Buffer2[18] = "Scream";
		_itoa_s(i + 1, Buffer, 10);
		strcat_s(Buffer2, 18, Buffer);

		string Copy = Buffer2;
		if (m_3DSound->GetClipState(Copy) == PLAYING)
		{
			isPlay = true;
			break;
		}
	}

	if(isPlay == false)
		m_3DSound->Play(SoundKey);	
}

void ST3_Slient::PlayerStateCheck(float DeltaTime)
{
	if((m_PlayerState & PSTATUS_CROUCHED) || (m_PlayerState & PSTATUS_CROUCHING) || (m_PlayerState & PSTATUS_STOPMOVE))
		return;
	else
		ChangeState(STS_USER_TRACE, m_AniName);
}

void ST3_Slient::DoorCheck()
{
	CDoor* getDoor = CGameManager::GetInst()->GetPlayerCollDoor(m_pScene);

	if (getDoor == NULLPTR)
	{
		m_PlayerCollDoor = NULLPTR;
		return;
	}

	if (getDoor->GetIsPlayerColl() == false)
	{
		m_PlayerCollDoor = NULLPTR;
		return;
	}

	if (getDoor->GetDoorType() == DOOR_STAGE)
	{
		m_PlayerCollDoor = NULLPTR;
		return;
	}

	string DoorTag = CGameManager::GetInst()->GetPlayerCollDoorMapKey(m_pScene);

	if (strstr(DoorTag.c_str(), "Fake") != NULLPTR)
	{
		m_PlayerCollDoor = NULLPTR;
		return;
	}

	m_PlayerCollDoor = getDoor;

	//3 = LockSound
	if (getDoor->Get3DSound()->GetClipState(3) == PLAYING)
	{
		if (getDoor->IsOpened() == false)
		{
			m_PathFind = false;
			//Matrix Temp = getDoor->GetTransformNonCount()->GetPosDelta();
			//m_MovingPos.x = Temp._41;
			//m_MovingPos.z = Temp._43;
			//m_MovingPos.y = 0.0f;

			m_MovingPos = getDoor->GetColliderCenter();
			m_MovingPos.y = 0.0f;

			ChangeState(STS_DOOR_TRACE, m_AniName);
		}
	}
}

void ST3_Slient::DoorCollCheck()
{
	if (m_isDoorColl == true)
	{
		m_MovingPos = Vector3(36.5f, 0.0f, 11.0f);
		m_PathFind = false;
		ChangeState(STS_WALK, m_AniName);
	}
}

void ST3_Slient::ParkourCheck()
{
}

void ST3_Slient::DoorCollFirst(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "DoorBody")
		m_isDoorColl = true;
}

void ST3_Slient::DoorCollEnd(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "DoorBody")
		m_isDoorColl = false;
}

void ST3_Slient::PlayerBulletHit(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "BulletBody")
	{
		m_isHit = true;
		m_SlowTimeVar = 0.0f;
	}
}

void ST3_Slient::MonsterColl(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MonsterBodyOBB")
	{
		Vector3 myAxis = Dest->GetTransformNonCount()->GetWorldAxis(AXIS_Z);

		Vector3 Pos = dynamic_cast<CColliderOBB3D*>(Src)->GetInfo().vCenter;
		Vector3 DestPos = dynamic_cast<CColliderOBB3D*>(Dest)->GetInfo().vCenter;

		Pos.y = 0.0f;
		DestPos.y = 0.0f;

		Vector3 Normal = Pos.Cross(DestPos);
		Normal.Normalize();

		XMVECTOR i = XMLoadFloat3((XMFLOAT3*)&myAxis);
		XMVECTOR n = XMLoadFloat3((XMFLOAT3*)&Normal);
		XMVECTOR Result = XMVector3Reflect(i, n);
		Vector3 Convert = Vector3(Result);
		Convert.Normalize();
		Convert.y = 0.0f;

		Vector3 Convert2 = Pos;

		Convert2.x += Convert.x * 10.0f * DeltaTime;
		Convert2.z += Convert.z * 10.0f * DeltaTime;
		
		if (CNavigationManager3D::GetInst()->FindNavMesh(m_pScene, Pos)->CheckCell(Convert2) == true)
			m_pTransform->Move(Convert, 10.0f, DeltaTime);
	}

}

void ST3_Slient::FS_IDLE(float DeltaTime)
{
	//어차피 한번하고 안들어옴
	if (JBH_Stage3::m_SlientMode == true)
	{
		m_MovingPos = m_pLayer->FindObjectNonCount("Suprise")->GetTransformNoneCount()->GetWorldPos();
		m_MovingPos.y = 0.0f;
		ChangeState(STS_SUPRISE_TRACE, m_AniName);
	}
}

void ST3_Slient::FS_WALK(float DeltaTime)
{
	Vector3 myPos = m_CenterDownCenter;

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, m_MovingPos);

		m_PathFind = true;

		m_PathList.clear();
		m_PathList = m_NaviMesh->GetPathList();

		if(m_PathList.empty() == false)
			m_MovePos = m_PathList.front();
	}

	Vector3 Dir = m_MovePos - myPos;
	Dir.Normalize();

	myPos += Dir * 10.0f * DeltaTime;

	m_pTransform->Move(Dir, 10.0f, DeltaTime);

	if (m_PathList.empty())
	{
		ChangeState(STS_WALK_IDLE, m_AniName);
		m_PathFind = false;
	}
	else
	{
		if (((myPos.x <= m_MovePos.x + 1.0f) && (myPos.x >= m_MovePos.x - 1.0f)) && (myPos.z <= m_MovePos.z + 1.0f && myPos.z >= m_MovePos.z - 1.0f))
		{
			m_pTransform->SetWorldRotY(GetYAngle(m_MovePos, m_CenterDownCenter));
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();
		}
	}

	if (m_TargetDistance <= 50.0f)
		PlayerStateCheck(DeltaTime);
}

void ST3_Slient::FS_DOOR_TRACE(float DeltaTime)
{
	Vector3 myPos = m_CenterDownCenter;

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, m_MovingPos);

		m_PathFind = true;

		m_PathList.clear();
		m_PathList = m_NaviMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3 Dir = m_MovePos - myPos;
	Dir.Normalize();

	myPos += Dir * m_MoveSpeed * DeltaTime;
	m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);
	float Ang = m_pTransform->GetWorldAxis(AXIS_Z).GetAngle(Dir);

	if (m_PathList.empty())
	{
		CDoor* getDoor = CGameManager::GetInst()->GetNearDoor(m_pScene, m_CenterDownCenter);
		Vector3 DoorAxis = getDoor->GetTransformNonCount()->GetWorldAxis(AXIS_Z);
		Vector3 MyAxis = m_pTransform->GetWorldAxis(AXIS_Z);
		float Ang2 = DoorAxis.GetAngle(MyAxis);

		if (fabsf(DoorAxis.x) > 0.999f)
		{
			if (DoorAxis.x > 0.f)
				m_pTransform->SetWorldRotY(90.f);
			else
				m_pTransform->SetWorldRotY(270.f);
		}
		else if (fabsf(DoorAxis.z) > 0.999f)
		{
			if (DoorAxis.z > 0.f)
				m_pTransform->SetWorldRotY(0.f);
			else
				m_pTransform->SetWorldRotY(180.f);
		}

		m_PathFind = false;
		//m_pTransform->SetWorldRotY(Ang + 180.0f);
		ChangeState(STS_BASH_DOOR, m_AniName);
	}               
	else
	{
		if (((m_CenterDownCenter.x <= m_MovePos.x + 1.0f) && (m_CenterDownCenter.x >= m_MovePos.x - 1.0f)) && (m_CenterDownCenter.z <= m_MovePos.z + 1.0f && m_CenterDownCenter.z >= m_MovePos.z - 1.0f))
		{
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();

			m_pTransform->SetWorldRotY(GetYAngle(m_MovePos, m_CenterDownCenter));
		}
	}
}

void ST3_Slient::FS_WALK_IDLE(float DeltaTime)
{
	if (m_TargetDistance <= 50.0f)
		PlayerStateCheck(DeltaTime);

	DoorCheck();
}

void ST3_Slient::FS_USER_TRACE(float DeltaTime)
{
	Vector3 myPos = m_CenterDownCenter;
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();

	m_FindTimeVar += DeltaTime;

	if (m_FindTimeVar >= m_FindTime)
	{
		m_FindTimeVar = 0.0f;
		m_PathFind = false;
	}

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, TargetPos);
		m_PathFind = true;

		m_PathList.clear();

		m_PathList = m_NaviMesh->GetPathList();
		m_MovePos = m_PathList.front();
	}

	Vector3 Dir = m_MovePos - myPos;
	Dir.Normalize();

	m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		if (((myPos.x <= m_MovePos.x + 1.0f) && (myPos.x >= m_MovePos.x - 1.0f)) && (myPos.z <= m_MovePos.z + 1.0f && myPos.z >= m_MovePos.z - 1.0f))
		{
			m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter));
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();
		}
	}

	if (m_TargetDistance <= 5.0f)
	{
		int RandNum = rand() % 3;

		switch (RandNum)
		{
		case 0:
			ChangeState(STS_JAP, m_AniName);
			break;
		case 1:
			ChangeState(STS_HOOK, m_AniName);
			break;
		case 2:
			ChangeState(STS_HEAD_ATTACK, m_AniName);
			break;
		}
	}

	//또는 Hide 조건 추가 (한다면)
	if (m_TargetDistance >= 50.0f)
	{
		m_MovingPos = Vector3(36.5f, 0.0f, 11.0);
		m_PathFind = false;
		ChangeState(STS_WALK, m_AniName);
	}
}

void ST3_Slient::FS_CAN_TRACE(float DeltaTime)
{
	Vector3 myPos = m_CenterDownCenter;

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, m_MovingPos);

		m_PathFind = true;

		m_PathList.clear();
		m_PathList = m_NaviMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3 Dir = m_MovePos - myPos;
	Dir.Normalize();

	myPos += Dir * m_MoveSpeed * DeltaTime;
	bool bMove = true;

	if (m_NaviMesh->CheckCell(myPos) == false)
		bMove = false;

	if (bMove == true)
		m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		if (((myPos.x <= m_MovePos.x + 1.0f) && (myPos.x >= m_MovePos.x - 1.0f)) && (myPos.z <= m_MovePos.z + 1.0f && myPos.z >= m_MovePos.z - 1.0f))
		{
			m_pTransform->SetWorldRotY(GetYAngle(m_MovePos, m_CenterDownCenter));
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();
		}
	}

	if (m_TargetDistance <= 55.0f)
	{
		int RandNum = rand() % 5;
		RandNum += 1;

		char B[2] = "";
		char B2[11] = "Engry";
		_itoa_s(RandNum, B, 10);

		strcat_s(B2, 11, B);
		string SoundKey = B2;

		bool isPlay = false;
		for (int i = 0; i < 5; i++)
		{
			char Buffer[2] = "";
			char Buffer2[11] = "Engry";
			_itoa_s(i + 1, Buffer, 10);
			strcat_s(Buffer2, 11, Buffer);

			string Copy = Buffer2;

			if (m_3DSound->GetClipState(Copy) == PLAYING)
			{
				isPlay = true;
				break;
			}
		}

		if (isPlay == false)
			m_3DSound->Play(SoundKey);
	}
	else
	{
		m_3DSound->StopClip(0);
		m_3DSound->StopClip(1);
		m_3DSound->StopClip(2);
		m_3DSound->StopClip(3);
		m_3DSound->StopClip(4);
	}

	float Distance = myPos.GetDistance(m_MovingPos);

	if (Distance < 0.3f)
	{
		m_pTransform->SetWorldRotY(-90.0f);
		ChangeState(STS_BASH_DOOR, m_AniName);
	}
}

void ST3_Slient::FS_SUPRISE_TRACE(float DeltaTime)
{
	CGameObject* Suprise = m_pLayer->FindObjectNonCount("Suprise");

	if (m_PathFind == false)
	{
		m_PathFind = true;  

		m_NaviMesh->FindPath(m_CenterDownCenter, m_MovingPos);

		m_PathList.clear();
		m_PathList = m_NaviMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3	Dir = Suprise->GetTransformNonCount()->GetWorldPos() - m_MovePos;
	Dir.Normalize();
	Dir.y = 0.0f;

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		m_MovePos = m_PathList.front();
		m_PathList.pop_front();
	}

	m_pTransform->LookAtY(m_MovePos * -1.0f);
	m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);

	if (m_TargetDistance < 55.0f)
	{
		int RandNum = rand() % 5;
		RandNum += 1;

		char B[2] = "";
		char B2[11] = "Engry";
		_itoa_s(RandNum, B, 10);
		strcat_s(B2, 11, B);

		string SoundKey = B2;

		bool isPlay = false;
		for (int i = 0; i < 5; i++)
		{
			char Buffer[2] = "";
			char Buffer2[11] = "Engry";
			_itoa_s(i + 1, Buffer, 10);
			strcat_s(Buffer2, 11, Buffer);

			string Copy = Buffer2;

			if (m_3DSound->GetClipState(Copy) == PLAYING)
			{
				isPlay = true;
				break;
			}
		}

		if (isPlay == false)
			m_3DSound->Play(SoundKey);
	}
	else
	{
		m_3DSound->StopClip(0);
		m_3DSound->StopClip(1);
		m_3DSound->StopClip(2);
		m_3DSound->StopClip(3);
		m_3DSound->StopClip(4);
	}

	if (JBH_Stage3::m_isCanDrop == true)
	{
		CSoundManager::GetInst()->SoundPlay("GlassBracking");
		ChangeState(STS_CAN_WATE, m_AniName);
	}
}

void ST3_Slient::FS_CAN_WATE(float DeltaTime)
{
	m_WateTimeVar += DeltaTime;

	if (m_WateTime <= m_WateTimeVar)
	{
		m_WateTimeVar = 0.0f;
		m_MovingPos = Vector3(39.0f, 0.0f, 55.0f);
		m_PathFind = false;
		ChangeState(STS_CAN_TRACE, m_AniName);
	}
}

void ST3_Slient::FS_HOOK(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter));

	if (m_Animation->GetCurFrame() == 8)
		m_AttackHookBox->SetEnable(true);

	if (m_Animation->IsCurAnimEnd() == true)
	{
		m_AttackHookBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(STS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(STS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(STS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(STS_USER_TRACE, m_AniName);
		}
	}
}

void ST3_Slient::FS_JAP(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter));

	if (m_Animation->GetCurFrame() == 8)
		m_AttackJapBox->SetEnable(true);

	if (m_Animation->IsCurAnimEnd() == true)
	{
		m_AttackJapBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(STS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(STS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(STS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(STS_USER_TRACE, m_AniName);
		}
	}
}

void ST3_Slient::FS_HEAD_ATTACK(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter));

	if (m_Animation->GetCurFrame() == 12)
		m_AttackHeadBox->SetEnable(true);

	if (m_Animation->IsCurAnimEnd() == true)
	{
		m_AttackHeadBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(STS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(STS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(STS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(STS_USER_TRACE, m_AniName);
		}
	}
}

void ST3_Slient::FS_BASH_DOOR_OPEN(float DeltaTime)
{
	CDoor* getDoor = CGameManager::GetInst()->GetNearDoor(m_pScene, m_CenterDownCenter);

	if (m_Animation->GetCurFrame() == 0)
	{
		string Temp = "BashOpen";
		m_3DSound->SetVolume("BashOpen", 3.0f);
		m_3DSound->Play(Temp);
		m_Renderer->SetPosCheckStart(true);
	}

	if (m_Animation->GetCurFrame() == 6)
		getDoor->OpenFast(m_pTransform->GetWorldAxis(AXIS_X));

	if (m_Animation->IsCurAnimEnd() == true)
	{
		Vector3 DoorAxis = getDoor->GetTransformNonCount()->GetWorldAxis(AXIS_Z);
		float Offset = m_Renderer->GetDiffrentPosZ();
		Vector3 Convert = m_CenterDownCenter;
		getDoor->OpenFast(m_pTransform->GetWorldAxis(AXIS_X));

		if (fabsf(DoorAxis.x) > 0.999f)
		{
			if (DoorAxis.x > 0.0f)
				Convert.x += Offset;
			else
				Convert.x -= Offset;
		}
		else if (fabsf(DoorAxis.z) > 0.999f)
		{
			if (DoorAxis.z > 0.0f)
				Convert.x += Offset;
			else
				Convert.x -= Offset;
		}

		Convert.y = 0.0f;

		if (m_NaviMesh->CheckCell(Convert) == false)
		{
			while (true)
			{
				if (m_NaviMesh->CheckCell(Convert) == true)
					break;

				Vector3 Axis = m_pTransform->GetWorldAxis();
				Convert += Axis * -1.0f;
			}
		}

		m_pTransform->SetWorldPos(Convert);

		m_PathFind = false;
		m_MovingPos = Vector3(36.5f, 0.0f, 11.0f);
		ChangeState(STS_WALK, m_AniName);
	}
}

void ST3_Slient::FS_BASH_DOOR(float DeltaTime)
{
	if (m_Animation->GetCurFrame() == 0)
		m_Renderer->SetPosCheckStart(true);

	if (m_Animation->IsCurAnimEnd() == true)
	{
		if (m_TargetDistance <= 50.0f)
			PlayerStateCheck(DeltaTime);

		m_BashCount++;

		if (m_BashCount == 3)
		{
			m_BashCount = 0;
			CDoor* getDoor = CGameManager::GetInst()->GetNearDoor(m_pScene, m_CenterDownCenter);

			if (getDoor->GetDoorType() == DOOR_STAGE)
			{
				m_PathFind = false;
				m_MovingPos = Vector3(36.5f, 0.0f, 11.0f);
				ChangeState(STS_WALK, m_AniName);
				return;
			}
			else
				ChangeState(STS_BASH_DOOR_OPEN, m_AniName);
		}
	}
}