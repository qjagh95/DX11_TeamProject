#include "../ClientHeader.h"
#include "Stage2Scene.h"
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
#include "Component/ColliderSphere.h"
#include "../GameManager.h"

CStage2Scene::CStage2Scene()
{
	m_bStart = false;
}

CStage2Scene::~CStage2Scene()
{
}

bool CStage2Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"SC.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* newLand = CGameObject::CreateObject("Stage2Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage2Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage2Nav.nav";
	Land->LoadLandScape(Temp);

	SAFE_RELEASE(pDefaultLayer);

	return true;
}

void CStage2Scene::AfterInit()
{
	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S2_S1_1");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_S1_S2_1");

	pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S2_S1_2");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_S1_S2_2");


	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	CColliderSphere* pSphere = nullptr;

	list<CGameObject*>* pList = pLayer->GetObjectList();

	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pList->end();

	for (iter = pList->begin(); iter != iterEnd; ++iter)
	{
		//여기서 미리 생성한 두가지 버젼의 방 내용물을 수정한다.
		string strTag = (*iter)->GetTag();
		char strName[256] = {};

		strcpy_s(strName, 256, strTag.c_str());

		_strupr_s(strName);

		if (strstr(strTag.c_str(), "REAL") != nullptr)
			m_vecRealObj.push_back((*iter));
		else if (strstr(strTag.c_str(), "ILLUSION") != nullptr)
		{
			m_vecIllusionObj.push_back((*iter));
			(*iter)->SetEnable(false);
		}
		else if (strstr(strTag.c_str(), "EVENT") != nullptr)
		{
			CRenderer* pRD = (*iter)->FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRD)
				pRD->SetEnable(false);

			pSphere = (*iter)->AddComponent<CColliderSphere>("EventBody");

			if (strstr(strTag.c_str(), "ILLUSION") != nullptr)
			{
				pSphere->SetInfo(Vector3::Zero, 3.0f);
				pSphere->SetCollisionCallback(CCT_ENTER, this, &CStage2Scene::InteractIllusion);
			}
			else if (strstr(strTag.c_str(), "REAL") != nullptr)
			{
				pSphere->SetInfo(Vector3::Zero, 3.0f);
				pSphere->SetCollisionCallback(CCT_ENTER, this, &CStage2Scene::InteractReal);
			}
		}
	}
}

int CStage2Scene::Update(float fTime)
{
	//Air_Vent_Ent_Low_Loop

	if (m_bStart)
	{
		CSoundManager::GetInst()->PlayBgm("Air_Vent_Ent_Low_Loop");
		m_bStart = false;
	}

	return 0;
}

void CStage2Scene::InteractIllusion(CCollider * pSrc, CCollider * pDest, float fTime)
{
	int iID = pDest->GetColliderID();

	if (iID == UCI_PLAYER_HIT)
	{
		pSrc->GetGameObjectNonCount()->SetEnable(false);

		for (int i = 0; i < m_vecRealObj.size(); ++i)
			m_vecRealObj[i]->SetEnable(false);

		for (int i = 0; i < m_vecIllusionObj.size(); ++i)
			m_vecIllusionObj[i]->SetEnable(true);

		GET_SINGLE(CGameManager)->BlinkAllSceneLight(2.0f, 0.25, Vector4::Red, false);
	}
}

void CStage2Scene::InteractReal(CCollider * pSrc, CCollider * pDest, float fTime)
{
	int iID = pDest->GetColliderID();

	if (iID == UCI_PLAYER_HIT)
	{
		pSrc->GetGameObjectNonCount()->SetEnable(false);

		for (int i = 0; i < m_vecRealObj.size(); ++i)
			m_vecRealObj[i]->SetEnable(true);

		for (int i = 0; i < m_vecIllusionObj.size(); ++i)
			m_vecIllusionObj[i]->SetEnable(false);

		GET_SINGLE(CGameManager)->BlinkAllSceneLight(2.0f, 0.25, Vector4::Red, true);
	}
}






