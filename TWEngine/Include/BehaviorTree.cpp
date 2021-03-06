#include "EngineHeader.h"
#include "EngineHeader.h"
#include "BTManager.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"

PUN_USING

BehaviorTree::BehaviorTree()
{
	m_BlackBoard = new BlackBoard();
	m_RootNode = NULLPTR;
	m_RootSequence = NULLPTR;
	m_RootSelector = NULLPTR;
	m_Object = NULLPTR;
	m_isOnePath = false;
	m_Type = ACTION_NONE;
	m_RootName = "RootNode";
	m_RootSequenceName = "RootSequence";
	m_RootSelectorName = "RootSelector";

	m_Count = 0;
}

BehaviorTree::~BehaviorTree()
{
	BTManager::Get()->DeleteTree(m_TagName);

	SAFE_DELETE(m_RootNode);
	SAFE_DELETE(m_RootSequence);
	SAFE_DELETE(m_RootSelector);
	SAFE_DELETE(m_BlackBoard);
}

void BehaviorTree::Update(float DeltaTime)
{
	m_Type = (BT_ACTION_TYPE)m_RootNode->Update(DeltaTime);
}

BehaviorTree::Sequence* BehaviorTree::AddRootSequenceInSequence(const string& NewSequenceKeyName)
{
	if (m_RootSequence == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Sequence* newSequence = FindSequence(NewSequenceKeyName);

	if (newSequence != NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	newSequence = new Sequence();
	newSequence->m_BlackBoard = m_BlackBoard;
	newSequence->SetObject(m_Object);
	newSequence->Init();
	newSequence->SetTag(NewSequenceKeyName);
	newSequence->SetKeepAction(m_RootSequence);
	newSequence->SetActionType(BT_SEQUENCE);

	m_RootSequence->AddChildNode(newSequence);
	m_SequenceMap.insert(make_pair(NewSequenceKeyName, newSequence));

	m_Count++;

	return newSequence;
}

BehaviorTree::Selector* BehaviorTree::AddRootSequenceInSelector(const string & NewSelectorKeyName)
{
	if (m_RootSequence == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Selector* newSelector = FindSelector(NewSelectorKeyName);

	if (newSelector != NULLPTR)
		return NULLPTR;

	newSelector = new Selector();
	newSelector->m_BlackBoard = m_BlackBoard;
	newSelector->SetObject(m_Object);
	newSelector->Init();
	newSelector->SetTag(NewSelectorKeyName);
	newSelector->SetKeepAction(m_RootSequence);
	newSelector->SetActionType(BT_SELECTOR);

	m_RootSequence->AddChildNode(newSelector);
	m_SelectorMap.insert(make_pair(NewSelectorKeyName, newSelector));

	m_Count++;

	return newSelector;
}

BehaviorTree::Sequence* BehaviorTree::AddRootSelectorInSequence(const string & NewSequenceKeyName)
{
	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	if (m_RootSelector == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Sequence* newSequence = new Sequence();
	newSequence->m_BlackBoard = m_BlackBoard;
	newSequence->SetObject(m_Object);
	newSequence->Init();
	newSequence->SetTag(NewSequenceKeyName);
	newSequence->SetKeepAction(m_RootSelector);
	newSequence->SetActionType(BT_SEQUENCE);

	m_RootSelector->AddChildNode(newSequence);
	m_SequenceMap.insert(make_pair(NewSequenceKeyName, newSequence));

	return newSequence;
}

BehaviorTree::Selector* BehaviorTree::AddRootSelectorInSelector(const string & NewSelectorKeyName)
{
	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	if (m_RootSelector == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Selector* newSelector = new Selector();
	newSelector->m_BlackBoard = m_BlackBoard;
	newSelector->SetObject(m_Object);
	newSelector->Init();
	newSelector->SetTag(NewSelectorKeyName);
	newSelector->SetKeepAction(m_RootSelector);
	newSelector->SetActionType(BT_SELECTOR);

	m_RootSelector->AddChildNode(newSelector);
	m_SelectorMap.insert(make_pair(NewSelectorKeyName, newSelector));

	m_Count++;

	return newSelector;
}

BehaviorTree::Selector* BehaviorTree::AddSequenceInSelector(const string & SequenceKeyName, const string & SelectorKeyName)
{
	Sequence* getSequence = FindSequence(SequenceKeyName);

	if (getSequence == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Selector* newSelector = FindSelector(SelectorKeyName);

	if (newSelector != NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	newSelector = new Selector();
	newSelector->m_BlackBoard = m_BlackBoard;
	newSelector->SetObject(m_Object);
	newSelector->Init();
	newSelector->SetTag(SelectorKeyName);
	newSelector->SetKeepAction(getSequence);
	newSelector->SetActionType(BT_SELECTOR);

	getSequence->AddChildNode(newSelector);
	m_SelectorMap.insert(make_pair(SelectorKeyName, newSelector));
	m_Count++;

	return newSelector;
}

BehaviorTree::Sequence* BehaviorTree::AddSelectorInSequence(const string & SelectorKeyName, const string & SequenceKeyName)
{
	Selector* getSelector = FindSelector(SelectorKeyName);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Sequence* newSequence = FindSequence(SequenceKeyName);

	if (newSequence != NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	newSequence = new Sequence();
	newSequence->m_BlackBoard = m_BlackBoard;
	newSequence->SetObject(m_Object);
	newSequence->Init();
	newSequence->SetTag(SequenceKeyName);
	newSequence->SetKeepAction(getSelector);
	newSequence->SetActionType(BT_SEQUENCE);

	getSelector->AddChildNode(newSequence);
	m_SequenceMap.insert(make_pair(SequenceKeyName, newSequence));
	m_Count++;

	return newSequence;
}

BehaviorTree::Sequence* BehaviorTree::AddSequenceInSequence(const string & OldSequenceKey, const string & NewSequenceKey)
{
	Sequence* getSelector = FindSequence(OldSequenceKey);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Sequence* newSequence = FindSequence(NewSequenceKey);

	if (newSequence != NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	newSequence = new Sequence();
	newSequence->m_BlackBoard = m_BlackBoard;
	newSequence->SetObject(m_Object);
	newSequence->Init();
	newSequence->SetTag(NewSequenceKey);
	newSequence->SetKeepAction(getSelector);
	newSequence->SetActionType(BT_SEQUENCE);

	getSelector->AddChildNode(newSequence);
	m_SequenceMap.insert(make_pair(NewSequenceKey, newSequence));
	m_Count++;

	return newSequence;
}

void BehaviorTree::SetSelectorRandomProcess(const string & SelectorKeyName, bool Value)
{
	Selector* getSelector = FindSelector(SelectorKeyName);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return;
	}

	getSelector->SetRandomProcess(Value);
}

void BehaviorTree::SequenceTickFuncDisable(const string & SequenceKeyName)
{
	Sequence* getSequence = FindSequence(SequenceKeyName);

	if (getSequence == NULLPTR)
	{
		assert(false);
		return;
	}

	getSequence->DisableTickFunc();
}

void BehaviorTree::SequenceTickFuncEnable(const string & SequenceKeyName)
{
	Sequence* getSequence = FindSequence(SequenceKeyName);

	if (getSequence == NULLPTR)
	{
		assert(false);
		return;
	}

	getSequence->EnableTickFunc();
}

void BehaviorTree::SelectorTickFuncDisable(const string & SelectorKeyName)
{
	Selector* getSelector = FindSelector(SelectorKeyName);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return;
	}

	getSelector->DisableTickFunc();
}

void BehaviorTree::SelectorTickFuncEnable(const string & SelectorKeyName)
{
	Selector* getSelector = FindSelector(SelectorKeyName);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return;
	}

	getSelector->EnableTickFunc();
}

BehaviorTree::Selector* BehaviorTree::AddSelectorInSelector(const string & OldSelectorKey, const string & NewSelector)
{
	Selector* getSelector = FindSelector(OldSelectorKey);

	if (getSelector == NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	Selector* newSelector = FindSelector(NewSelector);

	if (newSelector != NULLPTR)
	{
		assert(false);
		return NULLPTR;
	}

	newSelector = new Selector();
	newSelector->m_BlackBoard = m_BlackBoard;
	newSelector->SetObject(m_Object);
	newSelector->Init();
	newSelector->SetTag(NewSelector);
	newSelector->SetKeepAction(getSelector);
	newSelector->SetActionType(BT_SELECTOR);

	getSelector->AddChildNode(newSelector);
	m_SelectorMap.insert(make_pair(NewSelector, newSelector));
	m_Count++;

	return newSelector;
}

void BehaviorTree::AddRootChildSelector()
{
	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return;
	}

	if (m_RootSequence != NULLPTR)
	{
		assert(false);
		return;
	}

	m_RootSelector = new Selector();
	m_RootSelector->m_BlackBoard = m_BlackBoard;
	m_RootSelector->SetObject(m_Object);
	m_RootSelector->SetTag(m_RootSelectorName);
	m_RootSelector->SetKeepAction(m_RootNode);
	m_RootSelector->SetActionType(BT_SELECTOR);

	m_RootNode->SetChild(m_RootSelector);
}

void BehaviorTree::AddRootChildSequence()
{
	if (m_RootNode == NULLPTR)
	{
		assert(false);
		return;
	}

	if (m_RootSelector != NULLPTR)
	{
		assert(false);
		return;
	}

	m_RootSequence = new Sequence();
	m_RootSequence->m_BlackBoard = m_BlackBoard;
	m_RootSequence->SetObject(m_Object);
	m_RootSequence->SetTag(m_RootSequenceName);
	m_RootSequence->SetKeepAction(m_RootNode);
	m_RootSequence->SetActionType(BT_SEQUENCE);

	m_RootNode->SetChild(m_RootSequence);
}

void BehaviorTree::SetObject(CGameObject * Object)
{
	m_Object = Object;

	m_RootNode->SetObject(Object);

	if (m_RootSequence != NULLPTR)
		m_RootSequence->SetAllActionObject(Object);

	else if (m_RootSelector != NULLPTR)
		m_RootSelector->SetAllActionObject(Object);
}

void BehaviorTree::Init(BT_ROOT_CHILD_TYPE eStyle, CGameObject* Object)
{
	m_RootNode = new RootNode();
	m_Object = Object;
	m_RootNode->SetObject(Object);
	m_RootNode->SetTag(m_RootName);
	m_RootNode->m_BlackBoard = m_BlackBoard;

	switch (eStyle)
	{
	case BT_SELECTOR:
		AddRootChildSelector();
		break;
	case BT_SEQUENCE:
		AddRootChildSequence();
		break;
	}
}

BehaviorTree::Sequence * BehaviorTree::FindSequence(const string & KeyName)
{
	auto FindIter = m_SequenceMap.find(KeyName);

	if (FindIter == m_SequenceMap.end())
		return NULLPTR;

	return FindIter->second;
}

BehaviorTree::Selector * BehaviorTree::FindSelector(const string & KeyName)
{
	auto FindIter = m_SelectorMap.find(KeyName);

	if (FindIter == m_SelectorMap.end())
		return NULLPTR;

	return FindIter->second;
}

BehaviorTree::Action * BehaviorTree::FindAction(const string & KeyName)
{
	auto FindIter = m_ActionMap.find(KeyName);

	if (FindIter == m_ActionMap.end())
		return NULLPTR;

	return FindIter->second;
}

void BehaviorTree::CompositNode::SetAllActionObject(CGameObject * Object)
{
	for (size_t i = 0; i < m_ChildAction.size(); i++)
		m_ChildAction[i]->SetObject(Object);
}

int BehaviorTree::Selector::Update(float DeltaTime)
{
	return Process(DeltaTime);
}

int BehaviorTree::Selector::Process(float DeltaTime)
{
	if (m_isCheck == true)
	{
		m_TimeVar += DeltaTime;

		if (m_TimeVar >= m_CheckTime)
		{
			m_TimeVar = 0.0f;
			m_TickFunc(DeltaTime);
		}
	}

	if (m_bRandom == false)
	{
		for (size_t i = 0; i < m_ChildAction.size(); i++)
		{
			for (size_t j = 0; j < m_ChildAction[i]->GetDecoratorVec()->size(); j++)
			{
				if (m_ChildAction[i]->GetDecoratorVec()->at(j)(DeltaTime) == false)
				{
					if (i < m_ChildAction.size())
						i++;
					else
						return ACTION_FALSE;
				}
			}

			BT_ACTION_TYPE Type = (BT_ACTION_TYPE)m_ChildAction[i]->Update(DeltaTime);

			switch (Type)
			{
			case ACTION_SUCCED:
				m_ChildAction[i]->Ending(DeltaTime);
				return ACTION_SUCCED;
				break;
			case ACTION_FALSE:
				break;
			case ACTION_RUNNING:
				return ACTION_RUNNING;
				break;
			}
		}
		return ACTION_FALSE;
	}
	else
	{

		int RandomNum = rand() % m_ChildAction.size();

		for (size_t i = 0; i < m_ChildAction[RandomNum]->GetDecoratorVec()->size(); i++)
		{
			if (m_ChildAction[RandomNum]->GetDecoratorVec()->at(i)(DeltaTime) == false)
				return ACTION_FALSE;
		}


		BT_ACTION_TYPE Type = (BT_ACTION_TYPE)m_ChildAction[RandomNum]->Update(DeltaTime);

		switch (Type)
		{
		case ACTION_SUCCED:
			m_ChildAction[RandomNum]->Ending(DeltaTime);
			return ACTION_SUCCED;
			break;
		case ACTION_FALSE:
			break;
		case ACTION_RUNNING:
			return ACTION_RUNNING;
			break;
		}
		return ACTION_FALSE;
	}
}

int BehaviorTree::Sequence::Update(float DeltaTime)
{
	if (m_isCheck == true)
	{
		m_TimeVar += DeltaTime;

		if (m_TimeVar >= m_CheckTime)
		{
			m_TimeVar = 0.0f;
			m_TickFunc(DeltaTime);
		}
	}

	for (size_t i = 0; i < m_ChildAction.size(); i++)
	{
		for (size_t j = 0; j < m_ChildAction[i]->GetDecoratorVec()->size() - 1; j++)
		{
			if (m_ChildAction[i]->GetDecoratorVec()->at(j)(DeltaTime) == false)
			{
				if (i < m_ChildAction.size())
					i++;
				else
					return ACTION_FALSE;

				continue;
			}
		}

		BT_ACTION_TYPE Type = (BT_ACTION_TYPE)m_ChildAction[i]->Update(DeltaTime);

		switch (Type)
		{
		case ACTION_RUNNING:
			return ACTION_RUNNING;
			break;
		case ACTION_FALSE:
			m_ChildAction[i]->Ending(DeltaTime);
			return ACTION_FALSE;
			break;
		case ACTION_SUCCED:
			break;
		}
	}

	return ACTION_SUCCED;
}

void BehaviorTree::GUIRender()
{
	ImGui::Begin("I'm Groot");
	{
		ImGui::BulletText(m_RootNode->GetTag().c_str());
		ImGui::Text("-----------Child--------------");

		if (m_RootSequence != NULLPTR)
			ImGui::Text("RootSequence");
		else
			ImGui::Text("RootSelector");
	}
	ImGui::End();

	if (m_RootSequence != NULLPTR)
	{
		ImGui::Begin("RootSequence");
		{
			ImGui::BulletText("RootSequence");
			ImGui::Text("-----------Child--------------");

			for (size_t i = 0; i < m_RootSequence->m_ChildAction.size(); i++)
			{
				ImGui::Text(m_RootSequence->m_ChildAction[i]->GetTag().c_str());
				m_RootSequence->m_ChildAction[i]->Render();
			}
		}
		ImGui::End();
	}
	else
	{
		ImGui::Begin("RootSelector");
		{
			ImGui::BulletText("RootSelector");
			ImGui::Text("-----------Child--------------");

			for (size_t i = 0; i < m_RootSelector->m_ChildAction.size(); i++)
			{
				ImGui::Text(m_RootSelector->m_ChildAction[i]->GetTag().c_str());
				m_RootSelector->m_ChildAction[i]->Render();
			}
		}
		ImGui::End();
	}
}

void BehaviorTree::Selector::Render()
{
	ImGui::Begin(m_TagName.c_str());
	{
		ImGui::BulletText(m_TagName.c_str());
		ImGui::Text("-----------Child--------------");

		for (size_t i = 0; i < m_ChildAction.size(); i++)
		{
			ImGui::Text(m_ChildAction[i]->GetTag().c_str());

			ImGui::Begin(m_ChildAction[i]->GetTag().c_str());
			{
				ImGui::BulletText(m_ChildAction[i]->GetTag().c_str());
			}
			ImGui::End();
		}
	}
	ImGui::End();
}

void BehaviorTree::Sequence::Render()
{
	ImGui::Begin(m_TagName.c_str());
	{
		ImGui::BulletText(m_TagName.c_str());
		ImGui::Text("-----------Child--------------");

		for (size_t i = 0; i < m_ChildAction.size(); i++)
		{
			ImGui::Text(m_ChildAction[i]->GetTag().c_str());
			ImGui::Begin(m_ChildAction[i]->GetTag().c_str());
			{
				ImGui::BulletText(m_ChildAction[i]->GetTag().c_str());
			}
			ImGui::End();
		}
	}
	ImGui::End();
}