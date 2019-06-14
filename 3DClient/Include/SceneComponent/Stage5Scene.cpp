#include "../ClientHeader.h"
#include "Stage5Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST_Default.h"

#include "../GameManager.h"

#include <Component/LandScape.h>


CStage5Scene::CStage5Scene()
{
	m_fFade = 0.001f;
	m_isTemp = false;

}

CStage5Scene::~CStage5Scene()
{
}

bool CStage5Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Stage5.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* newLand = CGameObject::CreateObject("Stage1Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage1Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage5.nav";
	Land->LoadLandScape(Temp);

	SAFE_RELEASE(newLand);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(pDefaultLayer);
	return true;
}

void CStage5Scene::AfterInit()
{
	////GET_SINGLE(CGameManager)->PlayerSpon(Vector3(120.0f, 0.0f, 177.5f), Vector3::Zero);
	//CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S4");

	//pDoor->SetDoorType(DOOR_STAGE);
	//pDoor->SetTargetDoor("Stage4", "ChangeStageDoor");
	//pDoor->SetLeftRight(true);

	//pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S2_1");

	//pDoor->SetDoorType(DOOR_STAGE);
	//pDoor->SetTargetDoor("Stage2", "Door_S2_S1_1");
	//pDoor->SetLeftRight(true);

	//m_Default->SetTarget(_PLAYER->GetGameObjectNonCount());
	//_PLAYER->GetTransformNonCount()->SetWorldPos(20.0f, 0.0f, 150.0f);
}

int CStage5Scene::Input(float fTime)
{
	return 0;
}

int CStage5Scene::Update(float fTime)
{
	if (GET_SINGLE(CSceneManager)->GetChange())
	{
		static bool bAdd = true;

		if (bAdd)
		{
			bAdd = false;
			GET_SINGLE(CGameManager)->AddUILayer();
			GET_SINGLE(CGameManager)->SetPlayerNaviY(true);
		}

		m_fFade += 0.1f * fTime;
		GET_SINGLE(CRenderManager)->SetFadeAmount(m_fFade, fTime);

	}

	if (m_fFade > 1.f)
	{
		m_fFade = 1.f;
		GET_SINGLE(CSceneManager)->SetChange(false);
	}

	GET_SINGLE(CGameManager)->Update(fTime);

	return 0;
}

void CStage5Scene::Render(float fTime)
{

}