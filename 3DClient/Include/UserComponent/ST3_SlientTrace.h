#pragma once

#include <Component/NPCBase.h>

PUN_USING

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

public:
	ST3_SlientTrace();
	ST3_SlientTrace(const ST3_SlientTrace& CopyData);
	~ST3_SlientTrace();
};

