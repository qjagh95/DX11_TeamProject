#pragma once
#include "../Client.h"
#include "Component/UserComponent.h"

PUN_USING

class CDocxInven :
	public CUserComponent
{
	friend class CGameObject;

private:
	CDocxInven();
	CDocxInven(const CDocxInven& docx);
	virtual ~CDocxInven();

private:
	vector<class CGameObject*>	m_vecItem;
	class CGameObject*		m_pDocObj;
	class CDocument*		m_pDoc;
	DOCXINVEN_STATE			m_eState;
	int						m_iDocxMax;
	int						m_iFlag;
	int						m_iIndex;
	int						m_iMoveIndex;
	bool					m_bMouseOn;
	bool					m_bOnDocx;
	bool					m_bVisible;
	bool					m_bMouseOnDocx;
	Vector2					m_vMouse;
	float					m_fItemY;

public:
	void SetDocxMax(int iMax);
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetMouseOn(bool bMouseOn);
	void SetDocxnState(DOCXINVEN_STATE eState);
	void SetMouseOnDocx(bool bMouseOnInven);

public:
	bool	GetVisible()	const;
	int		GetDocxMax() const;
	bool	Full() const;
	void	AddItem(class CGameObject* pItem);
	DOCXINVEN_STATE GetDocxState() const;
	const Vector3 GetDocxPos()	const;
	bool	GetMouseOnDocx()	const;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CDocxInven* Clone();

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);
	void AddUILayer();
};

