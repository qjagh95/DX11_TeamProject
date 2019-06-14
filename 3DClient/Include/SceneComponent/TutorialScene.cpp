//#include "../ClientHeader.h"
//#include "TutorialScene.h"
//#include "Component/Arm.h"
//#include "Component/Light.h"
//#include "Component/Decal.h"
//#include "Component/Camera.h"
//#include "Component/Particle.h"
//#include "Component/VolumeFog.h"
//#include "Component/LandScape.h"
//#include "Component/Animation2D.h"
//#include "Component/ColliderOBB3D.h"
//#include <Component/Renderer.h>
//#include <Component/Material.h>
//#include <Component/SoundSource.h>
//#include <Component/Animation.h>
//#include "../UserComponent/Door.h"
//#include "Component/FreeCamera.h"
//#include "Component/Renderer.h"
//#include "Component/Material.h"
//#include "../UserComponent/Door.h"
//#include "../UserComponent/Player.h"
//#include "../UserComponent/Minion.h"
//#include "../UserComponent/Ghost.h"
//#include "../UserComponent/Human_Player.h"
//#include "../UserComponent/Battery.h"
//#include "../UserComponent/BatteryIcon.h"
//#include "../UserComponent/HealingPack.h"
//#include "../UserComponent/Cigarette.h"
//#include "../UserComponent/Tablet.h"
//#include "../UserComponent/PaperSilent.h"
//#include "../UserComponent/PaperGenerator.h"
//#include "../UserComponent/PaperGongji.h"
//#include "../UserComponent/PaperMsgTH.h"
//#include "../UserComponent/PaperPlanA.h"
//#include "../UserComponent/PaperTest.h"
//#include "../UserComponent/PaperBQ.h"
//#include "../UserComponent/PaperPL.h"
//#include "../UserComponent/KeyCard.h"
//#include "../UserComponent/EventCollider.h"
//#include <NavigationMesh.h>
//#include "../GameManager.h"
//#include "../UserComponent/StageKey.h"
//
//CTutorialScene::CTutorialScene()
//{
//	m_fFade = 0.0001f;
//	m_fAdaptationTime = 0.f;
//}
//
//CTutorialScene::~CTutorialScene()
//{
//}
//
//void CTutorialScene::AfterInit()
//{
//	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_Tutorial");
//	pDoor->Lock(true, "KeyCard");
//}
//
//bool CTutorialScene::Init()
//{
//	GET_SINGLE(CSoundManager)->CreateSoundEffect("TutorialBGM", L"ambient/Thunder_Roll_08.wav");
//	GET_SINGLE(CSoundManager)->PlayBgm("TutorialBGM");
//
//	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
//	wstr += L"TutorialScene.dat";
//	//wstr += L"TH2_Stage3.dat";
//	string filePath = CW2A(wstr.c_str());
//	m_pScene->Load(filePath);
//
//	GET_SINGLE(CRenderManager)->SetHDRValue(0.22f, 5.7f);
//	GET_SINGLE(CRenderManager)->SetDepthFog(true, 0.136f, 0.005f, 0.136f, 0.f, 168.f);
//
//	CCamera* pCamera = m_pScene->GetMainCamera();
//	pCamera->SetCameraType(CT_PERSPECTIVE);
//	pCamera->SetNear(0.03f);
//
//	CTransform* pCameraTr = pCamera->GetTransform();
//
//	//CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");
//
//	//SAFE_RELEASE(pFreeCam);
//	SAFE_RELEASE(pCameraTr);
//
//	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
//	CLayer*	pUILayer = m_pScene->FindLayer("UI");
//
//	CGameObject*	pLandScapeObj = CGameObject::CreateObject("TutorialScape", pDefaultLayer);
//
//	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("TutorialScape");
//
//	wstring nav = CPathManager::GetInst()->FindPath(DATA_PATH);
//	nav += L"HFTutorial.nav";
//	string navfilePath = CW2A(nav.c_str());
//
//	pLandScape->LoadLandScape(navfilePath);
//
//	SAFE_RELEASE(pLandScape);
//	SAFE_RELEASE(pLandScapeObj);	
//
//	CGameObject*	pKeyCardObj = CGameObject::CreateObject("KeyCard", pDefaultLayer);
//
//	CKeyCard*	pKeyCard = pKeyCardObj->AddComponent<CKeyCard>("KeyCard");
//
//	CTransform*	pKeyCardTr = pKeyCardObj->GetTransform();
//
//	pKeyCardTr->SetWorldPos(112.f, 17.f, 621.f);
//
//	Vector3 vKeyPos = pKeyCardTr->GetWorldPos();
//	pKeyCard->SetOutLinePos(vKeyPos);
//
//	SAFE_RELEASE(pKeyCardTr);
//	SAFE_RELEASE(pKeyCard);
//	SAFE_RELEASE(pKeyCardObj);
//
//	/*pKeyCardObj = CGameObject::CreateObject("StageKey", pDefaultLayer);
//
//	CStageKey*	pStageKey = pKeyCardObj->AddComponent<CStageKey>("StageKey");
//
//	pKeyCardTr = pKeyCardObj->GetTransform();
//
//	pKeyCardTr->SetWorldPos(316.f, 10.f, 748.f);
//
//	Vector3 vStageKeyPos = pKeyCardTr->GetWorldPos();
//	pStageKey->SetOutLinePos(vStageKeyPos);
//
//	SAFE_RELEASE(pKeyCardTr);
//	SAFE_RELEASE(pStageKey);
//	SAFE_RELEASE(pKeyCardObj);*/
//
//	CGameObject*	pEventObj = CGameObject::CreateObject("EventCollider", pDefaultLayer);
//
//	CEventCollider*	pEvent = pEventObj->AddComponent<CEventCollider>("EventCollider");
//
//	pEvent->SetTutorial();
//
//	SAFE_RELEASE(pEvent);
//	SAFE_RELEASE(pEventObj);
//
//	// MedicalKit
//	//CGameObject* pObjMedicalKit = CGameObject::CreateObject("HealingPack", pDefaultLayer);
//
//	//CTransform* pMedicalKitTr = pObjMedicalKit->GetTransform();
//	//pMedicalKitTr->SetWorldPos(356.f, 20.f, 650.f);
//
//	//CHealingPack* pUCHealingPack = pObjMedicalKit->AddComponent<CHealingPack>("HealingPack");
//	////pUCHealingPack->SetHPAmount(pPlayer->GetMaxHP());
//	//pUCHealingPack->SetMesh("MedicalKit", TEXT("MedicalKit.msh"));
//	//pMedicalKitTr->SetWorldScale(10.f);
//
//	//pUCHealingPack->SetOutLineScale(25.f, 25.f, 25.f);
//	//Vector3 vMKPos = pMedicalKitTr->GetWorldPos();
//	//pUCHealingPack->SetOutLinePos(vMKPos);
//
//	//SAFE_RELEASE(pUCHealingPack);
//	//SAFE_RELEASE(pMedicalKitTr);
//	//SAFE_RELEASE(pObjMedicalKit);
//
//	//// Lunch Box
//	//CGameObject* pObjLunchBox = CGameObject::CreateObject("LunchBox", pDefaultLayer);
//
//	//CTransform* pLuncBoxTr = pObjLunchBox->GetTransform();
//	//pLuncBoxTr->SetWorldPos(400.f, 20.f, 630.f);
//	//pLuncBoxTr->SetWorldScale(20.f);
//
//	//pUCHealingPack = pObjLunchBox->AddComponent<CHealingPack>("UC_HealingPack");
//	//pUCHealingPack->SetHPAmount(1);
//	//pUCHealingPack->SetMesh("LunchBox", TEXT("LunchBox.msh"));
//
//	//pUCHealingPack->SetOutLineScale(40.f, 40.f, 40.f);
//	//Vector3 vLunchPos = pLuncBoxTr->GetWorldPos();
//	//pUCHealingPack->SetOutLinePos(vLunchPos);
//
//	//SAFE_RELEASE(pUCHealingPack);
//	//SAFE_RELEASE(pLuncBoxTr);
//	//SAFE_RELEASE(pObjLunchBox);
//
//	//// Cigarette
//	CGameObject* pObjCigarette = CGameObject::CreateObject("Cigarette", pDefaultLayer);
//
//	CTransform* pCigaretteTr = pObjCigarette->GetTransform();
//	pCigaretteTr->SetWorldPos(300.f, 20.f, 620.f);
//
//	CCigarette* pUCCigarette = pObjCigarette->AddComponent<CCigarette>("UC_Cigarette");
//
//	Vector3 pCigaPos = pCigaretteTr->GetWorldPos();
//	pUCCigarette->SetOutLinePos(pCigaPos);
//
//	SAFE_RELEASE(pUCCigarette);
//	SAFE_RELEASE(pCigaretteTr);
//	SAFE_RELEASE(pObjCigarette);
//
//	//// Tablet
//	//CGameObject* pObjTablet = CGameObject::CreateObject("Tablet", pDefaultLayer);
//
//	//CTransform* pTabletTr = pObjTablet->GetTransform();
//	//pTabletTr->SetWorldPos(300.f, 20.f, 600.f);
//
//	//CTablet* pUCTablet = pObjTablet->AddComponent<CTablet>("UC_Tablet");
//
//	//Vector3 pTabletPos = pTabletTr->GetWorldPos();
//	//pUCTablet->SetOutLinePos(pTabletPos);
//
//	//SAFE_RELEASE(pUCTablet);
//	//SAFE_RELEASE(pTabletTr);
//	//SAFE_RELEASE(pObjTablet);		
//	//
//	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);
//
//	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");
//
//	CTransform*	pBatteryTr = pBatteryObj->GetTransform();
//
//	pBatteryTr->SetWorldPos(108.f, 17.f, 619.f);
//
//	Vector3 pBatPos = pBatteryTr->GetWorldPos();
//	pBattery->SetOutLinePos(pBatPos);
//
//	SAFE_RELEASE(pBatteryTr);
//	SAFE_RELEASE(pBattery);
//	SAFE_RELEASE(pBatteryObj);	
//
//	//CGameObject*	pPaperObj = CGameObject::CreateObject("Paper", pDefaultLayer);
//
//	//CPaperGenerator*	pPaper = pPaperObj->AddComponent<CPaperGenerator>("Paper");
//
//	//CTransform*	pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(300.f, 20.f, 640.f);
//
//	//Vector3 pPaperPos = pPaperTr->GetWorldPos();
//	//pPaper->SetOutLinePos(pPaperPos);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(pPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper2", pDefaultLayer);
//
//	//CPaperGongji*	pgPaper = pPaperObj->AddComponent<CPaperGongji>("Paper2");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(300.f, 20.f, 670.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//pgPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(pgPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper3", pDefaultLayer);
//
//	//CPaperMsgTH*	pmPaper = pPaperObj->AddComponent<CPaperMsgTH>("Paper3");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(320.f, 15.f, 680.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//pmPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(pmPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper4", pDefaultLayer);
//
//	//CPaperPlanA*	paPaper = pPaperObj->AddComponent<CPaperPlanA>("Paper4");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(320.f, 15.f, 690.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//paPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(paPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper5", pDefaultLayer);
//
//	//CPaperTest*	tmPaper = pPaperObj->AddComponent<CPaperTest>("Paper5");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(330.f, 15.f, 680.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//tmPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(tmPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper51", pDefaultLayer);
//
//	//CPaperBQ*	tmaPaper = pPaperObj->AddComponent<CPaperBQ>("Paper51");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(340.f, 15.f, 680.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//tmaPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(tmaPaper);
//	//SAFE_RELEASE(pPaperObj);
//
//	//pPaperObj = CGameObject::CreateObject("Paper52", pDefaultLayer);
//
//	//CPaperPL*	tmiPaper = pPaperObj->AddComponent<CPaperPL>("Paper52");
//
//	//pPaperTr = pPaperObj->GetTransform();
//
//	//pPaperTr->SetWorldPos(350.f, 15.f, 680.f);
//
//	//pPaperPos = pPaperTr->GetWorldPos();
//	//tmiPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);
//
//	//SAFE_RELEASE(pPaperTr);
//	//SAFE_RELEASE(tmiPaper);
//	//SAFE_RELEASE(pPaperObj);	
//
//	SAFE_RELEASE(pCamera);
//
//	SAFE_RELEASE(pDefaultLayer);
//	SAFE_RELEASE(pUILayer);
//
//	return true;
//}
//
//int CTutorialScene::Update(float fTime)
//{
//	if (GET_SINGLE(CSceneManager)->GetChange())
//	{
//		m_fFade += 0.1f * fTime;
//		GET_SINGLE(CRenderManager)->SetFadeAmount(1.f, fTime);
//	}
//
//	if (m_fFade > 1.f)
//	{
//		m_fFade = 1.f;
//		GET_SINGLE(CSceneManager)->SetChange(false);
//	}
//
//	m_fAdaptationTime += fTime;
//
//	if (m_fAdaptationTime <= 1.f)
//	{
//		GET_SINGLE(CRenderManager)->SetAdaptValue(0.5f, fTime, true);
//	}
//
//	if (m_fAdaptationTime > 1.f && m_fAdaptationTime <= 4.f)
//	{
//		GET_SINGLE(CRenderManager)->SetAdaptValue(3.f, fTime, true);
//	}
//
//	if (m_fAdaptationTime > 3.f)
//		m_fAdaptationTime = 0.f;
//
//	return 0;
//}
//
//int CTutorialScene::LateUpdate(float fTime)
//{
//	GET_SINGLE(CGameManager)->Update(fTime);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////

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
#include "../GameManager.h"
#include "../UserComponent/StageKey.h"

CTutorialScene::CTutorialScene()
{
	m_fFade = 0.f;
	m_fAdaptationTime = 0.f;
}

CTutorialScene::~CTutorialScene()
{
}

void CTutorialScene::AfterInit()
{
	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_Tutorial");
	pDoor->Lock(true, "KeyCard");
}

bool CTutorialScene::Init()
{
	GET_SINGLE(CSoundManager)->CreateSoundEffect("TutorialBGM", L"ambient/Thunder_Roll_08.wav");
	GET_SINGLE(CSoundManager)->PlayBgm("TutorialBGM");

	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"TutorialScene.dat";
	//wstr += L"TH2_Stage3.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	GET_SINGLE(CRenderManager)->SetHDRValue(0.22f, 5.7f);
	GET_SINGLE(CRenderManager)->SetDepthFog(true, 0.136f, 0.005f, 0.136f, 0.f, 168.f);

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

	Vector3 vKeyPos = pKeyCardTr->GetWorldPos();
	pKeyCard->SetOutLinePos(vKeyPos);

	SAFE_RELEASE(pKeyCardTr);
	SAFE_RELEASE(pKeyCard);
	SAFE_RELEASE(pKeyCardObj);

	/*pKeyCardObj = CGameObject::CreateObject("StageKey", pDefaultLayer);

	CStageKey*	pStageKey = pKeyCardObj->AddComponent<CStageKey>("StageKey");

	pKeyCardTr = pKeyCardObj->GetTransform();

	pKeyCardTr->SetWorldPos(316.f, 10.f, 748.f);

	Vector3 vStageKeyPos = pKeyCardTr->GetWorldPos();
	pStageKey->SetOutLinePos(vStageKeyPos);

	SAFE_RELEASE(pKeyCardTr);
	SAFE_RELEASE(pStageKey);
	SAFE_RELEASE(pKeyCardObj);*/

	CGameObject*	pEventObj = CGameObject::CreateObject("EventCollider", pDefaultLayer);

	CEventCollider*	pEvent = pEventObj->AddComponent<CEventCollider>("EventCollider");

	pEvent->SetTutorial();

	SAFE_RELEASE(pEvent);
	SAFE_RELEASE(pEventObj);

	// MedicalKit
	//CGameObject* pObjMedicalKit = CGameObject::CreateObject("HealingPack", pDefaultLayer);

	//CTransform* pMedicalKitTr = pObjMedicalKit->GetTransform();
	//pMedicalKitTr->SetWorldPos(356.f, 20.f, 650.f);

	//CHealingPack* pUCHealingPack = pObjMedicalKit->AddComponent<CHealingPack>("HealingPack");
	////pUCHealingPack->SetHPAmount(pPlayer->GetMaxHP());
	//pUCHealingPack->SetMesh("MedicalKit", TEXT("MedicalKit.msh"));
	//pMedicalKitTr->SetWorldScale(10.f);

	//pUCHealingPack->SetOutLineScale(25.f, 25.f, 25.f);
	//Vector3 vMKPos = pMedicalKitTr->GetWorldPos();
	//pUCHealingPack->SetOutLinePos(vMKPos);

	//SAFE_RELEASE(pUCHealingPack);
	//SAFE_RELEASE(pMedicalKitTr);
	//SAFE_RELEASE(pObjMedicalKit);

	//// Lunch Box
	//CGameObject* pObjLunchBox = CGameObject::CreateObject("LunchBox", pDefaultLayer);

	//CTransform* pLuncBoxTr = pObjLunchBox->GetTransform();
	//pLuncBoxTr->SetWorldPos(400.f, 20.f, 630.f);
	//pLuncBoxTr->SetWorldScale(20.f);

	//pUCHealingPack = pObjLunchBox->AddComponent<CHealingPack>("UC_HealingPack");
	//pUCHealingPack->SetHPAmount(1);
	//pUCHealingPack->SetMesh("LunchBox", TEXT("LunchBox.msh"));

	//pUCHealingPack->SetOutLineScale(40.f, 40.f, 40.f);
	//Vector3 vLunchPos = pLuncBoxTr->GetWorldPos();
	//pUCHealingPack->SetOutLinePos(vLunchPos);

	//SAFE_RELEASE(pUCHealingPack);
	//SAFE_RELEASE(pLuncBoxTr);
	//SAFE_RELEASE(pObjLunchBox);

	//// Cigarette
	CGameObject* pObjCigarette = CGameObject::CreateObject("Cigarette", pDefaultLayer);

	CTransform* pCigaretteTr = pObjCigarette->GetTransform();
	pCigaretteTr->SetWorldPos(300.f, 20.f, 620.f);

	CCigarette* pUCCigarette = pObjCigarette->AddComponent<CCigarette>("UC_Cigarette");

	Vector3 pCigaPos = pCigaretteTr->GetWorldPos();
	pUCCigarette->SetOutLinePos(pCigaPos);

	SAFE_RELEASE(pUCCigarette);
	SAFE_RELEASE(pCigaretteTr);
	SAFE_RELEASE(pObjCigarette);

	//// Tablet
	//CGameObject* pObjTablet = CGameObject::CreateObject("Tablet", pDefaultLayer);

	//CTransform* pTabletTr = pObjTablet->GetTransform();
	//pTabletTr->SetWorldPos(300.f, 20.f, 600.f);

	//CTablet* pUCTablet = pObjTablet->AddComponent<CTablet>("UC_Tablet");

	//Vector3 pTabletPos = pTabletTr->GetWorldPos();
	//pUCTablet->SetOutLinePos(pTabletPos);

	//SAFE_RELEASE(pUCTablet);
	//SAFE_RELEASE(pTabletTr);
	//SAFE_RELEASE(pObjTablet);		
	//
	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(108.f, 17.f, 619.f);

	Vector3 pBatPos = pBatteryTr->GetWorldPos();
	pBattery->SetOutLinePos(pBatPos);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);

	//CGameObject*	pPaperObj = CGameObject::CreateObject("Paper", pDefaultLayer);

	//CPaperGenerator*	pPaper = pPaperObj->AddComponent<CPaperGenerator>("Paper");

	//CTransform*	pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(300.f, 20.f, 640.f);

	//Vector3 pPaperPos = pPaperTr->GetWorldPos();
	//pPaper->SetOutLinePos(pPaperPos);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(pPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper2", pDefaultLayer);

	//CPaperGongji*	pgPaper = pPaperObj->AddComponent<CPaperGongji>("Paper2");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(300.f, 20.f, 670.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//pgPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(pgPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper3", pDefaultLayer);

	//CPaperMsgTH*	pmPaper = pPaperObj->AddComponent<CPaperMsgTH>("Paper3");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(320.f, 15.f, 680.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//pmPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(pmPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper4", pDefaultLayer);

	//CPaperPlanA*	paPaper = pPaperObj->AddComponent<CPaperPlanA>("Paper4");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(320.f, 15.f, 690.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//paPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(paPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper5", pDefaultLayer);

	//CPaperTest*	tmPaper = pPaperObj->AddComponent<CPaperTest>("Paper5");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(330.f, 15.f, 680.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//tmPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(tmPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper51", pDefaultLayer);

	//CPaperBQ*	tmaPaper = pPaperObj->AddComponent<CPaperBQ>("Paper51");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(340.f, 15.f, 680.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//tmaPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(tmaPaper);
	//SAFE_RELEASE(pPaperObj);

	//pPaperObj = CGameObject::CreateObject("Paper52", pDefaultLayer);

	//CPaperPL*	tmiPaper = pPaperObj->AddComponent<CPaperPL>("Paper52");

	//pPaperTr = pPaperObj->GetTransform();

	//pPaperTr->SetWorldPos(350.f, 15.f, 680.f);

	//pPaperPos = pPaperTr->GetWorldPos();
	//tmiPaper->SetOutLinePos(pPaperPos.x, pPaperPos.y, pPaperPos.z);

	//SAFE_RELEASE(pPaperTr);
	//SAFE_RELEASE(tmiPaper);
	//SAFE_RELEASE(pPaperObj);	

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);

	return true;
}

int CTutorialScene::Update(float fTime)
{
	if (GET_SINGLE(CSceneManager)->GetChange())
	{
		m_fFade += 0.1f * fTime;
		GET_SINGLE(CRenderManager)->SetFadeAmount(m_fFade, fTime);
	}

	if (m_fFade > 1.f)
	{
		m_fFade = 1.f;
		GET_SINGLE(CSceneManager)->SetChange(false);
	}

	/*m_fAdaptationTime += fTime;

	if (m_fAdaptationTime <= 1.f)
	{
		GET_SINGLE(CRenderManager)->SetAdaptValue(0.5f, fTime, true);
	}

	if (m_fAdaptationTime > 1.f && m_fAdaptationTime <= 4.f)
	{
		GET_SINGLE(CRenderManager)->SetAdaptValue(3.f, fTime, true);
	}

	if (m_fAdaptationTime > 3.f)
		m_fAdaptationTime = 0.f;*/

	return 0;
}

int CTutorialScene::LateUpdate(float fTime)
{
	GET_SINGLE(CGameManager)->Update(fTime);

	return 0;
}
