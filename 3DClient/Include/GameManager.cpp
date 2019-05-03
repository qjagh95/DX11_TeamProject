#include "ClientHeader.h"
#include "GameManager.h"
#include "UserComponent/Door.h"
#include "UserComponent/Human_Player.h"
#include "Scene/Layer.h"

DEFINITION_SINGLE(CGameManager)

CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
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
}

bool CGameManager::Init()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer* pLayer = pScene->FindLayer("Default");
	
	if (!pLayer)
		return false;

	m_pPlayerObj = CGameObject::CreateObject("Player", pLayer, true);

	m_pPlayer = m_pPlayerObj->AddComponent<CHuman_Player>("Player");

	m_pPlayerTr = m_pPlayerObj->GetTransform();
	m_pPlayerTr->SetLocalRot(0.f, 180.f, 0.f);
	m_pPlayerTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pPlayerTr->SetWorldPos(316.f, 20.f, 748.f);

	CGameObject* pDoorObj = pScene->FindObject("StageDoor0");
	CDoor* pDoor = pDoorObj->FindComponentFromType<CDoor>((COMPONENT_TYPE)UT_DOOR);

	AddDoor(pScene->GetTag(), pDoorObj->GetTag(), pDoor);

	SAFE_RELEASE(pDoor);
	SAFE_RELEASE(pDoorObj);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	pScene = GET_SINGLE(CSceneManager)->FindScene("Second");

	pDoorObj = pScene->FindObject("StageDoor0");
	pDoor = pDoorObj->FindComponentFromType<CDoor>((COMPONENT_TYPE)UT_DOOR);

	AddDoor(pScene->GetTag(), pDoorObj->GetTag(), pDoor);

	SAFE_RELEASE(pDoor);
	SAFE_RELEASE(pDoorObj);

	return true;
}

void CGameManager::AddKey(const string & strKeyName)
{
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

void CGameManager::AddItemObject(CScene * pScene, const string & strItemObjTag, CGameObject* pObj)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>>::iterator iterMap = m_mapItemObj.find(pScene);
	unordered_map<string, CGameObject*> mapInfo;

	if (iterMap == m_mapItemObj.end())
		m_mapItemObj.insert(make_pair(pScene, mapInfo));
	else
		mapInfo = iterMap->second;

	unordered_map<string, CGameObject*>::iterator iter = mapInfo.find(strItemObjTag);

	if (iter == mapInfo.end())
		mapInfo.insert(make_pair(strItemObjTag, pObj));
}

void CGameManager::AddItemObject(const string & strSceneKey, const string & strItemObjTag, CGameObject * pObj)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	AddItemObject(pScene, strItemObjTag, pObj);
}

void CGameManager::AddLight(CScene * pScene, const string & strLightObjTag, CLight* pLight)
{
	unordered_map<CScene*, unordered_map<string, CLight*>>::iterator iterMap = m_mapLight.find(pScene);
	unordered_map<string, CLight*> mapInfo;

	if (iterMap == m_mapLight.end())
		m_mapLight.insert(make_pair(pScene, mapInfo));
	else
		mapInfo = iterMap->second;

	unordered_map<string, CLight*>::iterator iter = mapInfo.find(strLightObjTag);

	if (iter == mapInfo.end())
		mapInfo.insert(make_pair(strLightObjTag, pLight));
}

void CGameManager::AddLight(const string & strSceneKey, const string & strLightObjTag, CLight * pLight)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	AddLight(pScene, strLightObjTag, pLight);
}

bool CGameManager::FindKey(const string & strKeyName)
{
	unordered_map<string, PDoorKeyInfo>::iterator iter = m_mapKey.find(strKeyName);

	if (iter == m_mapKey.end())
		return false;

	PDoorKeyInfo pKeyInfo = iter->second;

	return pKeyInfo->bKey;
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

CLight * CGameManager::FindLight(CScene * pScene, const string & strLightKey)
{
	unordered_map<CScene*, unordered_map<string, CLight*>>::iterator iterMap = m_mapLight.find(pScene);
	unordered_map<string, CLight*> mapInfo;

	if (iterMap == m_mapLight.end())
		return nullptr;

	mapInfo = iterMap->second;

	unordered_map<string, CLight*>::iterator iter = mapInfo.find(strLightKey);

	if (iter == mapInfo.end())
		return nullptr;

	return iter->second;
}

CLight * CGameManager::FindLight(const string & strSceneKey, const string & strLightKey)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	return FindLight(pScene, strLightKey);
}

CGameObject * CGameManager::FindItemObject(CScene * pScene, const string & strItemObjKey)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>>::iterator iterMap = m_mapItemObj.find(pScene);
	unordered_map<string, CGameObject*> mapInfo;

	if (iterMap == m_mapItemObj.end())
		return nullptr;

	mapInfo = iterMap->second;

	unordered_map<string, CGameObject*>::iterator iter = mapInfo.find(strItemObjKey);

	if (iter == mapInfo.end())
		return nullptr;

	return iter->second;
}

CGameObject * CGameManager::FindItemObject(const string & strSceneKey, const string & strItemObjKey)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(strSceneKey);

	return FindItemObject(pScene, strItemObjKey);
}

void CGameManager::PlayerSpon(const Vector3 & vPos, const Vector3 & vRot)
{
	m_pPlayerTr->SetWorldPos(vPos);
}

void CGameManager::SaveCheckPoint(const Vector3 & vPos, const Vector3 & vRot)
{
}

void CGameManager::LoadCheckPoint()
{
}


