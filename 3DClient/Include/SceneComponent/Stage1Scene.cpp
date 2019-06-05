#include "../ClientHeader.h"
#include "Stage1Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Door.h"
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
	wstr += L"Stage1.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");


	CGameObject* newLand = CGameObject::CreateObject("Stage1Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage1Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage1.nav";
	Land->LoadLandScape(Temp);

	SAFE_RELEASE(newLand);
	SAFE_RELEASE(Land);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

void CStage1Scene::AfterInit()
{
	//GET_SINGLE(CGameManager)->PlayerSpon(Vector3(120.0f, 0.0f, 177.5f), Vector3::Zero);
	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S4");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage4", "ChangeStageDoor");
	pDoor->SetLeftRight(true);

	pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S2_1");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage2", "Door_S2_S1_1");
	pDoor->SetLeftRight(true);

}

int CStage1Scene::Update(float fTime)
{
	if (GET_SINGLE(CSceneManager)->GetChange())
	{
		static bool bAdd = true;

		if (bAdd)
		{
			bAdd = false;
			GET_SINGLE(CGameManager)->AddUILayer();
		}

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
