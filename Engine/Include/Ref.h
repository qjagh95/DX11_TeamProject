#pragma once

#include "Engine.h"

PUN_BEGIN

class PUN_DLL CRef
{
protected:
	CRef();
	virtual ~CRef() = 0;

protected:
	string	m_strTag;
	int		m_iRefCount;
	bool	m_bEnable;
	bool	m_bActive;


public:
	string	GetTag()	const;
	bool	GetEnable()	const;
	bool	GetActive()	const;

public:
	void	SetTag(const string& strTag);
	int		AddRef();
	int		Release();
	void	Die();
	void	SetEnable(bool bEnable);
};

PUN_END