#pragma once

PUN_BEGIN

class PUN_DLL BlackBoard
{
public:
	template<typename T>
	void AddData(const string& VarName, T* Data)
	{
		T* Data = FindData(VarName);

		if (Data != NULLPTR)
			return Data;

		m_DataMap.insert(make_pair(VarName, Data));
	}

	template<typename T>
	T* GetData(const string& VarName)
	{
		auto FindIter = m_DataMap.find(VarName);

		if (FindIter == m_DataMap.end())
			return NULLPTR;

		return (T*)FindIter->second;
	}

	void* FindData(const string& VarName)
	{
		auto FindIter = m_DataMap.find(VarName);

		if (FindIter == m_DataMap.end())
			return NULLPTR;

		return FindIter->second;
	}

private:
	unordered_map<string, void*> m_DataMap;

private:
	BlackBoard();
	~BlackBoard();

public:
	friend class BehaviorTree;
	friend class BTManager;
};

PUN_END