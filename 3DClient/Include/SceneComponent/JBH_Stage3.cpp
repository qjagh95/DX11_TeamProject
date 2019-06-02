#include "../ClientHeader.h"
#include "JBH_Stage3.h"

#include <Component/Camera.h>
#include <Component/FreeCamera.h>
#include <Component/LandScape.h>

#include "../GameManager.h"

#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST3_Suprise.h"
#include "../UserComponent/ST3_SlientTrace.h"
#include "../UserComponent/Door.h"
#include "../CameraEff.h"

JBH_Stage3::JBH_Stage3()
{
	m_PlayerState = 0;
	m_Player = NULLPTR;
	m_PlayerObject = NULLPTR;
}

JBH_Stage3::~JBH_Stage3()
{
	SAFE_RELEASE(m_Player);
	SAFE_RELEASE(m_PlayerObject);
}

bool JBH_Stage3::Init()
{
	CCamera* pCamera = m_pScene->GetMainCameraNonCount();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	//pCamera->GetTransformNonCount()->SetLocalPos(Vector3(0.f, 5.0f, 5.0f));
	//pCamera->SetCameraInfo(CT_PERSPECTIVE, 1280.0f, 720.0f, 90.0f, 0.03f, 1000.0f);
	//CFreeCamera* FreeCam = pCamera->AddComponent<CFreeCamera>("FreeCam");

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "JBH_Stage3.dat";
	m_pScene->Load(Path);

	CGameObject* NavLandObject = CGameObject::CreateObject("Land", pDefaultLayer);
	CLandScape* Land = NavLandObject->AddComponent< CLandScape>("Land");
	
	Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "Stage3Nav.nav";
	Land->LoadLandScape(Path);

	m_PlayerObject = CGameObject::CreateObject("Player", pDefaultLayer, true);
	m_Player = m_PlayerObject->AddComponent<CHuman_Player>("Player");
	m_Player->GetTransformNonCount()->SetWorldPos(Vector3(238.0f, 0.0f, 68.0f));
	m_Player->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	m_Player->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, -90.0f, 0.0f));
	pCamera->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 270.0f, 0.0f));

	//CGameObject* TestNPC = CGameObject::CreateObject("TestCJH", pDefaultLayer);
	//ST3_Suprise* TestMob = TestNPC->AddComponent<ST3_Suprise>("TestCJH");
	//TestMob->GetTransformNonCount()->SetWorldPos(Vector3(200.0f, 20.0f, 68.0f));
	//TestMob->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 270.0f, 0.0f));
	//TestMob->SetTarget(PlayerObject);
	//TestMob->SetDoorName("TestDoor");

	CGameObject* TestNPC = CGameObject::CreateObject("TestCJH", pDefaultLayer);
	ST3_SlientTrace* TestMob = TestNPC->AddComponent<ST3_SlientTrace>("TestCJH");
	TestMob->GetTransformNonCount()->SetWorldPos(Vector3(220.0f, 0.0f, 68.0f));
	TestMob->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, -90.0f, 0.0f));
	TestMob->SetTarget(m_PlayerObject);

	CGameObject* StartDoorObj = CGameObject::CreateObject("StartDoor", pDefaultLayer);
	CDoor* StartDoor = StartDoorObj->AddComponent<CDoor>("StartDoor");
	StartDoor->GetTransformNonCount()->SetWorldPos(Vector3(220.0f, 0.0f, 68.0f));
	StartDoor->GetTransformNonCount()->SetLocalRotY(90.0f);

	CGameManager::GetInst()->AddDoor("Second", "StartDoor", StartDoor);

	SAFE_RELEASE(StartDoorObj);
	SAFE_RELEASE(StartDoor);
	//SAFE_RELEASE(FreeCam);
	SAFE_RELEASE(TestNPC);
	SAFE_RELEASE(TestMob);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(NavLandObject);
	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pDefaultLayer);

	m_DoorMap = CGameManager::GetInst()->GetDoorMap(m_pScene);

	return true;
}

void JBH_Stage3::AfterInit()
{

}

int JBH_Stage3::Update(float DeltaTime)
{
	m_PlayerState = m_Player->GetState();
	m_PlayerSection = m_PlayerObject->GetStageSection();

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
