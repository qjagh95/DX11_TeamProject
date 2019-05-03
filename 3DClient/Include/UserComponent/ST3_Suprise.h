#pragma once
#include <Component/NPCBase.h>

PUN_USING

enum SUPRISE_STATE
{
	SS_FIND,
	SS_IDLE,
	SS_DOWN,
	SS_MAX,
};

enum AXIS_SUPRISE
{
	AX_X = 1,
	AX_Z,
};

class ST3_Suprise : public NPCBase
{
public:
	bool Init() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Collision(float fTime) override;
	void Render(float fTime) override;
	ST3_Suprise* Clone() override;

	void SetDoorOpenMode(bool Value) { m_isDoorOpenMode = Value; }
	void SetDoorName(const string& DoorName) { m_isDoorOpenMode = true; m_DoorName = DoorName; }
	void SetOffSet(float Value) { if(m_isDoorOpenMode == false) m_Offset = Value; }

private:
	void FS_IDLE(float DeltaTime);
	void FS_DOWN(float DeltaTime);
	void FS_FIND(float DeltaTime);

private:
	float m_FindDist;
	string m_AniName[SS_MAX];
	bool m_isDoorOpenMode;
	string m_DoorName;
	AXIS_SUPRISE m_Axis;
	float m_Offset;

public:
	ST3_Suprise();
	ST3_Suprise(const ST3_Suprise& CopyData);
	~ST3_Suprise();
};

