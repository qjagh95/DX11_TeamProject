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
#include "Input.h"
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

	CLayer*	pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");
	CLayer* pStageLayer = m_pScene->FindLayer("Stage");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);	

	CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");

	SAFE_RELEASE(pPlayer);

	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);
	m_pTr = pTransform;

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(STANDARD_NORMAL_COLOR_SHADER);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("UI", pUILayer);
	CUIBar* pBar = pObject->AddComponent<CUIBar>("Bar");

	SAFE_RELEASE(pBar);
	SAFE_RELEASE(pObject);

	CreatePrototype();

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


	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pStageLayer);

	SAFE_RELEASE(pCamera);

	GET_SINGLE(CInput)->BindAxis("Move", this, &CMainScene::Move);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddKeyScale("Move", DIK_S, -1.f);

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
