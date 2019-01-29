#include "stdafx.h"
#include "Ref.h"

PUN_USING

CRef::CRef()	:
	m_iRefCount(1),
	m_bEnable(true),
	m_bActive(true)
{
}

CRef::~CRef()
{
}

string CRef::GetTag() const
{
	return m_strTag;
}

bool CRef::GetEnable() const
{
	return m_bEnable;
}

bool CRef::GetActive() const
{
	return m_bActive;
}

void CRef::SetTag(const string & strTag)
{
	m_strTag = strTag;
}

int CRef::AddRef()
{
	++m_iRefCount;
	return m_iRefCount;
}

int CRef::Release()
{
	--m_iRefCount;

	if (m_iRefCount == 0)

	{
		delete this;
		return 0;
	}

	return m_iRefCount;
}

void CRef::Die()
{
	m_bActive = false;
}

void CRef::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}
