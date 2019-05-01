#pragma once
#include "../Client.h"
#include <Component/Item.h>

PUN_USING

class CCigarette;
class CTabletIcon : public CItem
{
private:
	friend class CGameObject;

public:
	CTabletIcon();
	CTabletIcon(const CTabletIcon& _tabletIcon);
	~CTabletIcon();

public:
	void Start()					override {};
	void AfterClone()				override {};
	bool Init()						override;
	int	 Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override { return 0; };
	void Collision(float _fTime)	override {};
	void Render(float _fTime)		override {};
	CTabletIcon* Clone()			override { return nullptr; };

private:
	bool m_isUseItem;
	bool m_bMouseOn;

private:
	CCigarette* m_pCigarette;

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);
	void HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void MouseOut(class CCollider* pSrc, class CCollider* pDest, float fTime);
};