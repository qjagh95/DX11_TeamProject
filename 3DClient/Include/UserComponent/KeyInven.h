#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CKeyInven :
	public CUserComponent
{
	friend class CGameObject;

private:
	CKeyInven();
	CKeyInven(const CKeyInven& key);
	virtual ~CKeyInven();

private:
	vector<class CGameObject*>	m_vecKey;
	CGameObject*			m_pKeyBigObj;
	CKeyBigICon*			m_pKeyBig;
	int						m_iKeyMax;
	int						m_iFlag;
	int						m_iIndex;
	int						m_iMoveIndex;
	bool					m_bMouseOn;
	bool					m_bOnDocx;
	bool					m_bVisible;
	bool					m_bMouseOnKey;
	Vector2					m_vMouse;
	float					m_fItemY;

public:
	void SetKeyMax(int iMax);
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetMouseOn(bool bMouseOn);
	void SetMouseOnKey(bool bMouseOnInven);

public:
	bool	GetVisible()	const;
	int		GetKeyMax() const;
	bool	Full() const;
	void	AddItem(class CGameObject* pItem);
	const Vector3 GetKeyPos()	const;
	bool	GetMouseOnKey()	const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CKeyInven* Clone();

public:
	void AddUILayer();
};

