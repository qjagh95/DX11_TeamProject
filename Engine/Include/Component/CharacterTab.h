#pragma once
#include "UI.h"
#include "Animation2D.h"

PUN_BEGIN

class PUN_DLL CCharacterTab :
	public CUI
{
	friend class CGameObject;

protected:
	CCharacterTab();
	CCharacterTab(const CCharacterTab& ctab);
	virtual ~CCharacterTab();

private:
	class CColliderRect*	m_pCollider;
	CHARACTER_TAB_STATE		m_eState;
	function<void(float)>	m_ClickCallback;
	ButtonCBuffer			m_tCBuffer;
	string					m_strSound[BS_END];
	CAnimation2D*			m_pAnimation;

public:
	void Disable();
	void Enable();
	void SetSound(const string& strFileName, CHARACTER_TAB_STATE eState);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCharacterTab* Clone();

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