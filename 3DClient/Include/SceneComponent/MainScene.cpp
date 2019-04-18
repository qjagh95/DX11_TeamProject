#include "../ClientHeader.h"
#include "MainScene.h"
#include "Component/Arm.h"
#include "Component/Light.h"
#include "Component/Camera.h"
#include "Component/LandScape.h"
#include "../UserComponent/BlackCow.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CTransform*	pCameraTr = pCamera->GetTransform();

	pCameraTr->SetWorldPos(0.f, 3.f, -5.f);
	pCameraTr->SetWorldRot(20.f, 0.f, 0.f);

	SAFE_RELEASE(pCameraTr);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	CGameObject* pObject = CGameObject::CreateObject("Player", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	m_pTr = pTransform;

	pCamera->SetTarget(pTransform);


	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);

	SAFE_RELEASE(pTransform);

	BlackCow*	pPlayer = pObject->AddComponent<BlackCow>("Player");

	SAFE_RELEASE(pPlayer);

	SAFE_RELEASE(pObject);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);

	pTransform = pLandScapeObj->GetTransform();

	//pTransform->SetWorldScale(3.f, 1.f, 3.f);
	//pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
		TEXT("LandScape/Terrain_Cliff_11.dds"),
		TEXT("LandScape/Terrain_Cliff_11_NRM.bmp"),
		TEXT("LandScape/Terrain_Cliff_11_SPEC.bmp"),
		"LandScape/height1.bmp");

	vector<const TCHAR*>	vecSplatName;

	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));

	pLandScape->AddSplatDifTexture("LandScapeSplatDif",
		vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));

	pLandScape->AddSplatNrmTexture("LandScapeSplatNrm",
		vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));

	pLandScape->AddSplatSpcTexture("LandScapeSplatSpc",
		vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/RoadAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/StonAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/FlowerAlpha.bmp"));

	pLandScape->AddSplatAlphaTexture("LandScapeSplatAlpha",
		vecSplatName);

	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLandScapeObj);

	/*CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 1.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("ColorSphere");

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);*/

	CGameObject*	pLightObj = CGameObject::CreateObject("GlobalLight1",
		pDefaultLayer);

	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldRot(90.f, 0.f, 0.f);
	pTransform->SetWorldPos(0.f, 4.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLight*	pLight = pLightObj->AddComponent<CLight>("GlobalLight1");

	pLight->SetLightColor(Vector4::HotPink, Vector4::HotPink,
		Vector4::HotPink);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(10.f);
	pLight->SetAngle(60.f, 90.f);

	SAFE_RELEASE(pLight);

	SAFE_RELEASE(pLightObj);

	pLightObj = CGameObject::CreateObject("GlobalLight2",
		pDefaultLayer);

	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldPos(-2.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	pLight = pLightObj->AddComponent<CLight>("GlobalLight2");

	pLight->SetLightColor(Vector4::Blue, Vector4::Blue,
		Vector4::Blue);
	pLight->SetLightType(LT_POINT);
	pLight->SetLightRange(10.f);

	SAFE_RELEASE(pLight);

	SAFE_RELEASE(pLightObj);

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CMainScene::Update(float fTime)
{
	return 0;
}

void CMainScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CMainScene::CreatePrototype()
{
	
}

void CMainScene::NextScene(float fTime)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();
}
