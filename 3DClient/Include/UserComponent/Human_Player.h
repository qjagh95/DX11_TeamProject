#pragma once

#include "Component/UserComponent.h"
#include "Component/Animation.h"
#include "Component/SoundSource.h"
#include "../Client.h"

PUN_USING

enum PLAYER_DATA_TYPE 
{
	PDT_NONE,
	PDT_INT,
	PDT_FLOAT,
	PDT_STRING,
	PDT_SOUND,
	PDT_MESH,
	PDT_ANIM,
	PDT_BONE,
	PDT_SOCKET
};

enum PLAYER_STATUS 
{
	PSTATUS_NONE,
	PSTATUS_IDLE,
	PSTATUS_WOUNDED = 2,
	PSTATUS_WALK = 4,
	PSTATUS_SPRINT = 8,
	PSTATUS_STOPMOVE = 0x10,
	PSTATUS_ONAIR = 0x20,
	PSTATUS_JUMPED = 0x40,
	PSTATUS_CROUCHING = 0x80,
	PSTATUS_CROUCHED= 0x100,
	PSTATUS_INBED = 0x200,
	PSTATUS_BEDINTERACT = 0x400,
	PSTATUS_DOOR_OPEN = 0x800,
	PSTATUS_DOOR_CLOSE = 0x1000,
	PSTATUS_CAM_ON = 0x2000,
	PSTATUS_GUN = 0x4000,
};

class CHuman_Player : public PUN::CUserComponent
{
	friend class PUN::CGameObject;

protected:
	CHuman_Player();
	CHuman_Player(const CHuman_Player& player);
	~CHuman_Player();

public:
	bool Init();
	void AfterClone();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	CHuman_Player *Clone();
	
	/*
	TODO : 정범호
	상태별로 함수화 부탁드려Yo
	이렇게되면 코드 짠사람 제외 건들수가 없어요 ㅠㅠ
	↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

	void PSTATUS_NONE(float DeltaTime);
	void PSTATUS_IDLE(float DeltaTime);
	void PSTATUS_WOUNDED(float DeltaTime);
	void PSTATUS_WALK(float DeltaTime);
	void PSTATUS_SPRINT(float DeltaTime);
	void PSTATUS_STOPMOVE(float DeltaTime);
	void PSTATUS_ONAIR(float DeltaTime);
	void PSTATUS_JUMPED(float DeltaTime);
	void PSTATUS_CROUCHING(float DeltaTime);
	void PSTATUS_CROUCHED(float DeltaTime);
	void PSTATUS_INBED(float DeltaTime);
	void PSTATUS_BEDINTERACT(float DeltaTime);
	void PSTATUS_DOOR_OPEN(float DeltaTime);
	void PSTATUS_DOOR_CLOSE(float DeltaTime);
	void PSTATUS_CAM_ON(float DeltaTime);
	void PSTATUS_GUN(float DeltaTime);
	*/

private:
	class PUN::CGameObject *m_pHandSocketObj; //테스트용 저장
	class PUN::CAnimation *m_pAnimation;
	class PUN::CSoundSource *m_pSound;
	class CGameObject*	m_pInvenObj;
	class CInventory*	m_pInven;
	class CTransform*	m_pInvenTr;
	class CGameObject*	m_pHandycamObj;
	class CHandycam*	m_pHandycam;
	Vector3 m_vInitLocalPos;
	bool m_bLoadedOnce;
	int m_iState;
	float m_fTimerBuf;			//csv val :	NULL
	float m_fHideTime;			//csv val :	fHideTime
	float m_fDoorEffectDelay;	//csv val :	fDoorEffectDelay
	float m_fFWalkSpeed;		//csv val :	WalkFSpeed
	float m_fRWalkSpeed;		//csv val :	WalkRSpeed
	float m_fLWalkSpeed;		//csv val :	WalkLSpeed
	float m_fBackWalkSpeed;		//csv val :	WalkBSpeed
	float m_fSprintSpeed;		//csv val :	SprintSpeed
	float m_fCrouchToStandSpeed;//csv val : m_fCrouchToStandSpeed
	float m_fStandToCrouchSpeed;//csv val : m_fStandToCrouchSpeed
	float m_fCrouchHeight;		//csv val : m_fCrouchHeight
	float m_fTimerBuf_State;
	float m_fCrouchForwardSpd;	//m_fCrouchForwardSpd
	float m_fCrouchBackSpd;		//m_fCrouchBackSpd
	float m_fCrouchLSpd;		//m_fCrouchLSpd
	float m_fCrouchRSpd;		//m_fCrouchRSpd
	//Crouch는 스프린트가 없다
	/*
	other csv val

	mesh /anim 
		strMeshKey
		strMeshPath
		strAnimKey
		strAnimPath
		strBoneKey
		strBonePath
		
	sound : strKey, strPath, iIndex 순서
		
	*/

public:
	bool LoadData(const TCHAR *dataPath); //플레이어는 혼자니까 굳이 툴까지 만들어 다룰 필요는 없지만, 데이터 수치 및 리소스는 밖에서 준비해볼 필요가 있어 분리함
	void Open(float fTime);
	void Close(float fTime);
	void Hide_Locker(float fTime);
	void Fire_Weapon(float fTime);
};