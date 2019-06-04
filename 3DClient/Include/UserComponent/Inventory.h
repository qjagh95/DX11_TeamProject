#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Collider.h"
#include "Component/ColliderRect.h"
#include "Component/Number.h"

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
	vector<class CGameObject*>	m_vecNumber;
	vector<class CGameObject*>	m_vecList;
	CGameObject*			m_pZipObj;
	class CZipper*				m_pZipper;
	INVENTORY_STATE			m_eState;
	ICON_TYPE				m_eType;
	int						m_iInvenMax;
	int						m_iFlag;
	int						m_iIndex;
	int						m_iMoveIndex;
	int						m_iNumberIndex;
	bool					m_bMouseOn;
	bool					m_bOnInven;
	bool					m_bVisible;
	Vector2					m_vMouse;
	float					m_fItemY;

private:
	int				m_iZipCount;
	int				m_iCount;

private:
	int		m_iBatteryCnt;
	int		m_iMedicalKitCnt;
	int		m_iLunchBoxCnt;
	int		m_iTabletCnt;
	int		m_iDaemaCnt;

	class CGameObject*	m_pBatteryNumberObj;
	class CGameObject*	m_pMedicalKitNumberObj;
	class CGameObject*	m_pTabletNumberObj;
	class CGameObject*	m_pLunchBoxNumberObj;
	class CGameObject*	m_pDaemaNumberObj;

	class CGameObject*	m_pBigIconObj;
	class CBigIcon*		m_pBigIcon;

	class CNumber*	m_pBatteryNumber;
	class CNumber*	m_pMedicalKitNumber;
	class CNumber*	m_pTabletNumber;
	class CNumber*	m_pLunchBoxNumber;
	class CNumber*	m_pDaemaNumber;

	Vector3	m_vBatteryIconPos;
	Vector3	m_vMedicalKitIconPos;
	Vector3	m_vLunchBoxIconPos;
	Vector3	m_vDaemaIconPos;
	Vector3	m_vTabletIconPos;

	bool	m_bBatteryUse;
	bool	m_bMedicalKitUse;
	bool	m_bTabletUse;
	bool	m_bLunchBoxUse;
	bool	m_bDaemaUse;
	
public:
	void SetInvenMax(int iMax);
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetMouseOn(bool bMouseOn);
	void SetInvenState(INVENTORY_STATE eState);
	int GetItemCountNumber(ICON_TYPE eType) const;
	void SetItemCountNumber(ICON_TYPE eType, int iCount);

public:
	bool	GetVisible()	const;
	int		GetInvenMax() const;
	bool	Full() const;
	void	AddItem(class CGameObject* pItem);
	void	UseItem(class CGameObject* pItem);
	void	MoveItem(size_t iIndex);
	INVENTORY_STATE GetInvenState() const;
	const Vector3 GetInvenPos()	const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CInventory* Clone();
};

