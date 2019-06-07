#include "../ClientHeader.h"
#include "JBH_Stage3.h"

#include <Component/Camera.h>
#include <Component/FreeCamera.h>
#include <Component/LandScape.h>
#include <Component/SoundSource.h>

#include "../GameManager.h"

#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST3_Suprise.h"
#include "../UserComponent/ST3_Slient.h"
#include "../UserComponent/Door.h"
#include "../CameraEff.h"

bool JBH_Stage3::m_isCanDrop = false;
bool JBH_Stage3::m_SlientMode = false;
Vector3 JBH_Stage3::m_DoorPos[DN_MAX];

JBH_Stage3::JBH_Stage3()
{
	m_PlayerState = 0;
	m_Player = NULLPTR;
	m_PlayerObject = NULLPTR;
	m_ChaceObject = NULLPTR;
	m_ChaceNPC = NULLPTR;
	m_SlientMode = false;
	m_SupriseSound = false;
	m_isCanDrop = false;
	m_isChangeBGM = false;

	m_CanDelayTime = 4.0f;
	m_CanDelayTimeVar = 0.0f;
}

JBH_Stage3::~JBH_Stage3()
{
	SAFE_RELEASE(m_Player);
	SAFE_RELEASE(m_PlayerObject);

	SAFE_RELEASE(m_ChaceObject);
	SAFE_RELEASE(m_ChaceNPC);
}

bool JBH_Stage3::Init()
{
	BasicInit();
	//DoorInit();

	CSoundManager::GetInst()->CreateSoundEffect("CamSuprise", L"music/10-AI NPC CHASE INTRO 3.wav");
	CSoundManager::GetInst()->CreateSoundEffect("TraceBGM", L"music\\10-AI NPC CHASE LOOP 2 (Strings).wav");

	return true;
}

void JBH_Stage3::AfterInit()
{
	m_DoorMap = CGameManager::GetInst()->GetDoorMap(m_pScene);

	m_DoorPos[DN_START_DOOR] = Vector3(228.0f, 0.0f, 66.0f);
	m_DoorPos[DN_ROOM1_DOOR] = Vector3(170.0f, 0.0f, 33.0f);
	m_DoorPos[DN_ROOM2_DOOR] = Vector3(39.0f, 0.0f, 54.0f);
	m_DoorPos[DN_ROOM3_DOOR] = Vector3(39.0f, 0.0f, 96.0f);
	m_DoorPos[DN_OUT_STAGE1_DOOR] = Vector3(77.0f, 0.0f, 159.0f);
	m_DoorPos[DN_OUT_STAGE2_DOOR] = Vector3(157.0f, 0.0f, 159.0f);
	m_DoorPos[DN_END_DOOR] = Vector3(216.0f, 0.0f, 155.0f);
	m_DoorPos[DN_MID_DOOR] = Vector3(155.0f, 0.0f, 11.0f);

	// Àü¿ª±¤(Directional Light) ¼³Á¤
	Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	Vector4 vDarkColor = Vector4(0.005f, 0.005f, 0.005f, 1.0f);
	CLayer* pLayer = m_pScene->FindLayer("Light");

	list<CGameObject*>* pLightList = pLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pLightList->end();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CLight* pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
		pLight->SetLightColor(vDarkColor, vDarkColor, vDarkColor);
		SAFE_RELEASE(pLight);
	}
	SAFE_RELEASE(pLayer);
	NPCInit();

	auto StartIter = m_DoorMap->begin();
	auto EndIter = m_DoorMap->end();

	for (; StartIter != EndIter; StartIter++)
	{
		CGameObject* pObj = StartIter->second->GetGameObjectNonCount();
		string strTag = pObj->GetTag();
		char strName[256] = {};

		strcpy(strName, strTag.c_str());

		_strupr_s(strName);

		if (strstr(strName, "FAKE"))
			StartIter->second->Lock(true, "");
	}
}

int JBH_Stage3::Update(float DeltaTime)
{
	m_PlayerState = m_Player->GetState();
	m_PlayerSection = m_PlayerObject->GetStageSection();

	if (CGameManager::GetInst()->FindDoor(m_pScene, "MidDoor")->IsOpenFinished() == true)
		m_SlientMode = true;

	SupriseSound(DeltaTime);


	if (m_ChaceNPC->GetState() == STS_USER_TRACE
		|| m_ChaceNPC->GetState() == STS_JAP
		|| m_ChaceNPC->GetState() == STS_HOOK 
		|| m_ChaceNPC->GetState() == STS_HEAD_ATTACK
		|| m_ChaceNPC->GetState() == STS_DOOR_TRACE)
	{
		if (m_isChangeBGM == false)
		{
			CSoundManager::GetInst()->PlayBgm("TraceBGM");
			m_isChangeBGM = true;
		}
	}
	else
	{
		if (m_isChangeBGM = true)
		{
			CSoundManager::GetInst()->PlayBgm("ST3BGM");
			m_isChangeBGM = false;
		}
	}

	return 0;
}

int JBH_Stage3::LateUpdate(float DeltaTime)
{
	return 0;
}

void JBH_Stage3::Collision(float DeltaTime)
{
}

void JBH_Stage3::Render(float DeltaTime)
{
}

void JBH_Stage3::BasicInit()
{
	CCamera* pCamera = m_pScene->GetMainCameraNonCount();
	//pCamera->SetCameraType(CT_PERSPECTIVE);
	//pCamera->SetNear(0.03f);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "JBH_Stage3_Test.dat";
	m_pScene->Load(Path);

	CGameObject* NavLandObject = CGameObject::CreateObject("Land", pDefaultLayer);
	CLandScape* Land = NavLandObject->AddComponent< CLandScape>("Land");

	Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "Stage3Nav.nav";
	Land->LoadLandScape(Path);

	m_PlayerObject = CGameObject::CreateObject("Player", pDefaultLayer, true);
	m_Player = m_PlayerObject->AddComponent<CHuman_Player>("Player");
	m_Player->GetTransformNonCount()->SetWorldPos(Vector3(238.0f, 0.0f, 68.0f));
	m_Player->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, -90.0f, 0.0f));
	pCamera->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 270.0f, 0.0f));

	CSoundManager::GetInst()->CreateSoundEffect("ST3BGM", L"SurgeonAttack.wav");
	CSoundManager::GetInst()->CreateSoundEffect("GlassBracking", L"GlassBracking.wav");

	//CSoundManager::GetInst()->PlayBgm("ST3BGM");
	CSoundManager::GetInst()->SetBgmVolume(0.5f);
	CSoundManager::GetInst()->SetTransitionTime(2.0f);

	SAFE_RELEASE(Land);
	SAFE_RELEASE(NavLandObject);
	SAFE_RELEASE(pDefaultLayer);
}

void JBH_Stage3::DoorInit()
{
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* StartDoorObj = CGameObject::CreateObject("StartDoor", pDefaultLayer);
	CDoor* StartDoor = StartDoorObj->AddComponent<CDoor>("StartDoor");
	StartDoor->GetTransformNonCount()->SetWorldPos(Vector3(231.0f, 0.0f, 65.4f));
	StartDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	StartDoor->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	StartDoor->GetTransformNonCount()->SetLocalRotY(-90.0f);
	StartDoor->GetTransformNonCount()->SetWorldRotY(90.0f);
	StartDoor->SetLeftRight();

	CGameObject* FirstRoomDoorObj = CGameObject::CreateObject("FirstRoomDoor", pDefaultLayer);
	CDoor* FirstRoomDoor = FirstRoomDoorObj->AddComponent<CDoor>("FirstRoomDoor");
	FirstRoomDoorObj->GetTransformNonCount()->SetWorldPos(Vector3(174.4f, 0.0f, 38.0f));
	FirstRoomDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	FirstRoomDoor->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	FirstRoomDoorObj->GetTransformNonCount()->SetWorldRotY(-90.0f);
	FirstRoomDoor->SetLeftRight();
	FirstRoomDoor->Lock(true);

	CGameObject* SecondRoomDoorObj = CGameObject::CreateObject("SecondRoomDoor", pDefaultLayer);
	CDoor* SecondRoomDoor = SecondRoomDoorObj->AddComponent<CDoor>("SecondRoomDoor");
	SecondRoomDoorObj->GetTransformNonCount()->SetWorldPos(Vector3(42.0f, 0.0f, 57.9f));
	SecondRoomDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	SecondRoomDoor->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	SecondRoomDoorObj->GetTransformNonCount()->SetWorldRotY(-90.0f);
	SecondRoomDoor->SetLeftRight();
	SecondRoomDoor->Lock(true);

	CGameObject* ThirdRoomDoorObj = CGameObject::CreateObject("ThirdRoomDoor", pDefaultLayer);
	CDoor* ThirdRoomDoor = ThirdRoomDoorObj->AddComponent<CDoor>("ThirdRoomDoor");
	ThirdRoomDoorObj->GetTransformNonCount()->SetWorldPos(Vector3(42.0f, 0.0f, 97.9f));
	ThirdRoomDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	ThirdRoomDoorObj->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	ThirdRoomDoorObj->GetTransformNonCount()->SetWorldRotY(-90.0f);
	ThirdRoomDoor->SetLeftRight();
	ThirdRoomDoor->Lock(true);

	CGameObject* OutDoor1Obj = CGameObject::CreateObject("OutDoor1", pDefaultLayer);
	CDoor* OutDoor1 = OutDoor1Obj->AddComponent<CDoor>("OutDoor1");
	OutDoor1Obj->GetTransformNonCount()->SetWorldPos(Vector3(79.3f, 0.0f, 162.0f));
	OutDoor1->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	OutDoor1->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	OutDoor1->SetLeftRight();
	OutDoor1->Lock(true);

	CGameObject* OutDoor2Obj = CGameObject::CreateObject("OutDoor2", pDefaultLayer);
	CDoor* OutDoor2 = OutDoor2Obj->AddComponent<CDoor>("OutDoor2");
	OutDoor2->SetLeftRight();
	OutDoor2Obj->GetTransformNonCount()->SetWorldPos(Vector3(159.2f, 0.0f, 162.0f));
	OutDoor2->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	OutDoor2Obj->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	OutDoor2->Lock(true);

	CGameObject* EndDoorObj = CGameObject::CreateObject("EndDoor", pDefaultLayer);
	CDoor* EndDoor = EndDoorObj->AddComponent<CDoor>("EndDoor");
	EndDoorObj->GetTransformNonCount()->SetWorldPos(Vector3(219.3f, 0.0f, 152.4f));
	EndDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	EndDoor->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	EndDoorObj->GetTransformNonCount()->SetWorldRotY(90.0f);
	EndDoor->SetLeftRight();
	EndDoor->SetDoorType(DOOR_STAGE);
	EndDoor->Lock(true);

	CGameObject* MidDoorObj = CGameObject::CreateObject("MidDoor", pDefaultLayer);
	CDoor* MidDoor = MidDoorObj->AddComponent<CDoor>("MidDoor");
	MidDoorObj->GetTransformNonCount()->SetWorldPos(Vector3(157.7f, 0.0f, 10.4f));
	MidDoor->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	MidDoor->GetTransformNonCount()->SetWorldPivot(Vector3(0.25f, -0.5f, 0.0f));
	MidDoorObj->GetTransformNonCount()->SetWorldRotY(90.0f);
	MidDoor->SetLeftRight();

	CGameManager::GetInst()->AddDoor("Second", "StartDoor", StartDoor);
	CGameManager::GetInst()->AddDoor("Second", "OutDoor1", OutDoor1);
	CGameManager::GetInst()->AddDoor("Second", "OutDoor2", OutDoor2);
	CGameManager::GetInst()->AddDoor("Second", "FirstDoor", FirstRoomDoor);
	CGameManager::GetInst()->AddDoor("Second", "SecondDoor", SecondRoomDoor);
	CGameManager::GetInst()->AddDoor("Second", "ThirdDoor", ThirdRoomDoor);
	CGameManager::GetInst()->AddDoor("Second", "EndDoor", EndDoor);
	CGameManager::GetInst()->AddDoor("Second", "MidDoor", MidDoor);

	SAFE_RELEASE(MidDoorObj);
	SAFE_RELEASE(MidDoor);
	SAFE_RELEASE(StartDoorObj);
	SAFE_RELEASE(StartDoor);
	SAFE_RELEASE(FirstRoomDoorObj);
	SAFE_RELEASE(FirstRoomDoor);
	SAFE_RELEASE(SecondRoomDoorObj);
	SAFE_RELEASE(SecondRoomDoor);
	SAFE_RELEASE(ThirdRoomDoorObj);
	SAFE_RELEASE(ThirdRoomDoor);
	SAFE_RELEASE(OutDoor1Obj);
	SAFE_RELEASE(OutDoor1);
	SAFE_RELEASE(OutDoor2Obj);
	SAFE_RELEASE(OutDoor2);
	SAFE_RELEASE(EndDoorObj);
	SAFE_RELEASE(EndDoor);
	SAFE_RELEASE(StartDoorObj);
	SAFE_RELEASE(StartDoor);
	SAFE_RELEASE(pDefaultLayer);
}

void JBH_Stage3::NPCInit()
{
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	m_PlayerObject = pDefaultLayer->FindObjectNonCount("Player");

	m_ChaceObject = CGameObject::CreateObject("ChaceMonster", pDefaultLayer);
	m_ChaceNPC = m_ChaceObject->AddComponent<ST3_Slient>("ChaceMonster");
	m_ChaceNPC->GetTransformNonCount()->SetWorldPos(Vector3(36.5f, 0.0f, 11.0f));
	m_ChaceNPC->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, -90.0f, 0.0f));
	m_ChaceNPC->SetTarget(m_PlayerObject);

	CGameObject* SupriseObject = CGameObject::CreateObject("Suprise", pDefaultLayer);
	ST3_Suprise* Suprise = SupriseObject->AddComponent<ST3_Suprise>("Suprise");
	Suprise->GetTransformNonCount()->SetWorldPos(Vector3(155.0f, 30.0f, 11.0f));
	Suprise->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 180.0f, 0.0f));
	Suprise->SetTarget(m_PlayerObject);
	Suprise->SetDoorName("MidDoor");

	SAFE_RELEASE(SupriseObject);
	SAFE_RELEASE(Suprise);
	SAFE_RELEASE(pDefaultLayer);
}

void JBH_Stage3::SupriseSound(float DeltaTime)
{
	if (m_SlientMode == true)
	{
		if (m_isCanDrop == false)
		{
			m_CanDelayTimeVar += DeltaTime;

			if (m_CanDelayTime <= m_CanDelayTimeVar)
			{
				m_isCanDrop = true;
				CSoundManager::GetInst()->SoundPlay("GlassBracking");
			}
		}
	}
}
