#include "EngineHeader.h"
#include "EditorScene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Arm.h"
#include "Component/FreeCamera.h"
#include "EditManager.h"
#include "Rendering/RenderManager.h"
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
	pCameraTr->SetWorldPos(Vector3(0.f, 0.0f, -50.f));

	GET_SINGLE(CRenderManager)->SetHDRValue(0.22f, 5.5f, false);
	//////////////Camera Target Object///////////////////////
	//CGameObject* pObject = CGameObject::CreateObject("FreeCamObj", pDefaultLayer);
	//pObject->SetSave(false);
	//CEditManager::GetInst()->SetFreeCamObj(pObject);
	//CTransform* pTr = pObject->GetTransform();
	//pTr->SetWorldPos(0.f, 0.f, -20.f);

	CFreeCamera* FreeCamera = pCamera->AddComponent<CFreeCamera>("CameraArm");
	SAFE_RELEASE(FreeCamera);
	//SAFE_RELEASE(pTr);
	//SAFE_RELEASE(pObject);
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