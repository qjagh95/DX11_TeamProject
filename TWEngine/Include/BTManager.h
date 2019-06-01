#pragma once

PUN_BEGIN

class BehaviorTree;
class PUN_DLL BTManager
{
public:
	static BTManager* Get()
	{
		if (m_Instance == NULL)
			m_Instance = new BTManager();

		return m_Instance;
	}

	static void Delete()
	{
		delete m_Instance;
	}

	void DeleteTree(const string& BTName);

public:
	BehaviorTree* CreateBehaviorTree(const string& KeyName, CGameObject* object = NULLPTR, BT_ROOT_CHILD_TYPE eStyle = BT_SELECTOR);
	BehaviorTree* FindTree(const string& KeyName);
	void GUIRender();

private:
	unordered_map<string, BehaviorTree*> m_TreeMap;

	static BTManager* m_Instance;
	vector<string> m_vecItemsName;
	const char** m_Items;

private:
	BTManager();
	~BTManager();
};

PUN_END