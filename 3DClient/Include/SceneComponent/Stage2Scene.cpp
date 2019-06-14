#include "../ClientHeader.h"
#include "Stage2Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "Component/Gizmo.h"
#include "Component/ParkourTest.h"
#include <NavigationMesh.h>
#include "../UserComponent/Human_Player.h"
#include "Rendering/RenderManager.h"
#include "Component/FreeCamera.h"
#include "Component/ColliderSphere.h"
#include "../GameManager.h"

#define IS_LIGHT_TEST false;

CStage2Scene::CStage2Scene()
{
	m_bStart = true;
	m_bBGMChange = false;
	m_iIllusion = 0;
}

CStage2Scene::~CStage2Scene()
{
}

bool CStage2Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"SC_Test.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	//GET_SINGLE(CRenderManager)->SetHDRValue(0.f, 0.f, false);

	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* newLand = CGameObject::CreateObject("Stage2Navi", pDefaultLayer);
	CLandScape* Land = newLand->AddComponent< CLandScape>("Stage2Navi");
	string Temp = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Temp += "Stage2Nav.nav";
	Land->LoadLandScape(Temp);

	SAFE_RELEASE(pDefaultLayer);

	CInput::GetInst()->AddKey("Illusion", VK_END);

	// 전역광(Directional Light) 설정
	Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	Vector4 vTestColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 vDarkColor = Vector4(0.03f, 0.03f, 0.07f, 1.0f);

	CLayer* pLayer = m_pScene->FindLayer("Light");

	list<CGameObject*>* pLightList = pLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pLightList->end();
	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CLight* pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
#if IS_LIGHT_TEST
		pLight->SetLightColor(vTestColor, vTestColor, vTestColor);
#else
		pLight->SetLightColor(vDarkColor, vDarkColor, vDarkColor);
#endif
		SAFE_RELEASE(pLight);
	}
	SAFE_RELEASE(pLayer);

	CSoundManager::GetInst()->CreateSoundEffect("Illusion", L"music\\---SOLDIER_ETAT_02_(Intro_C)---.wav");
	CSoundManager::GetInst()->CreateSoundEffect("Real", L"enemies\\SFX_NANO_SCREAM_01.wav");
	CSoundManager::GetInst()->CreateSoundEffect("Ambient", L"ambient\\Air_Vent_Ent_Low_Loop.wav");
	CSoundManager::GetInst()->CreateSoundEffect("Stage2BGM", L"music\\13-Soldier AI ETAT 1  (MIX1-Trame).wav");

	return true;
}

void CStage2Scene::AfterInit()
{
	GET_SINGLE(CRenderManager)->SetSkyEnable(false);

	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S2_S1_1");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_S1_S2_1");

	pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S2_S1_2");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_S1_S2_2");
	
	pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "C0-3-1-1-Door0");
	pDoor->Lock(true, "Item_Key_Room");

	CLayer* pLayer = m_pScene->FindLayer("Default");

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

		if (strstr(strName, "REAL") != nullptr)
			m_vecRealObj.push_back((*iter));
		else if (strstr(strName, "ILLUSION") != nullptr)
		{
			m_vecIllusionObj.push_back((*iter));
			(*iter)->SetEnable(false);
		}
		else if (strstr(strName, "EVENT") != nullptr)
		{
			CRenderer* pRD = (*iter)->FindComponentFromType<CRenderer>(CT_RENDERER);

			if (pRD)
				pRD->SetEnable(false);

			pSphere = (*iter)->AddComponent<CColliderSphere>("EventBody");

			if (strstr(strName, "ILLUSION") != nullptr)
			{
				pSphere->SetInfo(Vector3::Zero, 3.0f);
				pSphere->SetCollisionCallback(CCT_ENTER, this, &CStage2Scene::InteractIllusion);
			}
			else if (strstr(strName, "REAL") != nullptr)
			{
				pSphere->SetInfo(Vector3::Zero, 3.0f);
				pSphere->SetCollisionCallback(CCT_ENTER, this, &CStage2Scene::InteractReal);
			}
		}
	}


	for (int i = 0; i < m_vecRealObj.size(); ++i)
		m_vecRealObj[i]->SetEnable(true);

	for (int i = 0; i < m_vecIllusionObj.size(); ++i)
		m_vecIllusionObj[i]->SetEnable(false);

	//GET_SINGLE(CGameManager)->BlinkAllSceneLight(2.0f, 0.25, Vector4::Red, false);

	CGameManager::GetInst()->GetPlayerTr()->SetWorldPos(150.0f, 0.0f, 95.0f);
}

int CStage2Scene::Update(float fTime)
{
	//Air_Vent_Ent_Low_Loop

	if (m_bStart)
	{
		CSoundManager::GetInst()->SetBgmVolume(0.5f);
		CSoundManager::GetInst()->PlayBgm("Ambient");
		m_bStart = false;
	}
	else
	{
		if (m_bBGMChange)
		{
			CSoundManager::GetInst()->SetBgmVolume(1.0f);
			CSoundManager::GetInst()->PlayBgm("Stage2BGM");
			m_bBGMChange = false;
		}
	}

	if (KEYDOWN("Illusion"))
	{
		if (m_iIllusion == 0)
		{
			for (int i = 0; i < m_vecRealObj.size(); ++i)
				m_vecRealObj[i]->SetEnable(false);

			for (int i = 0; i < m_vecIllusionObj.size(); ++i)
				m_vecIllusionObj[i]->SetEnable(true);

			GET_SINGLE(CGameManager)->BlinkAllSceneLight(0.5f, 0.01f, Vector4::Red, false);

			GET_SINGLE(CRenderManager)->EnableDistortion();

			CSoundManager::GetInst()->SoundPlay("Illusion");

			m_iIllusion++;

			m_bBGMChange = true;
		}
		else if (m_iIllusion == 1)
		{
			for (int i = 0; i < m_vecRealObj.size(); ++i)
				m_vecRealObj[i]->SetEnable(true);

			for (int i = 0; i < m_vecIllusionObj.size(); ++i)
				m_vecIllusionObj[i]->SetEnable(false);

			GET_SINGLE(CGameManager)->BlinkAllSceneLight(0.5f, 0.01f, Vector4::Red, true);

			GET_SINGLE(CRenderManager)->EnableDistortion();

			CSoundManager::GetInst()->SoundPlay("Real");

			m_iIllusion--;

			m_bStart = true;
		}
		
	}

	return 0;
}

void CStage2Scene::ChangeScene()
{
	GET_SINGLE(CRenderManager)->SetSkyEnable(false);
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

		GET_SINGLE(CGameManager)->BlinkAllSceneLight(2.5f, 0.33, Vector4::Red, false);
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






