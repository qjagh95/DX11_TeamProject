#pragma once

#include "Component/UserComponent.h"
#include "Component/Animation.h"
#include "Component/SoundSource.h"
#include "../CameraEff.h"
#include "../Client.h"

PUN_USING

enum PLAYER_MOVE_TAG
{
	PMT_NONE,
	PMT_FORWARD,
	PMT_BACKWARD,
	PMT_LEFT = 4,
	PMT_RIGHT = 8
};

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
	PSTATUS_CAM_TAKING_ON = 0x2000,
	PSTATUS_CAM_ON = 0x4000,
	PSTATUS_CAM_TAKING_OFF = 0x8000,
	PSTATUS_GUN_TAKING_ON = 0x10000,
	PSTATUS_GUN		= 0x20000,
	PSTATUS_GUN_TAKING_OFF = 0x40000,
	PSTATUS_ITEM = 0x80000,
	PSTATUS_UPDATEPOS = 0x100000,
	PSTATUS_INACTIVE = 0x10000000
};


static const char* strFootStepSndHeader			= "FTS_";
static const char* strWalkSndHeader				= "Walk";
static const char* strRunSndHeader				= "Run";
static const char* strLandSndHeader				= "Land";
static const char* strMatConcreteSndHeader		= "Concrete_";
static const char* strMatWoodSndHeader			= "Wood_";
static const char* strMatThinWoodSndHeader		= "ThinWood_";
static const char* strMatSmallWaterSndHeader	= "Smallwater_";
static const char* strMatHeavySndHeader			= "MtrlHeavy_";
static const char* strMatLightSndHeader			= "MtrlLight_";
static const char* strMatGrassSndHeader			= "Grass_";
static const char* strMatBloodSndHeader			= "Blood_";
static const char* strMatCarpetSndHeader		= "Carpet_";


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
	FOOTSTEP_ENVIRONMENT m_eFootStep;
	CCameraEff *pCamEffManager;
	class PUN::CGameObject *m_pHandSocketObj; //테스트용 저장
	class PUN::CGameObject *m_pHeadObj;
	class PUN::CGameObject *m_pHandGun;
	class PUN::CAnimation *m_pAnimation;
	class PUN::CSoundSource *m_pSound;
	class PUN::CGameObject *m_pCamModelObj;
	class PUN::CTransform *m_pRootBonePos;
	std::string m_strFTSKey;
	std::vector<int> m_vecIgnoreRightArmKey;
	std::vector<int> m_vecIgnoreLegsAndLeftArmKey;
	std::vector<int> m_vecIgnoreLegsKey;
	std::vector<int> m_vecIgnoreUppderBodyKey;

	PUN::PPARTANIM m_pPartCamAnim;

	PUN::ANIMATIONCALLBACK m_arrAnimCallbacks[16];
	class CInventory*	m_pInven;
	class CHandycam*	m_pHandycam;
	Vector3 m_vInitLocalPos;
	bool  m_bLoadedOnce;
	int	  m_iState;
	int   m_iPrevState;
	int   m_iDirFlag;
	int   m_iPrevDirFlag;
	Vector3 m_vMoveDirection;
	float m_fItemTakeTimerBuf	;
	float m_fCamTakeTime		;
	float m_fGunTakeTime		;
	float m_fTimerBuf		;	//csv val :	NULL
	float m_fHideTime		;	//csv val :	fHideTime
	float m_fDoorEffectDelay;	//csv val :	fDoorEffectDelay
	float m_fFWalkSpeed		;	//csv val :	WalkFSpeed
	float m_fRWalkSpeed		;		//csv val :	WalkRSpeed
	float m_fLWalkSpeed		;		//csv val :	WalkLSpeed
	float m_fBackWalkSpeed	;		//csv val :	WalkBSpeed
	float m_fSprintSpeed	;		//csv val :	SprintSpeed
	float m_fCrouchToStandSpeed	;//csv val : m_fCrouchToStandSpeed
	float m_fStandToCrouchSpeed	;//csv val : m_fStandToCrouchSpeed
	float m_fCrouchHeight		;		//csv val : m_fCrouchHeight
	float m_fTimerBuf_State		;
	float m_fCrouchForwardSpd	;	//m_fCrouchForwardSpd
	float m_fCrouchBackSpd		;		//m_fCrouchBackSpd
	float m_fCrouchLSpd			;		//m_fCrouchLSpd
	float m_fCrouchRSpd			;		//m_fCrouchRSpd
	float m_fViewMaxAngleY		;
	float m_fViewMinAngleY		;
	float m_fGunMaxAngleY		;
	float m_fGunMinAngleY		;
	bool  m_bNaviOn;
	Vector3 m_vCamWorldOffset	;
	Vector3 m_vCamLocalOffset;
	Vector3 m_vCamCrouchLocalOffset;
	Vector3 m_vTracerBuf;
#include "Player_Item_Value.hpp"
#include "Player_Interact_Value.hpp"
#include "Player_Cam_Value.hpp"
#include "Player_Move_Value.hpp"
#include "Player_Weapon_Value.hpp"

public:
	bool Init_Items();
	bool Init_Interact();
	bool Init_Cam();
	bool Init_Move();
	bool Init_Weapon();
	

	float GetWorlPosY() const;
	Vector3 GetWorldPos() const;
	bool LoadData(const TCHAR *dataPath); //플레이어는 혼자니까 굳이 툴까지 만들어 다룰 필요는 없지만, 데이터 수치 및 리소스는 밖에서 준비해볼 필요가 있어 분리함
	void Interact(float fTime);
	void Open_Door_Normal(float fTime);
	void Close_Door_Normal(float fTime);
	void Open_Door_Fast(float fTime);
	void Close_Door_Fast(float fTime);
	void Hide_Locker(float fTime);
	void Hiding_Locker(float fTime);
	void Exit_Locker(float fTime);
	void Hide_Bed(float fTime);
	void Hiding_Bed(float fTime);
	void Exit_Bed(float fTime);
	void Pakur_Doing(float fTime, int iHeightType);
	void OnWalk(float fTime);
	void OnSprint(float fTime);
	void OnCrouching(float fTime);
	void OnCrouchWalk(float fTime);
	void Pickup_Item(float fTime);
	void Open_Item(float fTime);
	void Close_Item(float fTime);
	void Pickup_Weapon(float fTime);
	void Aim_Weapon(float fTime);
	void Fire_Weapon(float fTime);
	void HandyCam_Start();
	void HandyCam_End();
	void HandyCam_On(float fTime);
	void HandyCam_Using(float fTime);
	void HandyCam_Off(float fTime);
	void Move(PUN::AXIS axis, float fSpeed, float fTime);
	void SetAnimNotify();
	const Vector3& GetMoveDirection() const;

	void FootStepWalkNormal		(float fTime);
	void FootStepRun			(float fTime);
	void FootStepCrouch			(float fTime);
	void FootStepFallDownLow	(float fTime);
	void FootStepFallDownHigh	(float fTime);
	void CrouchForwardMendPos	(float fTime);
	void CrouchBackwardMendPos	(float fTime);
	void CrouchSideMendPos		(float fTime);

	void SetFootStepEnvironment(FOOTSTEP_ENVIRONMENT eEnv);
	FOOTSTEP_ENVIRONMENT GetFootStepEnvironment() const;

	void AfterLoad();

	void OnDestroyInven();
	void OnDestroyInteract();
	void OnDestroyCam();
	void OnDestroyMove();
	void OnDestroyWeap();

	///////////////// 콜백 함수 선언부 //////////////////

	void InteractCallBackEnter(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void InteractCallBackStay(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void InteractCallBackLeave(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);

	void HitCallBackEnter(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void HitCallBackStay(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void HitCallBackLeave(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);

	void RayCallBackEnter(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void RayCallBackStay(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);
	void RayCallBackLeave(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime);


	//모듈화 기본함수
	int Input_Cam(float fTime);
	int CamUpdate(float fTime);
	int CamLateUpdate(float fTime);
	int Input_Move(float fTime);
	int MoveUpdate(float fTime);
	int MoveLateUpdate(float fTime);
	int Input_Interact(float fTime);
	int InteractUpdate(float fTime);
	int InteractLateUpdate(float fTime);
	int Input_Items(float fTime);
	int ItemUpdate(float fTime);
	int ItemLateUpdate(float fTime);
	int WeaponInput(float fTime);
	int WeaponUpdate(float fTime);
	int WeaponLateUpdate(float fTime);
	
	//custom public Functions
#include "Cam_Func_Header.hpp"
#include "Interact_Func_Header.hpp"
#include "Item_Func_Header.hpp"
#include "Move_Func_Header.hpp"
#include "Weap_Func_Header.hpp"
};