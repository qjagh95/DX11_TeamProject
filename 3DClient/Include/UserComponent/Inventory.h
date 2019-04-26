#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Collider.h"
#include "Component/ColliderRect.h"

PUN_USING

class CInventory :
	public CUserComponent
{
	friend class CGameObject;

private:
	CInventory();
	CInventory(const CInventory& inven);
	virtual ~CInventory();

private:
	vector<class CGameObject*>	m_vecItem;
	CLayer*					m_pUILayer;
	INVENTORY_STATE			m_eState;
	int						m_iInvenMax;
	int						m_iFlag;
	int						m_iIndex;
	int						m_iMoveIndex;
	bool					m_bMouseOn;
	bool					m_bOnInven;
	bool					m_bVisible;
	Vector2					m_vMouse;
	float					m_fItemY;

private:
	int				m_iZipCount;
	
public:
	void SetInvenMax(int iMax);
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetMouseOn(bool bMouseOn);
	void SetInvenState(INVENTORY_STATE eState);
	void SetMouseOnInven(bool bMouseOnInven);

public:
	bool	GetVisible()	const;
	int		GetInvenMax() const;
	bool	Full() const;
	void	AddItem(class CGameObject* pItem);
	void	DeleteItem(class CGameObject* pItem);
	INVENTORY_STATE GetInvenState() const;
	const Vector3 GetInvenPos()	const;
	bool	GetMouseOnInven()	const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CInventory* Clone();

public:
	void Hit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	void BottomHit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void BottomInven(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void BottomOut(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	void AddInvenList(class CGameObject* pItem);
};

