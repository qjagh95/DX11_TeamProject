#pragma once
#include "Ref.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class PUN_DLL CGameObject :
	public CRef
{
private:
	CGameObject();
	CGameObject(const CGameObject& obj);
	~CGameObject();

private:
	static unordered_map<class CScene*, unordered_map<string, CGameObject*>>	m_mapPrototype;
	bool m_isDontDestroy;

public:
	bool GetDontDestroy()
	{
		return m_isDontDestroy;
	}

public:
	static CGameObject* CreateObject(const string& strTag = "GameObject",
		class CLayer* pLayer = nullptr, bool bDontDestroy = false);
	//static CGameObject* CreateDonDestroyObject(const string& strTag = "GameObject",
	//	class CLayer* pLayer = nullptr, bool bDontDestroy = false , bool _bImmediatelyUse = true);
	static CGameObject* CreatePrototype(const string& strTag,
		bool bCurrent = true);
	static CGameObject* CreateClone(const string& strTag,
		const string& strProto, class CLayer* pLayer = nullptr,
		bool bCurrent = true);
	static CGameObject* FindObject(const string& strTag);

	static void DestroyPrototype(class CScene* pScene);
	static void DestroyPrototype(class CScene* pScene, const string& strName);
	static void DestroyPrototype();

private:
	static CGameObject* FindPrototype(class CScene* pScene, const string& strName);

private:
	class CScene*	m_pScene;
	class CLayer*	m_pLayer;
	string			m_strLayerName;
	RENDER_GROUP	m_eRenderGroup;
	int				m_iLayerZOrder;
	int				m_iObjectListIdx;
	bool			m_isFrustumCull;

public:
	void SetObjectListIndex(int _idx)
	{
		m_iObjectListIdx = _idx;
	}
	int GetObjectListIndex() const
	{
		return m_iObjectListIdx;
	}
	class CScene* GetScene()	const;
	class CLayer* GetLayer()	const;
	string GetLayerName()	const;
	int GetLayerZOrder()	const;

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);
	void AfterClone();
	void DontDestroyOnLoad();

private:
	list<class CComponent*>	m_ComList;
	class CTransform*		m_pTransform;
	list<CComponent*>		m_FindList;
	CGameObject*			m_pParent;
	list<CGameObject*>		m_ChildList;


public:
	class CTransform* GetTransform()	const;
	class CTransform * GetTransformNonCount() const;
	class CTransform* GetTransformNoneCount() const { return m_pTransform; }
	const list<class CComponent*>* GetComponentList()	const;
	RENDER_GROUP	 GetRenderGroup() const;
	bool EmptyComponent()	const;
	bool FrustumCull();
	bool IsFrustumCull() const;

public:
	void Start();
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	CGameObject* Clone();

public:
	void AddChild(CGameObject* pChild);

public:
	class CComponent* AddComponent(class CComponent* pCom);
	const list<CComponent*>* FindComponentsFromTag(const string& strTag);
	const list<CComponent*>* FindComponentsFromTagNonCount(const string & strTag);
	const list<CComponent*>* FindComponentsFromType(COMPONENT_TYPE eType);
	const list<CComponent*>* FindComponentsFromTypeNonCount(COMPONENT_TYPE eType);
	bool CheckComponentFromType(COMPONENT_TYPE eType);
	void RemoveComponentFromType(COMPONENT_TYPE eType);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

public:
	template <typename T>
	T* FindComponentFromTag(const string& strTag)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return nullptr;
	}

	template <typename T>
	T* FindComponentFromTagNonCount(const string& strTag)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				return (T*)*iter;
			}
		}

		return nullptr;
	}


	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE eType)
	{
		typename list<class CComponent*>::iterator	iter;
		typename list<class CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetComponentType() == eType)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return nullptr;
	}

	template <typename T>
	T* FindComponentFromTypeNonCount(COMPONENT_TYPE eType)
	{
		typename list<class CComponent*>::iterator	iter;
		typename list<class CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetComponentType() == eType)
			{
				return (T*)*iter;
			}
		}

		return nullptr;
	}


	template <typename T>
	T* AddComponent(const string& strTag)
	{
		T*	pCom = new T;

		pCom->SetTag(strTag);
		pCom->m_pScene = m_pScene;
		pCom->m_pLayer = m_pLayer;
		pCom->m_pTransform = m_pTransform;
		pCom->m_pObject = this;

		if (!pCom->Init())
		{
			SAFE_RELEASE(pCom);
			return nullptr;
		}

		if (pCom->GetComponentType() == CT_UI)
			m_eRenderGroup = RG_UI;
		else if (pCom->GetComponentType() == CT_STAGE2D)
			m_eRenderGroup = RG_LANDSCAPE;
		else if (pCom->GetComponentType() == CT_LIGHT)
			m_eRenderGroup = RG_LIGHT;

		return (T*)AddComponent(pCom);
	}
};

PUN_END