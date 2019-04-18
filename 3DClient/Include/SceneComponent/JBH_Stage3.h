#pragma once
#include <Scene/SceneComponent.h>

PUN_USING

class JBH_Stage3 : public CSceneComponent
{
public:
	bool Init() override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;

public:
	JBH_Stage3();
	~JBH_Stage3();
};
