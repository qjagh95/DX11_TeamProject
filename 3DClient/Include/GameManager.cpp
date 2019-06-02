#include "ClientHeader.h"
#include "GameManager.h"
#include "Input.h"
#include "Scene/Layer.h"
#include "UserComponent/Door.h"
#include "UserComponent/Human_Player.h"
#include "BinaryWriter.h"

DEFINITION_SINGLE(CGameManager)

// String Convert...
const string CGameManager::TCHARToString(const TCHAR* _ptsz)
{
	// TCHAR -> string 변환
	int len = (int)wcslen((wchar_t*)_ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)_ptsz, 2 * len + 1);
	string cvtStr = psz;
	delete[] psz;
	return cvtStr;
}

const TCHAR* CGameManager::StringToTCHAR(const string& _str)
{
	const char* all = _str.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
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
	m_pPlayerTr->SetLocalRot(0.f, 180.f, 0.f);
	m_pPlayerTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	//m_pPlayerTr->SetWorldPos(316.f, 20.f, 748.f);
	m_pPlayerTr->SetWorldPos(0.0f, 0.0f, 0.0f);

	AddToEachContainer();

	//GET_SINGLE(CInput)->AddKey("F9", VK_F9);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	return true;
}



bool CGameManager::AfterInit()
{
	return false;
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

	CDoor* pDoor = nullptr;
	CLight* pLight = nullptr;
	CGameObject* pObj = nullptr;

	for (iter = mapScene->begin(); iter != iterEnd; ++iter)
	{
		//각 씬에서 디폴트 레이어와 라이트 레이어를 받아놓는다.
		pDefaultLayer = iter->second->FindLayer("Default");
		pLightLayer = iter->second->FindLayer("Light");

		//레이어의 오브젝트 리스트를 받아와서
		pList = pDefaultLayer->GetObjectList();
		iterObjEnd = pList->end();
		
		//순회하며 각 컴포넌트가 있는 지 확인하여 각 컨테이너에 넣어준다.
		for (iterObj = pList->begin(); iterObj != iterObjEnd; ++iterObj)
		{
			pDoor = (*iterObj)->FindComponentFromType<CDoor>((COMPONENT_TYPE)UT_DOOR);
			
			if (pDoor)
			{
				AddDoor(iter->second, (*iterObj)->GetTag(), pDoor);
				SAFE_RELEASE(pDoor);
			}

			if (strstr((*iterObj)->GetTag().c_str(), "Item") != nullptr)
				AddItemObject(iter->second, (*iterObj)->GetTag(), (*iterObj));

			if (strstr((*iterObj)->GetTag().c_str(), "Key") != nullptr)
				AddKey((*iterObj)->GetTag());
		}
		
		pList = pLightLayer->GetObjectList();
		iterObjEnd = pList->end();

		for (iterObj = pList->begin(); iterObj != iterObjEnd; ++iterObj)
		{
			pLight = (*iterObj)->FindComponentFromType<CLight>(CT_LIGHT);

			if (pLight)
			{
				if(pLight->GetLightType() == LT_SPOT)
					AddLight(pLight);				

				SAFE_RELEASE(pLight);
			}
		}

		SAFE_RELEASE(pDefaultLayer);
		SAFE_RELEASE(pLightLayer);
	}



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

CTransform* CGameManager::GetPlayerTr() const
{
	return m_pPlayerTr;
}

CHuman_Player * CGameManager::GetPlayer() const
{
	return m_pPlayer;
}

void CGameManager::PlayerSpon(const Vector3 & vPos, const Vector3 & vRot)
{
	m_pPlayerTr->SetWorldPos(vPos);
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

	Vector3 vPos = m_pPlayerTr->GetWorldPos();
	Vector3 vRot = m_pPlayerTr->GetWorldRot();
	CGameObject* pObj = nullptr;

	//레이어 목록
	pInstBW.WriteData(vPos);
	pInstBW.WriteData(vRot);

	int iCount = m_ChangedDoorList.size();
	pInstBW.WriteData(iCount);

	list<CDoor*>::iterator iterDoor;
	list<CDoor*>::iterator iterDoorEnd = m_ChangedDoorList.end();

	for (iterDoor = m_ChangedDoorList.begin(); iterDoor != iterDoorEnd; ++iterDoor)
	{
		pObj = (*iterDoor)->GetGameObject();
		
		string strSceneName = pObj->GetScene()->GetTag();
		string strName		= pObj->GetTag();		
		bool   bLock		= (*iterDoor)->IsLock();

		pInstBW.WriteData(strSceneName);
		pInstBW.WriteData(strName);
		pInstBW.WriteData(bLock);

		SAFE_RELEASE(pObj);
	}

	iCount = m_ChangedLightList.size();
	pInstBW.WriteData(iCount);

	list<CLight*>::iterator iterLight;
	list<CLight*>::iterator iterLightEnd = m_ChangedLightList.end();

	for (iterLight = m_ChangedLightList.begin(); iterLight != iterLightEnd; ++iterLight)
	{
		pObj = (*iterLight)->GetGameObject();

		string  strSceneName = pObj->GetScene()->GetTag();
		string  strName		 = pObj->GetTag();
		Vector4 vColor		 = (*iterLight)->GetLightColor();

		pInstBW.WriteData(strSceneName);
		pInstBW.WriteData(strName);
		pInstBW.WriteData(vColor);

		SAFE_RELEASE(pObj);
	}

	iCount = m_ChangedItemObjList.size();
	pInstBW.WriteData(iCount);

	list<CGameObject*>::iterator iterItem;
	list<CGameObject*>::iterator iterItemEnd = m_ChangedItemObjList.end();

	for (iterItem = m_ChangedItemObjList.begin(); iterItem != iterItemEnd; ++iterItem)
	{
		string strSceneName = pObj->GetScene()->GetTag();
		string strName		= (*iterItem)->GetTag();
		bool   bEnable		= (*iterItem)->GetEnable();

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

	Vector3 vPos = pInstBR.ReadVector3();
	Vector3 vRot = pInstBR.ReadVector3();

	int iCount = pInstBR.ReadInt();

	CGameObject* pObj = nullptr;
	CDoor* pDoor = nullptr;
	CLight* pLight = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		string strSceneName = pInstBR.ReadString();
		string strName		= pInstBR.ReadString();
		bool   bLock		= pInstBR.ReadBool();

		pDoor = FindDoor(strSceneName, strName);
		if (!bLock)
			pDoor->UnLock();
		
		m_ChangedDoorList.push_back(pDoor);
	}

	iCount = pInstBR.ReadInt();

	for (int i = 0; i < iCount; ++i)
	{
		string  strSceneName = pInstBR.ReadString();
		string  strName		 = pInstBR.ReadString();
		Vector4 vColor		 = pInstBR.ReadVector4();

		pLight = FindLight(strSceneName, strName);
		pLight->SetLightColor(vColor, vColor, vColor);

		m_ChangedLightList.push_back(pLight);
	}

	iCount = pInstBR.ReadInt();

	for (int i = 0; i < iCount; ++i)
	{
		string strSceneName = pInstBR.ReadString();
		string strName		= pInstBR.ReadString();
		bool   bEnable		= pInstBR.ReadBool();

		pObj = FindItemObject(strSceneName, strName);
		pObj->SetEnable(bEnable);

		m_ChangedItemObjList.push_back(pObj);
	}


}

void CGameManager::Update(float fTime)
{
	if (KEYDOWN("F9"))
		SaveCheckPoint();
}

void CGameManager::CalculateShadowLight()
{
	int iSection = m_pPlayerObj->GetStageSection();
	if (m_vecLight[iSection].empty())
	{
		m_pCamera->NoShadow();
		return;
	}

	CLight* pLight = m_vecLight[iSection][0];
	Vector3 vPlayerPos = m_pPlayerTr->GetWorldPos();
	Vector3 vLightPos = pLight->GetWorldPos();
	Vector3 vDist = vLightPos - vPlayerPos;
	float	fMinDist = vDist.Length();
	float	fDist;
	
	for (int i = 1; i < m_vecLight[iSection].size(); ++i)
	{
		vLightPos = m_vecLight[iSection][i]->GetWorldPos();
		vDist = vLightPos - vPlayerPos;
		fDist = vDist.Length();

		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pLight = m_vecLight[iSection][i];
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

	for (iter = mapLight->begin();iter != iterEnd; ++iter)
		iter->second->StartBlink(fLimitTime, fDeltaTime, vColor, bFinalTurnOn);

	SAFE_RELEASE(pScene);
}

