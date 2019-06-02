#pragma once
#include "NPCBase.h"

PUN_USING

enum SEE_STATE
{
	SES_BASH,
	SES_SEE,
	SES_MAX,
};

class ST3_See : public NPCBase
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	ST3_See* Clone() override;
	
	void FS_SEE(float DeltaTime);
	void FS_BASH(float DeltaTime);

	void SetTurnRotY(float Value) { m_TurnRotY = Value; }
	float GetTurnRot() const { return m_TurnRotY; }

private:
	string m_AniName[SES_MAX];
	float m_SeeDist;
	float m_TurnRotY;

public:
	ST3_See();
	ST3_See(const ST3_See& CopyData);
	~ST3_See();
};

