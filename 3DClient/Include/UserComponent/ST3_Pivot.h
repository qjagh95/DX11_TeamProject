#pragma once
#include <Component/NPCBase.h>

PUN_USING

class ST3_Pivot : public NPCBase
{
public:
	bool Init() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Collision(float fTime) override;
	void Render(float fTime) override;
	ST3_Pivot* Clone() override;

public:
	ST3_Pivot();
	ST3_Pivot(const ST3_Pivot& CopyData);
	~ST3_Pivot();
};

