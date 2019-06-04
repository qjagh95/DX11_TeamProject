#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CPaperPL :
	public CUserComponent
{
	friend class CGameObject;

private:
	CPaperPL();
	CPaperPL(const CPaperPL& paper);
	virtual ~CPaperPL();

private:
	CGameObject*	m_pDocxInvenObj;
	class CDocxInven*		m_pDocxInven;
	CGameObject*	m_pOutLineObj;
	CTransform*		m_pOutLineTr;
	bool			m_bMouseOn;
	bool			m_bUseInven;
	bool			m_bOnInven;
	int				m_iCount;
	bool			m_bMotion;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CPaperPL* Clone();

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);

private:
	void SetOutLineVisible(bool bEnable);

public:
	void SetOutLinePos(const Vector3& vPos);
	void SetOutLinePos(float x, float y, float z);
};

