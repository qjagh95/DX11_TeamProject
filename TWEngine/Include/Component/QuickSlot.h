#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CQuickSlot :
	public CUI
{
	friend class CGameObject;

protected:
	CQuickSlot();
	CQuickSlot(const CQuickSlot& quickslot);
	virtual ~CQuickSlot();

protected:
	QUICK_SLOT_BUTTON_STATE	m_eState;
	function<void(float)>	m_ClickCallback;
	ButtonCBuffer			m_tCBuffer;
	bool					m_bMouseOn;
	int						m_iFlag;

public:

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CQuickSlot* Clone();

public:
	void Hit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void OnSlot(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	void SetCallback(void(*pFunc)(float));
	template <typename T>
	void SetCallback(void(T::*pFunc)(float), T* pObj)
	{
		m_ClickCallback = bind(pFunc, pObj, placeholders::_1);
	}
};

PUN_END