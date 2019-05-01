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
	void SetDoorName(const string& DoorName) { m_DoorName = DoorName; }

private:
	float m_FindDist;
	string m_AniName[SS_MAX];
	bool m_isDoorOpenMode;
	string m_DoorName;

private:
	void FS_IDLE(float DeltaTime);
	void FS_DOWN(float DeltaTime);
	void FS_FIND(float DeltaTime);

public:
	ST3_Suprise();
	ST3_Suprise(const ST3_Suprise& CopyData);
	~ST3_Suprise();
};

