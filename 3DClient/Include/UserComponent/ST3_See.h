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
	int Input(float fTime) override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Collision(float fTime) override;
	void Render(float fTime) override;
	ST3_See* Clone() override;
	
	void FS_SEE(float DeltaTime);
	void FS_BASH(float DeltaTime);

private:
	string m_AniName[SES_MAX];
	float m_SeeDist;

public:
	ST3_See();
	ST3_See(const ST3_See& CopyData);
	~ST3_See();
};

