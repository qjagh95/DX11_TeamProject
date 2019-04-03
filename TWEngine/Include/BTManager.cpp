#include "EngineHeader.h"
#include "BTManager.h"
#include "BehaviorTree.h"
#include "Core.h"

PUN_USING

BTManager* BTManager::m_Instance = NULLPTR;

BTManager::BTManager()
{
	m_Items = new const char*();
}

BTManager::~BTManager()
{
	unordered_map<string, BehaviorTree*>::iterator StartIter = m_TreeMap.begin();
	unordered_map<string, BehaviorTree*>::iterator EndIter = m_TreeMap.end();
	
	for (;StartIter != EndIter; StartIter++)
		SAFE_DELETE(StartIter->second);

	for (size_t i = 0; i < m_vecItemsName.size(); i++)
		SAFE_DELETE(m_Items[i]);

	SAFE_DELETE_ARRAY(m_Items);
}

void BTManager::Delete(const string & BTName)
{
	m_TreeMap.erase(BTName);
}

BehaviorTree* BTManager::CreateBehaviorTree(const string& KeyName, BT_ROOT_CHILD_TYPE eStyle)
{
	BehaviorTree* newTree = new BehaviorTree();
	newTree->m_TagName = KeyName;
	newTree->Init(eStyle, NULLPTR);

	m_TreeMap.insert(make_pair(KeyName, newTree));
	m_vecItemsName.push_back(KeyName);

	m_Items[m_vecItemsName.size() - 1] = new char[m_vecItemsName[m_vecItemsName.size() - 1].length()];
	memcpy_s((void*)m_Items[m_vecItemsName.size() - 1], m_vecItemsName[m_vecItemsName.size() - 1].length(), (void*)m_vecItemsName[m_vecItemsName.size() - 1].c_str(), m_vecItemsName[m_vecItemsName.size() - 1].length());

	return newTree;
}

BehaviorTree * BTManager::FindTree(const string & KeyName)
{
	auto FindIter = m_TreeMap.find(KeyName);

	if (FindIter == m_TreeMap.end())
		return NULLPTR;
	
	return FindIter->second;
}

void BTManager::GUIRender()
{
	if (CCore::GetInst()->m_bTreeOnOff == false)
		return;

	ImGui::Begin("BehaviorTree");
	{
		static int SelectIndex = 0;
		ImGui::Combo("BTList", &SelectIndex, m_Items, (int)m_vecItemsName.size());

		FindTree(m_vecItemsName[SelectIndex])->GUIRender();
	}
	ImGui::End();
}
