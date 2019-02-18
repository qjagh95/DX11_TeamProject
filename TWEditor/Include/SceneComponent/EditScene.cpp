#include "stdafx.h"
#include "EditScene.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderPixel.h"
#include "Component/Material.h"
#include "PathManager.h"
#include "Component/Sound.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Core.h"
#include "../MainFrm.h"
#include "../EditorForm.h"
#include "Input.h"
#include "Component/Light.h"

CEditScene::CEditScene()
{
}

CEditScene::~CEditScene()
{
}

bool CEditScene::Init()
{	
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer*	pLayer = pScene->FindLayer("Default");

	CGameObject*	pLightObj = CGameObject::CreateObject("GlobalLight1",
		pLayer);

	CTransform* pTransform = pLightObj->GetTransform();

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

	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return true;
}

int CEditScene::Update(float fTime)
{
	CMainFrame*	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEditorForm*	pForm = pMainFrame->GetEditorForm();

	return 0;
}
