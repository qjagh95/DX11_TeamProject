#include "../ClientHeader.h"
#include "Pistol.h"
#include <Component/Renderer.h>
#include <Component/Particle.h>
#include <Component/Animation2D.h>
#include <Component/Renderer.h>
#include <Component/Material.h>
#include <Component/ColliderOBB3D.h>
#include <Component/Light.h>
#include <Scene/Scene.h>
#include "BulletCase.h"
#include "Human_Player.h"

int CPistol::iBulletCaseCnt = 10;

CPistol::CPistol() :
	m_pRay(nullptr),
	m_fFireReloadSpd(0.5f),
	m_fFireTimer(0.5f),
	m_pSnd(nullptr),
	m_pAnim(nullptr),
	m_pGunSmoke(nullptr),
	m_pMuzzle(nullptr),
	m_vGunPointOffset(-0.1f, -.25f, -0.4f),
	//m_vSmokeSocketOffset(-0.3f, -.25f, -0.5f),
	m_vSmokeSocketOffset(-0.3f, -.2f, -.85f),
	m_pGunPointFake(nullptr),
	m_fSmokeTimerBuf(0.f),
	m_fSmokeTimer(0.85f),
	m_iCurrBulletCaseIdx(0)
{
	tRecoilEff = {};
	for (int i = 0; i < iBulletCaseCnt; ++i)
	{
		m_arrBulletCases[i] = nullptr;
	}

	m_fSmokeTimerBuf = m_fSmokeTimer;
}

CPistol::CPistol(const CPistol & pistol) :
	PUN::CUserComponent(pistol)
{
}

CPistol::~CPistol()
{
	SAFE_RELEASE(m_pRay);
	SAFE_RELEASE(m_pSnd);
	SAFE_RELEASE(m_pAnim);
	SAFE_RELEASE(m_pGunSmoke);
	SAFE_RELEASE(m_pMuzzle);
	SAFE_RELEASE(m_pGunMuzzleFlash);
	SAFE_RELEASE(m_pGunPointFake);

	for (int i = 0; i < iBulletCaseCnt; ++i)
	{

		SAFE_RELEASE(m_arrBulletCases[i]);
	}
}

bool CPistol::Init()
{
	
	AfterClone();
	return true;
}

int CPistol::Input(float fTime)
{
	return 0;
}

int CPistol::Update(float fTime)
{
	if (m_fFireTimer < m_fFireReloadSpd)
	{
		m_fFireTimer += fTime;
	}
	else
	{
		m_fFireTimer = m_fFireReloadSpd;

	}

	if (m_fSmokeTimerBuf < m_fSmokeTimer)
	{
		m_fSmokeTimerBuf += fTime;

		if (m_fSmokeTimerBuf > m_fSmokeTimer)
		{
			PUN::CAnimation2D* pPartAnim = m_pGunSmoke->FindComponentFromType<PUN::CAnimation2D>(PUN::CT_ANIMATION2D);
			pPartAnim->CurrentClipJumpToFrame(0);
			SAFE_RELEASE(pPartAnim);
			m_pGunSmoke->SetEnable(true);
		}
	}

	return 0;
}

int CPistol::LateUpdate(float fTime)
{

	if (m_pGunPointFake)
	{
		PUN::CTransform *pMuzzleTrans = m_pGunPointFake->GetTransform();
		PUN::CTransform *pObjTrans = m_pGunSmoke->GetTransform();

		Vector3 vMuzzleTrans = pMuzzleTrans->GetWorldPos();
		Vector3 vLocRot = m_pTransform->GetWorldRot();

		pObjTrans->SetWorldRot(vLocRot);

		//pObjTrans->Rotation(m_pTransform->GetLocalRot() * -1.f);

		Vector3 vSmokeWorldOffset;
		vSmokeWorldOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_X) * m_vSmokeSocketOffset.x);
		vSmokeWorldOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_Y) * m_vSmokeSocketOffset.y);
		vSmokeWorldOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_Z) * m_vSmokeSocketOffset.z);

		vSmokeWorldOffset += vMuzzleTrans;

		pObjTrans->SetWorldPos(vSmokeWorldOffset);

		SAFE_RELEASE(pObjTrans);

		pObjTrans = m_pMuzzle->GetTransform();
		pObjTrans->SetWorldRot(vLocRot);
		Vector3 vGunMuzzleOffset;
		vGunMuzzleOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_X) * m_vGunPointOffset.x);
		vGunMuzzleOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_Y) * m_vGunPointOffset.y);
		vGunMuzzleOffset += (m_pTransform->GetWorldAxis(PUN::AXIS_Z) * m_vGunPointOffset.z);

		vGunMuzzleOffset += vMuzzleTrans;
		pObjTrans->SetWorldPos(vGunMuzzleOffset);

		SAFE_RELEASE(pObjTrans);

		PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		Vector3 vCamRot = pCamTrans->GetWorldRot();

		SAFE_RELEASE(pCamTrans);
		pObjTrans = m_pGunMuzzleFlash->GetTransform();
		pObjTrans->SetWorldRot(vCamRot);
		pObjTrans->SetWorldPos(vGunMuzzleOffset);

		SAFE_RELEASE(pObjTrans);

		if (m_fFireTimer >= m_fFireReloadSpd * 0.125f)
		{
			if (m_pMuzzle->GetEnable())
			{
				m_pGunMuzzleFlash->SetEnable(false);
			}
		}
		

		SAFE_RELEASE(pMuzzleTrans);
	}

	return 0;
}

void CPistol::Collision()
{
}

void CPistol::Render()
{
}

CPistol * CPistol::Clone()
{
	return new CPistol(*this);
}

void CPistol::AfterClone()
{

	PUN::CRenderer *renderer = this->AddComponent<PUN::CRenderer>("renderer");
	renderer->SetMesh("gun", TEXT("glock_OnlyFire.msh"));
	SAFE_RELEASE(renderer);

	m_pAnim = this->AddComponent<PUN::CAnimation>("animation");
	m_pAnim->LoadBone(TEXT("glock_OnlyFire.bne"));
	m_pAnim->Load(TEXT("glock_idle.anm"));
	m_pAnim->LoadFileAsClip("fire", TEXT("glock_fire.anm"));
	m_pAnim->SetDefaultClip("Take 001");
	m_pAnim->SetClipOption("fire", PUN::AO_ONCE_RETURN);
	m_pAnim->SetBlendSkip(true);
	PUN::PANIMATIONCLIP clip = m_pAnim->GetAnimClip("fire");
	clip->fEndTime = 0.16666666666666666666666666666667f;
	clip->fTimeLength = 0.16666666666666666666666666666667f;
	//clip->fStartTime = 0.f;
	clip->fFrameTime = 0.01666666666666666666666666666667f;
	clip->iFrameMode = 60;

	m_pRay = FindComponentFromType<PUN::CColliderRay>(PUN::CT_COLLIDER);
	if (!m_pRay)
	{
		m_pRay = AddComponent<PUN::CColliderRay>("collider");
	}

	m_pRay->SetEnable(false);

	std::vector<std::string> vecSndList;
	vecSndList.resize(5);
	vecSndList[0] = "Gun_Fire1";
	vecSndList[1] = "Gun_Fire2";
	vecSndList[2] = "Gun_Fire3";
	vecSndList[3] = "Gun_Fire4";
	vecSndList[4] = "Gun_DrawOut";


	m_pSnd = FindComponentFromTag<PUN::CSoundSource>("Sound");
	if (!m_pSnd)
	{
		m_pSnd = AddComponent<PUN::CSoundSource>("Sound");
	}

	m_pSnd->LoadSounds(vecSndList);

	//Gun Smoke
	m_pGunSmoke = PUN::CGameObject::CreateObject("gunSmoke", m_pLayer, true);
	m_pGunSmoke->SetFrustrumCullUse(false);
	
	PUN::CParticle *pParticle = m_pGunSmoke->AddComponent<PUN::CParticle>("particle");
	SAFE_RELEASE(pParticle);
	PUN::CRenderer* pRenderer = m_pGunSmoke->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	pRenderer->SetMesh("Particle_GreenMat");
	SAFE_RELEASE(pRenderer);
	PUN::CMaterial*	pParticleMtrl = m_pGunSmoke->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
	vector<const TCHAR*>	vecExplosionName;
	vector<PUN::Clip2DFrame>	vecFrame;
	for (int i = 1; i <= 147; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(pFileName, TEXT("Particles\\GunSmoke\\GunSmoke%d.png"), i);
		vecExplosionName.push_back(pFileName);

		PUN::Clip2DFrame	tFrame = {};
		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(1.f, 1.f);
		vecFrame.push_back(tFrame);
	}
	pParticleMtrl->SetDiffuseTex(4, "GunSmoke", vecExplosionName);

	pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pParticleMtrl);
	PUN::CAnimation2D*	pParticleAnimation = m_pGunSmoke->AddComponent<PUN::CAnimation2D>("ParticleAnimation");

	pParticleAnimation->AddClip("GunSmoke", PUN::A2D_FRAME, PUN::AO_ONCE_DISABLE,
		3.f, vecFrame, "GunSmoke1", vecExplosionName);

	SAFE_RELEASE(pParticleAnimation);

	for (size_t i = 0; i < vecExplosionName.size(); ++i)
	{
		SAFE_DELETE_ARRAY(vecExplosionName[i]);
	}

	vecExplosionName.clear();
	vecFrame.clear();

	PUN::CTransform *pTransform = m_pGunSmoke->GetTransform();

	m_pGunSmoke->SetEnable(false);
	pTransform->SetWorldScale(0.8f, 0.8f, 0.8f);
	
	SAFE_RELEASE(pTransform);

	//Gun Muzzle
	m_pMuzzle = PUN::CGameObject::CreateObject("gunMuzzle", m_pLayer, true);
	m_pMuzzle->SetFrustrumCullUse(false);
	PUN::CColliderOBB3D *pCol = nullptr;
	//pCol = m_pMuzzle->AddComponent<PUN::CColliderOBB3D>("col");
	//pCol->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(.03125f, .0625f, .125f));
	//SAFE_RELEASE(pCol);

	pParticle = m_pMuzzle->AddComponent<PUN::CParticle>("particle");
	SAFE_RELEASE(pParticle);
	pRenderer = m_pMuzzle->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	pRenderer->SetMesh("Particle_BlackMat");
	SAFE_RELEASE(pRenderer);
	pParticleMtrl = m_pMuzzle->FindComponentFromType<PUN::CMaterial>(PUN::CT_MATERIAL);
	
	for (int i = 1; i <= 4; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(pFileName, TEXT("Particles\\MuzzleFlash\\MuzzleFlash%d.png"), i);
		vecExplosionName.push_back(pFileName);

		PUN::Clip2DFrame	tFrame = {};
		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(1.f, 1.f);
		vecFrame.push_back(tFrame);
	}
	pParticleMtrl->SetDiffuseTex(4, "GunMuzzle", vecExplosionName);

	pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pParticleMtrl);
	pParticleAnimation = m_pMuzzle->AddComponent<PUN::CAnimation2D>("ParticleAnimation");

	pParticleAnimation->AddClip("MuzzleFlash", PUN::A2D_FRAME, PUN::AO_ONCE_DISABLE,
		.089999999999999999f, vecFrame, "GunMuzzle", vecExplosionName);

	SAFE_RELEASE(pParticleAnimation);

	for (size_t i = 0; i < vecExplosionName.size(); ++i)
	{
		SAFE_DELETE_ARRAY(vecExplosionName[i]);
	}

	vecExplosionName.clear();
	vecFrame.clear();

	pTransform = m_pMuzzle->GetTransform();

	pTransform->SetWorldScale(1.f, 1.f, 1.f);
	//pTransform->SetLocalPos(0.f, 1.f, 0.f);
	//pTransform->SetWorldPivot(0.f, -10.f, 0.f);

	SAFE_RELEASE(pTransform);

	m_pGunMuzzleFlash = PUN::CGameObject::CreateObject("muzzle_light", m_pLayer, true);
	
	PUN::CLight *pLight = m_pGunMuzzleFlash->AddComponent<PUN::CLight>("muzzleFlash");
	pLight->SetLightDiffuse(Vector4(1.f, 0.75f, 0.25f, 0.75f));
	pLight->SetLightSpcular(Vector4(0.8f, 0.8f, 0.8f, 0.8f));
	pLight->SetLightType(PUN::LT_SPOT);
	pLight->SetLightRange(250.f);
	pLight->SetAngle(60.f, 100.f);
	SAFE_RELEASE(pLight);
	
	m_pGunMuzzleFlash->SetEnable(false);
	m_pMuzzle->SetEnable(false);

	m_pGunPointFake = PUN::CGameObject::CreateObject("GunPoint", m_pLayer, true);

	m_pAnim->AddSocket("Barrel", "GunPoint");
	m_pAnim->SetSocketObject("Barrel", "GunPoint", m_pGunPointFake);
	//pCol = m_pGunPointFake->AddComponent<PUN::CColliderOBB3D>("col");
	//pCol->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(.03125f, .0625f, .125f));
	//SAFE_RELEASE(pCol);

	//m_pGunSmoke->SetEnable(false);

	tRecoilEff.fEffectTime = 0.125f;
	tRecoilEff.funcEffect = std::bind(&CPistol::Recoil, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	PUN::CGameObject *pBulletCaseObj = nullptr;
	std::string strBulletCase = "bulletCase";
	std::string strBulletCaseIdx;
	for (int i = 0; i < iBulletCaseCnt; ++i)
	{
		strBulletCaseIdx = strBulletCase + std::to_string(i + 1);

		pBulletCaseObj = PUN::CGameObject::CreateObject("BCase_Prot", m_pLayer, true);
		
		m_arrBulletCases[i] = pBulletCaseObj->AddComponent<CBulletCase>("bulletCase");
		pBulletCaseObj->SetEnable(false);
		SAFE_RELEASE(pBulletCaseObj);
	}
}

bool CPistol::Fire()
{
	if (m_fFireTimer >= m_fFireReloadSpd)
	{
		m_fFireTimer = 0.f;
		if (m_pRay)
		{
			m_pRay->SetEnable(true);
		}

		//PUN::CAnimation *pAnim = FindComponentFromType<PUN::CAnimation>(PUN::CT_ANIMATION);
		m_pAnim->ChangeClip("fire");

		//SAFE_RELEASE(pAnim);

		int iRand = rand() % 4;

		if (m_pSnd->GetClipState(iRand) == PLAYING)
		{
			m_pSnd->StopClip(iRand);
		}

		float fInten = _PLAYER->GetBreathIntensity();
		
		if(fInten < 2.f)
			fInten += 0.15f;

		_PLAYER->SetBreathIntensity(fInten);
		//std::cout << "intensity set to " << fInten << std::endl;

		m_pSnd->Play(iRand);
		//m_pGunSmoke->SetEnable(true);
		m_fSmokeTimerBuf = 0.f;
		m_pMuzzle->SetEnable(true);
		m_pGunMuzzleFlash->SetEnable(true);
		CCameraEff::GetInst()->AddUpdateEffect(tRecoilEff);

		if (m_arrBulletCases[m_iCurrBulletCaseIdx])
		{
			PUN::CTransform *pTrans = m_pGunPointFake->GetTransform();
			Vector3 vPos = pTrans->GetWorldPos();
			SAFE_RELEASE(pTrans);
			
			pTrans = m_pScene->GetMainCameraTransform();
			Vector3 vRot = pTrans->GetWorldRot();
			SAFE_RELEASE(pTrans);

			m_arrBulletCases[m_iCurrBulletCaseIdx]->SetInstance(vPos, vRot);
		}
		++m_iCurrBulletCaseIdx;
		if (m_iCurrBulletCaseIdx >= iBulletCaseCnt)
			m_iCurrBulletCaseIdx = 0;
		//น฿ป็
		return true;
	}
	else
		return false;
}

void CPistol::DrawOut()
{
	m_pObject->SetEnable(true);
	SetEnable(true);
	m_pSnd->Play(4);
}

void CPistol::HitScan(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
}

void CPistol::Recoil(float fTime, float fCurrTime, float fEndTime, PUN::CTransform* pCam)
{
	
	Vector3 vCurrPos = pCam->GetWorldRot();

	if (vCurrPos.x < -75.5f)
		return;

	float fRate = fCurrTime / fEndTime;
	float fCos = cosf(fRate * PUN_PI * 0.75f) * 96.f ;
	
	vCurrPos.x -= fCos * fTime;
	if (vCurrPos.x <= -75.5f)
		vCurrPos.x = -75.5f;

	pCam->SetWorldRot(vCurrPos);
}
