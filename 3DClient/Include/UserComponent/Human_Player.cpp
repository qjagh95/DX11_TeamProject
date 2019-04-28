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

using namespace PUN;



CHuman_Player::CHuman_Player():
	m_pAnimation(nullptr),
	m_pSound(nullptr),
	m_fTimerBuf(0.f),
	m_iState(1),
	m_fTimerBuf_State(0.f),
	m_pHandSocketObj(nullptr),
	m_pHeadObj(nullptr),
	m_fViewMaxAngleY(90.f),
	m_fViewMinAngleY(-90.f),
	m_fGunMaxAngleY(80.f),
	m_fGunMinAngleY(-80.f),
	pCamEffManager(nullptr),
	m_vCamWorldOffset(0.f, -7.5f, -5.f),
	m_vCamLocalOffset(0.f, 2.65f, -1.1f),
	m_vCamCrouchLocalOffset(0.5f, 1.2f,-0.f),
	m_fItemTakeTimerBuf(0.f),
	m_fCamTakeTime(0.8f),
	m_fGunTakeTime(1.2f),
	m_eFootStep(FTSE_DEFAULT),
	m_pPartCamAnim(nullptr),
	m_iPrevDirFlag(0),
	m_iPrevState(0),
	m_bNaviOn(true)
{
#include "Player_Item_Values.txt"
#include "Player_Interact_Values.txt"
#include "Player_Cam_Values.txt"
#include "Player_Move_Values.txt"
#include "Player_Weapon_Values.txt"
	m_vecIgnoreRightArmKey.reserve(24);
	//m_vecIgnoreRightArmKey.push_back(33);
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


}

CHuman_Player::CHuman_Player(const CHuman_Player & player):
	PUN::CUserComponent(player)
{

	m_bLoadedOnce		   = player.m_bLoadedOnce;
	m_iState = 1;
	
	m_fHideTime = player.m_fHideTime;
	m_fDoorEffectDelay = player.m_fDoorEffectDelay;
	m_fFWalkSpeed = player.m_fFWalkSpeed;
	m_fRWalkSpeed = player.m_fRWalkSpeed;
	m_fLWalkSpeed = player.m_fLWalkSpeed;
	m_fBackWalkSpeed = player.m_fBackWalkSpeed;
	m_fSprintSpeed = player.m_fSprintSpeed;
	m_fCrouchToStandSpeed	  = player.m_fCrouchToStandSpeed;
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
	m_pHeadObj = nullptr; m_pAnimation = nullptr; m_pHandSocketObj = nullptr; m_pSound = nullptr;
	m_fTimerBuf = 0.f;
	m_fTimerBuf_State = 0.f;
}

CHuman_Player::~CHuman_Player()
{
	if(pCamEffManager->IsFirstPersonEnabled())
		pCamEffManager->SetFirstPersonViewEnable();
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pSound);
	SAFE_RELEASE(m_pHandSocketObj);
	SAFE_RELEASE(m_pHeadObj);
	SAFE_RELEASE(m_pInven);
	SAFE_RELEASE(m_pHandycam);
	SAFE_RELEASE(m_pCamModelObj);
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
	
	PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();
	_SMgr->SetAudioCoordSize(8.f);

	PUN::CTransform *pCamera = m_pScene->GetMainCameraTransform();
	pCamEffManager->SetCamTransform(pCamera);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);

	
	
	//Hero - L - Hand, player_cam_hand, 10, 0, 0.0, 0, 0, 0
	//GET_SINGLE(PUN::CInput)->BindAxis("MoveH", this, &CHuman_Player::Forward);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_W, 1.f);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_S, 2.f);
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
	m_pHeadObj = PUN::CGameObject::CreateObject("Player_Daegari", m_pLayer);
	m_pHeadObj->SetFrustrumCullUse(false);
	m_pObject->SetFrustrumCullUse(false);

	PUN::CTransform*	pTr = m_pHeadObj->GetTransform();
	pTr->SetWorldScale(0.05f, 0.05f, 0.05f);

	PUN::CRenderer*	pHeadRenderer = m_pHeadObj->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pHeadRenderer->SetMesh("head", TEXT("Head.FBX"), MESH_PATH);
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
	//pHandycamTr->SetWorldPos(0.f, 0.f, 0.f);
	//pHandycamTr->SetWorldPos(0.1f, 0.1f, 0.1f);
	//PUN::CRenderer *pCamRenderer = pHandycamObj->AddComponent<PUN::CRenderer>("renderer");
	//pCamRenderer->SetMesh("cam",TEXT("Handycam.FBX"), MESH_PATH);
	//pHandycamObj->SetEnable(false);

	//SAFE_RELEASE(pCamRenderer);
	SAFE_RELEASE(pHandycamTr);
	SAFE_RELEASE(pHandycamObj);

	//Handycam fake model
	m_pCamModelObj = PUN::CGameObject::CreateObject("Handycam_model", m_pLayer, true);
	pHandycamTr = m_pCamModelObj->GetTransform();
	//pHandycamTr->SetWorldPos(0.f, 0.f, 0.f);
	pHandycamTr->SetWorldScale(m_pTransform->GetWorldScale());
	pHandycamTr->SetLocalRot(-90.f, -60.f, -130.f);
	PUN::CRenderer *pCamRenderer = m_pCamModelObj->AddComponent<PUN::CRenderer>("renderer");
	pCamRenderer->SetMesh("cam", TEXT("Handycam.FBX"), MESH_PATH);
	m_pCamModelObj->SetEnable(false);

	SAFE_RELEASE(pCamRenderer);
	SAFE_RELEASE(pHandycamTr);

	m_pHandGun = PUN::CGameObject::CreateObject("Player_Gun", m_pLayer, true);
	pTr = m_pHandGun->GetTransform();
	pTr->SetWorldScale(m_pTransform->GetWorldScale() * 1.5f);
	pTr->SetLocalRot(-10.f, 145.f, 45.f);
	//pTr->SetWorldPos(0.f, 0.f, 15.f);
	pCamRenderer = m_pHandGun->AddComponent<PUN::CRenderer>("renderer");
	pCamRenderer->SetMesh("gun", TEXT("glock_OnlyFire.FBX"), MESH_PATH);
	//pCamRenderer->SetMesh("gun", TEXT("Enem_Cannibal.FBX"), MESH_PATH);
	//pCamRenderer->SetMesh("gun", TEXT("Waylon2.msh"));
	//pCamRenderer->SetMesh("gun", TEXT("shooter_anim.FBX"), MESH_PATH);
	m_pHandGun->SetEnable(false);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pCamRenderer);

	LoadData(TEXT("PlayerData.csv"));
}

int CHuman_Player::Input(float fTime)
{
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
	
	if (_Input->KeyPress("I"))
	{
		if (m_iState & PSTATUS_ITEM)
		{
			m_iState ^= (PSTATUS_ITEM | PSTATUS_INACTIVE);
			_Input->ShowMouse(false);
			Close_Item(fTime);
		}
		else
		{
			m_iState |= (PSTATUS_ITEM | PSTATUS_INACTIVE);
			_Input->ShowMouse(true);
			Open_Item(fTime);
		}
	}

	if (m_iState & PSTATUS_INACTIVE)
	{
		PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

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
	if (_Input->ShowMouse())
		_Input->ShowMouse(false);

	if (_Input->KeyPress("G"))
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
				//m_pPartCamAnim->pNextClip = m_pPartCamAnim->mapPartClips.find("player_stand_idle")->second;
				//player_stand_idle
				m_fItemTakeTimerBuf = 0.f;

				HandyCam_End();
				m_iState |= PSTATUS_CAM_TAKING_OFF;
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
	

	//Movement
	
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
		
		//m_pAnimation->ChangeClip("player_stand_to_crouch");
		m_fTimerBuf_State = 0.f;

		std::string strSitDown = "JacketSitDown";

		int iRand = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strSitDown);

		iRand = rand() % iRand + 1;

		strSitDown += std::to_string(iRand);

		m_pSound->Play(std::string(strSitDown), false);
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
			//m_pAnimation->ChangeClip("player_crouch_backward");
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


	//회전
	if ((m_iState & PSTATUS_INACTIVE) == 0)
	{
		//PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

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

		//pCamTrans->SetWorldPos(vCamPos);

		if (pCamEffManager->FirstPersonView(m_fViewMaxAngleY, m_fViewMinAngleY, m_pTransform, vCamPos))
		{
			if (m_iState & PSTATUS_STOPMOVE)
			{
				if (m_iState & PSTATUS_CROUCHED)
				{
				}
				else
				{
					//m_pAnimation->ChangeClip("player_walk_forward");
				}
			}
			
		}
		
		//SAFE_RELEASE(pCamTrans);
		SAFE_RELEASE(pHeadTrans);
				
	}
	
	


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
	int iDirDiff = m_iPrevDirFlag ^ m_iDirFlag;
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

	else if (iStateFlagDiff & PSTATUS_STOPMOVE)
	{
		if (m_iState & PSTATUS_STOPMOVE)
		{
			if (m_iState & PSTATUS_CROUCHED)
			{
				m_pAnimation->ChangeClip("player_crouch_idle");
			}
			else
			{
				m_pAnimation->ChangeClip("player_stand_idle");
			}
		}
	}

	Input_Items(fTime);
	Input_Interact(fTime);
	Input_Cam(fTime);
	Input_Move(fTime);
	WeaponInput(fTime);
	m_pAnimation->KeepBlendSet(bBlend);

	return 0;
}

int CHuman_Player::Update(float fTime)
{
	CNavigationMesh*	pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene,
		m_pTransform->GetWorldPos());

	if (pMesh)
	{
		if (m_bNaviOn)
		{
			float	fY = pMesh->GetY(m_pTransform->GetWorldPos());

			Vector3 vPos = m_pTransform->GetWorldPos();
			vPos.y = fY;
			cout << "y : " << fY << endl;
			m_pTransform->SetWorldPos(vPos);
		}
	}
	if (m_iState & PSTATUS_CROUCHING)
	{
		if (m_iState & PSTATUS_CROUCHED)
		{
			if (m_fTimerBuf_State < m_fStandToCrouchSpeed)
			{
				m_fTimerBuf_State += fTime;

				float fLerp = m_fTimerBuf_State / m_fStandToCrouchSpeed;

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

	pCamEffManager->Update(fTime);

	MoveUpdate(fTime);
	CamUpdate(fTime);
	InteractUpdate(fTime);
	ItemUpdate(fTime);
	
	WeaponUpdate(fTime);

	return 0;
}

int CHuman_Player::LateUpdate(float fTime)
{

	//Mend Pos
	if (m_iState & PSTATUS_UPDATEPOS)
	{
		m_vTracerBuf = m_vTracerBuf - m_pRootBonePos->GetWorldPos();

		m_pTransform->SetWorldPos(m_pTransform->GetWorldPos() + m_vTracerBuf);
		m_vTracerBuf = Vector3::Zero;
		m_iState ^= PSTATUS_UPDATEPOS;
	}

	if (m_vTracerBuf != Vector3::Zero)
	{
		m_iState |= PSTATUS_UPDATEPOS;
		
	}
	

	/*
	PUN::CTransform *pCam = m_pScene->GetMainCameraTransform();
	PUN::CTransform *pTr = m_pHandSocketObj->GetTransform();

	pCam->SetWorldPos(pTr->GetWorldPos());
	pCam->SetWorldRot(pTr->GetWorldRot());

	SAFE_RELEASE(pCam);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pTr);
	*/
	
	pCamEffManager->LateUpdate(fTime);
	m_iPrevDirFlag = m_iDirFlag;
	m_iPrevState = m_iState;

	MoveLateUpdate(fTime);
	CamLateUpdate(fTime);
	InteractLateUpdate(fTime);
	ItemLateUpdate(fTime);
	WeaponLateUpdate(fTime);

	return 0;
}

void CHuman_Player::Collision(float fTime)
{
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
	if(!m_pAnimation)
		m_pAnimation = AddComponent<PUN::CAnimation>("Animation");

	//m_pAnimation->SetAnimationComplex(true);

	m_pSound = m_pObject->FindComponentFromTag<PUN::CSoundSource>("Sound");
	if(!m_pSound)
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
		if(strBuf[iLast + 1] == 0)
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
	m_pAnimation->SetSocketObject("Hero-R-Hand", "gun",m_pHandGun);
	m_pAnimation->SetSocketObject("Hero-R-Hand", "camhand", m_pCamModelObj);

	PUN::CGameObject *pRoot = PUN::CGameObject::CreateObject("PlayerRoot", m_pLayer, true);
	m_pAnimation->SetSocketObject("Hero-Root", "root", pRoot);
	m_pRootBonePos = m_pHeadObj->GetTransform();

	SAFE_RELEASE(pRoot);

	if (vecSoundKey.size() > 0)
		m_pSound->LoadSounds(vecSoundKey);

	SetAnimNotify();
	//m_pAnimation->SetSocketOffset("Hero-L-Hand", "player_cam_hand", Vector3(5.0f, 10.0f, 0.0f));
	AfterLoad();

	return true;
}

void CHuman_Player::Move(PUN::AXIS axis, float fSpeed, float fTime)
{
	m_pTransform->Move(axis, fSpeed, fTime);
}

void CHuman_Player::SetAnimNotify()
{
	PUN::PANIMATIONCLIP pClip = m_pAnimation->GetAnimClip("player_crouch_forward");
	m_arrAnimCallbacks[0].fAnimationProgress = 0.95f;
	m_arrAnimCallbacks[0].func = std::bind(&CHuman_Player::CrouchForwardMendPos, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[0]));

	pClip = m_pAnimation->GetAnimClip("player_crouch_backward");
	m_arrAnimCallbacks[1].fAnimationProgress = 0.95f;
	m_arrAnimCallbacks[1].func = std::bind(&CHuman_Player::CrouchBackwardMendPos, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[1]));

	pClip = m_pAnimation->GetAnimClip("player_crouch_strafe_right");
	m_arrAnimCallbacks[2].fAnimationProgress = 0.95f;
	m_arrAnimCallbacks[2].func = std::bind(&CHuman_Player::CrouchSideMendPos, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[2]));

	pClip = m_pAnimation->GetAnimClip("player_crouch_strafe_left");
	m_arrAnimCallbacks[3].fAnimationProgress = 0.95f;
	m_arrAnimCallbacks[3].func = std::bind(&CHuman_Player::CrouchSideMendPos, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[3]));

	pClip = m_pAnimation->GetAnimClip("player_run_forward");
	m_arrAnimCallbacks[4].fAnimationProgress = 0.384615384f;
	m_arrAnimCallbacks[4].func = std::bind(&CHuman_Player::FootStepRun, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[4]));

	m_arrAnimCallbacks[5].fAnimationProgress = 0.846153846f;
	m_arrAnimCallbacks[5].func = std::bind(&CHuman_Player::FootStepRun, this, std::placeholders::_1);
	pClip->vecCallback.push_back(&(m_arrAnimCallbacks[5]));


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