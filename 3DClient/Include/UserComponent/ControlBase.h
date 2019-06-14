#pragma once
#include "Component/UserComponent.h"
#include "Component/ColliderOBB3D.h"

PUN_USING

typedef struct _tagKeyPad
{
	CGameObject* pObj;
	int			 iNumber;
}KEYPAD, *PKEYPAD;

class CControlBase :
	public CUserComponent
{
	friend class CGameObject;

protected:
	CControlBase();
	CControlBase(const CControlBase& pCom);
	~CControlBase();

	string					m_strKeyName;

	vector<class CDoor*>	m_vecDoor;
	CONTROL_TYPE			m_eControlType;

	int						m_iPassword[4];
	vector<int>				m_vecPassword;

	CTransform*				m_pLeverTr;

	bool					m_bShowKeyPad;
	bool					m_bLeverDown;
	float					m_fAccTime;
	float					m_fLimitTime;
	KEYPAD					m_pUIKeyPad[11];

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int  Input(float fTime);
	virtual int  Update(float fTime);
	virtual int  LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CControlBase* Clone();

public:
	void SetControlType(CONTROL_TYPE eType);
	void SetKeyName(const string& strKeyName);
	void SetPassWord(int iPassword[4]);
	void SetPassWord(int i1, int i2, int i3, int i4);
	bool Check();
	void AddDoor(class CDoor* pDoor);
	void Success();
	void LeverDown();
	void ShowKeyPad();

	void Interact();
	void Interact(CCollider* pSrc, CCollider* pDest, float fTime);
};

