#pragma once

PUN_BEGIN

class PUN_DLL BlackBoard
{
public:
	template<typename T>
	void AddData(const string& VarName, T** Data)
	{
		T** getData = (T**)FindData(VarName);

		if (getData != NULLPTR)
			return;

		m_DataMap.insert(make_pair(VarName, (void**)Data));
	}

	template<typename T>
	T* GetData(const string& VarName)
	{
		auto FindIter = m_DataMap.find(VarName);

		if (FindIter == m_DataMap.end())
			return NULLPTR;

		return (T*)*FindIter->second;
	}

	void** FindData(const string& VarName)
	{
		auto FindIter = m_DataMap.find(VarName);

		if (FindIter == m_DataMap.end())
			return NULLPTR;

		return FindIter->second;
	}

private:
	unordered_map<string, void**> m_DataMap;

private:
	BlackBoard();
	~BlackBoard();

public:
	friend class BehaviorTree;
	friend class BTManager;
};

PUN_END