#pragma once
#include "Scene/Scene.h"
#include "Component/Light.h"
#include "Component/Camera.h"

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

	CCamera*				m_pCamera;
	CGameObject*			m_pPlayerObj;
	CTransform*				m_pPlayerTr;
	class CHuman_Player*	m_pPlayer;
	class CNoticeMessage*	m_pNotice;

	unordered_map<CScene*, unordered_map<string, class CDoor*>*>	m_mapDoor;
	unordered_map<CScene*, unordered_map<string, CLight*>*>			m_mapLight;
	unordered_map<CScene*, unordered_map<string, CGameObject*>*>	m_mapItemObj;
	unordered_map<string, CGameObject*>								m_mapKey;
	vector<CLight*>													m_vecLight[SST_END];
	
	list<CGameObject*>												m_ChangedItemObjList;
	list<CDoor*>													m_ChangedDoorList;
	list<CLight*>													m_ChangedLightList;

// String Convert
public:
	const string TCHARToString(const TCHAR* _ptsz);
	const TCHAR* StringToTCHAR(const string& _str);

public:
	bool PostInit();
	bool Init();
	bool AfterInit();

	void AddKey(const string& strKeyName, CGameObject* pObj);
	void AddDoor(CScene* pScene, const string& strDoorObjTag, CDoor* pDoor);
	void AddDoor(const string& strSceneKey, const string& strDoorObjTag, CDoor* pDoor);
	void AddItemObject(CGameObject* pObj, char* strName);
	void AddItemObject(CScene* pScene, const string& strItemObjTag, CGameObject* pObj);
	void AddItemObject(const string& strSceneKey, const string& strItemObjTag, CGameObject* pObj);
	void AddLight(CLight* pLight);
	void AddToEachContainer();

	void AddChangedListDoor(CDoor* pDoor);
	void AddChangedListItemObj(CGameObject* pObj);
	void AddChangedListLight(CLight* pLight);

	unordered_map<string, class CDoor*>* GetDoorMap(CScene* Scene);
	unordered_map<string, class CLight*>* GetLightMap(CScene* Scene);
	unordered_map<string, class CGameObject*>* GetItemObjectMap(CScene* Scene);

	bool			FindKey(const string& strKeyName);
	CDoor*			FindDoor(CScene* pScene, const string& strDoorKey);
	CDoor*			FindDoor(const string& strSceneKey, const string& strDoorKey);
	CGameObject*	FindItemObject(CScene* pScene, const string& strItemObjKey);
	CGameObject*	FindItemObject(const string& strSceneKey, const string& strItemObjKey);
	CLight*			FindLight(CScene* pScene, const string& strLightObj);
	CLight*			FindLight(const string& strSceneKey, const string& strLightObj);
	CDoor*			GetNearDoor(CScene* scene, Vector3& Pos) const;
	string GetNearDoorString(CScene* scene, Vector3& Pos) const;

	CTransform*		GetPlayerTr()	const;
	CHuman_Player*	GetPlayer()		const;
	void PlayerSpon(const Vector3& vPos, const Vector3& vRot);
	void ChangeNoticeClip(const string& strName);

	void SaveCheckPoint();
	void LoadCheckPoint();

	void Update(float fTime);
	void CalculateShadowLight();

	void BlinkAllSceneLight(float fLimitTime, float fDeltaTime,
		const Vector4& vColor, bool bFinalTurnOn);

	void BlinkAllSectionLight(STAGE_SECTION_TYPE eSectionType, float fLimitTime, float fDeltaTime,
		const Vector4& vColor, bool bFinalTurnOn);

	void BlinkLight(string strSceneKey, string strLightKey, float fLimitTime, float fDeltaTime,
		const Vector4& vColor, bool bFinalTurnOn);
	void BlinkLight(CScene* pScene, string strLightKey, float fLimitTime, float fDeltaTime,
		const Vector4& vColor, bool bFinalTurnOn);

	CDoor* GetPlayerCollDoor(CScene* scene) const;
	string GetPlayerCollDoorMapKey(CScene* scene) const;

	void AddUILayer();

	DECLARE_SINGLE(CGameManager)
};

