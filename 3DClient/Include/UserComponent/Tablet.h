#pragma once
#include "../Client.h"
#include <Component/UserComponent.h>

PUN_USING

class CInventory;
class CTablet : public CUserComponent
{
private:
	friend class CGameObject;

public:
	CTablet();
	CTablet(const CTablet& _tablet);
	~CTablet();

public:
	void Start()					override {};
	void AfterClone()				override {};
	virtual void AfterInit();
	bool Init()						override;
	int	 Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override { return 0; };
	void Collision(float _fTime)	override {};
	void Render(float _fTime)		override {};
	CTablet* Clone()				override { return nullptr; };

private:
	bool	m_isInvenInItem;
	bool	m_bMotion;
	bool	m_bGetItem;

private:
	CRenderer*			m_pRenderer;
	CGameObject*		m_pObjInventory;
	CInventory*			m_pInventory;
	CGameObject*		m_pObjItemIcon;
	CGameObject*	m_pOutLineObj;
	CGameObject*	m_pBigObj;
	CTransform*		m_pOutLineTr;
	CTransform*		m_pBigTr;

public:
	void Hit(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void MouseOut(CCollider* _pSrc, CCollider* _pDest, float _fTime);

private:
	void SetOutLineVisible(bool bEnable);

public:
	void SetOutLinePos(const Vector3& vPos);
	void SetOutLinePos(float x, float y, float z);
};