#pragma once
#include <Scene/SceneComponent.h>

PUN_USING

class CHuman_Player;
class CDoor;
class ST3_Slient;
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
	void BasicInit();
	void DoorInit();
	void NPCInit();

private:
	void SupriseSound(float DeltaTime);

private:
	CHuman_Player* m_Player;
	CGameObject* m_PlayerObject;
	unordered_map<string, CDoor*>* m_DoorMap;

	CGameObject* m_ChaceObject;
	ST3_Slient* m_ChaceNPC;

	static bool m_SlientMode;
	static bool m_isCanDrop;
	int m_PlayerState;
	int m_PlayerSection;
	bool m_SupriseSound;

	float m_CanDelayTime;
	float m_CanDelayTimeVar;

public:
	friend class ST3_Slient;

public:
	JBH_Stage3();
	~JBH_Stage3();
};
