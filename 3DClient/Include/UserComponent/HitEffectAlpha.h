#pragma once
#include "Component/UserComponent.h"
#include "../Client.h"

PUN_USING

class CHitEffectAlpha :
	public CUserComponent
{
	friend class CGameObject;

private:
	CHitEffectAlpha();
	CHitEffectAlpha(const CHitEffectAlpha& effect);
	virtual ~CHitEffectAlpha();

private:
	float	m_fPlayTime;
	float	m_fElapsedTime;
	float	m_fAlpha;
	int		m_iFlag;

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int	Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CHitEffectAlpha* Clone();
};

