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

		// �迭�� ���� �������� �����͸� �־��� �Ŀ� �θ�� ���ϸ�
		// ��ü�Ѵ�. ����Լ��� �̿��ؼ� ó���Ѵ�.
		m_pHeap[m_iSize] = data;
		++m_iSize;
		_Insert(m_iSize - 1);
	}

	bool pop(T& data)
	{
		if (m_iSize == 0)
			return false;

		data = m_pHeap[0];

		// ���� ������ ��带 ù��°�� �Ű��ش�.
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
		// Root�� ��� ���̻� �θ� �����Ƿ� Ž���� �����Ѵ�.
		if (iIndex == 0)
			return;

		// �θ��� �ε����� ���Ѵ�.
		int	iParent = (iIndex - 1) / 2;

		// �θ�� ���� ���� ���Ѵ�. ���Լ��� m_CmpFunc�� ����Ѵ�.
		if (m_CmpFunc(m_pHeap[iIndex], m_pHeap[iParent]))
		{
			T	data = m_pHeap[iIndex];
			m_pHeap[iIndex] = m_pHeap[iParent];
			m_pHeap[iParent] = data;
			// �θ��� �ڸ��� �����Ƿ� �θ��� �ε����� �ְ�
			// �ٽ� ��������.
			_Insert(iParent);
		}
	}

	void _pop(int iIndex)
	{
		// ���� �ε����� �ڽ� ���� ���Ͽ� ���� ���ؾ� ��
		// �ڽĳ�带 ã�´�.
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

		// ���س��� ������ �ڽĳ��� ���Ͽ� �ٲ��� �����Ѵ�.
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