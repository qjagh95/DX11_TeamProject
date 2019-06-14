#pragma once
#include <Scene/SceneComponent.h>
#include <Component/Decal.h>

PUN_USING

class CDoor;
class CHuman_Player;
class ST_Default;
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

private:
	bool m_isChangeBGM;
	bool m_isShot;
	bool m_bMouseOn;
	bool m_bMotion;
	float m_fFade;

// 플레이어
private:
	CGameObject*	m_pObjPlayer;
	CTransform*		m_pPlayerTr;
	CHuman_Player*	m_pHumanPlayer;

// 몬스터
private:
	CGameObject*	m_newMonsterObject;
	ST_Default*		m_newMonster;

// 총
private:
	CGameObject*	m_pHandGun;
	CTransform*		m_pHandGunTr;

// 문
private:
	CDoor*			m_pHeavyDoor;

public:
	void DoorInit();

public:
	void ConvertCorridorFBXFiles();

	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(CCollider * pSrc, CCollider * pDest, float fTime);

};