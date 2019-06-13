#include "../ClientHeader.h"

#include "Human_Player.h"
#include <Input.h>
#include <PathManager.h>
#include <Resource/ResourcesManager.h>
#include <Component/Animation.h>
#include <Component/BoneSocket.h>
#include <Component/Renderer.h>
#include <Component/Transform.h>
#include <Component/SoundSource.h>
#include <Scene/Scene.h>
#include <Component/Camera.h>
#include <Component/ColliderOBB3D.h>
#include <Component/Arm.h>
#include <Device.h>
#include "Inventory.h"
#include "../CommonSoundLoader.h"
#include "Handycam.h"
#include "NavigationMesh.h"
#include "Locker.h"

static const char* strFootStepSndHeader = "FTS_";
static const char* strWalkSndHeader = "Walk";
static const char* strRunSndHeader = "Run";
static const char* strLandSndHeader = "Land";
static const char* strMatConcreteSndHeader = "Concrete_";
static const char* strMatWoodSndHeader = "Wood_";
static const char* strMatThinWoodSndHeader = "ThinWood_";
static const char* strMatSmallWaterSndHeader = "Smallwater_";
static const char* strMatHeavySndHeader = "MtrlHeavy_";
static const char* strMatLightSndHeader = "MtrlLight_";
static const char* strMatGrassSndHeader = "Grass_";
static const char* strMatBloodSndHeader = "Blood_";
static const char* strMatCarpetSndHeader = "Carpet_";

static const char* strVoiceStealth = "PlayerBreath_Stealth";
static const char* strVoiceBreathLow = "PlayerBreath_L";
static const char* strVoiceBreathMedium = "PlayerBreath_M";
static const char* strVoiceBreathHigh = "PlayerBreath_H";
static const char* strVoiceBreathSafe = "PlayerBreath_Safe";
static const char* strVoiceHurt = "Player_Hurt";
static const char* strVoiceChoke = "Player_Choke";
static const char* strVoiceCrawl = "Player_Crawl";
static const char* strVoiceHit = "Player_Hit";
static const char* strVoiceLand = "Player_Land";
static const char* strVoiceRun = "Player_Run";
static const char* strVoiceFall = "Player_Fall";
static const char* strVoiceDie = "Player_Die";

static const char* strVoices[14] =
{
	nullptr,
	strVoiceStealth,
	strVoiceCrawl,
	strVoiceBreathLow,
	strVoiceBreathMedium,
	strVoiceBreathHigh,
	strVoiceBreathSafe,
	strVoiceRun,
	strVoiceFall,
	strVoiceLand,
	strVoiceChoke,
	strVoiceHurt,
	strVoiceHit,
	strVoiceDie
};

using namespace PUN;

bool CHuman_Player::m_bHasGun = true;

CHuman_Player * CHuman_Player::m_pInst = nullptr;


CHuman_Player::CHuman_Player() :
	m_pAnimation(nullptr),
	m_pSound(nullptr),
	m_fTimerBuf(0.f),
	m_iState(1),
	m_fTimerBuf_State(0.f),
	m_pMovePointer(nullptr),
	m_pHeadObj(nullptr),
	m_fViewMaxAngleY(90.f),
	m_fViewMinAngleY(-90.f),
	m_fGunMaxAngleY(80.f),
	m_fGunMinAngleY(-80.f),
	pCamEffManager(nullptr),
	m_vCamWorldOffset(0.f, -7.5f, -5.f),
	m_vCamLocalOffset(0.f, 2.65f, -1.1f),
	m_vCamCrouchLocalOffset(-0.75f, 1.8f, 1.75f),
	m_fItemTakeTimerBuf(0.f),
	m_fCamTakeTime(0.8f),
	m_fGunTakeTime(1.2f),
	m_eFootStep(FTSE_DEFAULT),
	m_pPartCamAnim(nullptr),
	m_fBreathIntensity(0.f),
	m_iPrevState(0),
	m_bNaviOn(true),
	m_cInitLoopFinished(2),
	m_fMaxHideBedAngleY(15.f),
	m_fMaxHideBedAngleX(20.f),
	m_fMaxHideLockerAngleY(80.f),
	m_fMaxHideLockerAngleX(15.f),
	m_eVoiceType(PVT_NONE),
	m_ePlayingVoiceType(PVT_LOW),
	m_iPlayingVoiceNumber(0),
	m_vPrevWorldPos(Vector3::Zero),
	m_vVelocity(Vector3::Zero),
	m_pGeometryBody(nullptr),
	m_bGeomTest(false)
{
	m_pInst = this;
	m_eComType = (COMPONENT_TYPE)UT_PLAYER;
	m_vecCollidingGeom.clear();
#include "Player_Item_Values.txt"
#include "Player_Interact_Values.txt"
#include "Player_Cam_Values.txt"
#include "Player_Move_Values.txt"
#include "Player_Weapon_Values.txt"
	m_vecIgnoreRightArmKey.reserve(24);
	m_vecIgnoreRightArmKey.push_back(33);
	m_vecIgnoreRightArmKey.push_back(34);
	m_vecIgnoreRightArmKey.push_back(35);
	m_vecIgnoreRightArmKey.push_back(36);
	m_vecIgnoreRightArmKey.push_back(37);
	m_vecIgnoreRightArmKey.push_back(38);
	m_vecIgnoreRightArmKey.push_back(39);
	m_vecIgnoreRightArmKey.push_back(40);
	m_vecIgnoreRightArmKey.push_back(41);
	m_vecIgnoreRightArmKey.push_back(42);
	m_vecIgnoreRightArmKey.push_back(43);
	m_vecIgnoreRightArmKey.push_back(44);
	m_vecIgnoreRightArmKey.push_back(45);
	m_vecIgnoreRightArmKey.push_back(46);
	m_vecIgnoreRightArmKey.push_back(47);
	m_vecIgnoreRightArmKey.push_back(48);
	m_vecIgnoreRightArmKey.push_back(49);
	m_vecIgnoreRightArmKey.push_back(50);
	m_vecIgnoreRightArmKey.push_back(51);
	m_vecIgnoreRightArmKey.push_back(52);
	m_vecIgnoreRightArmKey.push_back(53);
	m_vecIgnoreRightArmKey.push_back(54);
	m_vecIgnoreRightArmKey.push_back(55);
	m_vecIgnoreRightArmKey.push_back(56);

	m_vecIgnoreLegsKey.reserve(12);
	m_vecIgnoreLegsKey.push_back(58);
	m_vecIgnoreLegsKey.push_back(60);
	m_vecIgnoreLegsKey.push_back(62);
	m_vecIgnoreLegsKey.push_back(65);
	m_vecIgnoreLegsKey.push_back(63);
	m_vecIgnoreLegsKey.push_back(64);
	m_vecIgnoreLegsKey.push_back(66);
	m_vecIgnoreLegsKey.push_back(68);
	m_vecIgnoreLegsKey.push_back(71);
	m_vecIgnoreLegsKey.push_back(69);
	m_vecIgnoreLegsKey.push_back(70);
	m_vecIgnoreLegsKey.push_back(72);


	m_vecIgnoreLegsAndLeftArmKey.reserve(49);
	m_vecIgnoreLegsAndLeftArmKey.push_back(0);
	m_vecIgnoreLegsAndLeftArmKey.push_back(1);
	m_vecIgnoreLegsAndLeftArmKey.push_back(2);
	m_vecIgnoreLegsAndLeftArmKey.push_back(3);
	m_vecIgnoreLegsAndLeftArmKey.push_back(4);
	m_vecIgnoreLegsAndLeftArmKey.push_back(5);
	m_vecIgnoreLegsAndLeftArmKey.push_back(6);
	m_vecIgnoreLegsAndLeftArmKey.push_back(7);
	m_vecIgnoreLegsAndLeftArmKey.push_back(8);
	m_vecIgnoreLegsAndLeftArmKey.push_back(9);
	m_vecIgnoreLegsAndLeftArmKey.push_back(10);
	m_vecIgnoreLegsAndLeftArmKey.push_back(11);
	m_vecIgnoreLegsAndLeftArmKey.push_back(12);
	m_vecIgnoreLegsAndLeftArmKey.push_back(13);
	m_vecIgnoreLegsAndLeftArmKey.push_back(14);
	m_vecIgnoreLegsAndLeftArmKey.push_back(15);
	m_vecIgnoreLegsAndLeftArmKey.push_back(16);
	m_vecIgnoreLegsAndLeftArmKey.push_back(17);
	m_vecIgnoreLegsAndLeftArmKey.push_back(18);
	m_vecIgnoreLegsAndLeftArmKey.push_back(19);
	m_vecIgnoreLegsAndLeftArmKey.push_back(20);
	m_vecIgnoreLegsAndLeftArmKey.push_back(21);
	m_vecIgnoreLegsAndLeftArmKey.push_back(22);
	m_vecIgnoreLegsAndLeftArmKey.push_back(23);
	m_vecIgnoreLegsAndLeftArmKey.push_back(24);
	m_vecIgnoreLegsAndLeftArmKey.push_back(25);
	m_vecIgnoreLegsAndLeftArmKey.push_back(26);
	m_vecIgnoreLegsAndLeftArmKey.push_back(27);
	m_vecIgnoreLegsAndLeftArmKey.push_back(28);
	m_vecIgnoreLegsAndLeftArmKey.push_back(29);
	m_vecIgnoreLegsAndLeftArmKey.push_back(30);
	m_vecIgnoreLegsAndLeftArmKey.push_back(31);
	m_vecIgnoreLegsAndLeftArmKey.push_back(32);
	m_vecIgnoreLegsAndLeftArmKey.push_back(57);
	m_vecIgnoreLegsAndLeftArmKey.push_back(58);
	m_vecIgnoreLegsAndLeftArmKey.push_back(59);
	m_vecIgnoreLegsAndLeftArmKey.push_back(60);
	m_vecIgnoreLegsAndLeftArmKey.push_back(61);
	m_vecIgnoreLegsAndLeftArmKey.push_back(62);
	m_vecIgnoreLegsAndLeftArmKey.push_back(63);
	m_vecIgnoreLegsAndLeftArmKey.push_back(64);
	m_vecIgnoreLegsAndLeftArmKey.push_back(65);
	m_vecIgnoreLegsAndLeftArmKey.push_back(66);
	m_vecIgnoreLegsAndLeftArmKey.push_back(67);
	m_vecIgnoreLegsAndLeftArmKey.push_back(68);
	m_vecIgnoreLegsAndLeftArmKey.push_back(69);
	m_vecIgnoreLegsAndLeftArmKey.push_back(70);
	m_vecIgnoreLegsAndLeftArmKey.push_back(71);
	m_vecIgnoreLegsAndLeftArmKey.push_back(72);


	m_vecIgnoreUppderBodyKey;
	m_vecIgnoreUppderBodyKey.push_back(1);
	m_vecIgnoreUppderBodyKey.push_back(4);
	m_vecIgnoreUppderBodyKey.push_back(5);
	m_vecIgnoreUppderBodyKey.push_back(6);
	m_vecIgnoreUppderBodyKey.push_back(7);
	m_vecIgnoreUppderBodyKey.push_back(8);
	m_vecIgnoreUppderBodyKey.push_back(9);
	m_vecIgnoreUppderBodyKey.push_back(10);
	m_vecIgnoreUppderBodyKey.push_back(11);
	m_vecIgnoreUppderBodyKey.push_back(12);
	m_vecIgnoreUppderBodyKey.push_back(13);
	m_vecIgnoreUppderBodyKey.push_back(14);
	m_vecIgnoreUppderBodyKey.push_back(15);
	m_vecIgnoreUppderBodyKey.push_back(16);
	m_vecIgnoreUppderBodyKey.push_back(17);
	m_vecIgnoreUppderBodyKey.push_back(18);
	m_vecIgnoreUppderBodyKey.push_back(19);
	m_vecIgnoreUppderBodyKey.push_back(20);
	m_vecIgnoreUppderBodyKey.push_back(21);
	m_vecIgnoreUppderBodyKey.push_back(22);
	m_vecIgnoreUppderBodyKey.push_back(23);
	m_vecIgnoreUppderBodyKey.push_back(24);
	m_vecIgnoreUppderBodyKey.push_back(25);
	m_vecIgnoreUppderBodyKey.push_back(26);
	m_vecIgnoreUppderBodyKey.push_back(27);
	m_vecIgnoreUppderBodyKey.push_back(28);
	m_vecIgnoreUppderBodyKey.push_back(29);
	m_vecIgnoreUppderBodyKey.push_back(30);
	m_vecIgnoreUppderBodyKey.push_back(31);
	m_vecIgnoreUppderBodyKey.push_back(32);
	m_vecIgnoreUppderBodyKey.push_back(33);
	m_vecIgnoreUppderBodyKey.push_back(34);
	m_vecIgnoreUppderBodyKey.push_back(35);
	m_vecIgnoreUppderBodyKey.push_back(36);
	m_vecIgnoreUppderBodyKey.push_back(37);
	m_vecIgnoreUppderBodyKey.push_back(38);
	m_vecIgnoreUppderBodyKey.push_back(39);
	m_vecIgnoreUppderBodyKey.push_back(40);
	m_vecIgnoreUppderBodyKey.push_back(41);
	m_vecIgnoreUppderBodyKey.push_back(42);
	m_vecIgnoreUppderBodyKey.push_back(43);
	m_vecIgnoreUppderBodyKey.push_back(44);
	m_vecIgnoreUppderBodyKey.push_back(45);
	m_vecIgnoreUppderBodyKey.push_back(46);
	m_vecIgnoreUppderBodyKey.push_back(47);
	m_vecIgnoreUppderBodyKey.push_back(48);
	m_vecIgnoreUppderBodyKey.push_back(49);
	m_vecIgnoreUppderBodyKey.push_back(50);
	m_vecIgnoreUppderBodyKey.push_back(51);
	m_vecIgnoreUppderBodyKey.push_back(52);
	m_vecIgnoreUppderBodyKey.push_back(53);
	m_vecIgnoreUppderBodyKey.push_back(54);
	m_vecIgnoreUppderBodyKey.push_back(55);
	m_vecIgnoreUppderBodyKey.push_back(56);

	m_vPrevMoveDirection = Vector3::Zero;
	m_vMoveDirection = Vector3::Zero;
}

CHuman_Player::CHuman_Player(const CHuman_Player & player) :
	PUN::CUserComponent(player)
{

	m_bLoadedOnce = player.m_bLoadedOnce;
	m_iState = 1;
	m_vecCollidingGeom.clear();
	m_fHideTime = player.m_fHideTime;
	m_fDoorEffectDelay = player.m_fDoorEffectDelay;
	m_fFWalkSpeed = player.m_fFWalkSpeed;
	m_fRWalkSpeed = player.m_fRWalkSpeed;
	m_fLWalkSpeed = player.m_fLWalkSpeed;
	m_fBackWalkSpeed = player.m_fBackWalkSpeed;
	m_fSprintSpeed = player.m_fSprintSpeed;
	m_fCrouchToStandSpeed = player.m_fCrouchToStandSpeed;
	m_fStandToCrouchSpeed = player.m_fStandToCrouchSpeed;
	m_fCrouchHeight = player.m_fCrouchHeight;

	m_fCrouchForwardSpd = player.m_fCrouchForwardSpd;
	m_fCrouchBackSpd = player.m_fCrouchBackSpd;
	m_fCrouchLSpd = player.m_fCrouchLSpd;
	m_fCrouchRSpd = player.m_fCrouchRSpd;
	m_fViewMaxAngleY = player.m_fViewMaxAngleY;
	m_fViewMinAngleY = player.m_fViewMinAngleY;
	m_fGunMaxAngleY = player.m_fGunMaxAngleY;
	m_fGunMinAngleY = player.m_fGunMinAngleY;
	m_vCamWorldOffset = player.m_vCamWorldOffset;

	pCamEffManager = nullptr;
	m_pHeadObj = nullptr; m_pAnimation = nullptr; m_pSound = nullptr;
	m_fTimerBuf = 0.f;
	m_fTimerBuf_State = 0.f;
}

CHuman_Player::~CHuman_Player()
{
	m_pInst = nullptr;
	if (pCamEffManager->IsFirstPersonEnabled())
		pCamEffManager->SetFirstPersonViewEnable();
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pSound);
	SAFE_RELEASE(m_pMovePointer);
	SAFE_RELEASE(m_pHeadObj);
	SAFE_RELEASE(m_pInven);
	SAFE_RELEASE(m_pHandycam);
	SAFE_RELEASE(m_pCamModelObj);
	SAFE_RELEASE(m_pGeometryBody);
	SAFE_RELEASE(m_pRootBonePos);

	SAFE_RELEASE(m_pHandGun);
	OnDestroyInteract();
	OnDestroyInven();
	OnDestroyCam();
	OnDestroyMove();
	OnDestroyWeap();
}

bool CHuman_Player::Init()
{
	pCamEffManager = CCameraEff::GetInst();
	pCamEffManager->Init();
	pCamEffManager->SetFirstPersonViewEnable();

	PUN::CGameObject*pObj = PUN::CGameObject::CreateObject("player_move_point", m_pLayer, true);

	m_pMovePointer = pObj->GetTransform();

	SAFE_RELEASE(pObj);

	PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();
	_SMgr->SetAudioCoordSize(8.f);

	PUN::CTransform *pCamera = m_pScene->GetMainCameraTransform();
	pCamEffManager->SetCamTransform(pCamera);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);

	PUN::CInput *_Input = PUN::CInput::GetInst();

	_Input->AddKey("W", 'W');
	_Input->AddKey("S", 'S');
	_Input->AddKey("D", 'D');
	_Input->AddKey("A", 'A');
	_Input->AddKey("E", 'E');
	_Input->AddKey("Shift", VK_SHIFT);
	_Input->AddKey("Space", VK_SPACE);
	_Input->AddKey("I", 'I');
	_Input->AddKey("G", 'G');
	_Input->AddKey("Ctrl", VK_CONTROL);
	_Input->AddKey("Tab", VK_TAB);
	_Input->AddKey("NumPad1", VK_NUMPAD1);
	_Input->AddKey("NumPad2", VK_NUMPAD2);
	_Input->ShowMouse(false);

	AfterClone();

	Init_Move();
	Init_Cam();
	Init_Interact();
	Init_Items();
	Init_Weapon();

	return true;
}

void CHuman_Player::AfterClone()
{

	m_pTransform->SetLocalRotY(180.f);
	m_pTransform->SetWorldScale(0.0375f, 0.0375f, 0.0375f);
	m_pHeadObj = PUN::CGameObject::CreateObject("Player_Daegari", m_pLayer, true);
	m_pHeadObj->SetFrustrumCullUse(false);
	m_pObject->SetFrustrumCullUse(false);

	/*
	PUN::CColliderOBB3D *pOBB = m_pHeadObj->AddComponent<PUN::CColliderOBB3D>("daegari");
	pOBB->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.5f, 1.5f, 1.5f));
	SAFE_RELEASE(pOBB);
	*/

	PUN::CTransform*	pTr = m_pHeadObj->GetTransform();
	pTr->SetWorldScale(0.0375f, 0.0375f, 0.0375f);

	PUN::CRenderer*	pHeadRenderer = m_pHeadObj->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pHeadRenderer->SetMesh("head", TEXT("Head.msh"));
	pHeadRenderer->DontRenderMat(true);
	pHeadRenderer->SetDecalEnable(false);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pHeadRenderer);

	// Inventory
	PUN::CGameObject *pInvObj = CGameObject::CreateObject("Inven", m_pLayer, true);

	m_pInven = pInvObj->AddComponent<CInventory>("Inven");
	m_pInven->SetInvenMax(19);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial = pInvObj->AddComponent<CMaterial>("Inven");
	pMaterial->SetDiffuseTex(0, "Inventory", TEXT("UI/Inven/InvenBack.png"));

	SAFE_RELEASE(pInvObj);
	SAFE_RELEASE(pMaterial);

	// Handycam
	PUN::CGameObject *pHandycamObj = CGameObject::CreateObject("Handycam", m_pLayer, true);

	m_pHandycam = pHandycamObj->AddComponent<CHandycam>("Handycam");
	pHandycamObj->SetFrustrumCullUse(false);
	m_pHandycam->Release();
	CTransform*	pHandycamTr = pHandycamObj->GetTransform();

	SAFE_RELEASE(pHandycamTr);
	SAFE_RELEASE(pHandycamObj);

	//Handycam fake model
	m_pCamModelObj = PUN::CGameObject::CreateObject("Handycam_model", m_pLayer, true);
	pHandycamTr = m_pCamModelObj->GetTransform();
	m_pCamModelObj->SetFrustrumCullUse(false);
	//pHandycamTr->SetWorldPos(0.f, 0.f, 0.f);
	pHandycamTr->SetWorldScale(m_pTransform->GetWorldScale());
	pHandycamTr->SetLocalRot(-90.f, -60.f, -130.f);
	PUN::CRenderer *pCamRenderer = m_pCamModelObj->AddComponent<PUN::CRenderer>("renderer");
	pCamRenderer->SetMesh("cam", TEXT("Handycam.msh"));
	m_pCamModelObj->SetEnable(false);

	SAFE_RELEASE(pCamRenderer);
	SAFE_RELEASE(pHandycamTr);

	m_pHandGun = PUN::CGameObject::CreateObject("Player_Gun", m_pLayer, true);
	m_pHandGun->SetFrustrumCullUse(false);
	pTr = m_pHandGun->GetTransform();
	pTr->SetWorldScale(m_pTransform->GetWorldScale() * 1.5f);
	pTr->SetLocalRot(170.f, 35.f, -135.f); //y가 180도를 넘지 않게 할것
	//pTr->SetWorldPos(0.f, 0.f, 15.f);

	SAFE_RELEASE(pTr);

	/*CGameObject*	pTestObj = CGameObject::CreateObject("HitEffect", m_pLayer);
	CHitEffectAlpha*	pTest = pTestObj->AddComponent<CHitEffectAlpha>("HitEffect");

	SAFE_RELEASE(pTest);
	SAFE_RELEASE(pTestObj);*/

	LoadData(TEXT("PlayerData.csv"));
}

int CHuman_Player::Input(float fTime)
{
	if (m_iState & (PSTATUS_DIE))
	{
		m_pTransform->SetLocalPos(0.f, -50.f, 0.f);

		PUN::CTransform *pTR = m_pScene->GetMainCameraTransform();
		PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

		Vector3 vCamPos = pHeadTrans->GetWorldPos();
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamCrouchLocalOffset.x;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamCrouchLocalOffset.y;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamCrouchLocalOffset.z;

		Vector3 vRot = pHeadTrans->GetWorldRot();
		pTR->SetWorldPos(vCamPos);
		pTR->SetWorldRot(vRot);

		SAFE_RELEASE(pTR);
		return -16;
	}

	if (m_pAnimation->GetCurrentClip()->strName == "player_stand_idle")
	{
		if (m_iState & PSTATUS_HIT)
		{
			m_iState ^= PSTATUS_HIT;
		}
		if (m_iState & PSTATUS_PICKUP)
		{
			m_iState ^= PSTATUS_PICKUP;
		}
	}
	
	m_bGeomTest = false;
	m_pMovePointer->SetWorldPos(m_pTransform->GetWorldPos());
	if (m_fBreathIntensity > 0.f)
	{
		if (m_fBreathIntensity < 0.25f)
			m_eVoiceType = PVT_LOW;
		else if (m_fBreathIntensity < 0.5f)
			m_eVoiceType = PVT_MED;
		else
			m_eVoiceType = PVT_HIGH;

		m_fBreathIntensity -= fTime * 0.125f;
	}
	else
	{
		m_fBreathIntensity = 0.f;
		m_eVoiceType = PVT_LOW;
	}
	
	if (m_iState & PSTATUS_CAMOUT)
	{
		m_iState ^= PSTATUS_TURNING;
	}
	
	PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

	Vector3 vTotMoveDiff = Vector3::Zero;
	
	m_iDirFlag = 0;
	m_iState |= PSTATUS_STOPMOVE;

	bool bBlend = false;

	PUN::CInput *_Input = PUN::CInput::GetInst();

	//Sound
	m_strFTSKey = strFootStepSndHeader;

	switch (m_eFootStep)
	{
	case FTSE_DEFAULT:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_BLOOD:
		m_strFTSKey += strMatBloodSndHeader;
		break;
	case FTSE_CARPET:
		m_strFTSKey += strMatCarpetSndHeader;
		break;
	case FTSE_CONCRETE:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_CRUNCH:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_DEEPWATER:
		m_strFTSKey += strMatSmallWaterSndHeader;
		break;
	case FTSE_GRASS:
		m_strFTSKey += strMatGrassSndHeader;
		break;
	case FTSE_LADDERSQUEAK:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_LADDER:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_MATL:
		m_strFTSKey += strMatHeavySndHeader;
		break;
	case FTSE_MATH:
		m_strFTSKey += strMatHeavySndHeader;
		break;
	case FTSE_SCRAPESQUEEZE:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	case FTSE_SMALLWATER:
		m_strFTSKey += strMatSmallWaterSndHeader;
		break;
	case FTSE_THINWOOD:
		m_strFTSKey += strMatThinWoodSndHeader;
		break;
	case FTSE_WOOD:
		m_strFTSKey += strMatWoodSndHeader;
		break;
	case FTSE_WATER:
		m_strFTSKey += strMatSmallWaterSndHeader;
		break;
	default:
		m_strFTSKey += strMatConcreteSndHeader;
		break;
	}

	//Other Keys
	if (_Input->KeyPress("Tab"))
	{
		if (m_iState & PSTATUS_CAMOUT)
		{
			m_iState ^= PSTATUS_CAMOUT;
			_Input->ShowMouse(false);
		}
		else
		{
			m_iState |= PSTATUS_CAMOUT;
			_Input->ShowMouse(true);
			
		}
	}


	if (_Input->KeyPress("I"))
	{
		if (m_iState & PSTATUS_ITEM)
		{
			m_iState ^= (PSTATUS_ITEM | PSTATUS_INACTIVE);

			if ((m_iState & PSTATUS_DOCX) == 0)
			{
				_Input->ShowMouse(false);
			}
			
			Close_Item(fTime);
		}
		else
		{
			m_iState |= (PSTATUS_ITEM | PSTATUS_INACTIVE);
			_Input->ShowMouse(true);
			Open_Item(fTime);
		}
	}

	Input_Items(fTime);
	Input_Interact(fTime);
	Input_Cam(fTime);
	Input_Move(fTime);
	WeaponInput(fTime);

	if (m_iState & PSTATUS_INACTIVE)
	{
		PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		
		//Vector3 vCamPos = pHeadTrans->GetWorldPos();
		Vector3 vCamPos = pHeadTrans->GetWorldPos() + m_vCamWorldOffset;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamLocalOffset.x;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamLocalOffset.y;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamLocalOffset.z;

		pCamTrans->SetWorldPos(vCamPos);

		SAFE_RELEASE(pCamTrans);
		SAFE_RELEASE(pHeadTrans);


		return 1; //Inactive로 인한 종료 1반환
	}
	
	if (_Input->KeyPress("G"))
	{
		if ((m_iState & PSTATUS_GUN) == 0)
		{
			if ((m_iState & PSTATUS_CAM_ON) == 0)
			{

				m_fItemTakeTimerBuf = 0.f;
				m_iState |= PSTATUS_CAM_TAKING_ON;
				m_iState |= PSTATUS_CAM_ON;
				m_pCamModelObj->SetEnable(true);
				m_pPartCamAnim->bActivated = true;
				m_pPartCamAnim->pNextClip = nullptr;
				m_pPartCamAnim->pCurClip = m_pPartCamAnim->mapPartClips.find("player_camcorder_raise")->second;
				m_pPartCamAnim->pDefaultClip = m_pPartCamAnim->mapPartClips.find("player_camcorder_idle")->second;
			}
			else
			{
				if ((m_iState & PSTATUS_CAM_TAKING_OFF) == 0)
				{

					m_pPartCamAnim->pNextClip = m_pPartCamAnim->mapPartClips.find("player_camcorder_lower")->second;
					m_pPartCamAnim->pDefaultClip = m_pPartCamAnim->mapPartClips.find("player_stand_idle")->second;
					m_fItemTakeTimerBuf = 0.f;

					HandyCam_End();
					m_iState |= PSTATUS_CAM_TAKING_OFF;
				}

			}
		}

	}

	if (m_iState & PSTATUS_CAM_ON)
	{
		if (m_iState & PSTATUS_CAM_TAKING_ON)
		{
			if (m_fItemTakeTimerBuf < m_fCamTakeTime)
			{
				m_fItemTakeTimerBuf += fTime;
				HandyCam_On(fTime);
			}
			else
			{
				m_fItemTakeTimerBuf = 0.f;
				m_iState ^= PSTATUS_CAM_TAKING_ON;
				HandyCam_Start();
			}
		}
		else if (m_iState & PSTATUS_CAM_TAKING_OFF)
		{
			if (m_fItemTakeTimerBuf < m_fCamTakeTime)
			{
				m_fItemTakeTimerBuf += fTime;
				HandyCam_Off(fTime);
			}
			else
			{
				m_fItemTakeTimerBuf = 0.f;
				m_iState ^= (PSTATUS_CAM_TAKING_OFF | PSTATUS_CAM_ON);
				m_pCamModelObj->SetEnable(false);
				m_pPartCamAnim->bActivated = false;
			}
		}
		else
		{
			HandyCam_On(fTime);
		}

	}

	if (_Input->KeyPress("E"))
	{
		if (m_iState & PSTATUS_BED)
		{
			m_iState |= PSTATUS_HIDEINTERACT_OUT;

			if(m_iState & PSTATUS_HIDEINTERACT)
				m_iState ^= PSTATUS_HIDEINTERACT;

			if (m_iState & PSTATUS_CROUCHED)
			{
				if (!_Input->KeyPush("Ctrl"))
				{
					m_iState ^= PSTATUS_CROUCHED;
					m_iState ^= PSTATUS_CROUCHING;
				}
				
			}
				
		}
		if (m_iState & PSTATUS_LOCKER)
		{
			m_iState |= PSTATUS_HIDEINTERACT_OUT;
			if(m_iState & PSTATUS_HIDEINTERACT)
				m_iState ^= PSTATUS_HIDEINTERACT;
			if (m_pHidingLocker)
				m_pHidingLocker->OpenDoor();

			m_pAnimation->SetDefaultClip("player_stand_idle");
		}
		//else
		//{
		//	m_iState |= PSTATUS_BED;
		//	m_iState |= PSTATUS_HIDEINTERACT;
		//}
	}
	
	if (m_iState & PSTATUS_BED)
	{
		if (m_iState & PSTATUS_HIDEINTERACT)
		{
			Hide_Bed(fTime); 
			if (pHeadTrans)
				InputRot(fTime, pHeadTrans, vTotMoveDiff);
			SAFE_RELEASE(pHeadTrans);
			return PSTATUS_HIDEINTERACT | PSTATUS_BED;
		}
		else if (m_iState & PSTATUS_HIDEINTERACT_OUT)
		{

			Exit_Bed(fTime);
			if (pHeadTrans)
				InputRot(fTime, pHeadTrans, vTotMoveDiff);
			SAFE_RELEASE(pHeadTrans);
			return PSTATUS_HIDEINTERACT_OUT | PSTATUS_BED;
		}
		else
		{
			Hiding_Bed(fTime);
			if (pHeadTrans)
				InputRot(fTime, pHeadTrans, vTotMoveDiff);
			SAFE_RELEASE(pHeadTrans);
			return PSTATUS_BED;
		}
	}
	else
	{
		if (m_iState & PSTATUS_LOCKER)
		{
			if (m_iState & PSTATUS_HIDEINTERACT)
			{
				Hide_Locker(fTime);
				if (pHeadTrans)
					InputRot(fTime, pHeadTrans, vTotMoveDiff);
				SAFE_RELEASE(pHeadTrans);
				return PSTATUS_LOCKER | PSTATUS_HIDEINTERACT;
			}

			else if (m_iState & PSTATUS_HIDEINTERACT_OUT)
			{
				Exit_Locker(fTime);
				if (pHeadTrans)
					InputRot(fTime, pHeadTrans, vTotMoveDiff);
				SAFE_RELEASE(pHeadTrans);
				return PSTATUS_LOCKER;
			}
			else
			{
				Hiding_Locker(fTime);
				if (pHeadTrans)
					InputRot(fTime, pHeadTrans, vTotMoveDiff);
				SAFE_RELEASE(pHeadTrans);
				return PSTATUS_LOCKER | PSTATUS_HIDEINTERACT_OUT;
			}
		}
		else if (m_iState & PSTATUS_FALLING)
		{
			m_eVoiceType = PVT_FALL;
			m_pAnimation->ChangeClip("player_falling_forward_loop");
			m_eVoiceType = PVT_FALL;
			if (pHeadTrans)
				InputRot(fTime, pHeadTrans, vTotMoveDiff);
			SAFE_RELEASE(pHeadTrans);
			return PSTATUS_FALLING;
		}
		
	}

	if (m_iState & PSTATUS_VAULT)
	{
		if (pHeadTrans)
			InputRot(fTime, pHeadTrans, vTotMoveDiff);
		SAFE_RELEASE(pHeadTrans);
		return PSTATUS_VAULT;
	}
	

	CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene,
		m_pTransform->GetWorldPos());

	if (pMesh)
	{
		if (m_bNaviOn)
		{
			float	fY = pMesh->GetY(m_pTransform->GetWorldPos());
			Vector3 vPos = m_pTransform->GetWorldPos();
			vPos.y = fY;
			m_pTransform->SetWorldPos(vPos);
			m_pMovePointer->SetWorldPos(vPos);
		}
		else
		{
			Vector3 vPos = m_pTransform->GetWorldPos();
			m_pMovePointer->SetWorldPos(vPos);
		}
	}
	

	//Movement
	InputMove(fTime, bBlend);

	std::vector<PUN::CColliderOBB3D*>::iterator Itr = m_vecCollidingGeom.begin();
	std::vector<PUN::CColliderOBB3D*>::iterator ItrEnd = m_vecCollidingGeom.end();

	for (; Itr != ItrEnd; ++Itr)
	{
		Geometry_Push(m_pGeometryBody, (*Itr), fTime);
	}
	//if (m_bGeomTest)
	//{
	//	m_pMovePointer->SetWorldPos(m_vPrevWorldPos);
	//}
	if (m_cMoveCheckFlag == 3)
	{
		m_pMovePointer->SetWorldPos(m_vPrevWorldPos);
	}
	
	else
	{
		Vector3 vMovePos = m_pMovePointer->GetWorldPos();
		CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene, vMovePos);
		if (m_bNaviOn && pMesh)
		{
			if (!pMesh->GetNavigationCell(vMovePos))
			{
				m_pMovePointer->SetWorldPos(m_vPrevWorldPos);
			}
			else
			{
				float fY = pMesh->GetY(vMovePos);
				if (fY != vMovePos.y)
				{
					vMovePos.y = fY;
					m_pMovePointer->SetWorldPos(vMovePos);
				}
			}
		}
	}

	m_vecCollidingGeom.clear();

	if (m_cInitLoopFinished < 1)
	{
		Vector3 vHeadPos = pHeadTrans->GetWorldPos();

		Vector3 vZdir = m_pTransform->GetWorldAxis(PUN::AXIS_Z);
		Vector3 vXdir = m_pTransform->GetWorldAxis(PUN::AXIS_X);
		Vector3 vPos = m_pMovePointer->GetWorldPos();
		Vector3 vCurrPos = m_pTransform->GetWorldPos();

		//y축 제외 대가리와 몸통 거리를 구하자

		Vector3 vBodytoHead = vHeadPos - vCurrPos;
		vBodytoHead.y = vCurrPos.y;
		//pMovePointer를 이 거리만큼 움직여본다
		m_pMovePointer->Move(vBodytoHead);
		//이동된 pMovePointer의 거리에서 이전 pMovePointer와 거리 구한다
		Vector3 vHeadNavPos = m_pMovePointer->GetWorldPos();
		Vector3 vMovePointerDiff = vHeadNavPos - vPos;
		//만약 포인터 이동 거리(y제외)가 머리와 몸통 사이보다 거리가 짧으면, 그만큼 못가는거다
		float fDistPointer = (vMovePointerDiff.x * vMovePointerDiff.x) + (vMovePointerDiff.z * vMovePointerDiff.z);
		float fDistBtoH = (vBodytoHead.x * vBodytoHead.x) + (vBodytoHead.z * vBodytoHead.z);

		//Move Pointer 초기화
		m_pMovePointer->SetWorldPos(vPos);
		if (fDistPointer < fDistBtoH - 0.00001f)
		{
			Vector3 vMoveDiff = Vector3(vHeadNavPos.x - vHeadPos.x, 0.f, vHeadNavPos.z - vHeadPos.z);

			if (m_vMoveDirection.z == 2.f)
			{
				m_iState ^= PSTATUS_SPRINT;
				m_fBreathIntensity += fTime;
				if (m_fBreathIntensity > 8.f)
					m_fBreathIntensity = 8.f;
				vMoveDiff *= 4.f;
				m_pAnimation->ChangeClip("player_walk_forward");
			}


			m_pMovePointer->Move(vMoveDiff);
		}

		vTotMoveDiff = m_pMovePointer->GetWorldPos() - vCurrPos;
	}
	
	//회전
	if ((m_iState & PSTATUS_INACTIVE) == 0)
	{
		if (pHeadTrans)
			InputRot(fTime, pHeadTrans, vTotMoveDiff);
	}

	SAFE_RELEASE(pHeadTrans);
	Vector3 vCurrPos = m_pMovePointer->GetWorldPos();
	m_pTransform->SetWorldPos(vCurrPos);
	
	m_pAnimation->KeepBlendSet(bBlend);

	return 0;
}

int CHuman_Player::Update(float fTime)
{
	m_bGeomTest = true;
	if ((m_iState & PSTATUS_LOCKER) || (m_iState & PSTATUS_BED))
	{
		m_eVoiceType = PVT_STEALTH;
	}

	if (m_iState & PSTATUS_CROUCHING)
	{
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_eVoiceType = PVT_STEALTH;
			if (m_fTimerBuf_State < m_fStandToCrouchSpeed)
			{
				m_fTimerBuf_State += fTime;

				float fLerp = m_fTimerBuf_State / m_fStandToCrouchSpeed;

				if (m_pScene)
				{
					PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
					if ((m_iState & PSTATUS_CAMOUT) == 0)
					{
						Vector3 vCamRot = pCamTrans->GetWorldRot();
						if (vCamRot.x > 5.f)
						{
							vCamRot.x -= 96.f * fTime;
							if (vCamRot.x < 5.f)
								vCamRot.x = 5.f;

							
						}
						pCamTrans->SetWorldRot(vCamRot);
					}

					SAFE_RELEASE(pCamTrans);
				}
				

				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y += m_fCrouchHeight * fLerp;

				m_pTransform->SetLocalPos(vLocPos);
			}
			else
			{
				m_iState &= ~PSTATUS_CROUCHING;
				m_fTimerBuf_State = 0.f;

				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y += m_fCrouchHeight;
				m_pTransform->SetLocalPos(vLocPos);
			}
		}
		else
		{
			if (m_fTimerBuf_State < m_fCrouchToStandSpeed)
			{
				m_fTimerBuf_State += fTime;

				m_fTimerBuf_State += fTime;

				float fLerp = m_fTimerBuf_State / m_fCrouchToStandSpeed;

				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y -= m_fCrouchHeight * fLerp;

				m_pTransform->SetLocalPos(vLocPos);
			}
			else
			{
				m_iState &= ~PSTATUS_CROUCHING;
				m_fTimerBuf_State = 0.f;

				m_pTransform->SetLocalPos(m_vInitLocalPos);
			}
		}

	}


	VoiceSound(fTime);
	pCamEffManager->Update(fTime);

	MoveUpdate(fTime);
	CamUpdate(fTime);
	InteractUpdate(fTime);
	ItemUpdate(fTime);

	WeaponUpdate(fTime);
	//m_pTransform->SetWorldPos(m_pMovePointer->GetWorldPos());
	return 0;
}

int CHuman_Player::LateUpdate(float fTime)
{
	//"player_crouch_object_pickup_h45v35", PUN::AO_ONCE_RETURN);
	//"player_object_pickup_h40v70", PUN::AO_ONCE_RETURN);

	pCamEffManager->LateUpdate(fTime);

	m_vPrevMoveDirection = m_vMoveDirection;
	m_iPrevState = m_iState;

	MoveLateUpdate(fTime);
	CamLateUpdate(fTime);
	InteractLateUpdate(fTime);
	ItemLateUpdate(fTime);
	WeaponLateUpdate(fTime);
	Vector3 vCurrPos = m_pTransform->GetWorldPos();
	m_vVelocity = vCurrPos - m_vPrevWorldPos;
	m_vPrevWorldPos = vCurrPos;
	return 0;
}

void CHuman_Player::Collision(float fTime)
{
	int bb = 0;
}

PLAYER_VOICE_TYPE CHuman_Player::GetVoiceType() const
{
	return m_eVoiceType;
}

void CHuman_Player::SetVoiceType(PLAYER_VOICE_TYPE eVoiceType)
{
	m_eVoiceType = eVoiceType;
}

void CHuman_Player::VoiceSound(float fTime)
{
	if (m_eVoiceType < 1)
		return;
	std::string strStopVoiceKey;

	if (m_ePlayingVoiceType != PVT_NONE)
	{
		if (m_iPlayingVoiceNumber > 0)
		{
			
			if (strVoices[m_ePlayingVoiceType])
			{
				strStopVoiceKey = strVoices[m_ePlayingVoiceType];
				strStopVoiceKey += std::to_string(m_iPlayingVoiceNumber);
				if (m_ePlayingVoiceType == m_eVoiceType)
				{
					if (m_pSound->GetClipState(strStopVoiceKey) == PLAYING)
					{
						return;
					}
				}
				else
				{

					m_pSound->StopClip(strStopVoiceKey, false);
				}
			}
			
		}
		
		
	}

	if (m_ePlayingVoiceType == PVT_HURT)
	{
		if (m_iPlayingVoiceNumber > 0)
		{

			if (strVoices[m_ePlayingVoiceType])
			{
				strStopVoiceKey = strVoices[m_ePlayingVoiceType];
				strStopVoiceKey += std::to_string(m_iPlayingVoiceNumber);
				if (m_ePlayingVoiceType == m_eVoiceType)
				{
					if (m_pSound->GetClipState(strStopVoiceKey) == PLAYING)
					{
						return;
					}
					else
					{
						m_ePlayingVoiceType = PVT_MED;
					}
				}
			}
		}
	}
	else if (m_ePlayingVoiceType == PVT_DIE)
	{
		return;
	}

	std::string strVoiceKey;
	CCommonSoundLoader* cLoader = CCommonSoundLoader::GetInst();
	unsigned int iTypeCnt = 1;
	
	strVoiceKey = strVoices[m_eVoiceType];
	iTypeCnt = cLoader->GetSoundRandomCnt(strVoiceKey);

	if (iTypeCnt > 1)
	{
		iTypeCnt = (rand() % iTypeCnt) + 1;
		strVoiceKey += std::to_string(iTypeCnt);
	}
	m_pSound->Play(strVoiceKey);

	m_iPlayingVoiceNumber = iTypeCnt;

	m_ePlayingVoiceType = m_eVoiceType;
}

void CHuman_Player::GetHit(int iDamage)
{
	m_playerHP -= iDamage;
}

void CHuman_Player::Render(float fTime)
{
	//이동된 MovePointer만큼에 머리의 위치를 뺀다
	Vector3 vPointerPos = m_pMovePointer->GetWorldPos() - m_pTransform->GetWorldPos();

	//Vector3 vPos;

	if (m_cInitLoopFinished > 0)
		--m_cInitLoopFinished;
}

CHuman_Player * CHuman_Player::Clone()
{
	return new CHuman_Player(*this);
}

//LoadData : Local Transform 설정을 모두 끝내고 실행하세요
bool CHuman_Player::LoadData(const TCHAR * dataPath)
{
	FILE *pFile = nullptr;

	m_vInitLocalPos = m_pTransform->GetLocalPos();

	std::wstring strPath = PUN::CPathManager::GetInst()->FindPath(DATA_PATH);
	strPath += dataPath;

	_wfopen_s(&pFile, strPath.c_str(), TEXT("rt"));

	if (!pFile)
		return false;

	PUN::CRenderer *pRenderer = m_pObject->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	if (!pRenderer)
		pRenderer = AddComponent<PUN::CRenderer>("Renderer");

	m_pAnimation = m_pObject->FindComponentFromType<PUN::CAnimation>(PUN::CT_ANIMATION);
	if (!m_pAnimation)
		m_pAnimation = AddComponent<PUN::CAnimation>("Animation");

	//m_pAnimation->SetAnimationComplex(true);

	m_pSound = m_pObject->FindComponentFromTag<PUN::CSoundSource>("Sound");
	if (!m_pSound)
		m_pSound = AddComponent<PUN::CSoundSource>("Sound");

	char strBuf[1024] = {};
	char strDataBuf[512] = {};
	char strDataBuf_1[512] = {};
	char strDataBuf_2[512] = {};
	char strDataBuf_3[512] = {};

	int iBufIdx = 0;

	std::vector<std::string> vecSoundKey;


	//소켓 offset 설정
	Vector3 vOffset;
	Vector3 vRot;

	while (fgets(strBuf, 1024, pFile))
	{
		int iSteps = 0;
		int iType = 0;
		iBufIdx = 0; //0번부터 쉼표 번호 이전까지
		//Parse by ','
		bool bCloseLoop = false;
		int iLast = (int)strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if (strBuf[iLast + 1] == 0)
			strBuf[iLast + 1] = ',';
		for (int i = 0; i < strnlen_s(strBuf, 1024); ++i)
		{

			if (strBuf[i] == ',')
			{
				memset(strDataBuf, 0, sizeof(char) * 512);
				int iDataSize = i - iBufIdx; // +1 하면 쉼표까지 붙어버림
				memcpy_s(strDataBuf, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);

				if (iSteps == 0)
				{
					//iStep 0 : 데이터의 종류를 나타내자
					if (!strcmp(strDataBuf, "int"))
					{
						iType = (int)PDT_INT;
					}

					else if (!strcmp(strDataBuf, "float"))
					{
						iType = (int)PDT_FLOAT;
					}

					else if (!strcmp(strDataBuf, "string"))
					{
						iType = (int)PDT_STRING;
					}

					else if (!strcmp(strDataBuf, "sound"))
					{
						iType = (int)PDT_SOUND;
					}

					else if (!strcmp(strDataBuf, "mesh"))
					{
						iType = (int)PDT_MESH;
					}

					else if (!strcmp(strDataBuf, "anim"))
					{
						iType = (int)PDT_ANIM;
					}

					else if (!strcmp(strDataBuf, "bone"))
					{
						iType = (int)PDT_BONE;
					}

					else if (!strcmp(strDataBuf, "socket"))
					{
						iType = (int)PDT_SOCKET;
					}
				}
				else
				{
					switch (iType)
					{
					case PDT_NONE:
						break;
					case PDT_INT:
					{
						//Todo : Int 타입 데이터들 변수를 설정
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{

						}
					}
					break;
					case PDT_FLOAT:
					{
						//Todo : float 타입 데이터들 변수를 설정
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							if (!strcmp(strDataBuf_1, "HideTime"))
							{
								m_fHideTime = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "DoorInteractDelay"))
							{
								m_fDoorEffectDelay = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkFSpeed"))
							{
								m_fFWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkBSpeed"))
							{
								m_fBackWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkLSpeed"))
							{
								m_fLWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkRSpeed"))
							{
								m_fRWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "SprintSpeed"))
							{
								m_fSprintSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchToStandSpeed"))
							{
								m_fCrouchToStandSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fStandToCrouchSpeed"))
							{
								m_fStandToCrouchSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchHeight"))
							{
								m_fCrouchHeight = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchForwardSpd"))
							{
								m_fCrouchForwardSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchBackSpd"))
							{
								m_fCrouchBackSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchLSpd"))
							{
								m_fCrouchLSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchRSpd"))
							{
								m_fCrouchRSpd = std::stof(strDataBuf);
							}
						}

					}
					break;
					case PDT_STRING:
					{
						//Todo : string(char*) 타입 데이터들 변수를 설정
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							if (!strcmp(strDataBuf_1, "DefaultAnimClip"))
							{
								if (m_pAnimation)
								{
									m_pAnimation->ChangeClip(strDataBuf);
									m_pAnimation->SetDefaultClip(strDataBuf);
								}
							}
						}
					}
					break;
					case PDT_SOUND:
					{
						//Todo : 사운드
						if (iSteps == 1)
						{
							std::string strData(strDataBuf);
							vecSoundKey.push_back(strDataBuf);
						}

					}
					break;
					case PDT_MESH:
					{
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							pRenderer->SetMesh(strDataBuf_1, widePath);
							//m_pTransform->SetWorldPivot(Vector3::Zero);
						}
					}
					break;
					case PDT_ANIM:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnimation->Load(widePath);
						}

					}
					break;
					case PDT_BONE:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnimation->LoadBone(widePath);
						}
					}
					break;
					case PDT_SOCKET:
					{
						if (iSteps == 1)
						{
							//소켓 뼈 이름
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							//소켓 이름
							memset(strDataBuf_3, 0, sizeof(char) * 512);
							if (!m_pAnimation->AddSocket(strDataBuf_1, strDataBuf))
							{
								m_pAnimation->SetSocketObject(strDataBuf_1, strDataBuf, nullptr);
							}

							memcpy_s(strDataBuf_3, sizeof(char) * 512, strDataBuf, sizeof(char) * 512);
						}
						else if (iSteps > 2)
						{
							switch (iSteps)
							{
							case 3:
								vOffset.x = std::stof(strDataBuf);
								break;
							case 4:
								vOffset.y = std::stof(strDataBuf);
								break;
							case 5:
								vOffset.z = std::stof(strDataBuf);
								break;
							case 6:
								vRot.x = std::stof(strDataBuf);
								break;
							case 7:
								vRot.y = std::stof(strDataBuf);
								break;
							case 8:
								vRot.z = std::stof(strDataBuf);
								{
									m_pAnimation->SetSocketOffset(strDataBuf_1, strDataBuf_3, vOffset);
									m_pAnimation->SetSocketRotation(strDataBuf_1, strDataBuf_3, vRot);

									//test
									//m_pAnimation->SetSocketObject(strDataBuf_1, strDataBuf_3, m_pHandSocketObj);
									bCloseLoop = true;
								}
								break;
							default:
								break;
							}

						}
					}
					break;
					default:
						break;
					}
				}

				++iSteps;
				iBufIdx = i + 1;
			}

			if (bCloseLoop)
				break;
		}
	}

	fclose(pFile);

	SAFE_RELEASE(pRenderer);

	m_pAnimation->SetSocketObject("Hero-Neck", "daegari", m_pHeadObj);
	m_pAnimation->SetSocketObject("Hero-R-Hand", "gun", m_pHandGun);
	m_pAnimation->SetSocketObject("Hero-R-Hand", "camhand", m_pCamModelObj);

	PUN::CGameObject *pRoot = PUN::CGameObject::CreateObject("PlayerRoot", m_pLayer, true);

	m_pGeometryBody = pRoot->AddComponent<PUN::CColliderOBB3D>("PlayerGeom");
	m_pGeometryBody->SetInfo(Vector3(0.f, -.25f, -1.f), Vector3::Axis, Vector3(1.25f, 2.55f, 1.25f));
	m_pGeometryBody->SetColliderID(UCI_PLAYER_HIT);
	m_pGeometryBody->SetCollisionCallback(PUN::CCT_ENTER, this, &CHuman_Player::HitBoxEnter);
	m_pGeometryBody->SetCollisionCallback(PUN::CCT_STAY, this, &CHuman_Player::Geometry_Push);
	m_pGeometryBody->SetCollisionCallback(PUN::CCT_LEAVE, this, &CHuman_Player::Geometry_Out);
	
	m_pAnimation->SetSocketObject("Hero-Spine2", "root", pRoot);

	m_pRootBonePos = pRoot->GetTransform();

	SAFE_RELEASE(pRoot);

	if (vecSoundKey.size() > 0)
		m_pSound->LoadSounds(vecSoundKey);

	SetAnimNotify();
	AfterLoad();

	return true;
}

void CHuman_Player::Move(PUN::AXIS axis, float fSpeed, float fTime)
{
	float fAddSpeed = fSpeed;

	if (m_isAccel)
	{
		float fAddSpeed = fSpeed;
		float fFirstSpeed = fAddSpeed;

		fAddSpeed = m_accelSpeed * fSpeed;
		
		GET_SINGLE(CRenderManager)->EnableFilter(CFT_MOTIONBLUR);
		GET_SINGLE(CRenderManager)->SetMotionBlur(1);

		m_fAccelTime += fTime;

		if (m_fAccelTime > m_accelDuration)
		{
			m_isAccel = false;
			fAddSpeed = fFirstSpeed;
			GET_SINGLE(CRenderManager)->DisableFilter(CFT_MOTIONBLUR);
			GET_SINGLE(CRenderManager)->SetMotionBlur(0);
		}

	}
	

	Vector3 vAxis = m_pTransform->GetWorldAxis(axis);
	Vector3	vPos = vAxis * (fAddSpeed * fTime);

	Move(vPos);
}

int CHuman_Player::Move(Vector3 vMove)
{
	if (vMove == Vector3::Zero)
		return 0;
	Vector3 vCurrPos = m_pTransform->GetWorldPos();
	Vector3 vRestorePos = vCurrPos;
	Vector3 vPos = vCurrPos + vMove;
	CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene, vPos);
	if (m_bNaviOn && pMesh)
	{
		float fVelocity = vMove.Length();
		if (fVelocity < 1.e-17f)
			return 1;
		bool	bMove = false;
		PUN::PNavigationCell pNavi = nullptr;
		//vCurrPos에서 가장 가까운 선 찾기
		//Dot = 사영 = Cos값
		float fMinDist = FLT_MAX;
		float fDot = 0.f;
		float fDist = FLT_MAX;
		float fBaseDist = 0.f;
		float fRadian = 0.f;
		Vector3 vGrid;
		Vector3 vRes = vMove;
		for (char i = 0; i < 4; ++i)
		{
			pNavi = pMesh->GetNavigationCell(vPos);
			if (i > 0)
				m_cMoveCheckFlag |= 2;
			if (pMesh->CheckCell(vPos))
			{
				if (vRes != Vector3::Zero)
				{
					float fResVel = vRes.Length();
					if (fResVel <= fVelocity)
					{
						bMove = true;
						m_pMovePointer->Move(vRes);
					}
					else
					{
						vRes.Normalize();
						vRes *= fVelocity;
						m_pMovePointer->Move(vRes);
					}
					if (i == 0)
						return 0;
					return 2;
				}
			}
			if (i == 3)
				break;
			vGrid = (vCurrPos - pNavi->vEdgeCenter[i]) * 2.f; //해당 선분의 중점이므로 이 거리에 2를 곱해야 함
			vGrid.y = pNavi->vEdgeCenter[i].y;
			fDot = pNavi->vEdge[i].Dot(vGrid);
			fBaseDist = vGrid.Length();
			float fCos = fDot / fBaseDist;
			fRadian = acosf(fCos);
			fDist = abs(sinf(fRadian) * fDot); //거리값
			Vector3 vEdge = pNavi->vEdge[i];
			vEdge.Normalize();
			Vector3 vAltRes = vEdge * fCos;
			Vector3 vAltResMinus = vAltRes * -1.f;
			float fTestSignAltRes[2] = { (vMove + vAltRes).Length(), (vMove + vAltResMinus).Length() };
			if (fTestSignAltRes[0] < fTestSignAltRes[1])
			{
				vRes = vAltResMinus;
				vPos = vCurrPos + vRes;
				pNavi = pMesh->GetNavigationCell(vPos);
				if (pMesh->CheckCell(vPos))
				{
					float fResVel = vRes.Length();
					if (fResVel <= fVelocity)
					{
						bMove = true;
						m_pMovePointer->Move(vRes);
					}
					else
					{
						vRes.Normalize();
						vRes *= fVelocity;
						m_pMovePointer->Move(vRes);
					}
					return 3;
				}
			}
			vPos = vCurrPos + vAltRes;
			vRes = vAltRes;
		}
		if (!bMove)
		{
			return 4;
		}
	}
	m_pMovePointer->Move(vMove);

	return 0;
}

void CHuman_Player::PlayerMove(const Vector3& vMove)
{
	m_pTransform->Move(vMove);
	
	m_pMovePointer->Move(vMove);
	PUN::CTransform *pHead = m_pHeadObj->GetTransform();
	pHead->Move(vMove);
	SAFE_RELEASE(pHead);

	if (m_pScene)
	{
		if ((m_iState & PSTATUS_CAMOUT) == 0)
		{
			PUN::CTransform *pMainCam = m_pScene->GetMainCameraTransform();
			
			Vector3 vCamPos = pMainCam->GetWorldPos();
			vCamPos += vMove;
			pMainCam->SetWorldPos(vCamPos);

			SAFE_RELEASE(pMainCam);
		}
	}
}

void CHuman_Player::PlayerRot(const Vector3& vRot)
{
	m_pTransform->Rotation(vRot);
	m_pMovePointer->Rotation(vRot);
	PUN::CTransform *pHead = m_pHeadObj->GetTransform();
	pHead->Rotation(vRot);
	SAFE_RELEASE(pHead);

	if (m_pScene)
	{
		if ((m_iState & PSTATUS_CAMOUT) == 0)
		{
			PUN::CTransform *pMainCam = m_pScene->GetMainCameraTransform();
			pMainCam->Rotation(vRot);

			SAFE_RELEASE(pMainCam);
		}
	}
}

void CHuman_Player::SetAnimNotify()
{
	//PUN::PANIMATIONCLIP pClip = nullptr;
	//
	//pClip = m_pAnimation->GetAnimClip("player_run_forward");
	//m_arrAnimCallbacks[0].fAnimationProgress = 0.384615384f;
	//m_arrAnimCallbacks[0].func = std::bind(&CHuman_Player::FootStepRun, this, std::placeholders::_1);
	//pClip->vecCallback.push_back(&(m_arrAnimCallbacks[1]));
	//
	//m_arrAnimCallbacks[1].fAnimationProgress = 0.846153846f;
	//m_arrAnimCallbacks[1].func = std::bind(&CHuman_Player::FootStepRun, this, std::placeholders::_1);
	//pClip->vecCallback.push_back(&(m_arrAnimCallbacks[2]));
}

const Vector3 & CHuman_Player::GetMoveDirection() const
{
	return m_vMoveDirection;
}


void CHuman_Player::SetFootStepEnvironment(FOOTSTEP_ENVIRONMENT eEnv)
{
	m_eFootStep = eEnv;
}

FOOTSTEP_ENVIRONMENT CHuman_Player::GetFootStepEnvironment() const
{
	return m_eFootStep;
}

void CHuman_Player::AfterLoad()
{
	m_pAnimation->SetDefaultClip("player_stand_idle");
	m_pAnimation->SetClipOption("player_camcorder_raise", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_camcorder_lower", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_stand_to_crouch", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_to_stand", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_camcorder_raise", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_camcorder_lower", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_look_correct_left", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_stand_look_correct_left", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_look_correct_right", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_stand_look_correct_right", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_locker_exit", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_locker_hide", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_jump_over_from_walk", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_crouch_object_pickup_h45v35", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_object_pickup_h40v70", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_hit_foward", PUN::AO_ONCE_RETURN);
	m_pAnimation->SetClipOption("player_stand_death", PUN::AO_ONCE_LAST);

	m_pAnimation->SetClipUseBoneTransform("player_crouch_forward", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_crouch_backward", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_crouch_strafe_left", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_crouch_strafe_right", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_locker_hide", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_locker_exit", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_jump_over_from_walk", "Hero-Pelvis");

	//std::vector<int> vecIgnoreBone;
	PUN::PPARTANIM partAnim = new PUN::PARTANIM;
	partAnim->vecPartIdx = m_vecIgnoreRightArmKey;
	partAnim->bActivated = false;
	partAnim->iRootParentIndex = 6;
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_stand_idle"));
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_camcorder_raise"));
	partAnim->mapPartClips.find("player_camcorder_raise")->second->eOption = PUN::AO_ONCE_RETURN;
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_camcorder_idle"));
	//partAnim->pDefaultClip = partAnim->mapPartClips.find("player_camcorder_lower")->second;
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_camcorder_lower"));
	partAnim->mapPartClips.find("player_camcorder_lower")->second->eOption = PUN::AO_ONCE_RETURN;
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_crouch_camcorder_idle"));
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_crouch_camcorder_raise"));
	partAnim->LoadPartAnimFromExistingClip(m_pAnimation->FindClip("player_crouch_camcorder_lower"));
	std::wstring wPath = L"shooter_takegun.anm";
	partAnim->LoadPartClipFromFile("gun_take", wPath);
	partAnim->mapPartClips.find("gun_take")->second->eOption = PUN::AO_ONCE_RETURN;
	wPath = TEXT("shooter_idle.anm");
	partAnim->LoadPartClipFromFile("gun_idle", wPath);
	m_pAnimation->AddPartialClip(partAnim);

	m_pPartCamAnim = partAnim;
	m_pPartCamAnim->pDefaultClip = m_pPartCamAnim->mapPartClips.find("player_camcorder_idle")->second;
	//m_pAnimation->SetIgnoreVector(m_vecIgnoreRightArmKey);
}


float CHuman_Player::GetWorlPosY() const {
	return m_pTransform->GetWorldPos().y;
};
Vector3 CHuman_Player::GetWorldPos() const {
	return m_pTransform->GetWorldPos();
};

void CHuman_Player::SetInInteractiveSceneChange(bool bIn)
{
	if (bIn)
		m_iState |= PSTATUS_CHANGESCENE;
	else
		m_iState ^= PSTATUS_CHANGESCENE;
}
bool CHuman_Player::IsPlayerOnSceneChange()
{
	return m_iState & PSTATUS_CHANGESCENE;
}

void CHuman_Player::InputMove(float fTime, bool& bBlend)
{

	PUN::CInput *_Input = PUN::CInput::GetInst();

	if (_Input->KeyPress("Shift"))
	{

		m_iState |= PSTATUS_SPRINT;
	}
	else if (_Input->KeyRelease("Shift"))
	{
		m_iState &= ~PSTATUS_SPRINT;
	}

	if (_Input->KeyPress("Ctrl"))
	{
		m_iState |= PSTATUS_CROUCHING;
		m_fTimerBuf_State = 0.f;

		std::string strSitDown = "JacketSitDown";

		int iRand = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strSitDown);

		iRand = rand() % iRand + 1;

		strSitDown += std::to_string(iRand);

		m_pSound->Play(std::string(strSitDown), false);


		if (m_pScene)
		{
			PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
			if ((m_iState & PSTATUS_CAMOUT) == 0)
			{
				Vector3 vCamRot = pCamTrans->GetWorldRot();
				if (vCamRot.x > 25.f)
				{
					vCamRot.x = 25.f;
				}
				
				pCamTrans->SetWorldRot(vCamRot);
			}

			SAFE_RELEASE(pCamTrans);
		}
	}
	else if (_Input->KeyRelease("Ctrl"))
	{
		m_iState &= ~PSTATUS_CROUCHED;
		m_iState |= PSTATUS_CROUCHING;
		//m_pAnimation->ChangeClip("player_crouch_to_stand");

		std::string strStandUp = "JacketStandUp";

		int iRand = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strStandUp);

		iRand = rand() % iRand + 1;

		strStandUp += std::to_string(iRand);

		m_pSound->Play(std::string(strStandUp), false);
	}

	if (_Input->KeyPush("Ctrl"))
	{
		m_iState |= PSTATUS_CROUCHED;
		m_iState &= ~PSTATUS_SPRINT;
		OnCrouching(fTime);
	}

	if (_Input->KeyPush("W"))
	{

		m_iState &= ~PSTATUS_STOPMOVE;
		m_iDirFlag |= PMT_FORWARD;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_forward");

			Move(PUN::AXIS_Z, m_fCrouchForwardSpd, fTime);
		}
		else
		{
			if (m_iState & PSTATUS_SPRINT)
			{

				m_pAnimation->ChangeClip("player_run_forward");
				OnSprint(fTime);
				Move(PUN::AXIS_Z, m_fSprintSpeed, fTime);
			}
			else
			{
				m_pAnimation->ChangeClip("player_walk_forward");

				Move(PUN::AXIS_Z, m_fFWalkSpeed, fTime);
			}
		}

	}
	else if (_Input->KeyPush("S"))
	{
		m_iState ^= PSTATUS_SPRINT;
		m_iState &= ~PSTATUS_STOPMOVE;
		m_iDirFlag |= PMT_BACKWARD;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_backward");
			Move(PUN::AXIS_Z, -m_fCrouchBackSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_backward");
			Move(PUN::AXIS_Z, -m_fBackWalkSpeed, fTime);
		}

	}

	if (_Input->KeyPush("D"))
	{
		m_iDirFlag |= PMT_RIGHT;

		m_iState &= ~PSTATUS_STOPMOVE;

		if (m_iDirFlag & (PMT_FORWARD | PMT_BACKWARD))
		{
			bBlend = true;
		}

		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_strafe_right");
			Move(PUN::AXIS_X, m_fCrouchRSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_strafe_right");
			Move(PUN::AXIS_X, m_fRWalkSpeed, fTime);
		}

	}
	else if (_Input->KeyPush("A"))
	{
		m_iDirFlag |= PMT_LEFT;

		if (m_iDirFlag & (PMT_FORWARD | PMT_BACKWARD))
		{
			bBlend = true;
		}

		m_iState &= ~PSTATUS_STOPMOVE;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_strafe_left");
			Move(PUN::AXIS_X, -m_fCrouchLSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_strafe_left");
			Move(PUN::AXIS_X, -m_fLWalkSpeed, fTime);
		}
	}

	/*
	if (_Input->KeyPress("Space"))
	{
		//m_iState |= PSTATUS_JUMPED;


		if (m_iState & PSTATUS_STOPMOVE)
		{
			//m_pAnimation->ChangeClip("player_jump_from_idle");
		}
		else if (m_iState & PSTATUS_SPRINT)
		{
			//m_pAnimation->ChangeClip("player_jump_over_from_run");
		}
		else
		{
			//m_pAnimation->ChangeClip("player_jump_over_from_walk");
		}
	}
	*/
	
	if (m_iState & PSTATUS_STOPMOVE)
	{
		m_vMoveDirection = Vector3::Zero;
	}
	else
	{
		if (m_iDirFlag & PMT_FORWARD)
		{
			if (m_iState & PSTATUS_SPRINT)
			{
				m_vMoveDirection.z = 2.f;
				m_eVoiceType = PVT_RUN;
			}

			else
			{
				m_vMoveDirection.z = 1.f;

			}
		}
		else
		{
			if (m_iState & PSTATUS_SPRINT)
			{
				m_iState ^= PSTATUS_SPRINT;
				FootStepWalkNormal(fTime);
			}

		}

		if (m_iDirFlag & PMT_BACKWARD)
		{
			if (m_iDirFlag & PMT_FORWARD)
				m_vMoveDirection.z = 0.f;
			else
				m_vMoveDirection.z = 1.f;

		}

		if (m_iDirFlag & PMT_RIGHT)
		{
			m_vMoveDirection.x = 1.f;
		}

		if (m_iDirFlag & PMT_LEFT)
		{
			if (m_iDirFlag & PMT_RIGHT)
				m_vMoveDirection.x = 0.f;
			else
				m_vMoveDirection.x = -1.f;
		}

		if (m_iState & PSTATUS_CROUCHED)
		{
			m_vMoveDirection * 0.5f;
			m_eVoiceType = PVT_CRAWL;
			OnCrouchWalk(fTime);
		}
		else
		{
			if ((m_iState & PSTATUS_SPRINT) == 0)
			{
				OnWalk(fTime);
				FootStepWalkNormal(fTime);
			}
		}
	}

	int iStateFlagDiff = m_iPrevState ^ m_iState;

	if (iStateFlagDiff & PSTATUS_CROUCHED)
	{
		if (m_iState & PSTATUS_CROUCHING)
		{
			if (m_iState & PSTATUS_CROUCHED) //지금이 숙이는거
			{
				bBlend = true;
				m_pAnimation->SetDefaultClip("player_crouch_idle");
				m_pAnimation->ChangeClip("player_stand_to_crouch");
			}
			else
			{
				bBlend = true;
				m_pAnimation->SetDefaultClip("player_stand_idle");
				m_pAnimation->ChangeClip("player_crouch_to_stand");
			}

		}

	}

	if (m_iState & PSTATUS_STOPMOVE)
	{
		if (m_iState & PSTATUS_ITEM)
		{

		}
		else
		{
			if (m_iState & PSTATUS_CROUCHED)
			{
				if ((m_iState & PSTATUS_TURNING) == 0)
					m_pAnimation->ChangeClip("player_crouch_idle");
			}
			else
			{
				if ((m_iState & PSTATUS_TURNING) == 0)
					m_pAnimation->ChangeClip("player_stand_idle");
			}
		}
		
	}
}

void CHuman_Player::InputRot(float fTime, PUN::CTransform *pHeadTrans, Vector3& vTotMoveDiff)
{
	
	Vector3 vCamPos;
	vCamPos = pHeadTrans->GetWorldPos() + m_vCamWorldOffset;
	
	if (m_iState & PSTATUS_CROUCHED)
	{
		//m_fTimerBuf_State
		if (m_iState & PSTATUS_CROUCHING)
		{
			float m_fCrouchRate = m_fTimerBuf_State / m_fStandToCrouchSpeed;
			Vector3 vecLoc = (m_vCamLocalOffset * (1.f - m_fCrouchRate)) +
				(m_vCamCrouchLocalOffset * m_fCrouchRate);

			//m_vCamCrouchLocalOffset
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * vecLoc.x;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * vecLoc.y;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * vecLoc.z;
		}
		else
		{
			//m_vCamCrouchLocalOffset
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamCrouchLocalOffset.x;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamCrouchLocalOffset.y;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamCrouchLocalOffset.z;
		}
	}
	else
	{
		if (m_iState & PSTATUS_CROUCHING)
		{
			float m_fCrouchRate = m_fTimerBuf_State / m_fCrouchToStandSpeed;
			Vector3 vecLoc = (m_vCamCrouchLocalOffset * (1.f - m_fCrouchRate)) +
				(m_vCamLocalOffset * m_fCrouchRate);

			//m_vCamCrouchLocalOffset
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * vecLoc.x;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * vecLoc.y;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * vecLoc.z;
		}
		else
		{
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamLocalOffset.x;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamLocalOffset.y;
			vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamLocalOffset.z;
		}

	}


	vCamPos += vTotMoveDiff;

	//pCamTrans->SetWorldPos(vCamPos);
	

	if ((m_iState & PSTATUS_LOCKER) && ((m_iState & PSTATUS_HIDEINTERACT) ^ (m_iState & PSTATUS_HIDEINTERACT_OUT)))
	{
		if (m_iState & PSTATUS_CAMOUT)
		{

		}
		else if (m_pScene)
		{
			PUN::CTransform *pCamTr = m_pScene->GetMainCameraTransform();
			PUN::CTransform *pNeckTr = m_pHeadObj->GetTransform();
			Vector3 vNecRot = pNeckTr->GetWorldRot();
			Vector3 vCamRot = pCamTr->GetWorldRot();

			//김벌락 보정... 왜 108도냐고? 나도 모름..
			if (m_pAnimation->GetCurrentClipTime() + 0.25f < m_pAnimation->GetCurrentClip()->fEndTime)
			{
				if (fabsf(vNecRot.x) > 108.f)
				{
					vCamRot.y = -vNecRot.y;
				}
				else
				{
					vCamRot.y = 180 + vNecRot.y;
				}

				pCamTr->SetWorldPos(vCamPos);
			}
			else
			{
				float fSpeed = 32.f * fTime;
				if (vCamRot.x <0.f)
				{
					if (vCamRot.x <= -fSpeed)
						vCamRot.x += fSpeed;
					else
						vCamRot.x = 0.f;

				}
				else if (vCamRot.x > 0.f)
				{
					if (vCamRot.x >= fSpeed)
						vCamRot.x -= fSpeed;
					else
						vCamRot.x = 0.f;
				}
			}
			
			pCamTr->SetWorldRot(vCamRot);
			SAFE_RELEASE(pNeckTr);

			

			SAFE_RELEASE(pCamTr);
		}
	}
	else if ((m_iState & PSTATUS_CAMOUT) == 0)
	{
		float fMinY = 0.f;
		float fMaxY = 0.f;

		if (m_iState & PSTATUS_GUN)
		{
			fMinY = m_fGunMinAngleY;
			fMaxY = m_fGunMaxAngleY;
		}
		else if (m_iState & PSTATUS_BED)
		{
			fMinY = -m_fMaxHideBedAngleY;
			fMaxY = m_fMaxHideBedAngleY;
		}
		else if (m_iState & PSTATUS_LOCKER)
		{
			fMinY = -m_fMaxHideLockerAngleX;
			fMaxY = m_fMaxHideLockerAngleX;
			
		}
		else
		{
			fMinY = m_fViewMinAngleY;
			fMaxY = m_fViewMaxAngleY;
		}

		float fYdir = pCamEffManager->FirstPersonView(fMaxY, fMinY, m_pTransform, vCamPos);
		if (fYdir != 0.f)
		{
			float fTurnMotion = 120.f * fTime;
			m_iState |= PSTATUS_TURNING;


			if (m_iState & PSTATUS_ITEM)
			{
				if ((m_iPrevState & PSTATUS_ITEM) == 0)
				{
					if (m_iState & PSTATUS_CROUCHED)
					{
						m_pAnimation->ChangeClip("player_crouch_object_pickup_h45v35");
					}
					else
					{
						m_pAnimation->ChangeClip("player_object_pickup_h40v70");
					}
				}
			}
			else if (m_iState & PSTATUS_STOPMOVE)
			{
				if (m_iState & PSTATUS_BED)
				{
				}
				else if (m_iState & PSTATUS_LOCKER)
				{
				}
				else
				{
					if (m_iState & PSTATUS_CROUCHED)
					{
						if (fYdir > fTurnMotion)
						{
							m_pAnimation->ChangeClip("player_crouch_look_correct_left");
						}
						else if (fYdir < -fTurnMotion)
						{
							m_pAnimation->ChangeClip("player_crouch_look_correct_right");
						}
					}

					else
					{
						if (fYdir > fTurnMotion)
						{
							m_pAnimation->ChangeClip("player_stand_look_correct_left");
						}
						else if (fYdir < -fTurnMotion)
						{
							m_pAnimation->ChangeClip("player_stand_look_correct_right");
						}

					}
				}
				
			}

		}
		else {
			m_iState ^= PSTATUS_TURNING;
		}

		InputRot_Interact(fTime);
	}


	
	//SAFE_RELEASE(pCamTrans);
	
}

bool CHuman_Player::IsHidingInBed()	
{
	if (m_iState & PSTATUS_HIDEINTERACT)
	{
		if (m_iState & PSTATUS_BED)
			return true;
	}
	return false;
}

bool CHuman_Player::IsHiddenInBed()	
{
	if (m_iState & PSTATUS_BED)
	{
		if (m_iState & PSTATUS_HIDEINTERACT)
			return false;
		else if (m_iState & PSTATUS_HIDEINTERACT_OUT)
			return false;
		else
			return true;
	}
	return false;
}

CInventory *CHuman_Player::GetInv()
{
	return m_pInven;
}

CHuman_Player* CHuman_Player::GetPlayer()
{
	return m_pInst;
}

bool CHuman_Player::IsHidingInLocker()
{
	if (m_iState & PSTATUS_HIDEINTERACT)
	{
		if (m_iState & PSTATUS_LOCKER)
			return true;
	}
	return false;
}
bool CHuman_Player::IsHiddenInLocker()
{
	if (m_iState & PSTATUS_LOCKER)
	{
		if (m_iState & PSTATUS_HIDEINTERACT)
			return false;
		else if (m_iState & PSTATUS_HIDEINTERACT_OUT)
			return false;
		else
			return true;
	}
	return false;
}


float CHuman_Player::GetBreathIntensity() const
{
	return m_fBreathIntensity;
}

void CHuman_Player::SetBreathIntensity(float fBreath)
{
	m_fBreathIntensity = fBreath;
}

bool CHuman_Player::IsCrouching() const 
{
	//if((m_iState & PSTATUS_CROUCHING) && (m_iState & PSTATUS_CROUCHED))
	//	std::cout << "player is crouching" << std::endl;
	return !((m_iState ^ (PSTATUS_CROUCHING | PSTATUS_CROUCHED) & (PSTATUS_CROUCHING | PSTATUS_CROUCHED)));
}
bool CHuman_Player::OnCrouchMoving() const 
{
	//if((m_iState & PSTATUS_CROUCHED) && !(m_iState & PSTATUS_STOPMOVE))
	//	std::cout << "player is crouched and moving" << std::endl;
	return (m_iState & PSTATUS_CROUCHED) && !(m_iState & PSTATUS_STOPMOVE);
}
bool CHuman_Player::IsIdle() const 
{
	//if(m_iState & PSTATUS_STOPMOVE)
	//	std::cout << "player is idle" << std::endl;
	return m_iState & PSTATUS_STOPMOVE;
}
bool CHuman_Player::IsCrouched() const 
{
	//if((m_iState & PSTATUS_CROUCHED) && !(m_iState & PSTATUS_CROUCHING))
	//	std::cout << "player is crouched" << std::endl;
	return (m_iState & PSTATUS_CROUCHED) && !(m_iState & PSTATUS_CROUCHING);
}


void CHuman_Player::Hit_By_Enemy(PUN::CCollider *pEnemCol, float fTime)
{
	std::string strVoiceKey;
	m_fBreathIntensity += fTime * 1.75f;
	CCommonSoundLoader* cLoader = CCommonSoundLoader::GetInst();
	unsigned int iTypeCnt = 1;
	if (m_playerHP < 1)
	{
		strVoiceKey = strVoices[PVT_DIE];
		iTypeCnt = cLoader->GetSoundRandomCnt(strVoiceKey);

		if (iTypeCnt > 1)
		{
			iTypeCnt = (rand() % iTypeCnt) + 1;
			strVoiceKey += std::to_string(iTypeCnt);
		}
		m_pSound->Play(strVoiceKey);

		m_iState |= PSTATUS_DIE;
		m_pAnimation->ChangeClip("player_stand_death");

		return;
	}

	strVoiceKey = strVoices[PVT_HURT];
	iTypeCnt = cLoader->GetSoundRandomCnt(strVoiceKey);

	if (iTypeCnt > 1)
	{
		iTypeCnt = (rand() % iTypeCnt) + 1;
		strVoiceKey += std::to_string(iTypeCnt);
	}
	m_pSound->Play(strVoiceKey);

	m_iPlayingVoiceNumber = iTypeCnt;

	m_iState |= PSTATUS_HIT;
	m_pAnimation->ChangeClip("player_hit_forward");

	//push away from enemy

	PUN::CTransform *pEnemTr = pEnemCol->GetTransform();
	Vector3 vEnemPos = pEnemTr->GetWorldPos();

	SAFE_RELEASE(pEnemTr);

	Vector3 vCurrPos = m_pTransform->GetWorldPos();

	Vector3 vXAxis = m_pTransform->GetWorldAxis(PUN::AXIS_X);

	Vector3 vP2E = vEnemPos - vCurrPos;
	
	float fDistM = (vP2E - vXAxis).Length();
	float fDistP = (vP2E + vXAxis).Length();

	if (fDistM == fDistP)
	{
		if(rand()%2 > 0)
			CCameraEff::GetInst()->AddUpdateEffect(tHitEffL);
		else
			CCameraEff::GetInst()->AddUpdateEffect(tHitEffR);
	}

	else if(fDistM > fDistP)
		CCameraEff::GetInst()->AddUpdateEffect(tHitEffR);
	else
		CCameraEff::GetInst()->AddUpdateEffect(tHitEffL);


	m_playerHP -= 1;


}


void CHuman_Player::HitBoxEnter(PUN::CCollider* pSrc, PUN::CCollider* pDest, float fTime)
{
	std::string strDestTag = pDest->GetTag();
	if (strDestTag == "JapBody" || strDestTag == "HookBody" || strDestTag == "HeadBody")
	{
		Hit_By_Enemy(pDest, fTime);
	}
	else
	{
		strDestTag = pSrc->GetTag();
		if (strDestTag == "JapBody" || strDestTag == "HookBody" || strDestTag == "HeadBody")
		{
			Hit_By_Enemy(pSrc, fTime);
		}
	}
}

void CHuman_Player::TakeItem()
{
	if (m_iState & PSTATUS_CROUCHED)
	{
		m_pAnimation->ChangeClip("player_crouch_doc_pickup_h30v-10");
	}
	else
	{
		m_pAnimation->ChangeClip("player_crouch_object_pickup_h60v60");
	}

	m_iState |= PSTATUS_PICKUP;
}

void CHuman_Player::GunCheat()
{
	m_bHasGun = true;
}
