#include "EngineHeader.h"
#include "EditorScene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Arm.h"
#include "Component/FreeCamera.h"
PUN_USING

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Init()
{
	// 레이어
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");

	// 메인 카메라
	CCamera* pCamera = m_pScene->GetMainCamera();
	CTransform* pCameraTr = pCamera->GetTransform();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	pCameraTr->SetWorldPos(Vector3(0.f, 0.0f, -50.f));

	//////////////Camera Target Object///////////////////////
	CGameObject* pObject = CGameObject::CreateObject("FreeCamObj", pDefaultLayer);

	CTransform* pTr = pObject->GetTransform();
	pTr->SetWorldPos(0.f, 200.f, 0.f);

	CFreeCamera* pFreeCamera = pObject->AddComponent<CFreeCamera>("FreeCam");

	SAFE_RELEASE(pFreeCamera);
	//pTr->SetWorldRotX(180.f);
	pCamera->SetTarget(pTr);
	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTr);

	SAFE_RELEASE(pArm);

	SAFE_RELEASE(pTr);


	SAFE_RELEASE(pObject);


	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);


	// TEST OBJECT 
	/*
	CGameObject* pObject = CGameObject::CreateObject("TEST OBJECT", pDefaultLayer);
	SAFE_RELEASE(pObject);
	*/

	// 레이어 해제
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

void EditorScene::CreatePrototype()
{
}

void EditorScene::Move(float fScale, float fTime)
{
}

int EditorScene::Update(float fTime)
{
	return 0;
}