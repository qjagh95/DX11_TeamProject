#pragma once
#include <Scene/SceneComponent.h>
#include <Component/Decal.h>

PUN_USING

class CDoor;
class CHuman_Player;
class CScene4 : public CSceneComponent
{
public:
	CScene4();
	~CScene4();

public:
	void Start()					override;
	bool Init()						override;
	void AfterInit()				override;
	int  Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override;
	void Collision(float _fTime)	override;
	void Render(float _fTime)		override;

// �÷��̾�
private:
	CGameObject*	m_pObjPlayer;
	CTransform*		m_pPlayerTr;
	CHuman_Player*	m_pHumanPlayer;

// ��
private:
	CDoor*			m_pHeavyDoor;

public:
	void DoorInit();

// ��Į(SSD)
private:
	CGameObject*	m_pObjBSDecal;
	CGameObject*	m_pObjBBDecal;
	CDecal*			m_pBSDecal;
	CDecal*			m_pBBDecal;

public:
	void CreateMilestoneDecal();

public:
	void ConvertCorridorFBXFiles();




	void InitDecal();
	//void CreateDecal();


	void SetAmbientSoundPlay(const string& _soundName);
};