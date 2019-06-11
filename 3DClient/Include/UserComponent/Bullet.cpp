#include "../ClientHeader.h"
#include "Bullet.h"
#include <Component/Light.h>
#include <Component/Material.h>
#include <Component/Renderer.h>
#include <Component/Decal.h>
#include <Component/ColliderOBB3D.h>
#include <Component/Particle.h>
#include <NavigationMesh.h>
#include <NavigationManager3D.h>
#include <Component/Animation2D.h>
#include <Scene/Layer.h>
#include <Scene/SceneManager.h>


Vector4 CBullet::vTracerDiffColor = Vector4(1.f, 0.25f, 0.f, 1.f);
Vector4 CBullet::vTracerSpcColor = Vector4(1.f, 0.75f, 0.25f, 1.f);
Vector3 CBullet::vBulletSize = Vector3(0.5f, 0.5f, 0.75f);
Vector3 CBullet::vDecalSize = Vector3(0.25f, 3.f, 0.25f);
bool CBullet::m_bLoaded = false;

float CBullet::fBulletFullLife = 3.f;
float CBullet::fBulletSpeed = 100.f;
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
	m_pBulletDecal->Die();
	SAFE_RELEASE(m_pBulletDecal);
}

void CBullet::Create(Vector3 vPos, Vector3 vRot, PUN::CLayer *pLayer)
{
	//rand + time = unique key
	time_t now = time(0);
	std::string strKey = "bullet_";

	strKey += to_string(now);
	strKey += to_string(rand());

	PUN::CLayer *ppLayer = PUN::CSceneManager::GetInst()->FindLayer("Default");

	PUN::CGameObject *pInstance = PUN::CGameObject::CreateObject(strKey, ppLayer, true);
	SAFE_RELEASE(ppLayer);
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
	if (!m_bLoaded)
	{
		PUN::CGameObject*	pParticleObj = PUN::CGameObject::CreatePrototype("DustParticle");
		PUN::CParticle*	pParticle = pParticleObj->AddComponent<PUN::CParticle>("Particle");
		SAFE_RELEASE(pParticle);

		PUN::CRenderer* pRenderer = pParticleObj->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
		pRenderer->SetMesh("Particle_GreenMat");
		SAFE_RELEASE(pRenderer);

		PUN::CColliderOBB3D*	pParticleSphere = pParticleObj->AddComponent<PUN::CColliderOBB3D>("DecalSphere");
		pParticleSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.f, 1.f, 1.f));
		SAFE_RELEASE(pParticleSphere);
		PUN::CMaterial*	pParticleMtrl = pParticleObj->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
		vector<const TCHAR*>	vecExplosionName;
		for (int i = 1; i <= 24; ++i)
		{
			TCHAR*	pFileName = new TCHAR[MAX_PATH];
			memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
			wsprintf(pFileName, TEXT("Particles\\Sand_Dust\\Dust_Sand%d.png"), i);
			vecExplosionName.push_back(pFileName);
		}
		pParticleMtrl->SetDiffuseTex(4, "BulletDust", vecExplosionName);
		pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
		SAFE_RELEASE(pParticleMtrl);
		PUN::CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<PUN::CAnimation2D>("ParticleAnimation");
		vector<PUN::Clip2DFrame>	vecFrame;
		for (int i = 1; i <= 24; ++i)
		{
			PUN::Clip2DFrame	tFrame = {};
			tFrame.vLT = Vector2(0.f, 0.f);
			tFrame.vRB = Vector2(1.f, 1.f);
			vecFrame.push_back(tFrame);
		}
		pParticleAnimation->AddClip("Idle", PUN::A2D_FRAME, PUN::AO_ONCE_DESTROY,
			1.f, vecFrame, "Dust", vecExplosionName);
		SAFE_RELEASE(pParticleAnimation);
		for (size_t i = 0; i < vecExplosionName.size(); ++i)
		{
			SAFE_DELETE_ARRAY(vecExplosionName[i]);
		}
		vecExplosionName.clear();
		SAFE_RELEASE(pParticleObj);


		pParticleObj = PUN::CGameObject::CreatePrototype("BloodParticle");
		pParticle = pParticleObj->AddComponent<PUN::CParticle>("Particle");
		SAFE_RELEASE(pParticle);

		pRenderer = pParticleObj->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
		pRenderer->SetMesh("Particle_GreenMat");
		SAFE_RELEASE(pRenderer);
		pParticleMtrl = pParticleObj->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
		vecExplosionName.clear();
		vecFrame.clear();
		for (int i = 1; i <= 14; ++i)
		{
			TCHAR*	pFileName = new TCHAR[MAX_PATH];
			memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
			wsprintf(pFileName, TEXT("Particles\\Blood1\\Blood%d.png"), i);
			vecExplosionName.push_back(pFileName);

			PUN::Clip2DFrame	tFrame = {};
			tFrame.vLT = Vector2(0.f, 0.f);
			tFrame.vRB = Vector2(1.f, 1.f);
			vecFrame.push_back(tFrame);
		}
		pParticleMtrl->SetDiffuseTex(4, "Gun_Blood", vecExplosionName);
		pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
		SAFE_RELEASE(pParticleMtrl);
		pParticleAnimation = pParticleObj->AddComponent<PUN::CAnimation2D>("ParticleAnimation");
		pParticleAnimation->AddClip("Idle", PUN::A2D_FRAME, PUN::AO_LOOP,
			0.333334f, vecFrame, "Gun_Blood", vecExplosionName);

		SAFE_RELEASE(pParticleAnimation);
		for (size_t i = 0; i < vecExplosionName.size(); ++i)
		{
			SAFE_DELETE_ARRAY(vecExplosionName[i]);
		}
		vecExplosionName.clear();

		SAFE_RELEASE(pParticleObj);
	}

	m_bLoaded = true;
	AfterClone();
	return true;
}

int CBullet::Update(float fTime)
{
	
	m_fCurrTimer += fTime;

	if (!m_bLeaveDecal)
	{
		m_vPrevPos = m_pTransform->GetWorldPos();
		m_pTransform->Move(PUN::AXIS_Z, fBulletSpeed, fTime);

		Vector3 vCurrPos = m_pTransform->GetWorldPos();

		Vector3 vMove = vCurrPos - m_vPrevPos;

		PUN::CNavigationMesh*	pNavMesh = GET_SINGLE(PUN::CNavigationManager3D)->FindNavMesh(m_pScene, vCurrPos);
		if (pNavMesh)
		{
			float fY = pNavMesh->GetY(vCurrPos);
			if (vCurrPos.y < fY)
			{
				PUN::CRenderer *pRend = FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
				if (pRend)
					pRend->SetEnable(false);

				SAFE_RELEASE(pRend);

				vCurrPos.y = fY + .125f;
				m_bLeaveDecal = true;
				m_pBulletDecal->SetEnable(true);
				m_pTransform->SetWorldPos(vCurrPos);
				PUN::CTransform *pDecalTrans = m_pBulletDecal->GetTransform();
				pDecalTrans->SetWorldPos(vCurrPos);
				SAFE_RELEASE(pDecalTrans);


				std::string strKey = "dust";
				strKey += to_string((unsigned long long)this + rand());
				PUN::CLayer *ppLayer = PUN::CSceneManager::GetInst()->FindLayer("Default");
				PUN::CGameObject *pDust = PUN::CGameObject::CreateClone(strKey, "DustParticle", ppLayer);
				SAFE_RELEASE(ppLayer);


				PUN::CTransform *pProtTr = pDust->GetTransform();
				pProtTr->SetWorldPos(vCurrPos);
				pProtTr->SetWorldScale(Vector3(2.5f, 5.f, 1.f));
				SAFE_RELEASE(pProtTr);

				SAFE_RELEASE(pDust);

				PUN::CLight* pLight = FindComponentFromType<PUN::CLight>(PUN::CT_LIGHT);
				pLight->SetLightColor(Vector4::Zero, Vector4::Zero, Vector4::Zero);
				SAFE_RELEASE(pLight);
			}
		}

		if (m_fCurrTimer > fBulletFullLife)
		{
			m_pObject->Die();
			this->Die();
		}
	}
	else
	{
		PUN::CRenderer *pRend = FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
		if (pRend)
			pRend->SetEnable(false);

		SAFE_RELEASE(pRend);

		if (m_fCurrTimer > fDecalLife)
		{
			m_pObject->Die();
			
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

	PUN::CColliderOBB3D *pCol = AddComponent<PUN::CColliderOBB3D>("BulletBody");
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
	m_pTransform->SetWorldScale(0.0375f, 0.0375f, .075f);

	//m_pBulletDecal = AddComponent<PUN::CDecal>("bulletDecal");
	std::string strKey = "bulletDecal_";
	strKey += std::to_string((ptrdiff_t)this);

	PUN::CLayer *ppLayer = PUN::CSceneManager::GetInst()->FindLayer("Default");
	m_pBulletDecal = PUN::CGameObject::CreateObject(strKey, ppLayer);
	
	SAFE_RELEASE(ppLayer);

	
	PUN::CDecal *pDecal = m_pBulletDecal->AddComponent<PUN::CDecal>("Decal");
	//PUN::CColliderOBB3D* pDecalSphere = m_pBulletDecal->AddComponent<PUN::CColliderOBB3D>("DecalSphere");
	//
	//pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.f, 2.f, 1.f));
	//
	//SAFE_RELEASE(pDecalSphere);
	SAFE_RELEASE(pDecal);
	
	PUN::CMaterial *pDecalMtrl = m_pBulletDecal->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);

	pDecalMtrl->SetDiffuseTex(0, "DecalCannonDif", TEXT("Decal/Decal.png"));
	pDecalMtrl->SetNormalTex(1, "DecalCannonNrm", TEXT("Decal/Decal_NRM.png"));
	pDecalMtrl->SetSpecularTex(2, "DecalCannonSpc", TEXT("Decal/Decal_SPEC.png"));

	pDecalMtrl->SetSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pDecalMtrl);

	PUN::CTransform *pDecalTr = m_pBulletDecal->GetTransform();
	pDecalTr->SetWorldScale(.5f, 3.f, .5f);
	SAFE_RELEASE(pDecalTr);
	
	m_pBulletDecal->SetEnable(false);
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::OnBulletHit(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	
	if (m_bLeaveDecal)
		return;
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

	USER_COLLIDER_ID collId = (USER_COLLIDER_ID)pOBBDest->GetColliderID();
	PUN::CTransform *pDecalTrans = m_pBulletDecal->GetTransform();

	const PUN::POBB3DInfo tDestInfo = &(pOBBDest->GetInfo());
	const PUN::POBB3DInfo tBulletInfo = &(pOBBBullet->GetInfo());

	

	switch (collId)
	{
	case UCI_DOOR:
	case UCI_PARKOUR:
	case UCI_BED:
	case UCI_LOCKER:
	case UCI_WALL:
	case UCI_GENERATOR:
		m_bLeaveDecal = true;
		m_pBulletDecal->SetEnable(true);
		break;
	//case UCI_ENEMY_ATTACK:
	//	break;
	//case UCI_ENEMY_INTERACT:
	//	break;
	//case UCI_ENEMY_HIT:
	//	break;
	//case UCI_DUCT:
	//	break;
	default:
		return;
		break;
	}


	Vector3 vRot = m_pTransform->GetWorldRot();
	//pDecalTrans->SetWorldRot(vRot);
	//pDecalTrans->RotationX(90.f);

	Vector3 vMoveDir = m_pTransform->GetWorldAxis(PUN::AXIS_Z);
	//Get bullet to obj vector

	Vector3 vDir = tDestInfo->vCenter - tBulletInfo->vCenter;
	Vector3 vDirNorm = vDir;
	vDirNorm.Normalize();
	float fDirLeng = vDir.Length();

	Vector3 vEulerRot;

	PUN::CTransform *pTR = pOBBDest->GetTransform();
	vEulerRot = pTR->GetWorldRot();
	SAFE_RELEASE(pTR);

	Vector3 vP1mP2 = vMoveDir * fBulletSpeed;
	Vector3 vPoint;
	Vector3 vMinPoint;
	//for loop get smilar axis vector

	float fMinDist = FLT_MAX;
	char cMaxIdx = 0;
	char cMaxSign = 1;
	
	for (char i = 0; i < 6; ++i)
	{
		char cAxis = i / 2;
		char cSign = (i % 2) * 2 - 1;
		Vector3 vNormalSurf = (tDestInfo->vAxis[cAxis] * (float)cSign);
		Vector3 vPlus = vNormalSurf - vDir;
		float fLengthBuf = vPlus.Length();

		Vector3 vP3 = tDestInfo->vCenter;

		vP3 += tDestInfo->vAxis[0] * ((float)cSign * tDestInfo->vLength.x);
		vP3 += tDestInfo->vAxis[1] * ((float)cSign * tDestInfo->vLength.y);
		vP3 += tDestInfo->vAxis[2] * ((float)cSign * tDestInfo->vLength.z);

		float fDot = vNormalSurf.Dot(vP1mP2);

		float fResDelta = vNormalSurf.Dot(vP3 - tBulletInfo->vCenter) / fDot;

		if (fResDelta < 0.f)
			continue;

		if (fResDelta > 1.f)
			continue;

		vPoint = vP1mP2;
		vPoint = tBulletInfo->vCenter + (vPoint * fResDelta);

		float fDist = (vPoint - tDestInfo->vCenter).Length();

		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			cMaxIdx = i;
			vMinPoint = vPoint;
		}
	}
	cMaxIdx /= 2;
	vP1mP2.Normalize();
	
	switch (cMaxIdx)
	{
	case 0:
		vEulerRot.z += 90.f;
		break;

	case 1:

		break;
	case 2:
		vEulerRot.x += 90.f;
		break;
	}
	/*
	switch (cMaxIdx)
	{
	case 0: //X
		pDecalTrans->RotationZ(90.f);

		break;
	case 1:
		pDecalTrans->RotationZ(-90.f);
		break;
	case 2: //Y
		//¾ç¸é ´Ù µÇ´Ï±î ³ÀµÐ´Ù
		break;
	case 3:
		//¿ø·¡ À§¸¦ º¸´Ï±î ³ÀµÐ´Ù
		break;
	case 4: //Z
		pDecalTrans->RotationX(90.f);
		break;
	case 5:
		pDecalTrans->RotationX(-90.f);
		break;
	}
	*/
	
	Vector3 vCenter = (m_vPrevPos + m_pTransform->GetWorldPos()) * 0.5f - (m_pTransform->GetWorldAxis(PUN::AXIS_Z) * tBulletInfo->vLength.z);

	if (pDest->GetTag() == "MonsterBodyOBB")
	{
		std::string strKey = "blood";
		strKey += to_string((unsigned long long)this + rand());
		PUN::CLayer *ppLayer = PUN::CSceneManager::GetInst()->FindLayer("Default");
		PUN::CGameObject *pDust = PUN::CGameObject::CreateClone(strKey, "BloodParticle", ppLayer);
		SAFE_RELEASE(ppLayer);


		PUN::CTransform *pProtTr = pDust->GetTransform();
		pProtTr->SetWorldPos(vCenter);
		pProtTr->SetWorldScale(Vector3(2.5f, 5.f, 1.f));
		SAFE_RELEASE(pProtTr);

		SAFE_RELEASE(pDust);

		m_pObject->Die();
		return;
	}

	vRot.x += 90.f;
	pDecalTrans->SetWorldRot(vRot);

	std::string strKey = "dust";
	strKey += to_string((unsigned long long)this + rand());
	PUN::CLayer *ppLayer = PUN::CSceneManager::GetInst()->FindLayer("Default");
	PUN::CGameObject *pDust = PUN::CGameObject::CreateClone(strKey, "DustParticle", ppLayer);
	SAFE_RELEASE(ppLayer);
	

	PUN::CTransform *pProtTr = pDust->GetTransform();
	pProtTr->SetWorldPos(vCenter);
	pProtTr->SetWorldScale(Vector3(2.5f, 5.f, 1.f));
	SAFE_RELEASE(pProtTr);

	SAFE_RELEASE(pDust);

	pDecalTrans->SetWorldPos(vCenter);
	//Decal Should be set this Axis as Up Vector
}
