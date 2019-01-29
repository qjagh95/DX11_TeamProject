#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CQucikSlot :
	public CUI
{
	friend class CGameObject;

protected:
	CQucikSlot();
	CQucikSlot(const CQucikSlot& quickslot);
	virtual ~CQucikSlot();

protected:
	QUICK_SLOT_BUTTON_STATE	m_eState;
	function<void(float)>	m_ClickCallback;
	ButtonCBuffer			m_tCBuffer;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CQucikSlot* Clone();

public:
	void Hit(class CCollider* pSrc, class CCollider* pDest, float fTime);
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