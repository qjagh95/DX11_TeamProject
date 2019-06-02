#include "../ClientHeader.h"
#include "Stage1Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Field.h"
#include "Component/Gizmo.h"
#include "Component/ParkourTest.h"
#include <NavigationMesh.h>
#include "../UserComponent/Human_Player.h"
#include "Rendering/RenderManager.h"
#include "Component/FreeCamera.h"
#include "../GameManager.h"

CStage1Scene::CStage1Scene()
{
	m_fFade = 0.001f;
}

CStage1Scene::~CStage1Scene()
{
}

bool CStage1Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Stage1EditingTest_2.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");

	SAFE_RELEASE(pFreeCam);

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

	pObject = CGameObject::CreateObject("Field", pDefaultLayer);
	CField*	pField = pObject->AddComponent<CField>("Field");

	SAFE_RELEASE(pField);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("TestObject", pDefaultLayer);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldPos(1.f, 1.f, 1.f);
	//pCamera->SetTarget(pObject);

	CGameObject* newLand = CGameObject::CreateObject("Stage1Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage1Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage1.nav";
	Land->LoadLandScape(Temp);

	SAFE_RELEASE(newLand);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CStage1Scene::Update(float fTime)
{
	if (GET_SINGLE(CSceneManager)->GetChange())
	{
		m_fFade += 0.1f * fTime;
		GET_SINGLE(CRenderManager)->SetFadeAmount(m_fFade, fTime);

	}
	
	if (m_fFade > 1.f)
	{
		m_fFade = 1.f;
		GET_SINGLE(CSceneManager)->SetChange(false);
	}

	return 0;
}
