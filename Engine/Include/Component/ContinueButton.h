#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CContinueButton :
	public CUI
{
	friend class CGameObject;

protected:
	CContinueButton();
	CContinueButton(const CContinueButton& button);
	virtual ~CContinueButton();

protected:
	CBUTTON_STATE			m_eState;
	function<void(float)>	m_ClickCallback;
	Vector4					m_vBSColor[CS_END];
	ButtonCBuffer			m_tCBuffer;
	string					m_strSound[CS_END];

public:
	void Disable();
	void Enable();
	void SetStateColor(CBUTTON_STATE eState, const Vector4& vColor);
	void SetStateColor(CBUTTON_STATE eState, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetSound(const string& strFileName, CBUTTON_STATE eState);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CContinueButton* Clone();

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