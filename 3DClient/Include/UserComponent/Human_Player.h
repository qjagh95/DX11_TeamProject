#pragma once

#include "Component/UserComponent.h"
#include "Component/Animation.h"
#include "Component/SoundSource.h"

enum PLAYER_DATA_TYPE {
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

enum PLAYER_STATUS {
	PSTATUS_NONE,
	PSTATUS_IDLE,
	PSTATUS_WOUNDED = 2,
	PSTATUS_WALK = 4,
	PSTATUS_SPRINT = 8,
	PSTATUS_STOPMOVE = 0x10,
	PSTATUS_ONAIR = 0x20,
	PSTATUS_JUMPED = 0x40,
	PSTATUS_CROUCHED = 0x80,
	PSTATUS_INBED = 0x100,
	PSTATUS_BEDINTERACT = 0x200,
	PSTATUS_DOOR_OPEN = 0x400,
	PSTATUS_DOOR_CLOSE = 0x800,
	PSTATUS_CAM_ON = 0x1000,
	PSTATUS_GUN = 0x2000
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
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	CHuman_Player *Clone();

private:
	class PUN::CAnimation *m_pAnimation;
	class PUN::CSoundSource *m_pSound;
	int m_iState;
	float m_fTimerBuf;			//csv val :	NULL
	float m_fHideTime;			//csv val :	fHideTime
	float m_fDoorEffectDelay;	//csv val :	fDoorEffectDelay
	float m_fFWalkSpeed;		//csv val :	WalkFSpeed
	float m_fRWalkSpeed;		//csv val :	WalkRSpeed
	float m_fLWalkSpeed;		//csv val :	WalkLSpeed
	float m_fBackWalkSpeed;		//csv val :	WalkBSpeed
	float m_fSprintSpeed;		//csv val :	SprintSpeed
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
	void MoveL(float fTime);
	void MoveR(float fTime);
	void Forward(float fScale, float fTime);
	void Back(float fTime);
	void Sprint(float fTime);
	void Open(float fTime);
	void Close(float fTime);
	void Hide_Locker(float fTime);
	void Fire_Weapon(float fTime);
};