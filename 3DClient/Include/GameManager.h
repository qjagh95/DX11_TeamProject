#pragma once
#include "Scene/Scene.h"
#include "Component/Light.h"

PUN_USING

typedef struct _tagKeyInfo
{
	string	strName;
	bool	bKey;
}DoorKeyInfo, *PDoorKeyInfo;

class CGameManager
{
	Vector3					m_vSponPos;
	Vector3					m_vSponRot;
	CGameObject*			m_pPlayerObj;
	CTransform*				m_pPlayerTr;
	class CHuman_Player*	m_pPlayer;

	unordered_map<CScene*, unordered_map<string, class CDoor*>*> m_mapDoor;
	unordered_map<CScene*, unordered_map<string, CLight*>>		m_mapLight;
	unordered_map<CScene*, unordered_map<string, CGameObject*>> m_mapItemObj;
	unordered_map<string, PDoorKeyInfo>							m_mapKey;
	vector<vector<CGameObject*>>								m_vecLight;

public:
	bool Init();
	void AddKey(const string& strKeyName);
	void AddDoor(CScene* pScene, const string& strDoorObjTag, CDoor* pDoor);
	void AddDoor(const string& strSceneKey, const string& strDoorObjTag, CDoor* pDoor);
	void AddItemObject(CScene* pScene, const string& strItemObjTag, CGameObject* pObj);
	void AddItemObject(const string& strSceneKey, const string& strItemObjTag, CGameObject* pObj);
	void AddLight(CScene* pScene, const string& strDoorObjTag, CLight* pLight);
	void AddLight(const string& strSceneKey, const string& strLightObjTag, CLight* pLight);

	bool			FindKey(const string& strKeyName);
	CDoor*			FindDoor(CScene* pScene, const string& strDoorKey);
	CDoor*			FindDoor(const string& strSceneKey, const string& strDoorKey);
	CLight*			FindLight(CScene* pScene, const string& strLightKey);
	CLight*			FindLight(const string& strSceneKey, const string& strLightKey);
	CGameObject*	FindItemObject(CScene* pScene, const string& strItemObjKey);
	CGameObject*	FindItemObject(const string& strSceneKey, const string& strItemObjKey);

	void PlayerSpon(const Vector3& vPos, const Vector3& vRot);

	void SaveCheckPoint(const Vector3& vPos, const Vector3& vRot);
	void LoadCheckPoint();

	DECLARE_SINGLE(CGameManager)
};