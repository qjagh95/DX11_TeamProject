#include "../ClientHeader.h"
#include "MainScene.h"

#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"

#include "../UserComponent/Player.h"

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
	SAFE_RELEASE(pTransform);

	//CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");

	//SAFE_RELEASE(pPlayer);

	SAFE_RELEASE(pObject);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);

	pTransform = pLandScapeObj->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	//pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
	//	TEXT("LandScape/Terrain_Cliff_11.dds"), "LandScape/height1_32.bmp");

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
		pDefaultLayer, true);

	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldRot(90.f, 0.f, 0.f);
	pTransform->SetWorldPos(0.f, 4.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLight*	pLight = pLightObj->AddComponent<CLight>("GlobalLight1");

	pLight->SetLightColor(Vector4::White, Vector4::White,
		Vector4::White);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(10.f);
	pLight->SetAngle(60.f, 90.f);

	SAFE_RELEASE(pLight);

	SAFE_RELEASE(pLightObj);

	pLightObj = CGameObject::CreateObject("GlobalLight2",
		pDefaultLayer, true);

	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldPos(-2.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	pLight = pLightObj->AddComponent<CLight>("GlobalLight2");

	pLight->SetLightColor(Vector4::White, Vector4::White,
		Vector4::White);
	pLight->SetLightType(LT_POINT);
	pLight->SetLightRange(10.f);

	SAFE_RELEASE(pLight);

	SAFE_RELEASE(pLightObj);

	SAFE_RELEASE(pCamera);

	pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);
	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldRot(-90.f, 0.f, 0.f);
	pTransform->SetWorldPos(0.f, -1.f, 0.f);

	SAFE_RELEASE(pTransform);

	pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(10.f);
	pLight->SetAngle(60.f, 90.f);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);


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
	GET_SINGLE(CNavigationManager)->ClearStage2D();
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CCraidenScene>("Craiden", false);
}
