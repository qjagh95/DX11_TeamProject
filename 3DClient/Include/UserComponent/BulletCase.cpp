#include "../ClientHeader.h"
#include "BulletCase.h"

#include <Component/Renderer.h>

bool CBulletCase::bLoaded = false;
Vector3 CBulletCase::vCaseGravity = Vector3(0.f, -9.8f, 0.f);
Vector3 CBulletCase::vCaseInitVel = Vector3(7.5f, 15.5f, 0.f);
float CBulletCase::fLifeTime = 1.5f;
float CBulletCase::fThrustTime = 0.5f;
std::vector<std::string> CBulletCase::strSoundList;

CBulletCase::CBulletCase():
	m_fCurrLife(0.f),
	m_pSndInstance(nullptr)
{
}

CBulletCase::CBulletCase(const CBulletCase & _BCase)
{
	*this = _BCase;
	m_fCurrLife = 0.f;
	m_pSndInstance = nullptr;
	m_vCurrVelocity = vCaseInitVel;
}

CBulletCase::~CBulletCase()
{
}

bool CBulletCase::Init()
{
	strSoundList.clear();

	
	bLoaded = true;
	AfterClone();
	return true;
}

int CBulletCase::Update(float fTime)
{
	if (m_fCurrLife < fLifeTime)
	{
		if (m_fCurrLife < fThrustTime)
		{
			float fThrustRate = 1.f - (m_fCurrLife / fThrustTime);

			Vector3 vMove_Prev = m_pTransform->GetWorldPos();
			
			Vector3 vMove_Speed_noAxis = m_vCurrVelocity * fThrustRate * fTime;
			Vector3 vMove_Speed;

			vMove_Speed += m_pTransform->GetWorldAxis(PUN::AXIS_X) * vMove_Speed_noAxis.x;
			vMove_Speed += m_pTransform->GetWorldAxis(PUN::AXIS_Y) * vMove_Speed_noAxis.y;
			vMove_Speed += m_pTransform->GetWorldAxis(PUN::AXIS_Z) * vMove_Speed_noAxis.z;

			vMove_Speed += (vCaseGravity * fTime);

			Vector3 vMove_Expected = vMove_Prev + vMove_Speed;
			m_pTransform->Move(vMove_Speed);
			Vector3 vMove_Post = m_pTransform->GetWorldPos();
			

			//if (fabsf(vMove_Post.x - vMove_Expected.x) < fabsf(vMove_Speed.x))
			//{
			//	m_vCurrVelocity.x = -m_vCurrVelocity.x;
			//}
			//if (fabsf(vMove_Post.z - vMove_Expected.z) < fabsf(vMove_Speed.z))
			//{
			//	m_vCurrVelocity.z = -m_vCurrVelocity.z;
			//}

			

		}
		else
		{
			m_pTransform->Move(vCaseGravity * fTime);
		}

		m_fCurrLife += fTime;
	}
	else
	{
		m_pObject->SetEnable(false);
	}

	return 0;
}

int CBulletCase::LateUpdate(float fTime)
{
	return 0;
}

void CBulletCase::AfterClone()
{
	//m_pSndInstance = FindComponentFromTag<PUN::CSoundSource>("sound");

	if (!m_pSndInstance)
	{
		//m_pSndInstance = AddComponent<PUN::CSoundSource>("sound");
	}

	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
	
	PUN::CRenderer *pRend = AddComponent<PUN::CRenderer>("renderer");
	pRend->SetMesh("BulletCase", TEXT("9mm_bullet_case.msh"));

	SAFE_RELEASE(pRend);
}

CBulletCase * CBulletCase::Clone()
{
	return new CBulletCase(*this);
}

void CBulletCase::Kill()
{
	this->Die();
	m_pObject->Die();
}

void CBulletCase::SetInstance(const Vector3 & vecPos, const Vector3& vecRot)
{
	m_pTransform->SetWorldPos(vecPos);
	m_pTransform->SetWorldRot(vecRot);
	m_fCurrLife = 0.f;
	m_vCurrVelocity = vCaseInitVel;
	m_pObject->SetEnable(true);
}
