#include "../ClientHeader.h"
#include "TestScene.h"
#include "Component/Arm.h"
#include "Component/Light.h"
#include "Component/Decal.h"
#include "Component/Camera.h"
#include "Component/Particle.h"
#include "Component/VolumeFog.h"
#include "Component/LandScape.h"
#include "Component/Animation2D.h"
#include "Component/ColliderOBB3D.h"
#include <Component/Renderer.h>
#include <Component/Material.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "Component/Gizmo.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/Battery.h"
#include "../UserComponent/BatteryIcon.h"

CTestScene::CTestScene()
{
	m_fPlayTime = 0.f;
	m_bGhostOn = false;
	m_bIncrease = false;
	m_fX = 0.f;
	m_fY = 0.f;
}

CTestScene::~CTestScene()
{
	SAFE_RELEASE(m_pUILayer);

	if (m_bGhostOn)
		SAFE_RELEASE(m_pGhostTr);
}

bool CTestScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Test.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);
	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->PlayBgm("bgm1");
	//
	//_SMgr->SetAudioCoordSize(16.f);

	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->SoundPlay("bgm1", ST_BGM);


	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");
	m_pUILayer = m_pScene->FindLayer("UI");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	CLayer* pLightLayer = m_pScene->FindLayer("Light");

	pObject = CGameObject::CreateObject("SpotLight", pLightLayer);

	CLight* pLight = pObject->AddComponent<CLight>("Light");
	pLight->SetLightDiffuse(Vector4::Red);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(100.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(50.0f, 70.0f, 0.0f);

	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLightLayer);


	//CGameObject* pFogObj = CGameObject::CreateObject("FogObj", pDefaultLayer);
	//pTransform = pFogObj->GetTransform();

	//pTransform->SetWorldScale(50, 50, 50);
	//pTransform->SetWorldPos(3, 4, 3);

	//SAFE_RELEASE(pTransform);

	//CVolumeFog* pFog = pFogObj->AddComponent<CVolumeFog>("Fog");
	//pFog->SetFogColor(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	//SAFE_RELEASE(pFog);
	//SAFE_RELEASE(pFogObj);


//////////////////////////////////////////Terrain 배치 //////////////////////////////////////////////////
///////////////////////////////X Tile 배치///////////////////////////////////////////////////////

	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(20.f, 0.f, 0.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.2f);


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);


	for (int i = 1; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(i * 40.f, 0.f, 0.f);
		pTransform->SetWorldRot(0.f, 0.f, 0.f);
		pTransform->SetWorldScale(0.2f);


		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	///////////////////////XCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(240.f, 0.f, 0.f);
	pTransform->SetWorldRot(0.f, 90.f, 0.f);
	pTransform->SetWorldScale(0.2f);


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);
	//////////////////////////////////Z배치//////////////////////////////////

	for (int i = 0; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(240.f, 0.f, i * 40.f + 40.f);
		pTransform->SetWorldRot(0.f, 90.f, 0.f);
		pTransform->SetWorldScale(0.2f);


		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	///////////////////////ZCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(240.f, 0.f, 280.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.2f);


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	///////////////////////////////X Tile 배치///////////////////////////////////////////////////////
	for (int i = 0; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(200.f - (i * 40.f), 0.f, 280.f);
		pTransform->SetWorldRot(0.f, 180.f, 0.f);
		pTransform->SetWorldScale(0.2f);


		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}

	///////////////////////XCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(-30.f, 0.f, 280.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(0.2f);


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	///////////////////////////////Z Tile 배치///////////////////////////////////////////////////////
	for (int i = 0; i < 5; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(-30.f, 0.f, 240.f - (i * 40.f));
		pTransform->SetWorldRot(0.f, 90.f, 0.f);
		pTransform->SetWorldScale(0.2f);


		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx2m.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(-30.f, 0.f, 50.f);
	pTransform->SetWorldRot(0.f, 90.f, 0.f);
	pTransform->SetWorldScale(0.2f);


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	////////////////////////////////////////////////////////////////////////

	pObject = CGameObject::CreateObject("Door", pDefaultLayer);
	pTransform = pObject->GetTransform();

	CDoor* pDoor = pObject->AddComponent<CDoor>("door");


	pTransform->SetWorldPos(0.f, 0.f, 5.f);
	pTransform->SetWorldScale(0.1f, .1f, 0.1f);

	pTransform->RotationY(-90.f);
	
	pTransform->SetWorldPivot(0.5f, .0f, .0f);
	pDoor->SetOpenTime(1.5f);
	pDoor->SetCloseTime(1.5f);
	
	std::string names[3];
	names[0] = "wood_Door_Open1";
	names[1] = "wood_Door_Close1";
	names[2] = "wood_Door_Bash1";

	TCHAR *strPaths[3]; 
	strPaths[0] = (TCHAR*)TEXT("WoodenDoor_OPENING_01.wav");
	strPaths[1] = (TCHAR*)TEXT("WoodenDoor_CLOSING_03.wav");
	strPaths[2] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");

	pDoor->SetSounds(names, (const TCHAR**)strPaths);

	PUN::CRenderer* renderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	renderer->SetMesh("wdoor_1", TEXT("wood_door.FBX"), MESH_PATH);

	
	SAFE_RELEASE(pDoor);

	SAFE_RELEASE(renderer);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = PUN::CGameObject::CreateObject("hPlayer", pDefaultLayer, true);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.1f, .1f, .1f);
	CHuman_Player *pHPlayer = pObject->AddComponent<CHuman_Player>("Player");
	pHPlayer->LoadData(TEXT("PlayerData.csv"));

	SAFE_RELEASE(pHPlayer);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(-10.f, 5.f, 30.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);

	CGameObject*	pBattery2Obj = CGameObject::CreateObject("Battery2", pDefaultLayer);

	CBattery*	pBattery2 = pBattery2Obj->AddComponent<CBattery>("Battery2");

	CTransform*	pBattery2Tr = pBattery2Obj->GetTransform();

	pBattery2Tr->SetWorldPos(-10.f, 5.f, 10.f);

	SAFE_RELEASE(pBattery2Tr);
	SAFE_RELEASE(pBattery2);
	SAFE_RELEASE(pBattery2Obj);

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CTestScene::Update(float fTime)
{
	static bool bPush = false;
	if (GetAsyncKeyState('P') & 0x8000)
	{
		bPush = true;
	}
	else if (bPush)
	{
		bPush = false;
		m_bGhostOn = true;
		m_bIncrease = true;

		CGameObject*	pGhostObj = CGameObject::CreateObject("Ghost", m_pUILayer);

		CGhost*	pGhost = pGhostObj->AddComponent<CGhost>("Ghost");

		m_pGhostTr = pGhostObj->GetTransform();

		//SAFE_RELEASE(pGhostTr);
		SAFE_RELEASE(pGhost);
		SAFE_RELEASE(pGhostObj);
	}

	if (m_bIncrease)
	{
		m_fPlayTime += fTime;

		if (m_fX <= 4096.f && m_fY <= 4096.f)
		{
			m_fX += 9000 * fTime;
			m_fY += 9000 * fTime;
			m_pGhostTr->SetWorldScale(m_fX, m_fY, 1.f);
		}
	}

	if (m_fPlayTime >= 2.f)
	{
		m_bIncrease = false;
		m_fPlayTime = 0.f;
		m_fX = 1.f;
		m_fY = 1.f;
	}

	return 0;
}

void CTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CTestScene::CreatePrototype()
{
}

void CTestScene::NextScene(float fTime)
{
}
