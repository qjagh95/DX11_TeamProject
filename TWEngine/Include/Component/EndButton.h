#pragma once
#include "UI.h"

PUN_BEGIN

class PUN_DLL CEndButton :
	public CUI
{
	friend class CGameObject;

protected:
	CEndButton();
	CEndButton(const CEndButton& button);
	virtual ~CEndButton();

protected:
	EBUTTON_STATE			m_eState;
	function<void(float)>	m_ClickCallback;
	Vector4					m_vBSColor[ES_END];
	ButtonCBuffer			m_tCBuffer;
	string					m_strSound[ES_END];

public:
	void Disable();
	void Enable();
	void SetStateColor(EBUTTON_STATE eState, const Vector4& vColor);
	void SetStateColor(EBUTTON_STATE eState, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetSound(const string& strFileName, EBUTTON_STATE eState);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEndButton* Clone();

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