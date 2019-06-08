#include "../ClientHeader.h"
#include "Bullet.h"
#include <Component/Light.h>
#include <Component/Material.h>
#include <Component/Renderer.h>
#include <Component/ColliderOBB3D.h>

Vector4 CBullet::vTracerDiffColor = Vector4(1.f, 0.25f, 0.f, 1.f);
Vector4 CBullet::vTracerSpcColor = Vector4(1.f, 0.75f, 0.25f, 1.f);
Vector3 CBullet::vBulletSize = Vector3(0.25f, 0.25f, 1.25f);
Vector3 CBullet::vDecalSize = Vector3(1.f, 1.f, 1.f);

float CBullet::fBulletFullLife = 3.f;
float CBullet::fBulletSpeed = 350.f;
float CBullet::fDecalLife = 15.f;

CBullet::CBullet():
	m_fCurrTimer(0.f),
	m_pBulletDecal(nullptr),
	m_bLeaveDecal(false),
	m_iDamage(25)
{
}

CBullet::CBullet(const CBullet & bullet)
{
	m_fCurrTimer = 0.f;
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pBulletDecal);
}

void CBullet::Create(Vector3 vPos, Vector3 vRot, PUN::CLayer *pLayer)
{
	//rand + time = unique key
	time_t now = time(0);
	std::string strKey = "bullet_";

	strKey += to_string(now);
	strKey += to_string(rand());

	PUN::CGameObject *pInstance = PUN::CGameObject::CreateObject(strKey, pLayer);

	PUN::CTransform* pTr = pInstance->GetTransform();

	pTr->SetWorldPos(vPos);
	pTr->SetWorldRot(vRot);

	SAFE_RELEASE(pTr);

	CBullet *pBullet = pInstance->AddComponent<CBullet>("bullet");

	SAFE_RELEASE(pBullet);

	SAFE_RELEASE(pInstance);
}

bool CBullet::Init()
{
	AfterClone();
	return true;
}

int CBullet::Update(float fTime)
{
	m_pTransform->Move(PUN::AXIS_Z, fBulletSpeed, fTime);

	m_fCurrTimer += fTime;

	if (m_bLeaveDecal)
	{
	}
	else
	{
		if (m_fCurrTimer > fBulletFullLife)
		{
			m_pObject->Die();
			this->Die();
		}
	}

	return 0;
}

int CBullet::LateUpdate(float fTime)
{
	return 0;
}

void CBullet::Collision(float fTime)
{
}

void CBullet::AfterClone()
{
	PUN::CRenderer *pRend = AddComponent<PUN::CRenderer>("renderer");
	pRend->SetMesh("tracer", TEXT("tracer.msh"));
	

	SAFE_RELEASE(pRend);
	//
	//PUN::CMaterial *pMat = FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
	//pMat->SetDiffuseTex(0, "empty-white", TEXT("Particles\\bullet\\tracer_d.png"),);
	//pMat->SetDiffuseTex(1, "empty-white", TEXT("Particles\\bullet\\tracer_norm.png"));
	////pMat->SetDiffuseTex(2, "empty-white", TEXT("Particles\\empty_black.png"));
	//
	//SAFE_RELEASE(pMat);

	PUN::CColliderOBB3D *pCol = AddComponent<PUN::CColliderOBB3D>("BulletCol");
	pCol->SetColliderID(UCI_BULLET);
	pCol->SetInfo(Vector3::Zero, Vector3::Axis, vBulletSize);
	pCol->SetCollisionCallback(PUN::CCT_ENTER, this, &CBullet::OnBulletHit);

	SAFE_RELEASE(pCol);


	PUN::CLight *pTracer = AddComponent<PUN::CLight>("Tracer");
	pTracer->SetLightType(PUN::LT_POINT);
	pTracer->SetLightDiffuse(vTracerDiffColor);
	pTracer->SetLightSpcular(vTracerSpcColor);
	pTracer->SetLightRange(32.f);
	
	pTracer->SetLightAmbient(Vector4(0.75f, 0.5f, 0.5f, 0.75f));

	SAFE_RELEASE(pTracer);

	m_pTransform->SetLocalRot(90.f, 0.f, 0.f);
	m_pTransform->SetWorldScale(0.0375f, 0.0375f, 1.5f);

	//m_pBulletDecal = AddComponent<PUN::CDecal>("bulletDecal");
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::OnBulletHit(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	PUN::CColliderOBB3D *pOBBBullet = nullptr;
	PUN::CColliderOBB3D *pOBBDest = nullptr;

	if (pSrc->GetColliderID() == UCI_BULLET)
	{
		if (pDest->GetColliderID() == UCI_BULLET)
		{
			return;
		}
		else
		{
			pOBBBullet = (PUN::CColliderOBB3D*)pSrc;
			pOBBDest = (PUN::CColliderOBB3D*)pDest;
		}
		
	}
	else if(pDest->GetColliderID() == UCI_BULLET)
	{
		pOBBDest = (PUN::CColliderOBB3D*)pSrc;
		pOBBBullet = (PUN::CColliderOBB3D*)pDest;
	}


}
