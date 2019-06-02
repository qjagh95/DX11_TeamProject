#pragma once
#include "NPCBase.h"

PUN_USING

enum SILENT_TRACE_STATE
{
	STS_IDLE,
	STS_SUPRISE_TRACE,
	STS_USER_TRACE,
	STS_CAN_TRACE,
	STS_BED_FIND,
	STS_DESK_FIND,
	STS_LOCKER_FIND,
	STS_HOOK,
	STS_CAN_WATE,
	STS_JAP,
	STS_HEAD_ATTACK,
	STS_BASH_DOOR,
	STS_BASH_DOOR_OPEN,
	STS_NORMAL_DOOR_OPEN_LEFT,
	STS_NORMAL_DOOR_OPEN_RIGHT,
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

private:
	void FS_IDLE(float DeltaTime);
	void FS_USER_TRACE(float DeltaTime);
	void FS_CAN_TRACE(float DeltaTime);
	void FS_SUPRISE_TRACE(float DeltaTime);
	void FS_BASH_DOOR(float DeltaTime);
	void FS_CAN_WATE(float DeltaTime);
	void FS_BED_FIND(float DeltaTime) {}
	void FS_DESK_FIND(float DeltaTime) {}
	void FS_LOCKER_FIND(float DeltaTime) {}
	void FS_HOOK(float DeltaTime) {}
	void FS_JAP(float DeltaTime) {}
	void FS_HEAD_ATTACK(float DeltaTime) {}
	void FS_BASH_DOOR_OPEN(float DeltaTime) {}
	void FS_NORMAL_DOOR_OPEN_LEFT(float DeltaTime) {}
	void FS_NORMAL_DOOR_OPEN_RIGHT(float DeltaTime) {}
	
	void Walk1Sound(float DeltaTime);
	void Walk2Sound(float DeltaTime);
	void BashSound(float DeltaTime);

	void PlayerStateCheck(float DeltaTime);

private:
	string m_AniName[STS_MAX];
	int m_Attack;
	int m_BashCount;
	float m_TraceDist;
	float m_WateTime;
	float m_WateTimeVar;
	Vector3 m_MovePos;

	int m_PlayerState;

	CGameObject* m_3DSoundObject;
	CSoundSource* m_3DSound;

public:
	ST3_Slient();
	ST3_Slient(const ST3_Slient& CopyData);
	~ST3_Slient();
};

