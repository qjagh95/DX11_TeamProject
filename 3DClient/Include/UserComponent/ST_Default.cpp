#include "../ClientHeader.h"
#include "ST_Default.h"
#include "Human_Player.h"
#include "Door.h"

#include "../GameManager.h"
#include "../SceneComponent/Stage1Scene.h"
#include <Component/SoundSource.h>

ST_Default::ST_Default()
{
	m_isCJHStage = true;
	m_Attack = 1;
	m_PatrolSpeed = 10.0f;
	m_HitMoveSpeed = 10.0f;
	m_FindTime = 2.0f;
	m_FindTimeVar = 0.0f;
	m_RanSelect = 0;
	m_PlayerFindDist = 55.0f;
	m_MoveSpeed = 22.5f;
	m_Hp = 5;

	m_isHit = false; 
	m_SlowTime = 0.5f;
	m_SlowTimeVar = 0.0f;

	m_3DSoundObject = NULLPTR;
	m_3DSound = NULLPTR;
	m_AttackJap = NULLPTR;
	m_AttackJapBox = NULLPTR;
	m_AttackHook = NULLPTR;
	m_AttackHookBox = NULLPTR;
	m_AttackHead = NULLPTR;
	m_AttackHeadBox = NULLPTR;
}

ST_Default::ST_Default(const ST_Default & CopyData)
	:NPCBase(CopyData)
{
}

ST_Default::~ST_Default()
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

bool ST_Default::Init()
{
	NPCBase::Init();

	m_AniName[DS_IDLE] = "idle_search_forward";
	m_AniName[DS_PATROL] = "move_forward_slow";
	m_AniName[DS_USER_TRACE] = "move_forward_fast";
	m_AniName[DS_BASH_DOOR] = "bash_door_loop";
	m_AniName[DS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[DS_HOOK] = "attack_left";
	m_AniName[DS_JAP] = "attack_middle";

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
	m_AttackJap->SetEnable(false);

	m_AttackHook = CGameObject::CreateObject("HookBody", m_pLayer);
	m_AttackHookBox = m_AttackHook->AddComponent< CColliderOBB3D>("HookBody");
	m_AttackHookBox->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.5f, 0.5f, 0.5f));
	m_AttackHookBox->SetContinueTypeName("MonsterBodyOBB");
	m_AttackHookBox->SetContinueTypeName("JapBody");
	m_AttackHookBox->SetContinueTypeName("HeadBody");
	m_AttackHook->SetEnable(false);

	m_AttackHead = CGameObject::CreateObject("HeadBody", m_pLayer);
	m_AttackHeadBox = m_AttackHead->AddComponent< CColliderOBB3D>("HeadBody");
	m_AttackHeadBox->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.5f, 0.5f, 0.5f));
	m_AttackHeadBox->SetContinueTypeName("MonsterBodyOBB");
	m_AttackHeadBox->SetContinueTypeName("HookBody");
	m_AttackHeadBox->SetContinueTypeName("JapBody");
	m_AttackHead->SetEnable(false);

	ChangeState(DS_PATROL, m_AniName);

	m_3DSoundObject = CGameObject::CreateObject("3DCanSound", m_pLayer);
	m_3DSound = m_3DSoundObject->AddComponent<PUN::CSoundSource>("3DCanSound");
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

	m_Animation->SetCallback("move_forward_fast", this, 8, &ST_Default::Walk1Sound);
	m_Animation->SetCallback("move_forward_fast", this, 19, &ST_Default::Walk2Sound);
	m_Animation->SetCallback("bash_door_loop", this, 10, &ST_Default::BashSound);
	m_Animation->SetCallback("bash_door_loop", this, 1, &ST_Default::BashMobSound);
	m_Animation->SetCallback("attack_right", this, 6, &ST_Default::HookSound);
	m_Animation->SetCallback("attack_left", this, 6, &ST_Default::JabSound);
	m_Animation->SetCallback("attack_middle", this, 7, &ST_Default::HeadAttackSound);

	m_Animation->SetCallback("attack_right", this, 1, &ST_Default::AttackScream);
	m_Animation->SetCallback("attack_left", this, 1, &ST_Default::AttackScream);
	m_Animation->SetCallback("attack_middle", this, 1, &ST_Default::AttackScream);

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

	Vector3 Axis[3];
	Axis[0] = m_pTransform->GetWorldAxis(AXIS_X);
	Axis[1] = m_pTransform->GetWorldAxis(AXIS_Y);
	Axis[2] = m_pTransform->GetWorldAxis(AXIS_Z);

	m_BodyOBB->SetInfo(Vector3(0.0f, 7.0f, 0.0f), Axis, Vector3(0.8f,7.0f, 0.8f));
	m_BodyOBB->SetCollisionCallback(CCT_ENTER, this, &ST_Default::DoorCollFirst);
	m_BodyOBB->SetCollisionCallback(CCT_LEAVE, this, &ST_Default::DoorCollEnd);
	m_BodyOBB->SetCollisionCallback(CCT_ENTER, this, &ST_Default::PlayerBulletHit);

	return true;
}

int ST_Default::Input(float DeltaTime)
{
	NPCBase::Input(DeltaTime);

	return 0;
}

int ST_Default::Update(float DeltaTime)
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
	case DS_IDLE:
		FS_IDLE(DeltaTime);
		break;
	case DS_PATROL:
		FS_PATROL(DeltaTime);
		break;
	case DS_USER_TRACE:
		FS_USER_TRACE(DeltaTime);
		break;
	case DS_HOOK:
		FS_HOOK(DeltaTime);
		break;
	case DS_JAP:
		FS_JAP(DeltaTime);
		break;
	case DS_HEAD_ATTACK:
		FS_HEAD_ATTACK(DeltaTime);
		break;
	case DS_BASH_DOOR:
		FS_BASH_DOOR(DeltaTime);
		break;
	case DS_BASH_DOOR_OPEN:
		FS_BASH_DOOR_OPEN(DeltaTime);
		break;
	}

	return 0;
}

int ST_Default::LateUpdate(float DeltaTime)
{
	return 0;
}

void ST_Default::Collision(float DeltaTime)
{
}

void ST_Default::Render(float DeltaTime)
{
}

ST_Default * ST_Default::Clone()
{
	return new ST_Default(*this);
}

void ST_Default::DoorCollFirst(CCollider * Src, CCollider * Dest, float DeltaTime)
{
}

void ST_Default::DoorCollEnd(CCollider * Src, CCollider * Dest, float DeltaTime)
{
}

void ST_Default::PlayerBulletHit(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "BulletBody")
	{
		m_isHit = true;
		m_SlowTimeVar = 0.0f;
	}
}

void ST_Default::MonsterColl(CCollider * Src, CCollider * Dest, float DeltaTime)
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

void ST_Default::FS_IDLE(float DeltaTime)
{
	//스테이지 변경 bool값
	if (CStage1Scene::m_isEventColl == true && m_isCJHStage == true)
		ChangeState(DS_BASH_DOOR_OPEN, m_AniName);

	else if (m_isCJHStage == false)
		ChangeState(DS_PATROL, m_AniName);
}

void ST_Default::FS_BASH_DOOR(float DeltaTime)
{
}

void ST_Default::FS_BASH_DOOR_OPEN(float DeltaTime)
{
	CDoor* getDoor = CGameManager::GetInst()->GetNearDoor(m_pScene, m_CenterDownCenter);

	if (m_Animation->GetCurFrame() == 1)
	{
		string Temp = "BashOpen";
		m_3DSound->SetVolume("BashOpen", 3.0f);
		m_3DSound->Play(Temp);
		string Temp2 = "BashMob1";
		m_3DSound->SetVolume(Temp2, 3.0f);
		m_3DSound->Play(Temp2);
		m_Renderer->SetPosCheckStart(true);
	}

	if (m_Animation->GetCurFrame() == 6)
		getDoor->OpenFast(m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f);

	if (m_Animation->IsCurAnimEnd() == true)
	{
		float Offset = m_Renderer->GetDiffrentPosZ();
		Vector3 Convert = m_WorldPos;
		Convert.z += Offset * -1.0f;
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
		ChangeState(DS_USER_TRACE, m_AniName);
	}
}

void ST_Default::FS_PATROL(float DeltaTime)
{
	Vector3 PatrolPos = m_vecPatrolPos[m_RanSelect];
	Vector3 NextPos;

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, PatrolPos);
		m_PathFind = true;

		m_PathList.clear();

		m_PathList = m_NaviMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3 Dir = m_MovePos - m_CenterDownCenter;
	Dir.Normalize();

	m_pTransform->Move(Dir, 10.0f, DeltaTime);

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		if (((m_CenterDownCenter.x <= m_MovePos.x + 1.0f) && (m_CenterDownCenter.x >= m_MovePos.x - 1.0f)) && (m_CenterDownCenter.z <= m_MovePos.z + 1.0f && m_CenterDownCenter.z >= m_MovePos.z - 1.0f))
		{
			m_pTransform->SetWorldRotY(GetYAngle(m_MovePos, m_CenterDownCenter));
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();
		}
	}
	
	if (m_CenterDownCenter.Distance(PatrolPos) < 5.0f)
	{
		m_RanSelect = rand() % (int)m_vecPatrolPos.size();
		m_PathFind = false;
	}

	if (m_TargetDistance < 30.0f)
	{
		m_PathFind = false;
		ChangeState(DS_USER_TRACE, m_AniName);
	}
}

void ST_Default::DoorCollCheck(float DeltaTime)
{
	CDoor* getDoor = CGameManager::GetInst()->GetNearDoor(m_pScene, m_CenterDownCenter);

	Matrix Temp = getDoor->GetTransformNonCount()->GetPosDelta();
	Vector3 Pos;
	Pos.x = Temp._41;
	Pos.z = Temp._43;

	float Dist = m_CenterDownCenter.GetDistance(Pos);

	if (Dist <= 3.0f)
	{
		m_PathFind = false;
		ChangeState(DS_PATROL, m_AniName);
	}

}

void ST_Default::FS_USER_TRACE(float DeltaTime)
{
	Vector3 myPos = m_CenterDownCenter;
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();

	m_FindTimeVar += DeltaTime;

	if (m_PathFind == false)
	{
		m_NaviMesh->FindPath(m_CenterDownCenter, TargetPos);
		m_PathFind = true;

		m_PathList.clear();

		m_PathList = m_NaviMesh->GetPathList();

		if(m_PathList.empty() == false)
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
			m_pTransform->SetWorldRotY(GetYAngle(TargetPos, myPos));
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
			ChangeState(DS_JAP, m_AniName);
			break;
		case 1:
			ChangeState(DS_HOOK, m_AniName);
			break;
		case 2:
			ChangeState(DS_HEAD_ATTACK, m_AniName);
			break;
		}
	}

	if (m_TargetDistance > m_PlayerFindDist)
	{
		m_PathFind = false;
		m_RanSelect = rand() % (int)m_vecPatrolPos.size();
		ChangeState(DS_PATROL, m_AniName);
	}

	DoorCollCheck(DeltaTime);
}

void ST_Default::FS_HOOK(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter) );

	if (m_Animation->GetCurFrame() == 8)
	{
		if (m_TargetDistance <= 3.0f)
			_PLAYER->Hit_By_Enemy(m_BodyOBB, DeltaTime);

		//m_AttackHookBox->SetEnable(true);
	}

	if (m_Animation->IsCurAnimEnd() == true)
	{
		//m_AttackHookBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(DS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(DS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(DS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(DS_USER_TRACE, m_AniName);
		}
	}
}

void ST_Default::FS_JAP(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter));

	if (m_Animation->GetCurFrame() == 8)
	{
		if (m_TargetDistance <= 3.0f)
			_PLAYER->Hit_By_Enemy(m_BodyOBB, DeltaTime);
	}

	if (m_Animation->IsCurAnimEnd() == true)
	{
		//m_AttackJapBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(DS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(DS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(DS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(DS_USER_TRACE, m_AniName);
		}
	}
}

void ST_Default::FS_HEAD_ATTACK(float DeltaTime)
{
	Vector3 TargetPos = m_TargetTransform->GetWorldPos();
	m_pTransform->SetWorldRotY(GetYAngle(TargetPos, m_CenterDownCenter) + 180.0f);

	if (m_Animation->GetCurFrame() == 12)
	{
		if (m_TargetDistance <= 3.0f)
			_PLAYER->Hit_By_Enemy(m_BodyOBB, DeltaTime);
	}

	if (m_Animation->IsCurAnimEnd() == true)
	{
		//m_AttackHeadBox->SetEnable(false);

		if (m_TargetDistance <= 5.0f)
		{
			int RandNum = rand() % 3;

			switch (RandNum)
			{
			case 0:
				ChangeState(DS_JAP, m_AniName);
				break;
			case 1:
				ChangeState(DS_HOOK, m_AniName);
				break;
			case 2:
				ChangeState(DS_HEAD_ATTACK, m_AniName);
				break;
			}
		}
		else
		{
			m_PathFind = false;
			ChangeState(DS_USER_TRACE, m_AniName);
		}
	}
}

void ST_Default::PlayerRangeCheck(float DeltaTime)
{
	if (m_TargetDistance <= m_PlayerFindDist)
	{
		m_PathFind = false;
		ChangeState(DS_USER_TRACE, m_AniName);
	}
}

void ST_Default::Walk1Sound(float DeltaTime)
{
	string temp = "Walk1";
	m_3DSound->Play(temp);
}

void ST_Default::Walk2Sound(float DeltaTime)
{
	string temp = "Walk2";
	m_3DSound->Play(temp);
}

void ST_Default::BashSound(float DeltaTime)
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

void ST_Default::BashMobSound(float DeltaTime)
{
	if (m_TargetDistance > 60.0f)
		return;

	string SoundKey = "BashMob1";
	m_3DSound->Play(SoundKey);
}

void ST_Default::HookSound(float DeltaTime)
{
	string SoundKey = "HookSound";
	m_3DSound->Play(SoundKey);
}

void ST_Default::JabSound(float DeltaTime)
{
	string SoundKey = "JabSound";
	m_3DSound->Play(SoundKey);
}

void ST_Default::HeadAttackSound(float DeltaTime)
{
	string SoundKey = "HeadAttackSound";
	m_3DSound->Play(SoundKey);
}

void ST_Default::AttackScream(float DeltaTime)
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

	if (isPlay == false)
		m_3DSound->Play(SoundKey);
}

void ST_Default::Hit(float DeltaTime)
{
}
