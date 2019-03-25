#include "EngineHeader.h"
#include "EditorScene.h"
#include "Component/Camera.h"

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
	pCameraTr->SetWorldPos(Vector3(0.f, 0.0f, -200.0f));
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