#include "../ClientHeader.h"
#include "Human_Player.h"
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "../CommonSoundLoader.h"

/*
방향 알아내기 : const Vector3& GetMoveDirection()
정속 속도 : Vector3에 1.f 출력
앉기 속도 : Vector3에 0.5f 출력
달리기 속도 : Vector3에 2.f 출력(앞으로 달리기만 해당)
*/

#define RUN_STEP1 0.3f
#define RUN_STEP2 0.7333333334f
#define STAND_WALK_FORWARD2 0.6666666667f

#define CROUCH_CLOTHES_BIG 0.2f
#define CROUCH_CLOTHES_LIGHT 0.5f


void CHuman_Player::OnCrouching(float fTime) 
{

}
void CHuman_Player::Pakur_Doing(float fTime, int iHeightType)
{

}
void CHuman_Player::OnWalk(float fTime)
{

}
void CHuman_Player::OnSprint(float fTime)
{
	float fAnimTime = m_pAnimation->GetCurrentClipTime();
	float fPrevTime = fAnimTime - fTime;
	
	if (fAnimTime > RUN_STEP1 && fPrevTime < RUN_STEP1 || fAnimTime > RUN_STEP2 && fPrevTime < RUN_STEP2)
	{
		std::string strKey = m_strFTSKey + "Run";
		int iRandNum = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strKey);

		if (iRandNum > 1)
		{
			iRandNum = (rand() % iRandNum) + 1;
			strKey += std::to_string(iRandNum);
		}
		
		//m_pSound->StopClip(m_strFTSKey);
		m_pSound->Play(strKey);
	}

}
void CHuman_Player::OnCrouchWalk(float fTime)
{
	float fAnimTime = m_pAnimation->GetCurrentClipTime();
	float fPrevTime = fAnimTime - fTime;

	if (fAnimTime > CROUCH_CLOTHES_BIG && fPrevTime < CROUCH_CLOTHES_BIG)
	{
		std::string strKey = "otStrong";
		int iRandNum = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strKey);

		if (iRandNum > 1)
		{
			iRandNum = (rand() % iRandNum) + 1;
			strKey += std::to_string(iRandNum);
		}

		//m_pSound->StopClip(m_strFTSKey);
		m_pSound->Play(strKey);
	}

	if (fAnimTime > CROUCH_CLOTHES_LIGHT && fPrevTime < CROUCH_CLOTHES_LIGHT)
	{
		std::string strKey = "otSoft";
		int iRandNum = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strKey);

		if (iRandNum > 1)
		{
			iRandNum = (rand() % iRandNum) + 1;
			strKey += std::to_string(iRandNum);
		}

		//m_pSound->StopClip(m_strFTSKey);
		m_pSound->Play(strKey);
	}
}

void CHuman_Player::FootStepWalkNormal(float fTime)
{
	float fAnimTime = m_pAnimation->GetCurrentClipTime();
	float fPrevTime = fAnimTime - fTime;

	if (fAnimTime > RUN_STEP1 && fPrevTime < RUN_STEP1 || fAnimTime > STAND_WALK_FORWARD2 && fPrevTime < STAND_WALK_FORWARD2)
	{
		std::string strKey = m_strFTSKey + "Walk";
		int iRandNum = CCommonSoundLoader::GetInst()->GetSoundRandomCnt(strKey);

		if (iRandNum > 1)
		{
			iRandNum = (rand() % iRandNum) + 1;
			strKey += std::to_string(iRandNum);
		}

		m_pSound->Play(strKey);
	}

}

void CHuman_Player::FootStepRun(float fTime)
{

}

void CHuman_Player::FootStepCrouch(float fTime)
{

}

void CHuman_Player::FootStepFallDownLow(float fTime)
{

}

void CHuman_Player::FootStepFallDownHigh(float fTime)
{

}

void CHuman_Player::CrouchForwardMendPos(float fTime)
{
	//std::cout << "hahahaha" << std::endl;
	m_vTracerBuf = m_pRootBonePos->GetWorldPos();
}

void CHuman_Player::CrouchBackwardMendPos(float fTime)
{

}

void CHuman_Player::CrouchSideMendPos(float fTime)
{

}
