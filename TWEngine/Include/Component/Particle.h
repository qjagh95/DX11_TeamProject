#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CParticle :
	public CComponent
{
	friend class CGameObject;

protected:
	CParticle();
	CParticle(const CParticle& particle);
	~CParticle();

private:
	ParticleCBuffer	m_tCBuffer;

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CParticle* Clone();
};

PUN_END