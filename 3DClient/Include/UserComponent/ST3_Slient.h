#pragma once
#include "NPCBase.h"

PUN_USING

enum SILENT_TRACE_STATE
{
	STS_IDLE,
	STS_WALK,
	STS_WALK_IDLE,
	STS_DOOR_TRACE,
	STS_SUPRISE_TRACE,
	STS_USER_TRACE,
	STS_CAN_TRACE,
	STS_HOOK,
	STS_CAN_WATE,
	STS_JAP,
	STS_HEAD_ATTACK,
	STS_BASH_DOOR,
	STS_BASH_DOOR_OPEN,
	STS_MAX,
};

class ST3_Slient : public NPCBase
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	ST3_Slient* Clone() override;

	void SetAttack(int Value) { m_Attack = Value; }
	int GetAttack() const { return m_Attack; }
	void DoorCollFirst(CCollider* Src, CCollider* Dest, float DeltaTime);
	void DoorCollEnd(CCollider* Src, CCollider* Dest, float DeltaTime);
	void PlayerBulletHit(CCollider* Src, CCollider* Dest, float DeltaTime);
	void MonsterColl(CCollider* Src, CCollider* Dest, float DeltaTime);

private:
	void FS_IDLE(float DeltaTime);
	void FS_WALK(float DeltaTime);
	void FS_DOOR_TRACE(float DeltaTime);
	void FS_WALK_IDLE(float DeltaTime);
	void FS_USER_TRACE(float DeltaTime);
	void FS_CAN_TRACE(float DeltaTime);
	void FS_SUPRISE_TRACE(float DeltaTime);
	void FS_BASH_DOOR(float DeltaTime);
	void FS_CAN_WATE(float DeltaTime);
	void FS_HOOK(float DeltaTime);
	void FS_JAP(float DeltaTime);
	void FS_HEAD_ATTACK(float DeltaTime);
	void FS_BASH_DOOR_OPEN(float DeltaTime);
	
	void Walk1Sound(float DeltaTime);
	void Walk2Sound(float DeltaTime);
	void BashSound(float DeltaTime);
	void BashMobSound(float DeltaTime);
	void HookSound(float DeltaTime);
	void JabSound(float DeltaTime);
	void HeadAttackSound(float DeltaTime);
	void AttackScream(float DeltaTime);

	void PlayerStateCheck(float DeltaTime);
	void DoorCheck();
	void DoorCollCheck();
	void ParkourCheck();

private:
	string m_AniName[STS_MAX];
	int m_Attack;
	int m_BashCount;
	float m_TraceDist;
	float m_WateTime;
	float m_WateTimeVar;
	Vector3 m_MovePos;

	int m_PlayerState;
	float m_FindTime;
	float m_FindTimeVar;
	float m_HitMoveSpeed;

	CGameObject* m_3DSoundObject;
	CSoundSource* m_3DSound;

	CGameObject* m_AttackJap;
	CColliderOBB3D* m_AttackJapBox;

	CGameObject* m_AttackHook;
	CColliderOBB3D* m_AttackHookBox;

	CGameObject* m_AttackHead;
	CColliderOBB3D* m_AttackHeadBox;
	
	CDoor* m_PlayerCollDoor;

	bool m_isDoorColl;
	bool m_isParkourColl;

	bool m_isHit;
	float m_SlowTime;
	float m_SlowTimeVar;

public:
	ST3_Slient();
	ST3_Slient(const ST3_Slient& CopyData);
	~ST3_Slient();
};

