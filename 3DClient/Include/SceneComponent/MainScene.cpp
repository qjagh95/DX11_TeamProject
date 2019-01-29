#include "stdafx.h"
#include "MainScene.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/Material.h"
#include "Component/Stage2D.h"
#include "Component/UIBar.h"
#include "NavigationManager.h"
#include "Component/Sound.h"
#include "SoundManager.h"
#include "Component/Light.h"

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

	CLayer*	pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");
	CLayer* pStageLayer = m_pScene->FindLayer("Stage");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);	

	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 2.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(STANDARD_NORMAL_COLOR_SHADER);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pObject);

	CreatePrototype();

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pStageLayer);

	SAFE_RELEASE(pCamera);

	return true;
}

int CMainScene::Update(float fTime)
{
	return 0;
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
