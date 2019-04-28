#pragma once

PUN_BEGIN
class CGameObject;
class PUN_DLL CObjectManager
{
public:
	CGameObject* FindObject(const string& KeyName);
	void PushDontDestoryObject(CGameObject* _pObject);
	bool RemoveDontDestroyObject(CGameObject* _pObject);
	unordered_map<string, CGameObject*>* GetMap() { return &m_DontMap; }

private:
	unordered_map<string, CGameObject*> m_DontMap;

public:
	bool Init();

	DECLARE_SINGLE(CObjectManager)
};

PUN_END