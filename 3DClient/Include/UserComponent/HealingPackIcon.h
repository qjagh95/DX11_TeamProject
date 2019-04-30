#pragma once
#include "../Client.h"
#include <Component/Item.h>

PUN_USING

class CHealingPack;
class CHealingPackIcon : public CItem
{
private: 
	friend class CGameObject;

public:
	CHealingPackIcon();
	CHealingPackIcon(const CHealingPackIcon& _healingPackIcon);
	~CHealingPackIcon() override;

public:
	void Start()					override {};
	void AfterClone()				override {};
	bool Init()						override;
	int	 Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override { return 0; };
	void Collision(float _fTime)	override {};
	void Render(float _fTime)		override {};
	CHealingPackIcon* Clone()		override { return nullptr; };

private:
	bool m_isUseItem;

private:
	CHealingPack* m_pHealingPack;

public:
	void SetHealingPackInst(const CHealingPack* _pInst);
	void SetMaterial();
	void HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
};