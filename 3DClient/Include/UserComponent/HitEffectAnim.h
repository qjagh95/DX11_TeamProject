#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"
#include "Component/Animation2D.h"

PUN_USING

class CHitEffectAnim :
	public CUserComponent
{
	friend class CGameObject;

private:
	CHitEffectAnim();
	CHitEffectAnim(const CHitEffectAnim& effect);
	virtual ~CHitEffectAnim();

private:
	class CAnimation2D*	m_pAnimation;
	class CGameObject*		m_pPlayerObj;
	class CHuman_Player*	m_pPlayer;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHitEffectAnim* Clone();
};

