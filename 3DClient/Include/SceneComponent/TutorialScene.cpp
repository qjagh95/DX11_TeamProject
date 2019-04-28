#include "../ClientHeader.h"
#include "TutorialScene.h"
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
#include "Component/FreeCamera.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/Battery.h"
#include "../UserComponent/BatteryIcon.h"

CTutorialScene::CTutorialScene()
{
}

CTutorialScene::~CTutorialScene()
{
}

bool CTutorialScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Tutorial.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	GET_SINGLE(CRenderManager)->SetHDRValue(0.22f, 5.7f);

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CTransform* pCameraTr = pCamera->GetTransform();

	//CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");

	//SAFE_RELEASE(pFreeCam);
	SAFE_RELEASE(pCameraTr);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");


	CGameObject*	pLandScapeObj = CGameObject::CreateObject("TutorialScape", pDefaultLayer);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("TutorialScape");

	wstring nav = CPathManager::GetInst()->FindPath(DATA_PATH);
	nav += L"Tutorial.nav";
	string navfilePath = CW2A(nav.c_str());

	pLandScape->LoadLandScape(navfilePath);

	CTransform*	pLandScapeTr = pLandScapeObj->GetTransform();

	pLandScapeTr->SetWorldPos(0.f, -70.f, 0.f);

	SAFE_RELEASE(pLandScapeTr);
	SAFE_RELEASE(pLandScape);
	SAFE_RELEASE(pLandScapeObj);
	

	CGameObject*	pPlayerObj = CGameObject::CreateObject("Player", pDefaultLayer);

	CHuman_Player*	pPlayer = pPlayerObj->AddComponent<CHuman_Player>("Player");
	//CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");

	//pPlayer->LoadData(TEXT("PlayerData.csv"));

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	pPlayerTr->SetLocalRot(0.f, 180.f, 0.f);
	pPlayerTr->SetWorldScale(0.1f, .1f, .1f);
	pPlayerTr->SetWorldPos(-62.65f, -2.21f, 421.36f);

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	CLayer* pLightLayer = m_pScene->FindLayer("Light");

	pObject = CGameObject::CreateObject("SpotLight", pLightLayer);

	CLight* pLight = pObject->AddComponent<CLight>("Light");
	pLight->SetLightDiffuse(Vector4::White);
	pLight->SetLightType(LT_DIR);
	pLight->SetLightRange(100.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(50.0f, 70.0f, 0.0f);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLightLayer);

	//////////////////////////////////////////Terrain 배치 //////////////////////////////////////////////////
	///////////////////////////////X Tile 배치///////////////////////////////////////////////////////

	/*pObject = CGameObject::CreateObject("Daema", pDefaultLayer);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("DaemaRenderer");

	pRenderer->SetMesh("Daema", L"Daema.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(0, 0, 0.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(100.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("LunchBox", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("LunchBoxRenderer");

	pRenderer->SetMesh("LunchBox", L"LunchBox.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(100, 0, 0.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(100.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("MedicalKit", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("MedicalKitRenderer");

	pRenderer->SetMesh("MedicalKit", L"MedicalKit.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(200, 0, 0.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(100.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Paper", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("LPaperRenderer");

	pRenderer->SetMesh("Paper", L"Paper.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(300, 0, 0.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(100.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Tablet", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TabletRenderer");

	pRenderer->SetMesh("Tablet", L"Tablet.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(400, 0, 0.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(100.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);*/

	/*CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(-10.f, 5.f, 30.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);

	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(-100.f, 5.f, 10.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);*/
	

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pUILayer);

	return true;
}

int CTutorialScene::Update(float fTime)
{
	return 0;
}
