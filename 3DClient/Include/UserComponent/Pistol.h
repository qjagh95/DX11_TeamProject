#pragma once
#include "Component/UserComponent.h"
#include <Component/ColliderRay.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>

#include "../Client.h"

class CPistol :
	public PUN::CUserComponent
{
	friend class PUN::CGameObject;
protected:
	CPistol();
	CPistol(const CPistol& pistol);
	~CPistol();

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision();
	void Render();

	CPistol *Clone();
	void AfterClone();

private:
	PUN::CColliderRay *m_pRay;
	PUN::CSoundSource *m_pSnd;
	PUN::CAnimation *m_pAnim;
	float m_fFireDmg;
	float m_fFireReloadSpd;
	float m_fFireTimer;
	Vector3 m_vGunPointOffset;

public:
	bool Fire();
	void HitScan(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime);
};