#include "../ClientHeader.h"
#include "Human_Player.h"
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "../CommonSoundLoader.h"
#include "Component/ColliderOBB3D.h"

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

bool CHuman_Player::Init_Move()
{
	return true;
}

void CHuman_Player::OnDestroyMove()
{
	
}

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
	//m_vTracerBuf = m_pRootBonePos->GetWorldPos();
}

int CHuman_Player::Input_Move(float fTime) 
{
	return 0;
}
int CHuman_Player::MoveUpdate(float fTime) 
{
	return 0;
}
int CHuman_Player::MoveLateUpdate(float fTime) 
{
	return 0;
}


void CHuman_Player::Geometry_Push(CCollider *pSrc, CCollider *pDest, float fTime)
{

	if (m_iState & PSTATUS_BED)
		return;
	if (m_iState & PSTATUS_LOCKER)
		return;


	PUN::CColliderOBB3D * pSrcCol = nullptr;
	PUN::CColliderOBB3D * pDestCol = nullptr;

	if (pSrc->GetTag() == "PlayerGeom")
	{
		pSrcCol = dynamic_cast<PUN::CColliderOBB3D*>(pSrc);
		pDestCol = dynamic_cast<PUN::CColliderOBB3D*>(pDest);
	}
	else if (pDest->GetTag() == "PlayerGeom")
	{
		pDestCol = dynamic_cast<PUN::CColliderOBB3D*>(pSrc);
		pSrcCol = dynamic_cast<PUN::CColliderOBB3D*>(pDest);
	}

	if (!pDestCol)
		return;
	if (!pSrcCol)
		return;

	if (pDestCol->GetColliderID() > (PUN::COLLIDER_TYPE)UCI_LOCKER)
	{
		return;
	}

	CHuman_Player *pHuman = pDestCol->FindComponentFromType<CHuman_Player>((PUN::COMPONENT_TYPE)UT_PLAYER);
	if (pHuman)
	{
		SAFE_RELEASE(pHuman);
		return;
	}

	PUN::CTransform *pSrcTr = pSrcCol->GetTransform();
	PUN::CTransform *pDestTr = pDestCol->GetTransform();

	PUN::OBB3DInfo _tDestInfo = pDestCol->GetInfo();
	PUN::OBB3DInfo _tSrcInfo = pSrcCol->GetInfo();

	//콜라이더 가운데 위치 알아내기
	Vector3 vDestCenter = pDestTr->GetWorldPos() + _tDestInfo.vCenter;
	Vector3 vSrcCenter = pSrcTr->GetWorldPos() + _tSrcInfo.vCenter;

	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);
	//두 콜라이더 사이의 방향벡터 구하기
	Vector3 vVectorDiffUnNorm = vDestCenter - vSrcCenter;//vSrcCenter에서 vDestCenter
	Vector3 vVectorDiff = vVectorDiffUnNorm;
	vVectorDiff.Normalize();

	//Vector3 vVelocityNorm = m_vVelocity;
	//vVelocityNorm.Normalize();

	float fMagnitude = 1.f;
	if (vVectorDiff.x != 0.f)
		fMagnitude = vVectorDiffUnNorm.x / vVectorDiff.x;
	else if (vVectorDiff.y != 0.f)
		fMagnitude = vVectorDiffUnNorm.y / vVectorDiff.y;
	else if (vVectorDiff.z != 0.f)
		fMagnitude = vVectorDiffUnNorm.z / vVectorDiff.z;
	//pDest-> 방향벡터 쪽으로 최소값 가지는 정점 구하기

	float fVectorDotMax = FLT_MAX;
	float fVectorDotBuf = 0.f;

	for (char i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * 0.5f, _tDestInfo.vLength.y * 0.5f, _tDestInfo.vLength.z * 0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 1:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * 0.5f, _tDestInfo.vLength.y * 0.5f, _tDestInfo.vLength.z * -0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 2:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * 0.5f, _tDestInfo.vLength.y * -0.5f, _tDestInfo.vLength.z * 0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 3:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * 0.5f, _tDestInfo.vLength.y * -0.5f, _tDestInfo.vLength.z * -0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 4:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * -0.5f, _tDestInfo.vLength.y * 0.5f, _tDestInfo.vLength.z * 0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 5:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * -0.5f, _tDestInfo.vLength.y * 0.5f, _tDestInfo.vLength.z * -0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 6:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * -0.5f, _tDestInfo.vLength.y * -0.5f, _tDestInfo.vLength.z * 0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 7:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tDestInfo.vLength.x * -0.5f, _tDestInfo.vLength.y * -0.5f, _tDestInfo.vLength.z *-0.5f));
			if (fVectorDotBuf < fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		}
	}

	Vector3 vDestPoint = vVectorDiff * (fVectorDotMax / fMagnitude);
	//pSrc-> 방향벡터 쪽으로 최대값 가지는 정점 구하기
	fVectorDotMax = -FLT_MAX;
	fVectorDotBuf = 0.f;
	for (char i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * 0.5f, _tSrcInfo.vLength.y * 0.5f, _tSrcInfo.vLength.z * 0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 1:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * 0.5f, _tSrcInfo.vLength.y * 0.5f, _tSrcInfo.vLength.z * -0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 2:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * 0.5f, _tSrcInfo.vLength.y * -0.5f, _tSrcInfo.vLength.z * 0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 3:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * 0.5f, _tSrcInfo.vLength.y * -0.5f, _tSrcInfo.vLength.z * -0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 4:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * -0.5f, _tSrcInfo.vLength.y * 0.5f, _tSrcInfo.vLength.z * 0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 5:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * -0.5f, _tSrcInfo.vLength.y * 0.5f, _tSrcInfo.vLength.z * -0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 6:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * -0.5f, _tSrcInfo.vLength.y * -0.5f, _tSrcInfo.vLength.z * 0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		case 7:
			fVectorDotBuf = vVectorDiff.Dot(Vector3(_tSrcInfo.vLength.x * -0.5f, _tSrcInfo.vLength.y * -0.5f, _tSrcInfo.vLength.z *-0.5f));
			if (fVectorDotBuf > fVectorDotMax)
				fVectorDotMax = fVectorDotBuf;
			break;
		}
	}

	Vector3 vSrcPoint = vVectorDiff * (fVectorDotMax / fMagnitude);

	//두 정점 사이를 0으로 만드는 벡터값 구하기
	Vector3 vRes = vDestPoint - vSrcPoint;
	vRes.y = 0.f;
	//vRes *= f;
	//vRes -= m_vVelocity;
	//그만큼 밀기

	PlayerMove(vRes);
}

void CHuman_Player::Geometry_Out(CCollider *pSrc, CCollider *pDest, float fTime)
{

	PUN::CColliderOBB3D * pSrcCol = nullptr;
	PUN::CColliderOBB3D * pDestCol = nullptr;

	if (pSrc->GetTag() == "PlayerGeom")
	{
		pSrcCol = dynamic_cast<PUN::CColliderOBB3D*>(pSrc);
		pDestCol = dynamic_cast<PUN::CColliderOBB3D*>(pDest);
	}
	else if (pDest->GetTag() == "PlayerGeom")
	{
		pDestCol = dynamic_cast<PUN::CColliderOBB3D*>(pSrc);
		pSrcCol = dynamic_cast<PUN::CColliderOBB3D*>(pDest);
	}

	if (!pDestCol)
		return;
	if (!pSrcCol)
		return;

	//std::cout << "OBB Exit call" << std::endl;

}