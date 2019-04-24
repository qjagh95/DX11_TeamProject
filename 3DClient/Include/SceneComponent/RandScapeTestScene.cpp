#include "../ClientHeader.h"
#include "RandScapeTestScene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Field.h"
#include "Component/Gizmo.h"
#include "Component/ParkourTest.h"
CRandScapeTestScene::CRandScapeTestScene()
{
}

CRandScapeTestScene::~CRandScapeTestScene()
{
}

bool CRandScapeTestScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Field", pDefaultLayer, true);

	CField*	pField = pObject->AddComponent<CField>("Field");

	SAFE_RELEASE(pField);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("TestObject", pDefaultLayer, true);
	pTransform = pObject->GetTransform();

	//pTransform->SetWorldPos(0.f, 0.f, 0.f);
	pTransform->SetWorldPos(1.f, 1.f, 1.f);
	pCamera->SetTarget(pObject);

	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);
	CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pObject);

	//pObject = CGameObject::CreateObject("Minion", pDefaultLayer);

	//CMinion3D*	pMinion = pObject->AddComponent<CMinion3D>("Minion");

	//pMinion->SetTargetTransform(pTransform);

	//SAFE_RELEASE(pMinion);

	//SAFE_RELEASE(pObject);
	SAFE_RELEASE(pTransform);


	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	pLight->SetLightType(LT_DIR);
	pLight->SetLightDirection(Vector3(1.0f, -1.0f, 1.0f));

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);

	//CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);
	//pTransform = pLandScapeObj->GetTransform();

	//SAFE_RELEASE(pTransform);

	//CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");
	////pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif", NULLPTR, NULLPTR, NULLPTR, NULLPTR);
	//string strTemp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	//strTemp += "Nav.nav";
	//pLandScape->LoadLandScape(strTemp);
	//GET_SINGLE(CNavigationManager3D)->CreateNavMesh(m_pScene,
	//	"Nav.nav");


	//SAFE_RELEASE(pLandScape);
	//SAFE_RELEASE(pLandScapeObj);

	CGameObject* pParkourObj = CGameObject::CreateObject("ParkourTestObj", pDefaultLayer);

	CParkourTest* pParkourTest = pParkourObj->AddComponent<CParkourTest>("ParkourTestCom");

	SAFE_RELEASE(pParkourTest);
	SAFE_RELEASE(pParkourObj);
	
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CRandScapeTestScene::Update(float fTime)
{
	return 0;
}

void CRandScapeTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CRandScapeTestScene::CreatePrototype()
{
}

void CRandScapeTestScene::NextScene(float fTime)
{
}
