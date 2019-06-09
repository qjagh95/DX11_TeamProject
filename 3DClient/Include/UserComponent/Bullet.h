#pragma once

#include "Component/UserComponent.h"
#include <Scene/Layer.h>
#include "Component/Collider.h"


class CBullet :
	public PUN::CUserComponent
{
	friend PUN::CGameObject;
protected:
	CBullet();
	CBullet(const CBullet& bullet);
	~CBullet();
	
private:
	PUN::CGameObject *m_pBulletDecal;
	float m_fCurrTimer;
	int m_iDamage;
	bool m_bLeaveDecal;
	Vector3 m_vPrevPos;
	static bool m_bLoaded;
public:
	static Vector4 vTracerDiffColor;
	static Vector4 vTracerSpcColor;
	static Vector3 vBulletSize;
	static Vector3 vDecalSize;
	static float fBulletFullLife;
	static float fBulletSpeed;
	static float fDecalLife;

public:
	static void Create(Vector3 vPos, Vector3 vRot, PUN::CLayer *pLayer);
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);

	void AfterClone();

	CBullet *Clone();
	void OnBulletHit(PUN::CCollider *pSrc, PUN::CCollider* pDest, float fTime);
};