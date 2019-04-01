#include "../ClientHeader.h"
#include "StartScene.h"
#include "Device.h"
#include "GameObject.h"
#include "MainScene.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Camera.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/UIButton.h"
#include "Component/Transform.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderPixel.h"


CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	CCamera*	pCamera = m_pScene->GetMainCamera();

	pCamera->SetCameraType(CT_ORTHO);
	pCamera->SetNear(0.f);

	CLayer*	pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");
	CLayer* pStageLayer = m_pScene->FindLayer("Stage");

	CGameObject*	pBackObj = CGameObject::CreateObject("BackGround", pStageLayer);

	CRenderer*	pRenderer = pBackObj->AddComponent<CRenderer>("BackRenderer");

	pRenderer->SetMesh("TexRect");

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = pBackObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "StartScene", TEXT("StartScene/StartScene.png"));

	SAFE_RELEASE(pMaterial);
	
	CTransform*	pTransform = pBackObj->GetTransform();

	pTransform->SetWorldScale(1280, 720, 1.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pBackObj);

	CGameObject*	pStartBtnObj = CGameObject::CreateObject("StartButton",
		pUILayer);

	CTransform*	pBtnTransform = pStartBtnObj->GetTransform();

	pBtnTransform->SetWorldPos(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight / 2.f - 150.f,
		0.f);

	SAFE_RELEASE(pBtnTransform);

	CUIButton*	pButton = pStartBtnObj->AddComponent<CUIButton>("StartButton");

	pButton->SetCallback(&CStartScene::StartBtnCallback, this);

	SAFE_RELEASE(pButton);

	CMaterial*	pBtnMaterial = pStartBtnObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pBtnMaterial->SetDiffuseTex(0, "StartButton", TEXT("StartScene/Start.png"));

	SAFE_RELEASE(pBtnMaterial);

	SAFE_RELEASE(pStartBtnObj);		

	SAFE_RELEASE(pStageLayer);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);

	SAFE_RELEASE(pCamera);

	return true;
}

int CStartScene::Update(float fTime)
{
	return 0;
}

void CStartScene::StartBtnCallback(float fTime)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->AddSceneComponent<CMainScene>("MainScene", false);
}
