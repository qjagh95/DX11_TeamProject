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
#include "../UserComponent/HealingPack.h"
#include "../UserComponent/Cigarette.h"
#include "../UserComponent/Tablet.h"
#include "../UserComponent/PaperSilent.h"

#include <NavigationMesh.h>

CTutorialScene::CTutorialScene()
{
}

CTutorialScene::~CTutorialScene()
{
}

bool CTutorialScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"TutorialHalf_Pivot.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	GET_SINGLE(CRenderManager)->SetHDRValue(0.22f, 5.7f);

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CTransform* pCameraTr = pCamera->GetTransform();

	/*CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");

	SAFE_RELEASE(pFreeCam);*/
	SAFE_RELEASE(pCameraTr);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	/*CGameObject*	pLandScapeObj = CGameObject::CreateObject("TutorialScape", pDefaultLayer);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("TutorialScape");

	wstring nav = CPathManager::GetInst()->FindPath(DATA_PATH);
	nav += L"HFTest.nav";
	string navfilePath = CW2A(nav.c_str());

	pLandScape->LoadLandScape(navfilePath);

	SAFE_RELEASE(pLandScape);
	SAFE_RELEASE(pLandScapeObj);*/	

	CGameObject*	pPlayerObj = CGameObject::CreateObject("Player", pDefaultLayer);

	CHuman_Player*	pPlayer = pPlayerObj->AddComponent<CHuman_Player>("Player");

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	pPlayerTr->SetLocalRot(0.f, 180.f, 0.f);
	pPlayerTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pPlayerTr->SetWorldPos(316.f, 20.f, 748.f);

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

	// MedicalKit
	CGameObject* pObjMedicalKit = CGameObject::CreateObject("MedicalKit", pDefaultLayer);
	CTransform* pMedicalKitTr = pObjMedicalKit->GetTransform();
	pMedicalKitTr->SetWorldPos(356.f, 20.f, 650.f);
	CHealingPack* pUCHealingPack = pObjMedicalKit->AddComponent<CHealingPack>("UC_HealingPack");
	//pUCHealingPack->SetHPAmount(pPlayer->GetMaxHP());
	pUCHealingPack->SetMesh("MedicalKit", TEXT("MedicalKit.msh"));
	SAFE_RELEASE(pUCHealingPack);
	SAFE_RELEASE(pMedicalKitTr);
	SAFE_RELEASE(pObjMedicalKit);

	// Lunch Box
	CGameObject* pObjLunchBox = CGameObject::CreateObject("LunchBox", pDefaultLayer);
	CTransform* pLuncBoxTr = pObjLunchBox->GetTransform();
	pLuncBoxTr->SetWorldPos(400.f, 20.f, 630.f);
	pUCHealingPack = pObjLunchBox->AddComponent<CHealingPack>("UC_HealingPack");
	pUCHealingPack->SetHPAmount(1);
	pUCHealingPack->SetMesh("LunchBox", TEXT("LunchBox.msh"));
	SAFE_RELEASE(pUCHealingPack);
	SAFE_RELEASE(pLuncBoxTr);
	SAFE_RELEASE(pObjLunchBox);

	// Cigarette
	CGameObject* pObjCigarette = CGameObject::CreateObject("Cigarette", pDefaultLayer);
	CTransform* pCigaretteTr = pObjCigarette->GetTransform();
	pCigaretteTr->SetWorldPos(300.f, 20.f, 620.f);
	CCigarette* pUCCigarette = pObjCigarette->AddComponent<CCigarette>("UC_Cigarette");
	SAFE_RELEASE(pUCCigarette);
	SAFE_RELEASE(pCigaretteTr);
	SAFE_RELEASE(pObjCigarette);

	// Tablet
	CGameObject* pObjTablet = CGameObject::CreateObject("Tablet", pDefaultLayer);
	CTransform* pTabletTr = pObjTablet->GetTransform();
	pTabletTr->SetWorldPos(300.f, 20.f, 600.f);
	CTablet* pUCTablet = pObjTablet->AddComponent<CTablet>("UC_Tablet");
	SAFE_RELEASE(pUCTablet);
	SAFE_RELEASE(pTabletTr);
	SAFE_RELEASE(pObjTablet);

	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(316.f, 20.f, 648.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);

	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(216.f, 20.f, 748.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);		

	CGameObject*	pPSObj = CGameObject::CreateObject("PaperSilent", pDefaultLayer);

	CPaperSilent*	pPS = pPSObj->AddComponent<CPaperSilent>("PaperSilent");

	CTransform*	pPSTr = pPSObj->GetTransform();

	pPSTr->SetWorldPos(116.f, 2.f, 648.f);

	SAFE_RELEASE(pPSTr);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pPSObj);

	/*pObject = CGameObject::CreateObject("GroundCap1ex", pDefaultLayer);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("GroundCap1Renderer");

	pRenderer->SetMesh("GroundCap1ex", L"GroundCap1ex.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(20.f, 10.f, 0.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);
*/
	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);

	return true;
}

int CTutorialScene::Update(float fTime)
{
	return 0;
}
