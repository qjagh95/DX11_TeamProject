#pragma once

PUN_BEGIN

class PUN_DLL CObjectManager
{
private:
	std::list<class CGameObject*> m_DontDestroyObjList;
public:
	void ChangeScene(class CScene* _pScene , class CGameObject* _pObject , const std::string& _strLayerName = "Default");
	void ChangeSceneFromDontDestroyObj(class CScene* _pScene);
	void PushDontDestoryObject(class CGameObject* _pObject);
	bool RemoveDontDestroyObject(class CGameObject* _pObject);
	bool RemoveDontDestroyObject(const std::string& _strTag);
	class CGameObject* FindDonDestoryObject(const std::string& _strTag);
	class CGameObject* FindDonDestoryObject(class CGameObject* _pObject);

public:
	bool Init();

	DECLARE_SINGLE(CObjectManager)
};

PUN_END