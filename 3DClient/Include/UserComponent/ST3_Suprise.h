#pragma once
#include <Component/NPCBase.h>

PUN_USING

enum SUPRISE_STATE
{
	SS_IDLE,
	SS_TURN,
	SS_MOVE,
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

public:
	ST3_Suprise();
	ST3_Suprise(const ST3_Suprise& CopyData);
	~ST3_Suprise();
};

