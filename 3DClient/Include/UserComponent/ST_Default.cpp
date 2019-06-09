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

	m_3DSoundObject = NULLPTR;
	m_3DSound = NULLPTR;
}

ST_Default::ST_Default(const ST_Default & CopyData)
	:NPCBase(CopyData)
{
}

ST_Default::~ST_Default()
{
	SAFE_RELEASE(m_3DSoundObject);
	SAFE_RELEASE(m_3DSound);
}

bool ST_Default::Init()
{
	NPCBase::Init();

	m_AttackBoxObject = CGameObject::CreateObject("AttackBox", m_pLayer);
	m_AttackBox = m_AttackBox->AddComponent< CColliderOBB3D>("AttackBox");
	m_AttackBox->SetInfo(Vector3(0.0f, 0.0f, 1.0f), Vector3::Axis, Vector3(1.0f, 6.0f, 1.0f));
	m_AttackBox->SetEnable(false);

	m_AniName[DS_IDLE] = "idle_search_forward";
	m_AniName[DS_WALK] = "move_forward_slow";
	m_AniName[DS_PATROL] = "move_forward_fast";
	m_AniName[DS_USER_TRACE] = "move_forward_slow";
	m_AniName[DS_BASH_DOOR] = "bash_door_loop";
	m_AniName[DS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[DS_HOOK] = "attack_left";
	m_AniName[DS_JAP] = "attack_middle";
	m_AniName[DS_HIT] = "move_forward_fast";

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Renderer->SetSelectBoneName("NPCMedium-Pelvis");

	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("enem_def_2.anm");

	ChangeState(DS_IDLE, m_AniName);

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

	m_BodyOBB->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(0.8f, 6.0f, 0.8f));
	m_BodyOBB->SetCollisionCallback(CCT_ENTER, this, &ST_Default::DoorCollFirst);
	m_BodyOBB->SetCollisionCallback(CCT_LEAVE, this, &ST_Default::DoorCollEnd);

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

	switch (m_State)
	{
	case DS_IDLE:
		FS_IDLE(DeltaTime);
		break;
	case DS_WALK:
		FS_WALK(DeltaTime);
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
	case DS_HIT:
		FS_HIT(DeltaTime);
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
}

void ST_Default::FS_IDLE(float DeltaTime)
{
	//스테이지 변경 bool값
	if (true)
	{
	}
	else if (m_isCJHStage == false)
		ChangeState(DS_PATROL, m_AniName);
}

void ST_Default::FS_HIT(float DeltaTime)
{
}

void ST_Default::FS_BASH_DOOR(float DeltaTime)
{
}

void ST_Default::FS_BASH_DOOR_OPEN(float DeltaTime)
{
}

void ST_Default::FS_PATROL(float DeltaTime)
{

}

void ST_Default::FS_WALK(float DeltaTime)
{
}

void ST_Default::FS_USER_TRACE(float DeltaTime)
{
}

void ST_Default::FS_HOOK(float DeltaTime)
{
}

void ST_Default::FS_JAP(float DeltaTime)
{
}

void ST_Default::FS_HEAD_ATTACK(float DeltaTime)
{
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
