#include "ClientHeader.h"
#include "GameManager.h"
#include "Input.h"
#include "Scene/Layer.h"
#include "BinaryWriter.h"
#include "CommonSoundLoader.h"
#include "UserComponent/Door.h"
#include "UserComponent/Tablet.h"
#include "UserComponent/PaperBQ.h"
#include "UserComponent/KeyCard.h"
#include "UserComponent/PaperPL.h"
#include "UserComponent/Battery.h"
#include "UserComponent/Cigarette.h"
#include "UserComponent/PaperTest.h"
#include "UserComponent/PaperMsgTH.h"
#include "UserComponent/PaperPlanA.h"
#include "UserComponent/HealingPack.h"
#include "UserComponent/PaperGongji.h"
#include "UserComponent/PaperSilent.h"
#include "UserComponent/Human_Player.h"
#include "UserComponent/NoticeMessage.h"
#include "UserComponent/PaperGenerator.h"
#include "UserComponent/Inventory.h"
#include "UserComponent/DocxInven.h"
#include "UserComponent/KeyInven.h"
#include "UserComponent/Bed.h"
#include "UserComponent/Locker.h"
#include "UserComponent/Parkour.h"
#include "Component/ColliderSphere.h"

DEFINITION_SINGLE(CGameManager)

// String Convert...
const string CGameManager::TCHARToString(const TCHAR* _ptsz)
{
	// TCHAR -> string 변환
	int len = (int)wcslen((wchar_t*)_ptsz);
	char* psz = new char[2 * len + 1];
	size_t Temp = 0;
	wcstombs_s(&Temp, psz, wcslen(_ptsz), (wchar_t*)_ptsz, 2 * len + 1);
	string cvtStr = psz;
	delete[] psz;
	return cvtStr;
}

const TCHAR* CGameManager::StringToTCHAR(const string& _str)
{
	const char* all = _str.c_str();
	int len = (int)(1 + strlen(all));
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();

	size_t Temp = 0;
	mbstowcs_s(&Temp, t, (size_t)len, all, len);
	return (TCHAR*)t;
}

CGameManager::CGameManager()
{
	m_pNotice = NULLPTR;
	m_pCamera = NULLPTR;
	m_pPlayer = NULLPTR;
	m_pPlayerTr = NULLPTR;
	m_pPlayerObj = NULLPTR;
	m_mapKey.clear();
}

CGameManager::~CGameManager()
{
	SAFE_RELEASE(m_pNotice);
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pPlayerTr);
	SAFE_RELEASE(m_pPlayerObj);

	unordered_map<CScene*, unordered_map<string, CDoor*>*>::iterator iterDoor;
	unordered_map<CScene*, unordered_map<string, CDoor*>*>::iterator iterDoorEnd = m_mapDoor.end();

	unordered_map<string, CDoor*>* mapDoor = nullptr;

	for (iterDoor = m_mapDoor.begin(); iterDoor != iterDoorEnd; ++iterDoor)
	{
		mapDoor = iterDoor->second;
		mapDoor->clear();
		SAFE_DELETE(mapDoor);
	}
	m_mapDoor.clear();

	unordered_map<CScene*, unordered_map<string, CGameObject*>*>::iterator iterItem;
	unordered_map<CScene*, unordered_map<string, CGameObject*>*>::iterator iterItemEnd = m_mapItemObj.end();

	unordered_map<string, CGameObject*>* mapItemObj = nullptr;

	for (iterItem = m_mapItemObj.begin(); iterItem != iterItemEnd; ++iterItem)
	{
		mapItemObj = iterItem->second;
		mapItemObj->clear();
		SAFE_DELETE(mapItemObj);
	}
	m_mapItemObj.clear();

	unordered_map<CScene*, unordered_map<string, CLight*>*>::iterator iterLight;
	unordered_map<CScene*, unordered_map<string, CLight*>*>::iterator iterLightEnd = m_mapLight.end();

	unordered_map<string, CLight*>* mapLightObj = nullptr;

	for (iterLight = m_mapLight.begin(); iterLight != iterLightEnd; ++iterLight)
	{
		mapLightObj = iterLight->second;
		mapLightObj->clear();
		SAFE_DELETE(mapLightObj);
	}
	m_mapLight.clear();

	for (int i = 0; i < SST_END; ++i)
	{
		m_vecLight[i].clear();
	}

}

bool CGameManager::PostInit()
{
	CCameraEff::GetInst()->Init();

	char* pEntry[3] = {};
	pEntry[ST_VERTEX] = (char*)"ParticleVS";
	pEntry[ST_PIXEL] = (char*)"ParticlePS_GreenMat";
	pEntry[ST_GEOMETRY] = (char*)"ParticleGS";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_GreenMat", TEXT("Particle.fx"), pEntry))
		return false;

	Vector3	vParticle;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_GreenMat", "Particle_GreenMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pEntry[ST_PIXEL] = (char*)"ParticlePS_BlueMat";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_BlueMat", TEXT("Particle.fx"), pEntry))
		return false;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_BlueMat", "Particle_BlueMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pEntry[ST_PIXEL] = (char*)"ParticlePS_BlackMat";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_BlackMat", TEXT("Particle.fx"), pEntry))
		return false;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_BlackMat", "Particle_BlackMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	CCommonSoundLoader::GetInst()->LoadSoundCSVList(TEXT("CommonSound.csv"));
	CCommonSoundLoader::GetInst()->LoadSoundRandomSeedCnt(TEXT("CommonSound_SoundCnt.csv"));



	return true;
}

bool CGameManager::Init()
{
	m_pCamera = GET_SINGLE(CSceneManager)->GetMainCamera();

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	if (!pLayer)
		return false;

	m_pPlayerObj = CGameObject::CreateObject("Player", pLayer, true);

	m_pPlayer = m_pPlayerObj->AddComponent<CHuman_Player>("Player");

	m_pPlayerTr = m_pPlayerObj->GetTransform();
	m_pPlayerTr->SetWorldRot(Vector3::Zero);
	//m_pPlayerTr->SetLocalRot(0.f, 180.f, 0.f);
	//m_pPlayerTr->SetWorldScale(0.0375f, 0.0375f, 0.0375f);
	////m_pPlayer->PlayerRot(Vector3(0.f, 180.f, 0.f));

	CGameObject*	pNoticeObj = CGameObject::CreateObject("NoticeMessage", pLayer, true);

	m_pNotice = pNoticeObj->AddComponent<CNoticeMessage>("NoticeMessage");

	m_pNotice->ChangeClip("Button_Empty");

	SAFE_RELEASE(pNoticeObj);

	AddToEachContainer();

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	return true;
}

bool CGameManager::AfterInit()
{
	return true;
}

void CGameManager::AddKey(const string & strKeyName, CGameObject* pObj)
{
	unordered_map<string, CGameObject*>::iterator iter = m_mapKey.find(strKeyName);

	if (iter == m_mapKey.end())
		m_mapKey.insert(make_pair(strKeyName, pObj));
}

void CGameManager::AddDoor(CScene * pScene, const string & strDoorObjTag, CDoor * pDoor)
{
	unordered_map<CScene*, unordered_map<string, CDoor*>*>::iterator iterMap = m_mapDoor.find(pScene);
	unordered_map<string, CDoor*>* mapInfo;

	if (iterMap == m_mapDoor.end())
	{
		mapInfo = new unordered_map<string, CDoor*>;
		m_mapDoor.insert(make_pair(pScene, mapInfo));
	}
	else
		mapInfo = iterMap->second;

	unordered_map<string, CDoor*>::iterator iter = mapInfo->find(strDoorObjTag);

	if (iter == mapInfo->end())
		mapInfo->insert(make_pair(strDoorObjTag, pDoor));
}

void CGameManager::AddDoor(const string & strSceneKey, const string & strDoorObjTag, CDoor * pDoor)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	AddDoor(pScene, strDoorObjTag, pDoor);
}

void CGameManager::AddItemObject(CGameObject* pObj, char* strName)
{
	CScene* pScene = pObj->GetScene();

	if (strstr(strName, "BATTERY") != nullptr)
	{
		CBattery* pBattery = pObj->AddComponent<CBattery>("Battery");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pBattery);
	}
	else if (strstr(strName, "CIGARETTE") != nullptr)
	{
		CCigarette* pCigarette = pObj->AddComponent<CCigarette>("Ciagrette");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pCigarette);
	}
	else if (strstr(strName, "HEALINGPACK") != nullptr)
	{
		CHealingPack* pHealingPack = pObj->AddComponent<CHealingPack>("HealingPack");
		pHealingPack->SetMesh("MedicalKit", TEXT("MedicalKit.msh"));
		pHealingPack->SetHPAmount(m_pPlayer->GetMaxHP());
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pHealingPack);
	}
	else if (strstr(strName, "LUNCHBOX") != nullptr)
	{
		CHealingPack* pHealingPack = pObj->AddComponent<CHealingPack>("LunchBox");
		pHealingPack->SetMesh("LunchBox", TEXT("LunchBox.msh"));
		pHealingPack->SetHPAmount(1);
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pHealingPack);
	}
	else if (strstr(strName, "TABLET") != nullptr)
	{
		CTablet* pTablet = pObj->AddComponent<CTablet>("Tablet");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pTablet);
	}
	else if (strstr(strName, "KEY") != nullptr)
	{
		CKeyCard* pKey = pObj->AddComponent<CKeyCard>("KeyCard");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pKey);
	}
	else if (strstr(strName, "PAPER_GENERATOR") != nullptr)
	{
		CPaperGenerator* pPG = pObj->AddComponent<CPaperGenerator>("PaperGenerator");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPG);
	}
	else if (strstr(strName, "GONGJI") != nullptr)
	{
		CPaperGongji* pPG = pObj->AddComponent<CPaperGongji>("PaperGongji");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPG);
	}
	else if (strstr(strName, "MSGTH") != nullptr)
	{
		CPaperMsgTH* pPM = pObj->AddComponent<CPaperMsgTH>("PaperMsgTH");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPM);
	}
	else if (strstr(strName, "PLANA") != nullptr)
	{
		CPaperPlanA* pPP = pObj->AddComponent<CPaperPlanA>("PaperPlanA");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPP);
	}
	else if (strstr(strName, "PAPERTEST") != nullptr)
	{
		CPaperTest* pPT = pObj->AddComponent<CPaperTest>("PaperTest");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPT);
	}
	else if (strstr(strName, "PAPERBQ") != nullptr)
	{
		CPaperBQ* pBQ = pObj->AddComponent<CPaperBQ>("PaperBQ");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pBQ);
	}
	else if (strstr(strName, "PAPERPL") != nullptr)
	{
		CPaperPL* pPL = pObj->AddComponent<CPaperPL>("PaperPL");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPL);
	}
	else if (strstr(strName, "SILENT") != nullptr)
	{
		CPaperSilent* pPS = pObj->AddComponent<CPaperSilent>("PaperSilent");
		AddItemObject(pScene, pObj->GetTag(), pObj);
		SAFE_RELEASE(pPS);
	}
}

void CGameManager::AddItemObject(CScene * pScene, const string & strItemObjTag, CGameObject* pObj)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>*>::iterator iterMap = m_mapItemObj.find(pScene);
	unordered_map<string, CGameObject*>* mapInfo = nullptr;

	if (iterMap == m_mapItemObj.end())
	{
		mapInfo = new unordered_map<string, CGameObject*>;
		m_mapItemObj.insert(make_pair(pScene, mapInfo));
	}
	else
		mapInfo = iterMap->second;

	unordered_map<string, CGameObject*>::iterator iter = mapInfo->find(strItemObjTag);

	if (iter == mapInfo->end())
		mapInfo->insert(make_pair(strItemObjTag, pObj));
}

void CGameManager::AddItemObject(const string & strSceneKey, const string & strItemObjTag, CGameObject * pObj)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	AddItemObject(pScene, strItemObjTag, pObj);
}

void CGameManager::AddLight(CLight * pLight)
{
	CScene* pScene = pLight->GetScene();
	CGameObject* pObj = pLight->GetGameObject();

	unordered_map<CScene*, unordered_map<string, CLight*>*>::iterator iterMap = m_mapLight.find(pScene);
	unordered_map<string, CLight*>* mapInfo = nullptr;

	if (iterMap == m_mapLight.end())
	{
		mapInfo = new unordered_map<string, CLight*>;
		m_mapLight.insert(make_pair(pScene, mapInfo));
	}
	else
		mapInfo = iterMap->second;

	unordered_map<string, CLight*>::iterator iter = mapInfo->find(pObj->GetTag());

	if (iter == mapInfo->end())
		mapInfo->insert(make_pair(pObj->GetTag(), pLight));

	//if (pLight->GetLightType() == LT_SPOT)
	//	m_vecLight[pObj->GetStageSection()].push_back(pLight);

	SAFE_RELEASE(pObj);
}

void CGameManager::AddToEachContainer()
{
	const unordered_map<string, CScene*>* mapScene = GET_SINGLE(CSceneManager)->GetSceneMap();

	unordered_map<string, CScene*>::const_iterator iter;
	unordered_map<string, CScene*>::const_iterator iterEnd = mapScene->end();

	CLayer* pDefaultLayer = nullptr;
	CLayer* pLightLayer = nullptr;

	list<CGameObject*>* pList = nullptr;
	list<CGameObject*>::const_iterator iterObj;
	list<CGameObject*>::const_iterator iterObjEnd;

	CDoor*					pDoor		= nullptr;
	CLight*					pLight		= nullptr;
	CGameObject*			pObj		= nullptr;
	CBed*					pBed		= nullptr;
	CLocker*				pLocker		= nullptr;
	CParkour*				pParkour	= nullptr;
	CColliderSphere*		pSphere		= nullptr;
	for (iter = mapScene->begin(); iter != iterEnd; ++iter)
	{
		//각 씬에서 디폴트 레이어와 라이트 레이어를 받아놓는다.
		pDefaultLayer = iter->second->FindLayer("Default");
		pLightLayer = iter->second->FindLayer("Light");

		//레이어의 오브젝트 리스트를 받아와서
		pList = pDefaultLayer->GetObjectList();
		iterObjEnd = pList->end();

		//오브젝트 리스트를 순회하며 이름을 확인하여 컴포넌트를 붙혀주며 각 컨테이너에 넣어준다.
		for (iterObj = pList->begin(); iterObj != iterObjEnd; ++iterObj)
		{
			string strTag = (*iterObj)->GetTag();
			char strName[256] = {};

			strcpy_s(strName, 256, strTag.c_str());

			_strupr_s(strName);

			if (strstr(strName, "DOOR") != nullptr)
			{
				if (strstr(strName, "FRAME") == nullptr)
				{
					pDoor = (*iterObj)->AddComponent<CDoor>("Door");
					AddDoor(iter->second, (*iterObj)->GetTag(), pDoor);

					if (strstr(strName, "LOCK") != nullptr)
					{
						if (strstr(strName, "FRONT") != nullptr)
							pDoor->LockIn(1);
						else if (strstr(strName, "BACK") != nullptr)
							pDoor->LockIn(-1);
					}
					else if (strstr(strName, "BROKEN") != nullptr || strstr(strName, "FAKE") != nullptr)
						pDoor->Break();

					SAFE_RELEASE(pDoor);
				}
			}

			else if (strstr(strName, "ITEM") != nullptr)
				AddItemObject((*iterObj), strName);

			else if (strstr(strName, "BED") != nullptr)
			{
				pBed = (*iterObj)->AddComponent<CBed>("Bed");
				SAFE_RELEASE(pBed);
			}
			else if (strstr(strName, "LOCKER") != nullptr)
			{
				pLocker = (*iterObj)->AddComponent<CLocker>("Locker");
				SAFE_RELEASE(pLocker);
			}
			else if (strstr(strName, "PARKOUR") != nullptr)
			{
				pParkour = (*iterObj)->AddComponent<CParkour>("Parkour");
				SAFE_RELEASE(pParkour);
			}
			else if (strstr(strName, "LEVER") != nullptr)
			{
				pSphere = (*iterObj)->AddComponent<CColliderSphere>("LeverBody");
				pSphere->SetInfo(Vector3::Zero, 2.0f);
				pSphere->SetCollisionCallback(CCT_STAY, this, &CGameManager::CallBack);

				SAFE_RELEASE(pSphere);
			}
			else if (strstr(strName, "GENERATOR") != nullptr)
			{
				CControlBase* pControl = (*iterObj)->AddComponent<CControlBase>("GeneratorControl");

				pControl->SetControlType(CONTROL_LIGHT);

				SAFE_RELEASE(pControl);
			}
			else if (strstr(strName, "ELECTRICALBOX") != nullptr)
			{
				CControlBase* pControl = (*iterObj)->AddComponent<CControlBase>("ElectricalControl");

				pControl->SetControlType(CONTROL_DOOR);
				pControl->SetKeyName("Lever");
				
				SAFE_RELEASE(pControl);
			}

			pLight = (*iterObj)->FindComponentFromType<CLight>(CT_LIGHT);

			if (pLight)
			{
				if (pLight->GetLightType() != LT_DIR)
					AddLight(pLight);

				SAFE_RELEASE(pLight);
			}
		}

		pList = pLightLayer->GetObjectList();
		iterObjEnd = pList->end();

		for (iterObj = pList->begin(); iterObj != iterObjEnd; ++iterObj)
		{
			pLight = (*iterObj)->FindComponentFromType<CLight>(CT_LIGHT);

			if (pLight)
			{
				if (pLight->GetLightType() != LT_DIR)
					AddLight(pLight);

				SAFE_RELEASE(pLight);
			}
		}

		SAFE_RELEASE(pDefaultLayer);
		SAFE_RELEASE(pLightLayer);
	}
}

void CGameManager::AddChangedListDoor(CDoor* pDoor)
{
	list<CDoor*>::iterator iter;
	list<CDoor*>::iterator iterEnd = m_ChangedDoorList.end();

	for (iter = m_ChangedDoorList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == pDoor)
			return;
	}

	m_ChangedDoorList.push_back(pDoor);
}

void CGameManager::AddChangedListItemObj(CGameObject * pObj)
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_ChangedItemObjList.end();

	for (iter = m_ChangedItemObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == pObj)
			return;
	}

	m_ChangedItemObjList.push_back(pObj);
}

void CGameManager::AddChangedListLight(CLight * pLight)
{
	list<CLight*>::iterator iter;
	list<CLight*>::iterator iterEnd = m_ChangedLightList.end();

	for (iter = m_ChangedLightList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == pLight)
			return;
	}

	m_ChangedLightList.push_back(pLight);
}

unordered_map<string, class CDoor*>* CGameManager::GetDoorMap(CScene * Scene)
{
	unordered_map<CScene*, unordered_map<string, class CDoor*>*>::iterator FindIter = m_mapDoor.find(Scene);

	if (FindIter == m_mapDoor.end())
		return NULLPTR;

	return FindIter->second;
}

unordered_map<string, class CLight*>* CGameManager::GetLightMap(CScene * Scene)
{
	unordered_map<CScene*, unordered_map<string, class CLight*>*>::iterator FindIter = m_mapLight.find(Scene);

	if (FindIter == m_mapLight.end())
		return NULLPTR;

	return FindIter->second;
}

unordered_map<string, class CGameObject*>* CGameManager::GetItemObjectMap(CScene * Scene)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>*>::iterator FindIter = m_mapItemObj.find(Scene);

	if (FindIter == m_mapItemObj.end())
		return NULLPTR;

	return FindIter->second;
}

bool CGameManager::FindKey(const string & strKeyName)
{
	unordered_map<string, CGameObject*>::iterator iter = m_mapKey.find(strKeyName);

	if (iter == m_mapKey.end())
		return false;

	return true;
}

CDoor* CGameManager::FindDoor(CScene * pScene, const string & strDoorKey)
{
	unordered_map<CScene*, unordered_map<string, CDoor*>*>::iterator iterMap = m_mapDoor.find(pScene);
	unordered_map<string, CDoor*>* mapInfo;

	if (iterMap == m_mapDoor.end())
		return nullptr;

	mapInfo = iterMap->second;

	unordered_map<string, CDoor*>::iterator iter = mapInfo->find(strDoorKey);

	if (iter == mapInfo->end())
		return nullptr;

	return iter->second;
}

CDoor * CGameManager::FindDoor(const string & strSceneKey, const string & strDoorKey)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	return FindDoor(pScene, strDoorKey);
}

CGameObject * CGameManager::FindItemObject(CScene * pScene, const string & strItemObjKey)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>*>::iterator iterMap = m_mapItemObj.find(pScene);
	unordered_map<string, CGameObject*>* mapInfo = nullptr;

	if (iterMap == m_mapItemObj.end())
		return nullptr;

	mapInfo = iterMap->second;

	unordered_map<string, CGameObject*>::iterator iter = mapInfo->find(strItemObjKey);

	if (iter == mapInfo->end())
		return nullptr;

	return iter->second;
}

CGameObject * CGameManager::FindItemObject(const string & strSceneKey, const string & strItemObjKey)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	return FindItemObject(pScene, strItemObjKey);
}

CLight * CGameManager::FindLight(CScene * pScene, const string & strLightObj)
{
	unordered_map<CScene*, unordered_map<string, CLight*>*>::iterator iterMap = m_mapLight.find(pScene);
	unordered_map<string, CLight*>* mapInfo = nullptr;

	if (iterMap == m_mapLight.end())
		return nullptr;

	mapInfo = iterMap->second;

	unordered_map<string, CLight*>::iterator iter = mapInfo->find(strLightObj);

	if (iter == mapInfo->end())
		return nullptr;

	return iter->second;
}

CLight * CGameManager::FindLight(const string & strSceneKey, const string & strLightObj)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	return FindLight(pScene, strLightObj);
}

CDoor * CGameManager::GetNearDoor(CScene * scene, Vector3 & Pos) const
{
	auto FindIter = m_mapDoor.find(scene);

	if(FindIter == m_mapDoor.end())
		return NULLPTR;

	auto getMap = FindIter->second;

	auto StartIter = getMap->begin();
	auto EndIter = getMap->end();

	CDoor* FindDoor = NULLPTR;
	FindDoor = StartIter->second;

	Matrix Temp = StartIter->second->GetTransformNonCount()->GetPosDelta();
	Vector3 getPos;
	getPos.x = Temp._41;
	getPos.z = Temp._43;
	
	getPos.y = 0.0f;

	float Dist = getPos.GetDistance(Pos);
	StartIter++;

	for (; StartIter != EndIter; StartIter++)
	{
		Temp = StartIter->second->GetTransformNonCount()->GetPosDelta();

		Vector3 DoorPos;
		DoorPos.x = Temp._41;
		DoorPos.z = Temp._43;

		float DoorDist = Pos.Distance(DoorPos);

		if (DoorDist <= Dist)
		{
			Dist = DoorDist;
			FindDoor = StartIter->second;
		}
	}

	return FindDoor;
}

CTransform* CGameManager::GetPlayerTr() const
{
	return m_pPlayerTr;
}

CHuman_Player * CGameManager::GetPlayer() const
{
	return m_pPlayer;
}

void CGameManager::SetPlayerNaviY(bool bEnable)
{
	m_pPlayer->SetNaviY(bEnable);
}

void CGameManager::PlayerSpon(const Vector3 & vPos, const Vector3 & vRot)
{
	m_pPlayerTr->SetWorldPos(vPos);
}

void CGameManager::ChangeNoticeClip(const string & strName)
{
	m_pNotice->ChangeClip(strName);
}

void CGameManager::SaveCheckPoint()
{
	/*
	1. 플레이어 위치 및 회전 정보
	2. 문이 열려있는지
	3. 월드 공간상의 아이템을 습득했는지
	4. 빛이 꺼졌는지
	5. 아이템의 개수
	6. 보스가 뒤지셨는지?
	*/
	// 파일 객체(ofstream) 생성

	string strFileName = "CheckPoint.cpd";
	BinaryWrite pInstBW(strFileName);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetSceneNonCount();
	
	string strSceneKey = pScene->GetTag();
	Vector3 vPos = m_pPlayerTr->GetWorldPos();
	Vector3 vRot = m_pPlayerTr->GetWorldRot();
	CGameObject* pObj = nullptr;

	//레이어 목록
	pInstBW.WriteData(vPos);
	pInstBW.WriteData(vRot);

	int iCount = (int)m_ChangedDoorList.size();
	pInstBW.WriteData(iCount);

	list<CDoor*>::iterator iterDoor;
	list<CDoor*>::iterator iterDoorEnd = m_ChangedDoorList.end();

	for (iterDoor = m_ChangedDoorList.begin(); iterDoor != iterDoorEnd; ++iterDoor)
	{
		pObj = (*iterDoor)->GetGameObject();

		string strSceneName = pObj->GetScene()->GetTag();
		string strName = pObj->GetTag();
		bool   bLock = (*iterDoor)->IsLock();

		pInstBW.WriteData(strSceneName);
		pInstBW.WriteData(strName);
		pInstBW.WriteData(bLock);

		SAFE_RELEASE(pObj);
	}

	iCount = (int)m_ChangedLightList.size();
	pInstBW.WriteData(iCount);

	list<CLight*>::iterator iterLight;
	list<CLight*>::iterator iterLightEnd = m_ChangedLightList.end();

	for (iterLight = m_ChangedLightList.begin(); iterLight != iterLightEnd; ++iterLight)
	{
		pObj = (*iterLight)->GetGameObject();

		string  strSceneName = pObj->GetScene()->GetTag();
		string  strName = pObj->GetTag();
		Vector4 vColor = (*iterLight)->GetLightColor();

		pInstBW.WriteData(strSceneName);
		pInstBW.WriteData(strName);
		pInstBW.WriteData(vColor);

		SAFE_RELEASE(pObj);
	}

	iCount = (int)m_ChangedItemObjList.size();
	pInstBW.WriteData(iCount);

	list<CGameObject*>::iterator iterItem;
	list<CGameObject*>::iterator iterItemEnd = m_ChangedItemObjList.end();

	for (iterItem = m_ChangedItemObjList.begin(); iterItem != iterItemEnd; ++iterItem)
	{
		string strSceneName = pObj->GetScene()->GetTag();
		string strName = (*iterItem)->GetTag();
		bool   bEnable = (*iterItem)->GetEnable();

		pInstBW.WriteData(strSceneName);
		pInstBW.WriteData(strName);
		pInstBW.WriteData(bEnable);
	}



	//list<CLayer*>::iterator iter;
	//list<CLayer*>::iterator iterEnd = m_LayerList.end();
	//for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	//{
	//	string strLayerTag = (*iter)->GetTag();
	//	instBW.WriteData(strLayerTag);
	//	// Layer Save 함수 호출
	//	(*iter)->Save(&instBW);
	//}
	//delete pInstBW;
}

void CGameManager::LoadCheckPoint()
{
	string strFileName = "CheckPoint.cpd";
	BinaryRead pInstBR(strFileName);

	string strSceneKey = pInstBR.ReadString();
	Vector3 vPos = pInstBR.ReadVector3();
	Vector3 vRot = pInstBR.ReadVector3();

	int iCount = pInstBR.ReadInt();

	CGameObject* pObj = nullptr;
	CDoor* pDoor = nullptr;
	CLight* pLight = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		string strSceneName = pInstBR.ReadString();
		string strName = pInstBR.ReadString();
		bool   bLock = pInstBR.ReadBool();

		pDoor = FindDoor(strSceneName, strName);
		if (!bLock)
			pDoor->UnLock();

		m_ChangedDoorList.push_back(pDoor);
	}

	iCount = pInstBR.ReadInt();

	for (int i = 0; i < iCount; ++i)
	{
		string  strSceneName = pInstBR.ReadString();
		string  strName = pInstBR.ReadString();
		Vector4 vColor = pInstBR.ReadVector4();

		pLight = FindLight(strSceneName, strName);
		pLight->SetLightColor(vColor, vColor, vColor);

		m_ChangedLightList.push_back(pLight);
	}

	iCount = pInstBR.ReadInt();

	for (int i = 0; i < iCount; ++i)
	{
		string strSceneName = pInstBR.ReadString();
		string strName = pInstBR.ReadString();
		bool   bEnable = pInstBR.ReadBool();

		pObj = FindItemObject(strSceneName, strName);
		pObj->SetEnable(bEnable);

		m_ChangedItemObjList.push_back(pObj);
	}

	GET_SINGLE(CSceneManager)->ChangeScene(strSceneKey);
	PlayerSpon(vPos, vRot);
	CTransform* pTr = m_pCamera->GetTransformNonCount();
	pTr->SetWorldRot(vRot);
}

void CGameManager::Update(float fTime)
{
	CalculateShadowLight();
}

void CGameManager::CalculateShadowLight()
{
	//int iSection = m_pPlayerObj->GetStageSection();

	//if (m_vecLight[iSection].empty())
	//{
	//	m_pCamera->NoShadow();
	//	return;
	//}

	CScene* pScene = GET_SINGLE(CSceneManager)->GetSceneNonCount();

	unordered_map<string, CLight*>* map = GetLightMap(pScene);

	unordered_map<string, CLight*>::const_iterator iter = map->begin();
	unordered_map<string, CLight*>::const_iterator iterEnd = map->end();

	CLight* pLight = iter->second;

	Vector3 vPlayerPos = m_pPlayerTr->GetWorldPos();
	Vector3 vLightPos = pLight->GetWorldPos();
	Vector3 vDist = vLightPos - vPlayerPos;
	float	fMinDist = vDist.Length();
	float	fDist;

	++iter;

	for (; iter != iterEnd; ++iter)
	{
		//if (!m_vecLight[iSection][i]->IsTurnOn())
		//	continue;

		vLightPos = iter->second->GetWorldPos();
		vDist = vLightPos - vPlayerPos;
		fDist = vDist.Length();

		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pLight = iter->second;
		}
	}

	m_pCamera->SetShadowVP(pLight->ComputeLightView());
}

void CGameManager::BlinkAllSceneLight(float fLimitTime, float fDeltaTime,
	const Vector4 & vColor, bool bFinalTurnOn)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	unordered_map<CScene*, unordered_map<string, CLight*>*>::iterator iterMap = m_mapLight.find(pScene);
	unordered_map<string, CLight*>* mapLight = iterMap->second;

	unordered_map<string, CLight*>::iterator iter;
	unordered_map<string, CLight*>::iterator iterEnd = mapLight->end();

	for (iter = mapLight->begin(); iter != iterEnd; ++iter)
	{
		if (iter->second->IsTurnOn() != bFinalTurnOn)
			AddChangedListLight(iter->second);

		iter->second->StartBlink(fLimitTime, fDeltaTime, vColor, bFinalTurnOn);
	}

	SAFE_RELEASE(pScene);
}

void CGameManager::BlinkAllSectionLight(STAGE_SECTION_TYPE eSectionType, float fLimitTime, float fDeltaTime, const Vector4 & vColor, bool bFinalTurnOn)
{
	if (!m_vecLight[eSectionType].empty())
	{
		for (int i = 0; i < m_vecLight[eSectionType].size(); ++i)
		{
			if (m_vecLight[eSectionType][i]->IsTurnOn() != bFinalTurnOn)
				AddChangedListLight(m_vecLight[eSectionType][i]);
			
			m_vecLight[eSectionType][i]->StartBlink(fLimitTime, fDeltaTime, vColor, bFinalTurnOn);
		}
	}
}

void CGameManager::BlinkLight(string strSceneKey, string strLightKey, float fLimitTime, float fDeltaTime, const Vector4 & vColor, bool bFinalTurnOn)
{
	CLight* pLight = FindLight(strSceneKey, strLightKey);

	if (pLight->IsTurnOn() != bFinalTurnOn)
		AddChangedListLight(pLight);

	pLight->StartBlink(fLimitTime, fDeltaTime, vColor, bFinalTurnOn);
}

void CGameManager::BlinkLight(CScene * pScene, string strLightKey, float fLimitTime, float fDeltaTime, const Vector4 & vColor, bool bFinalTurnOn)
{
	CLight* pLight = FindLight(pScene, strLightKey);

	if (pLight->IsTurnOn() != bFinalTurnOn)
		AddChangedListLight(pLight);

	pLight->StartBlink(fLimitTime, fDeltaTime, vColor, bFinalTurnOn);
}

CDoor * CGameManager::GetPlayerCollDoor(CScene* scene) const
{
	auto FindIter = m_mapDoor.find(scene);

	if (FindIter == m_mapDoor.end())
		return NULLPTR;

	auto StartIter = FindIter->second->begin();
	auto EndIter = FindIter->second->end();

	for (; StartIter != EndIter; StartIter++)
	{
		if (StartIter->second->GetIsPlayerColl() == true)
			return StartIter->second;
	}

	return NULLPTR;
}

void CGameManager::AddUILayer()
{
	CGameObject*	pInvenObj = CGameObject::FindObject("Inven");

	CInventory*	pInven = pInvenObj->FindComponentFromType<CInventory>((COMPONENT_TYPE)UT_INVENTORY);

	pInven->AddUILayer();

	SAFE_RELEASE(pInven);
	SAFE_RELEASE(pInvenObj);

	CGameObject*	pDocxObj = CGameObject::FindObject("DocxInven");

	CDocxInven*	pDocx = pDocxObj->FindComponentFromType<CDocxInven>((COMPONENT_TYPE)UT_DOCXINVEN);

	pDocx->AddUILayer();

	SAFE_RELEASE(pDocx);
	SAFE_RELEASE(pDocxObj);

	CGameObject*	pKeyInvenObj = CGameObject::FindObject("KeyInven");

	CKeyInven*	pKeyInven = pKeyInvenObj->FindComponentFromType<CKeyInven>((COMPONENT_TYPE)UT_KEYINVEN);

	pKeyInven->AddUILayer();

	SAFE_RELEASE(pKeyInven);
	SAFE_RELEASE(pKeyInvenObj);
}

void CGameManager::CallBack(CCollider* pSrc, CCollider* pDest, float fTime)
{
	int iID = pDest->GetColliderID();

	if (iID == UCI_PLAYER_RAY)
	{
		GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_F_Pickup");
		if (KEYDOWN("F"))
		{
			pSrc->GetGameObjectNonCount()->SetEnable(false);
			AddKey(pSrc->GetGameObjectNonCount()->GetTag(), pSrc->GetGameObjectNonCount());
		}
	}
}


string CGameManager::GetPlayerCollDoorMapKey(CScene * scene) const
{
	string Return = "";

	auto FindIter = m_mapDoor.find(scene);

	if (FindIter == m_mapDoor.end())
		return Return;

	auto StartIter = FindIter->second->begin();
	auto EndIter = FindIter->second->end();

	for (; StartIter != EndIter; StartIter++)
	{
		if (StartIter->second->GetIsPlayerColl() == true)
			return StartIter->first;
	}

	return Return;
}
