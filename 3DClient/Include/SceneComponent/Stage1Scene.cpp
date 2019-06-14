#include "../ClientHeader.h"
#include "Stage1Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST_Default.h"
#include "../UserComponent/EventCollider.h"
#include "../GameManager.h"
#include "../UserComponent/Minion.h"
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
	wstr += L"Stage1.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	//CGameObject* newLand = CGameObject::CreateObject("Stage1Navi", pDefaultLayer);
	//CLandScape* Land = newLand->AddComponent< CLandScape>("Stage1Navi");
	//string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	//Temp += "Stage1.nav";
	//Land->LoadLandScape(Temp);

	// Àü¿ª±¤(Directional Light) ¼³Á¤
	Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	Vector4 vTestColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 vDarkColor = Vector4(0.03f, 0.03f, 0.03f, 1.0f);
	CLayer* pLayer = m_pScene->FindLayer("Light");
	list<CGameObject*>* pLightList = pLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pLightList->end();
	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CLight* pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
#if _NDEBUG
		pLight->SetLightColor(vDarkColor, vDarkColor, vDarkColor);
#else
		pLight->SetLightColor(vDarkColor, vDarkColor, vDarkColor);
		//pLight->SetLightColor(vTestColor, vTestColor, vTestColor);
#endif
		SAFE_RELEASE(pLight);
	}
	SAFE_RELEASE(pLayer);

	//m_CollObject = CGameObject::CreateObject("EventColl", pDefaultLayer);
	//m_EventColl = m_CollObject->AddComponent<CColliderOBB3D>("EventColl");
	//m_EventColl->SetInfo(Vector3(0.1f, 0.0f, 0.0f), Vector3::Axis, Vector3(0.1f, 11.0f, 3.5f));
	//m_EventColl->GetTransformNonCount()->SetWorldPos(107.4f, 0.0f, 30.3f);
	//m_EventColl->SetCollisionCallback(CCT_ENTER, this, &CStage1Scene::EventCallback);

	//m_DefaultObject = CGameObject::CreateObject("Stage1Mob", pDefaultLayer);
	//m_Default = m_DefaultObject->AddComponent<ST_Default>("Stage1Mob");
	//m_DefaultObject->GetTransformNonCount()->SetWorldPos(100.0f, 0.0f, 10.0f);
	//m_DefaultObject->GetTransformNonCount()->SetWorldRotY(180.0f);

	//m_Default->AddPatrolPos(Vector3(100.0f, 0.0f, 45.f)); 
	//m_Default->AddPatrolPos(Vector3(100.0f, 0.0f, 177.0f));
	//m_Default->AddPatrolPos(Vector3(45.0f, 0.0f, 177.0f));
	//m_Default->AddPatrolPos(Vector3(70.0f, 0.0f, 177.0f));
	//m_Default->AddPatrolPos(Vector3(90.0f, 0.0f, 177.0f));	

	//CGameObject*	pEventObj = CGameObject::CreateObject("EventCollider", pDefaultLayer);

	//CEventCollider*	pEvent = pEventObj->AddComponent<CEventCollider>("EventCollider");

	//pEvent->NoticeCamera();

	//SAFE_RELEASE(pEvent);
	//SAFE_RELEASE(pEventObj);

	CSoundManager::GetInst()->CreateSoundEffect("ST1_TraceBGM", L"music\\10-AI NPC CHASE LOOP 4 (Percs_02).wav");

	//SAFE_RELEASE(newLand);
	//SAFE_RELEASE(Land);
	SAFE_RELEASE(pDefaultLayer);
	return true;
}

void CStage1Scene::AfterInit()
{
	

	GET_SINGLE(CGameManager)->PlayerSpon(Vector3(120.0f, 0.0f, 177.5f), Vector3::Zero);
	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S4");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage4", "Door_S4_S1");
	pDoor->SetLeftRight(true);

	pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S1_S2_1");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage2", "Door_S2_S1_1");
	pDoor->SetLeftRight(true);

	//m_Default->SetTarget(_PLAYER->GetGameObjectNonCount());
	//_PLAYER->GetTransformNonCount()->SetWorldPos(20.0f, 0.0f, 150.0f);

	/*CLayer*	pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject*	pMonsterObj = CGameObject::CreateObject("Minion", pDefaultLayer);

	CMinion3D*	pMinion = pMonsterObj->AddComponent<CMinion3D>("Minion");

	CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

	CTransform*	pPlayerTr = pPlayerObj->GetTransform();

	pMinion->SetTargetTransform(pPlayerTr);

	CTransform*	pMinionTr = pMonsterObj->GetTransform();

	pMinionTr->SetWorldPos(100.0f, 0.0f, 10.0f);
	pMinionTr->SetWorldScale(0.1f, 0.1f, 0.1f);

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayerObj);

	SAFE_RELEASE(pMinionTr);
	SAFE_RELEASE(pMinion);
	SAFE_RELEASE(pMonsterObj);

	SAFE_RELEASE(pDefaultLayer);*/
}

int CStage1Scene::Input(float fTime)
{
	//if (m_isEventColl == true)
	//{
	//	if (m_isTemp == false)
	//	{
	//		m_isTemp = true;
	//		m_CollObject->SetEnable(false);

	//		SAFE_RELEASE(m_CollObject);
	//		SAFE_RELEASE(m_EventColl);
	//	}
	//}
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

	//if (m_Default->GetState() == DS_USER_TRACE
	//	|| m_Default->GetState() == DS_JAP
	//	|| m_Default->GetState() == DS_HOOK
	//	|| m_Default->GetState() == DS_HEAD_ATTACK)
	//{
	//	if (m_isChangeBGM == false)
	//	{
	//		CSoundManager::GetInst()->PlayBgm("ST1_TraceBGM");
	//		m_isChangeBGM = true;
	//	}
	//}
	//else
	//{
	//	if (m_isChangeBGM = true)
	//	{
	//		CSoundManager::GetInst()->PlayBgm("AMB_Heater_loop");
	//		m_isChangeBGM = false;
	//	}
	//}

	GET_SINGLE(CGameManager)->Update(fTime);

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