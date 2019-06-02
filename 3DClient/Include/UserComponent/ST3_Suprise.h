#pragma once
#include <Component/SoundSource.h>
#include "NPCBase.h"

PUN_USING

enum SUPRISE_STATE
{
	SS_FIND,
	SS_IDLE,
	SS_DOWN,
	SS_MAX,
};

enum SUPRISE_AXIS
{
	AX_X = 1,
	AX_Z,
};

class ST3_Suprise : public NPCBase
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	ST3_Suprise* Clone() override;

	void SetDoorName(const string& DoorName) { m_isDoorOpenMode = true; m_DoorName = DoorName; }
	void SetOffSet(float Value) { if(m_isDoorOpenMode == false) m_Offset = Value; }
	void SetYOffset(float Value) { m_YOffset = Value; }

private:
	void FS_IDLE(float DeltaTime);
	void FS_DOWN(float DeltaTime);
	void FS_FIND(float DeltaTime);

private:
	float m_FindDist;
	string m_AniName[SS_MAX];
	bool m_isDoorOpenMode;
	string m_DoorName;
	SUPRISE_AXIS m_Axis;
	float m_Offset;
	float m_YOffset;

	CGameObject* m_3DSoundObject;
	CSoundSource* m_3DSound;

public:
	ST3_Suprise();
	ST3_Suprise(const ST3_Suprise& CopyData);
	~ST3_Suprise();
};

