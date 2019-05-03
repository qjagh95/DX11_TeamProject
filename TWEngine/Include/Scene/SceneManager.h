#pragma once
PUN_BEGIN

class CScene;
class CCamera;
class CGameObject;
class PUN_DLL CSceneManager
{
public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);

	class CScene* GetScene()	const;
	class CScene * GetSceneNonCount() const;
	CScene* FindScene(const string& KeyName);
	void ChangeScene(const string& KeyName);
	void DeleteScene(const string& SceneName);

	CGameObject*	GetMainCameraObj()					const;
	CGameObject*	GetMainCameraObjNonCount()			const;
	CCamera*		GetMainCamera()						const;
	CCamera *		GetMainCameraNonCount()				const;
	CTransform*		GetMainCameraTransform()			const;
	CTransform *	GetMainCameraTransformNonCount()	const;
	CGameObject*	GetUICameraObj()					const;
	CGameObject *	GetUICameraObjNonCount()			const;
	CCamera*		GetUICamera()						const;
	CCamera *		GetUICameraNonCount()				const;
	CTransform*		GetUICameraTransform()				const;
	CTransform *	GetUICameraTransformNonCount()		const;

	CGameObject* CreateCamera(const string& strTag, const Vector3& vPos, CAMERA_TYPE eType, float fWidth, float fHeight, float fViewAngle, float fNear, float fFar);
	void ChangeCamera(const string& strTag);
	CGameObject* FindCamera(const string& TagName);
	void AddLayer(const string& strTag, int iZOrder, bool bCurrent = true);
	void ChangeLayerZOrder(const string& strTag, int iZOrder, bool bCurrent = true);
	class CLayer* FindLayer(const string& strTag, bool bCurrent = true);
	CGameObject* FindObject(const string& TagName);
	void AfterInit();
	void Access();

	const unordered_map<string, CScene*>* GetSceneMap() { return &m_SceneMap; }

public:
	template <typename T>
	void AddScene(const string & SceneKeyName, const string& ComponentTag)
	{
		CScene* newScene = FindScene(SceneKeyName);

		if (newScene != NULLPTR)
			return;

		newScene = new CScene();
		newScene->Init();
		newScene->SetTag(SceneKeyName);
		newScene->AddSceneComponent<T>(ComponentTag);
		m_SceneMap.insert(make_pair(SceneKeyName, newScene));

		if(m_bStart == false)
			m_pCurScene = m_SceneMap.begin()->second;

		m_bStart = true;

		GET_SINGLE(CInput)->ChangeMouseScene(m_pCurScene);
		GET_SINGLE(CRenderManager)->SetSkyObject(m_pCurScene->GetSkyObjectNonCount());

		m_vecTemp.push_back(newScene);
	}

private:
	template <typename T>
	bool AddSceneComponent(const string& SceneKeyName, const string& ComponentTag)
	{
		CScene* getScene = FindScene(SceneKeyName);

		if (getScene == NULLPTR)
		{
			TrueAssert(true);
			return false;
		}

		return getScene->AddSceneComponent<T>(ComponentTag);
	}

private:
	CScene*	m_pCurScene;
	bool	m_bChange;
	bool m_bStart;

	CGameObject*	m_pMainCameraObj;
	CTransform*		m_pMainCameraTr;
	CCamera*		m_pMainCamera;

	CGameObject*	m_pUICameraObj;
	CTransform*		m_pUICameraTr;
	CCamera*		m_pUICamera;

	unordered_map<string, CScene*> m_SceneMap;
	unordered_map<string, CGameObject*>	m_mapCamera;
	vector<CScene*> m_vecTemp;

	DECLARE_SINGLE(CSceneManager)
};

PUN_END