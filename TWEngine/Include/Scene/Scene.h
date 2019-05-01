#pragma once
#include "../Ref.h"

PUN_BEGIN

class CLayer;
class CGameObject;
class CTransform;
class CCamera;
class CMaterial;
class CSceneComponent;
class CEditManager;
class CSceneManager;
class PUN_DLL CScene : public CRef
{
	friend class CEditManager;
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	list<CLayer*> m_LayerList;
	CSceneComponent* m_pSceneComponent;

	// 레이어
public:
	void AddLayer(const string& strTag, int iZOrder);
	void SortLayer();
	void EnableLayer(const string& strTag, bool bEnable);
	void ChangeLayerZOrder(const string& strTag, int iZOrder);
	void GetLayerTagList(vector<string>* _pVec);
	CLayer* FindLayer(const string& strTag);
	CGameObject* m_LandObject;

public:
	static bool SortLayerZOrder(const CLayer* pSrc, const  CLayer* pDest);

	// 오브젝트
public:
	CGameObject* FindObject(const string& strTag);
	CGameObject* FindObjectNonCount(const string& strTag);

	// 카메라
private:
	vector<float>* m_vecInput;
	vector<float>* m_vecUpdate;
	vector<float>* m_vecLateUpdate;
	vector<float>* m_vecCollsion;
	vector<float>* m_vecRender;
	FileStream* m_LogText;
	bool m_bHeader;

public:
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

	CGameObject*	GetSkyObject()						const;
	CGameObject*	GetSkyObjectNonCount()				const;

	CGameObject* CreateCamera(const string& strTag, const Vector3& vPos, CAMERA_TYPE eType,
		float fWidth, float fHeight, float fViewAngle, float fNear, float fFar);
	void ChangeCamera(const string& strTag);

private:
	CGameObject* FindCamera(const string& strTag);
	void ProfileInit();

	// 스카이 박스
private:
	CGameObject*	m_pSkyObj;
	CMaterial*		m_pSkyMtrl;

public:
	CGameObject*	GetSkyObj()			const;
	CMaterial*		GetSkyMaterial()	const;

public:
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
		m_pSceneComponent = pCom;
		return true;
	}

	void EnableSceneComponent(const string& strTag, bool bEnable);

public:
	void Save(string _fullPath);
	void Load(string _fullPath);
	void SetPickingColliderEnable(bool _bEnable);
	void SetColliderEnable(bool _bEnable);
public:
	void Start();
	bool Init();
	int  Input(float fTime);
	int  Update(float fTime);
	int  LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
};

PUN_END