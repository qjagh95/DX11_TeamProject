#pragma once
#include "NPCBase.h"

PUN_USING

enum DEFULT_STATE
{
	DS_IDLE,
	DS_PATROL,
	DS_USER_TRACE,
	DS_HOOK,
	DS_JAP,
	DS_HEAD_ATTACK,
	DS_BASH_DOOR,
	DS_BASH_DOOR_OPEN,
	DS_MAX,
};

class ST_Default : public NPCBase
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	ST_Default* Clone() override;

	void SetAttack(int Value) { m_Attack = Value; }
	int GetAttack() const { return m_Attack; }
	void DoorCollFirst(CCollider* Src, CCollider* Dest, float DeltaTime);
	void DoorCollEnd(CCollider* Src, CCollider* Dest, float DeltaTime);
	void PlayerBulletHit(CCollider* Src, CCollider* Dest, float DeltaTime);
	void MonsterColl(CCollider* Src, CCollider* Dest, float DeltaTime);

	void AddPatrolPos(const Vector3& Pos) { m_vecPatrolPos.push_back(Pos); }
	void SetPatrolSpeed(float Value) { m_PatrolSpeed = Value; }
	void SetPlayerFindDistance(float Dist) { m_PlayerFindDist = Dist; }

	void IamNotFirstStage() { m_isCJHStage = false; }
	void PlayerRangeCheck(float DeltaTime);

	void Walk1Sound(float DeltaTime);
	void Walk2Sound(float DeltaTime);
	void BashSound(float DeltaTime);
	void BashMobSound(float DeltaTime);
	void HookSound(float DeltaTime);
	void JabSound(float DeltaTime);
	void HeadAttackSound(float DeltaTime);
	void AttackScream(float DeltaTime);

private:
	void FS_IDLE(float DeltaTime);
	void FS_USER_TRACE(float DeltaTime);
	void FS_HOOK(float DeltaTime);
	void FS_JAP(float DeltaTime);
	void FS_HEAD_ATTACK(float DeltaTime);
	void FS_BASH_DOOR(float DeltaTime);
	void FS_BASH_DOOR_OPEN(float DeltaTime);
	void FS_PATROL(float DeltaTime);

	void DoorCollCheck(float DeltaTime);

private:
	string m_AniName[DS_MAX];
	float m_PlayerFindDist;
	float m_PatrolSpeed;
	int m_Attack;
	float m_TraceDist;
	Vector3 m_MovePos;
	vector<Vector3> m_vecPatrolPos;

	int m_PlayerState;
	float m_FindTime;
	float m_FindTimeVar;
	float m_HitMoveSpeed;
	int m_RanSelect;

	CGameObject* m_3DSoundObject;
	CSoundSource* m_3DSound;

	CGameObject* m_AttackJap;
	CColliderOBB3D* m_AttackJapBox;

	CGameObject* m_AttackHook;
	CColliderOBB3D* m_AttackHookBox;

	CGameObject* m_AttackHead;
	CColliderOBB3D* m_AttackHeadBox;

	bool m_isDoorColl;
	bool m_isCJHStage;

	bool m_isHit;
	float m_SlowTime;
	float m_SlowTimeVar;

public:
	ST_Default();
	ST_Default(const ST_Default& CopyData);
	~ST_Default();
};

