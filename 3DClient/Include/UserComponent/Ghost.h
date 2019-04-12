#pragma once
#include "../Client.h"
#include "Component/UserComponent.h"
#include "Component/Animation2D.h"
#include "Component/Material.h"

PUN_USING

class CGhost :
	public CUserComponent
{
	friend class CGameObject;

protected:
	CGhost();
	CGhost(const CGhost& com);
	virtual ~CGhost();

private:
	CAnimation2D*	m_pAnimation;
	CMaterial*		m_pMaterial;
	float			m_fPlayTime;
	float			m_fVisableTime;
	float			m_fTime;

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CGhost* Clone();
};

