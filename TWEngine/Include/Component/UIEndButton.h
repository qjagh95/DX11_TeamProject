#pragma once
#include "UI.h"
#include "Animation2D.h"

PUN_BEGIN

class PUN_DLL CUIEndButton :
	public CUI
{
	friend class CGameObject;

protected:
	CUIEndButton();
	CUIEndButton(const CUIEndButton& endbutton);
	virtual ~CUIEndButton();

protected:
	END_BUTTON_STATE		m_eState;
	function<void(float)>	m_ClickCallback;
	Vector4					m_vBSColor[BS_END];
	ButtonCBuffer			m_tCBuffer;
	string					m_strSound[BS_END];
	CAnimation2D*			m_pAnimation;

public:
	void Disable();
	void Enable();
	void SetSound(const string& strFileName, END_BUTTON_STATE eState);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUIEndButton* Clone();

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