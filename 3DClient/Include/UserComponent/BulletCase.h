#pragma once
#include <Component/UserComponent.h>
#include <Component/SoundSource.h>

class CBulletCase :
	public PUN::CUserComponent
{
	friend class PUN::CGameObject;

protected:
	CBulletCase();
	CBulletCase(const CBulletCase& _BCase);
	~CBulletCase();

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);

	void AfterClone();
	CBulletCase *Clone();
	void Kill();
	void SetInstance(const Vector3& vecPos, const Vector3& vecRot);
	static std::vector<std::string> strSoundList;
	static Vector3 vCaseGravity;
	static Vector3 vCaseInitVel;
	static float fLifeTime;
	static float fThrustTime;
	
private:
	static bool bLoaded;
	float m_fCurrLife;
	Vector3 m_vCurrVelocity;
	PUN::CSoundSource *m_pSndInstance;

};