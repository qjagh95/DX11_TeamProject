#include "../ClientHeader.h"
#include "JBH_Stage3.h"

#include <Component/Camera.h>
#include <Component/FreeCamera.h>
#include <Component/LandScape.h>

#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST3_Suprise.h"
#include "../CameraEff.h"

JBH_Stage3::JBH_Stage3()
{
}

JBH_Stage3::~JBH_Stage3()
{
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
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "JBH_Stage3.dat";
	m_pScene->Load(Path);

	CGameObject* NavLandObject = CGameObject::CreateObject("Land", pDefaultLayer);
	CLandScape* Land = NavLandObject->AddComponent< CLandScape>("Land");
	
	Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "Stage3Nav.nav";
	Land->LoadLandScape(Path);

	CGameObject* PlayerObject = CGameObject::CreateObject("Player", pDefaultLayer, true);
	CHuman_Player* newPlayer = PlayerObject->AddComponent<CHuman_Player>("Player");
	newPlayer->GetTransformNonCount()->SetWorldPos(Vector3(238.0f, 0.0f, 68.0f));
	newPlayer->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	newPlayer->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, -90.0f, 0.0f));
	pCamera->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 270.0f, 0.0f));

	CGameObject* TestNPC = CGameObject::CreateObject("TestCJH", pDefaultLayer);
	ST3_Suprise* TestMob = TestNPC->AddComponent<ST3_Suprise>("TestCJH");
	TestMob->GetTransformNonCount()->SetWorldPos(Vector3(200.0f, 20.0f, 68.0f));
	TestMob->GetTransformNonCount()->SetWorldRot(Vector3(0.0f, 180.0f, 0.0f));
	TestMob->SetTarget(PlayerObject);

	//SAFE_RELEASE(FreeCam);
	SAFE_RELEASE(TestNPC);
	SAFE_RELEASE(TestMob);
	SAFE_RELEASE(newPlayer);
	SAFE_RELEASE(PlayerObject);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(NavLandObject);
	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);

	return true;
}

int JBH_Stage3::Update(float DeltaTime)
{
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
