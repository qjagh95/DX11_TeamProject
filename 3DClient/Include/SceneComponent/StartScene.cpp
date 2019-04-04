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
#include "Component/Material.h"
#include "FirTestScene.h"
#include "Core.h"
#include "TestScene.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	CCamera*	pCamera = m_pScene->GetMainCamera();

	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CLayer*	pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	CGameObject*	pStartBtnObj = CGameObject::CreateObject("StartButton",
		pUILayer);

	CTransform*	pTransform = pStartBtnObj->GetTransform();
	
	pTransform->SetWorldPos(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight / 2.f + 100.f,
		0.f);

	SAFE_RELEASE(pTransform);

	CUIButton*	pButton = pStartBtnObj->AddComponent<CUIButton>("StartButton");

	//pButton->SetStateColor(BS_CLICK, Vector4::Red);
	//pButton->SetStateColor(BS_MOUSEON, Vector4::White * 2.f);
	pButton->SetCallback(&CStartScene::StartBtnCallback, this);

	SAFE_RELEASE(pButton);

	SAFE_RELEASE(pStartBtnObj);

	////////////////////////////////////////////////////////////////////////////////
	CGameObject*	pExitBtnObj = CGameObject::CreateObject("ExitButton",
		pUILayer);

	pTransform = pExitBtnObj->GetTransform();

	pTransform->SetWorldPos(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight / 2.f - 100.f,
		0.f);

	SAFE_RELEASE(pTransform);

	pButton = pExitBtnObj->AddComponent<CUIButton>("ExitButton");

	//pButton->SetStateColor(BS_CLICK, Vector4::Red);
	//pButton->SetStateColor(BS_MOUSEON, Vector4::White * 2.f);
	pButton->SetCallback(&CStartScene::ExitBtnCallback, this);

	SAFE_RELEASE(pButton);

	CMaterial* pMaterial = pExitBtnObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "EndButton", TEXT("End.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pExitBtnObj);

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

	GET_SINGLE(CSceneManager)->AddSceneComponent<CTestScene>("TestScene", false);
}

void CStartScene::ExitBtnCallback(float fTime)
{
	// CCore::GetInst()->SetLoop(false);
	PostQuitMessage(0);
}
