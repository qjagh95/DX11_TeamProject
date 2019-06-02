#pragma once
#include <Scene/SceneComponent.h>

PUN_USING

class CHuman_Player;
class CDoor;
class JBH_Stage3 : public CSceneComponent
{
public:
	bool Init() override;
	void AfterInit() override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;

private:
	CHuman_Player* m_Player;
	CGameObject* m_PlayerObject;
	int m_PlayerState;
	int m_PlayerSection;

	unordered_map<string, CDoor*>* m_DoorMap;

public:
	JBH_Stage3();
	~JBH_Stage3();
};
