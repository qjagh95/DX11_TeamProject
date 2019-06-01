#pragma once
#include "Component/UserComponent.h"
#include <Component/ColliderRay.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>

#include "../Client.h"
#include "../CameraEff.h"

class CPistol :
	public PUN::CUserComponent
{
	friend class PUN::CGameObject;
protected:
	CPistol();
	CPistol(const CPistol& pistol);
	~CPistol();

public:
	static int	iBulletCaseCnt;
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
	PUN::CGameObject *m_pMuzzle;
	PUN::CGameObject *m_pGunSmoke;
	PUN::CGameObject *m_pGunPointFake;
	PUN::CGameObject *m_pGunMuzzleFlash;

	class CBulletCase *m_arrBulletCases[10];
	int		m_iCurrBulletCaseIdx;
	float m_fFireDmg;
	float m_fFireReloadSpd;
	float m_fFireTimer;
	float m_fSmokeTimer;
	float m_fSmokeTimerBuf;
	Vector3 m_vGunPointOffset;
	Vector3 m_vSmokeSocketOffset;

public:
	bool Fire();
	void DrawOut();
	void HitScan(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime);
	void Recoil(float fTime, float fCurrTime, float fEndTime, PUN::CTransform* pCam);

private:
	tCameraEffects tRecoilEff;
};