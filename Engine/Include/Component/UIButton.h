#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CUIButton :
	public CUI
{
	friend class CGameObject;

protected:
	CUIButton();
	CUIButton(const CUIButton& button);
	virtual ~CUIButton();

protected:
	BUTTON_STATE			m_eState;
	function<void(float)>	m_ClickCallback;
	Vector4					m_vBSColor[BS_END];
	ButtonCBuffer			m_tCBuffer;
	string					m_strSound[BS_END];
	class CAnimation2D*		m_pAnimation;

public:
	void Disable();
	void Enable();
	void SetStateColor(BUTTON_STATE eState, const Vector4& vColor);
	void SetStateColor(BUTTON_STATE eState, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetSound(const string& strFileName, BUTTON_STATE eState);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUIButton* Clone();

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