#pragma once

PUN_BEGIN

class PUN_DLL CObjectManager
{
private:
	list<class CGameObject*>	m_DontDestroyObj;
	class CGameObject*			m_pObj;

public:
	void AddDontDestroyObj(class CGameObject* pObj);
	void ChangeScene(class CScene*  pScene);
	bool CheckDontDestroyObj(const string& strTag);
	class CGameObject* FindDontDestroyObj(const string& strTag);
	class CGameObject * FindDontDestroyObjNonCount(const string & strTag);

public:
	void SetObj(class CGameObject* pObj);
	void ChangeSceneObject(class CScene* pScene);
public:
	class CGameObject*	GetObj() const;

public:
	bool Init();

	DECLARE_SINGLE(CObjectManager)
};

PUN_END