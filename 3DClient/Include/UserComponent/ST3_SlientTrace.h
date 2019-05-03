#pragma once

#include <Component/NPCBase.h>

PUN_USING

enum SILENT_TRACE_STATE
{
	STS_IDLE,
	STS_RUN,
	STS_BED_FIND,
	STS_DESK_FIND,
	STS_LOCKER_FIND,
	STS_LEFT_DURIBUN,
	STS_RIGHT_DURIBUN,
	STS_WATE,
	STS_TRACE,
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

private:
	string m_AniName[STS_MAX];

public:
	ST3_SlientTrace();
	ST3_SlientTrace(const ST3_SlientTrace& CopyData);
	~ST3_SlientTrace();
};

