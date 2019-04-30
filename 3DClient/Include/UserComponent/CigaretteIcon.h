#pragma once
#include "../Client.h"
#include <Component/Item.h>

PUN_USING

class CCigarette;
class CCigaretteIcon : public CItem
{
private:
	friend class CGameObject;

public:
	CCigaretteIcon();
	CCigaretteIcon(const CCigaretteIcon& _cigaretteIcon);
	~CCigaretteIcon();

public:
	void Start()					override {};
	void AfterClone()				override {};
	bool Init()						override;
	int	 Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override { return 0; };
	void Collision(float _fTime)	override {};
	void Render(float _fTime)		override {};
	CCigaretteIcon* Clone()			override { return nullptr; };

private:
	bool m_isUseItem;

private:
	CCigarette* m_pCigarette;

public:
	void HitStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
};