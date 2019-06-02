#pragma once
#include "NPCBase.h"

PUN_USING

enum SILENT_TRACE_STATE
{
	STS_IDLE,
	STS_TRACE,
	STS_BED_FIND,
	STS_DESK_FIND,
	STS_LOCKER_FIND,
	STS_HOOK,
	STS_JAP,
	STS_HEAD_ATTACK,
	STS_BASH_DOOR,
	STS_BASH_DOOR_OPEN,
	STS_NORMAL_DOOR_OPEN_LEFT,
	STS_NORMAL_DOOR_OPEN_RIGHT,
	STS_MAX,
};

class ST3_SlientTrace : public NPCBase
{
public:
	bool Init() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Collision(float fTime) override;
	void Render(float fTime) override;
	ST3_SlientTrace* Clone() override;

	void SetAttack(int Value) { m_Attack = Value; }
	int GetAttack() const { return m_Attack; }

private:
	void FS_IDLE(float DeltaTime);
	void FS_TRACE(float DeltaTime);
	void FS_BED_FIND(float DeltaTime);
	void FS_DESK_FIND(float DeltaTime);
	void FS_LOCKER_FIND(float DeltaTime);
	void FS_HOOK(float DeltaTime);
	void FS_JAP(float DeltaTime);
	void FS_HEAD_ATTACK(float DeltaTime);
	void FS_BASH_DOOR(float DeltaTime);
	void FS_BASH_DOOR_OPEN(float DeltaTime);
	void FS_NORMAL_DOOR_OPEN_LEFT(float DeltaTime);
	void FS_NORMAL_DOOR_OPEN_RIGHT(float DeltaTime);

private:
	string m_AniName[STS_MAX];
	int m_Attack;
	int m_BashCount;

	float m_TraceDist;

public:
	ST3_SlientTrace();
	ST3_SlientTrace(const ST3_SlientTrace& CopyData);
	~ST3_SlientTrace();
};

