#pragma once

#include "EngineHeader.h"

PUN_BEGIN

template <typename T>
class PUN_DLL CHeap
{
public:
	CHeap()
	{
		m_iSize = 0;
		m_iCapacity = 100;
		m_pHeap = new T[m_iCapacity];

		SetSortFunc(CHeap<T>::Sort);
	}

	~CHeap()
	{
		SAFE_DELETE_ARRAY(m_pHeap);
	}

private:
	T*		m_pHeap;
	int		m_iSize;
	int		m_iCapacity;
	function<bool(const T&, const T&)>	m_CmpFunc;

public:
	void SetSortFunc(bool(*pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(pFunc, placeholders::_1, placeholders::_2);
	}

	template <typename ClassType>
	void SetSortFunc(ClassType* pObj,
		bool(ClassType::*pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
	}

public:
	void Resize()
	{
		if (m_iCapacity == m_iSize)
		{
			m_iCapacity *= 2;
			T*	pList = new T[m_iCapacity];
			memset(pList, 0, sizeof(T) * m_iCapacity);

			memcpy(pList, m_pHeap, sizeof(T) * m_iSize);

			SAFE_DELETE_ARRAY(m_pHeap);

			m_pHeap = pList;
		}
	}

	void Resize(int iCapacity)
	{
		m_iCapacity = iCapacity;

		T*	pList = new T[m_iCapacity];
		memset(pList, 0, sizeof(T) * m_iCapacity);

		memcpy(pList, m_pHeap, sizeof(T) * m_iSize);

		SAFE_DELETE_ARRAY(m_pHeap);

		m_pHeap = pList;
	}

	void Insert(const T& data)
	{
		if (m_iSize == m_iCapacity)
			Resize();

		// 배열의 가장 마지막에 데이터를 넣어준 후에 부모와 비교하며
		// 교체한다. 재귀함수를 이용해서 처리한다.
		m_pHeap[m_iSize] = data;
		++m_iSize;
		_Insert(m_iSize - 1);
	}

	bool pop(T& data)
	{
		if (m_iSize == 0)
			return false;

		data = m_pHeap[0];

		// 가장 마지막 노드를 첫번째로 옮겨준다.
		m_pHeap[0] = m_pHeap[m_iSize - 1];
		--m_iSize;

		_pop(0);

		return true;
	}

	void Clear()
	{
		m_iSize = 0;
	}

	bool empty()	const
	{
		return m_iSize == 0;
	}

private:
	void _Insert(int iIndex)
	{
		// Root일 경우 더이상 부모가 없으므로 탐색을 종료한다.
		if (iIndex == 0)
			return;

		// 부모의 인덱스를 구한다.
		int	iParent = (iIndex - 1) / 2;

		// 부모와 나의 값을 비교한다. 비교함수는 m_CmpFunc을 사용한다.
		if (m_CmpFunc(m_pHeap[iIndex], m_pHeap[iParent]))
		{
			T	data = m_pHeap[iIndex];
			m_pHeap[iIndex] = m_pHeap[iParent];
			m_pHeap[iParent] = data;
			// 부모의 자리로 갔으므로 부모의 인덱스를 넣고
			// 다시 돌려본다.
			_Insert(iParent);
		}
	}

	void _pop(int iIndex)
	{
		// 기준 인덱스의 자식 둘을 비교하여 나와 비교해야 할
		// 자식노드를 찾는다.
		int	iLeftChild = iIndex * 2 + 1;

		if (iLeftChild >= m_iSize)
			return;

		int	iRightChild = iLeftChild + 1;
		int	iChildIndex = iLeftChild;

		if (iRightChild < m_iSize)
		{
			if (m_CmpFunc(m_pHeap[iRightChild], m_pHeap[iLeftChild]))
				iChildIndex = iRightChild;
		}

		// 기준노드와 구해준 자식노드와 비교하여 바꿀지 결정한다.
		if (m_CmpFunc(m_pHeap[iChildIndex], m_pHeap[iIndex]))
		{
			T	data = m_pHeap[iIndex];
			m_pHeap[iIndex] = m_pHeap[iChildIndex];
			m_pHeap[iChildIndex] = data;

			_pop(iChildIndex);
		}
	}

private:
	static bool Sort(const T& src, const T& dest)
	{
		return src < dest;
	}
};

PUN_END