#include "../ClientHeader.h"
#include "Pistol.h"
#include <Component/Renderer.h>

CPistol::CPistol():
	m_pRay(nullptr),
	m_fFireReloadSpd(0.5f),
	m_fFireTimer(0.5f),
	m_pSnd(nullptr),
	m_pAnim(nullptr)
{
}

CPistol::CPistol(const CPistol & pistol):
	PUN::CUserComponent(pistol)
{
}

CPistol::~CPistol()
{
	SAFE_RELEASE(m_pRay);
	SAFE_RELEASE(m_pSnd);
	SAFE_RELEASE(m_pAnim);
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
	return 0;
}

int CPistol::LateUpdate(float fTime)
{
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
	vecSndList.resize(4);
	vecSndList[0] = "Gun_Fire1";
	vecSndList[1] = "Gun_Fire2";
	vecSndList[2] = "Gun_Fire3";
	vecSndList[3] = "Gun_Fire4";


	m_pSnd = FindComponentFromTag<PUN::CSoundSource>("Sound");
	if (!m_pSnd)
	{
		m_pSnd = AddComponent<PUN::CSoundSource>("Sound");
	}

	m_pSnd->LoadSounds(vecSndList);
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

		m_pSnd->Play(iRand);
		//น฿ป็
		return true;
	}
	else
		return false;
}

void CPistol::HitScan(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
}
