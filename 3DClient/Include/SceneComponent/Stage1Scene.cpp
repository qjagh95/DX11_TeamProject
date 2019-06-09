#include "../ClientHeader.h"
#include "Stage1Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST_Default.h"

#include "../GameManager.h"

#include <Component/LandScape.h>

bool CStage1Scene::m_isEventColl = false;

CStage1Scene::CStage1Scene()
{
	m_fFade = 0.001f;
	m_isTemp = false;
	m_isChangeBGM = true;
	m_CollObject = NULLPTR;
	m_EventColl = NULLPTR;
	
	m_DefaultObject = NULLPTR;
	m_Default = NULLPTR;
}

CStage1Scene::~CStage1Scene()
{
	SAFE_RELEASE(m_CollObject);
	SAFE_RELEASE(m_EventColl);

	SAFE_RELEASE(m_DefaultObject);
	SAFE_RELEASE(m_Default);
}

bool CStage1Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Stage1EditingTest_2.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* newLand = CGameObject::CreateObject("Stage1Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage1Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage1_Test.nav";
	Land->LoadLandScape(Temp);

	m_CollObject = CGameObject::CreateObject("EventColl", pDefaultLayer);
	m_EventColl = m_CollObject->AddComponent<CColliderOBB3D>("EventColl");
	m_EventColl->SetInfo(Vector3(0.1f, 0.0f, 0.0f), Vector3::Axis, Vector3(0.1f, 11.0f, 3.5f));
	m_EventColl->GetTransformNonCount()->SetWorldPos(107.4f, 0.0f, 30.3f);
	m_EventColl->SetCollisionCallback(CCT_ENTER, this, &CStage1Scene::EventCallback);

	m_DefaultObject = CGameObject::CreateObject("Stage1Mob", pDefaultLayer);
	m_Default = m_DefaultObject->AddComponent<ST_Default>("Stage1Mob");
	m_DefaultObject->GetTransformNonCount()->SetWorldPos(100.0f, 0.0f, 10.0f);
	m_DefaultObject->GetTransformNonCount()->SetWorldRotY(180.0f);

	m_Default->AddPatrolPos(Vector3(100.0f, 0.0f, 45.f));
	m_Default->AddPatrolPos(Vector3(100.0f, 0.0f, 177.0f));
	m_Default->AddPatrolPos(Vector3(45.0f, 0.0f, 177.0f));
	m_Default->AddPatrolPos(Vector3(70.0f, 0.0f, 177.0f));
	m_Default->AddPatrolPos(Vector3(90.0f, 0.0f, 177.0f));

	CSoundManager::GetInst()->CreateSoundEffect("ST1_TraceBGM", L"music\\10-AI NPC CHASE LOOP 4 (Percs_02).wav");

	SAFE_RELEASE(newLand);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(pDefaultLayer);
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

	m_Default->SetTarget(_PLAYER->GetGameObjectNonCount());
	_PLAYER->GetTransformNonCount()->SetWorldPos(20.0f, 0.0f, 150.0f);
}

int CStage1Scene::Input(float fTime)
{
	if (m_isEventColl == true)
	{
		if (m_isTemp == false)
		{
			m_isTemp = true;
			m_CollObject->SetEnable(false);

			SAFE_RELEASE(m_CollObject);
			SAFE_RELEASE(m_EventColl);
		}
	}
	return 0;
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

	if (m_Default->GetState() == DS_USER_TRACE
		|| m_Default->GetState() == DS_JAP
		|| m_Default->GetState() == DS_HOOK
		|| m_Default->GetState() == DS_HEAD_ATTACK)
	{
		if (m_isChangeBGM == false)
		{
			CSoundManager::GetInst()->PlayBgm("ST1_TraceBGM");
			m_isChangeBGM = true;
		}
	}
	else
	{
		if (m_isChangeBGM = true)
		{
			CSoundManager::GetInst()->PlayBgm("AMB_Heater_loop");
			m_isChangeBGM = false;
		}
	}

	return 0;
}

void CStage1Scene::Render(float fTime)
{

}

void CStage1Scene::EventCallback(CCollider * Src, CCollider * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "PlayerGeom")
		m_isEventColl = true;
}