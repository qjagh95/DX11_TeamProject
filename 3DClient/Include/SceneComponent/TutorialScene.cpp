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
#include "../UserComponent/Door.h"
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
#include "../UserComponent/HealingPack.h"
#include "../UserComponent/Cigarette.h"
#include "../UserComponent/Tablet.h"
#include "../UserComponent/PaperSilent.h"
#include "../UserComponent/PaperGenerator.h"
#include "../UserComponent/PaperGongji.h"
#include "../UserComponent/PaperMsgTH.h"
#include "../UserComponent/PaperPlanA.h"
#include "../UserComponent/PaperTest.h"
#include "../UserComponent/PaperBQ.h"
#include "../UserComponent/PaperPL.h"
#include "../UserComponent/KeyCard.h"
#include "../UserComponent/EventCollider.h"
#include <NavigationMesh.h>

CTutorialScene::CTutorialScene()
{
}

CTutorialScene::~CTutorialScene()
{
}

void CTutorialScene::AfterInit()
{
	// CedarBush
	AdaptAlpha("CedarBush1");
	AdaptAlpha("CedarBush2");
	AdaptAlpha("CedarBush4");
	AdaptAlpha("CedarBush5");
	AdaptAlpha("CedarBush6");
	AdaptAlpha("CedarBush7");
	AdaptAlpha("CedarBush8");
	AdaptAlpha("CedarBush9");
	AdaptAlpha("CedarBush10");
	AdaptAlpha("CedarBush11");

	// Grass
	AdaptAlpha("Grass1");
	AdaptAlpha("Grass4");
	AdaptAlpha("Grass5");
	AdaptAlpha("Grass6");
	AdaptAlpha("Grass7");
	AdaptAlpha("Grass8");
	AdaptAlpha("Grass9");

	// LeftTree
	AdaptAlpha("LeftTree1");
	AdaptAlpha("LeftTree2");
	AdaptAlpha("LeftTree3");
	AdaptAlpha("LeftTree4");
	AdaptAlpha("LeftTree5");

	// RightTree
	AdaptAlpha("RightTree1");
	AdaptAlpha("RightTree2");
	AdaptAlpha("RightTree3");
	AdaptAlpha("RightTree4");
	AdaptAlpha("RightTree5");
	AdaptAlpha("RightTree6");
	AdaptAlpha("RightTree7");
	AdaptAlpha("RightTree8");
	AdaptAlpha("RightTree9");
	AdaptAlpha("RightTree10");
	
	// Gate
	AdaptAlpha("Gate1");
	AdaptAlpha("Gate2");
}

bool CTutorialScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"LightTest.dat";
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

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	CGameObject*	pLandScapeObj = CGameObject::CreateObject("TutorialScape", pDefaultLayer);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("TutorialScape");

	wstring nav = CPathManager::GetInst()->FindPath(DATA_PATH);
	nav += L"HFTutorial.nav";
	string navfilePath = CW2A(nav.c_str());

	pLandScape->LoadLandScape(navfilePath);

	SAFE_RELEASE(pLandScape);
	SAFE_RELEASE(pLandScapeObj);	

	CGameObject*	pKeyCardObj = CGameObject::CreateObject("KeyCard", pDefaultLayer);

	CKeyCard*	pKeyCard = pKeyCardObj->AddComponent<CKeyCard>("KeyCard");

	CTransform*	pKeyCardTr = pKeyCardObj->GetTransform();

	pKeyCardTr->SetWorldPos(112.f, 17.f, 621.f);

	SAFE_RELEASE(pKeyCardTr);
	SAFE_RELEASE(pKeyCard);
	SAFE_RELEASE(pKeyCardObj);

	CGameObject*	pDoorObj = CGameObject::CreateObject("TutorialDoor", pDefaultLayer);

	CDoor*	pDoor = pDoorObj->AddComponent<CDoor>("TutorialDoor");

	pDoor->Lock(true, "KeyCard");

	CTransform*	pDoorTr = pDoorObj->GetTransform();

	pDoorTr->SetWorldPos(304.5f, 29.2f, 557.3f);
	//pDoorTr->SetLocalRot(0.f, 90.f, 0.f);
	pDoorTr->SetWorldRot(0.f, 180.f, 0.f);

	SAFE_RELEASE(pDoorTr);
	SAFE_RELEASE(pDoor);
	SAFE_RELEASE(pDoorObj);

	CGameObject*	pEventObj = CGameObject::CreateObject("EventCollider", pDefaultLayer);

	CEventCollider*	pEvent = pEventObj->AddComponent<CEventCollider>("EventCollider");

	pEvent->SetTutorial();
	pEvent->NoticeCamera();

	SAFE_RELEASE(pEvent);
	SAFE_RELEASE(pEventObj);

	// MedicalKit
	//CGameObject* pObjMedicalKit = CGameObject::CreateObject("MedicalKit", pDefaultLayer);
	//CTransform* pMedicalKitTr = pObjMedicalKit->GetTransform();
	//pMedicalKitTr->SetWorldPos(356.f, 20.f, 650.f);
	//CHealingPack* pUCHealingPack = pObjMedicalKit->AddComponent<CHealingPack>("UC_HealingPack");
	////pUCHealingPack->SetHPAmount(pPlayer->GetMaxHP());
	//pUCHealingPack->SetMesh("MedicalKit", TEXT("MedicalKit.msh"));
	//pMedicalKitTr->SetWorldScale(10.f);
	//SAFE_RELEASE(pUCHealingPack);
	//SAFE_RELEASE(pMedicalKitTr);
	//SAFE_RELEASE(pObjMedicalKit);

	//// Lunch Box
	//CGameObject* pObjLunchBox = CGameObject::CreateObject("LunchBox", pDefaultLayer);
	//CTransform* pLuncBoxTr = pObjLunchBox->GetTransform();
	//pLuncBoxTr->SetWorldPos(400.f, 20.f, 630.f);
	//pLuncBoxTr->SetWorldScale(100.f);
	//pUCHealingPack = pObjLunchBox->AddComponent<CHealingPack>("UC_HealingPack");
	//pUCHealingPack->SetHPAmount(1);
	//pUCHealingPack->SetMesh("LunchBox", TEXT("LunchBox.msh"));
	//SAFE_RELEASE(pUCHealingPack);
	//SAFE_RELEASE(pLuncBoxTr);
	//SAFE_RELEASE(pObjLunchBox);

	//// Cigarette
	//CGameObject* pObjCigarette = CGameObject::CreateObject("Cigarette", pDefaultLayer);
	//CTransform* pCigaretteTr = pObjCigarette->GetTransform();
	//pCigaretteTr->SetWorldPos(300.f, 20.f, 620.f);
	//CCigarette* pUCCigarette = pObjCigarette->AddComponent<CCigarette>("UC_Cigarette");
	//SAFE_RELEASE(pUCCigarette);
	//SAFE_RELEASE(pCigaretteTr);
	//SAFE_RELEASE(pObjCigarette);

	//// Tablet
	//CGameObject* pObjTablet = CGameObject::CreateObject("Tablet", pDefaultLayer);
	//CTransform* pTabletTr = pObjTablet->GetTransform();
	//pTabletTr->SetWorldPos(300.f, 20.f, 600.f);
	//CTablet* pUCTablet = pObjTablet->AddComponent<CTablet>("UC_Tablet");
	//SAFE_RELEASE(pUCTablet);
	//SAFE_RELEASE(pTabletTr);
	//SAFE_RELEASE(pObjTablet);		
	
	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(108.f, 17.f, 619.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);	

	/*CGameObject*	pBushObject = CGameObject::CreateObject("inside", pDefaultLayer);

	CRenderer*	pBushRenderer = pBushObject->AddComponent<CRenderer>("insideRenderer");

	pBushRenderer->SetMesh("inside", L"DoorInside.fbx", MESH_PATH);

	SAFE_RELEASE(pBushRenderer);

	CTransform*	pBushTransform = pBushObject->GetTransform();

	pBushTransform->SetWorldPos(316.f,10.f, 700.f);
	pBushTransform->SetWorldRot(0.f, 90.f, 0.f);
	pBushTransform->SetWorldScale(0.08f);

	SAFE_RELEASE(pBushTransform);

	SAFE_RELEASE(pBushObject);		*/

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);

	return true;
}

int CTutorialScene::Update(float fTime)
{
	return 0;
}

void CTutorialScene::AdaptAlpha(const string & strName)
{
	CGameObject*	pTreeObj = CGameObject::FindObject(strName);

	CRenderer*	pTreeRenderer = pTreeObj->FindComponentFromType<CRenderer>(CT_RENDERER);

	pTreeRenderer->SetAlphaEnable(true);

	m_vecAlpha.push_back(strName);

	SAFE_RELEASE(pTreeRenderer);
	SAFE_RELEASE(pTreeObj);
}
