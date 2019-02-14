#pragma once
#include "../Ref.h"

PUN_BEGIN

class PUN_DLL CScene :
	public CRef
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	list<class CLayer*>	m_LayerList;
	list<class CSceneComponent*>	m_SceneComponentList;
	unordered_map<string, class CGameObject*>	m_mapCamera;
	class CGameObject*	m_pMainCameraObj;
	class CTransform*	m_pMainCameraTr;
	class CCamera*		m_pMainCamera;

	class CGameObject*	m_pUICameraObj;
	class CTransform*	m_pUICameraTr;
	class CCamera*		m_pUICamera;

	class CGameObject*	m_pSkyObj;
	class CMaterial*	m_pSkyMtrl;
public:
	class CGameObject* GetMainCameraObj()	const;
	class CGameObject * GetMainCameraObjNonCount() const;
	class CCamera* GetMainCamera()	const;
	class CCamera * GetMainCameraNonCount() const;
	class CTransform* GetMainCameraTransform()	const;
	class CTransform * GetMainCameraTransformNonCount() const;
	class CGameObject* GetUICameraObj()	const;
	class CGameObject * GetUICameraObjNonCount() const;
	class CCamera* GetUICamera()	const;
	class CCamera * GetUICameraNonCount() const;
	class CTransform* GetUICameraTransform()	const;
	class CTransform * GetUICameraTransformNonCount() const;
	class CGameObject* GetSkyObj()	const;
	class CMaterial* GetSkyMaterial()	const;

public:
	void Start();
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	void AddLayer(const string& strTag, int iZOrder);
	void ChangeLayerZOrder(const string& strTag, int iZOrder);
	class CLayer* FindLayer(const string& strTag);
	void SortLayer();
	void EnableLayer(const string& strTag, bool bEnable);
	class CGameObject* FindObject(const string & strTag);
	class CGameObject* FindObjectNonCount(const string & strTag);
	class CGameObject* CreateCamera(const string& strTag,
		const Vector3& vPos,
		CAMERA_TYPE eType, float fWidth, float fHeight,
		float fViewAngle, float fNear, float fFar);
	void ChangeCamera(const string& strTag);

private:
	class CGameObject* FindCamera(const string& strTag);
	void Debug();

public:
	static bool SortLayerZOrder(const class CLayer* pSrc, const class CLayer* pDest);

public:
	void EnableSceneComponent(const string& strTag, bool bEnable);

	template <typename T>
	bool AddSceneComponent(const string& strTag)
	{
		T*	pCom = new T;

		pCom->m_pScene = this;
		pCom->SetTag(strTag);

		if (!pCom->Init())
		{
			SAFE_RELEASE(pCom);
			return false;
		}

		m_SceneComponentList.push_back(pCom);

		return true;
	}
};

PUN_END
