#include "../ClientHeader.h"
#include "Human_Player.h"
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "../CommonSoundLoader.h"
#include "Component/ColliderOBB3D.h"
#define EPSILON 0.0000019073486328125f
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
	
	if (m_cInitLoopFinished > 0)
		return;

	if (m_iState & PSTATUS_BED)
		return;
	if (m_iState & PSTATUS_LOCKER)
		return;

	if (m_iState & PSTATUS_VAULT)
	{
		if (m_pAnimation->GetCurrentClip()->strName != "player_stand_idle")
			return;
	}

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


	if (pDestCol->GetColliderID() == UCI_DUCT)
	{
		
		return;
	}


	if (pDestCol->GetColliderID() > (PUN::COLLIDER_TYPE)UCI_LOCKER)
	{
		return;
	}

	if (pDestCol->GetColliderID() <= (PUN::COLLIDER_TYPE)UCI_NONE)
	{
		return;
	}

	CHuman_Player *pHuman = pDestCol->FindComponentFromType<CHuman_Player>((PUN::COMPONENT_TYPE)UT_PLAYER);
	if (pHuman)
	{
		SAFE_RELEASE(pHuman);
		return;
	}

	PUN::OBB3DInfo _tDestInfo = pDestCol->GetInfo();
	PUN::OBB3DInfo _tSrcInfo = pSrcCol->GetInfo();

	Vector3 vRes;
	Vector3 vMaxPoint;
	Vector3 vMaxPointTangent;
	Vector3 vMinPoint;
	Vector3 vMinPointTanget;


	std::vector<PUN::CColliderOBB3D*>::iterator Itr = m_vecCollidingGeom.begin();
	std::vector<PUN::CColliderOBB3D*>::iterator ItrEnd = m_vecCollidingGeom.end();

	bool bHasIt = false;
	for (; Itr != ItrEnd; ++Itr)
	{
		if (*Itr == pDestCol)
		{
			bHasIt = true;
			break;
		}
	}
	if (!bHasIt)
		m_vecCollidingGeom.push_back(pDestCol);

	if (!m_bGeomTest)
	{
		PUN::CTransform *pSrcTr = pSrcCol->GetTransform();
		PUN::CTransform *pDestTr = pDestCol->GetTransform();

		pSrcTr->Update(fTime);
		pDestTr->Update(fTime);

		pSrcTr->LateUpdate(fTime);
		pDestTr->LateUpdate(fTime);

		SAFE_RELEASE(pSrcTr);
		SAFE_RELEASE(pDestTr);
	}
	
	//콜라이더 가운데 위치 알아내기
	Vector3 vDestCenter = _tDestInfo.vCenter;
	Vector3 vSrcCenter = _tSrcInfo.vCenter;

	//두 콜라이더 사이의 방향벡터 구하기
	Vector3 vVectorDiffUnNorm = vSrcCenter - vDestCenter;
	Vector3 vVectorDiff = vVectorDiffUnNorm;
	Vector3 vVectorBuf;
	Vector3 vVectorDirBuf;
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

	float fVectorDotMax = -FLT_MAX;
	float fVectorDotMin = FLT_MAX;
	float fVectorDotBuf = 0.f;

	for (char i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			vVectorBuf = Vector3(_tDestInfo.vLength.x, _tDestInfo.vLength.y, _tDestInfo.vLength.z);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;
			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
			{
				fVectorDotMin = fVectorDotBuf;
				
			}
				
			break;
		case 1:
			vVectorBuf = Vector3(_tDestInfo.vLength.x, _tDestInfo.vLength.y, _tDestInfo.vLength.z * -1.f);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;
			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
			{
				fVectorDotMin = fVectorDotBuf;
				
			}
				
			break;
		case 2:
			vVectorBuf =Vector3(_tDestInfo.vLength.x, _tDestInfo.vLength.y * -1.f, _tDestInfo.vLength.z);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
			{
				fVectorDotMin = fVectorDotBuf;
				
			}
			break;
		case 3:
			vVectorBuf = Vector3(_tDestInfo.vLength.x, _tDestInfo.vLength.y * -1.f, _tDestInfo.vLength.z * -1.f);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
			{
				fVectorDotMin = fVectorDotBuf;
				
			}
			break;
		case 4:
			vVectorBuf = Vector3(_tDestInfo.vLength.x * -1.f, _tDestInfo.vLength.y, _tDestInfo.vLength.z);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
			{
				fVectorDotMin = fVectorDotBuf;
			}
			break;
		case 5:
			vVectorBuf = Vector3(_tDestInfo.vLength.x * -1.f, _tDestInfo.vLength.y, _tDestInfo.vLength.z * -1.f);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
				fVectorDotMin = fVectorDotBuf;
			break;
		case 6:
			vVectorBuf = Vector3(_tDestInfo.vLength.x * -1.f, _tDestInfo.vLength.y * -1.f, _tDestInfo.vLength.z);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);
			if (fVectorDotBuf < fVectorDotMin)
				fVectorDotMin = fVectorDotBuf;
			break;
		case 7:
			vVectorBuf = Vector3(_tDestInfo.vLength.x * -1.f, _tDestInfo.vLength.y * -1.f, _tDestInfo.vLength.z *-1.f);
			vVectorDirBuf = Vector3::Zero;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
			vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

			fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf); 
			if (fVectorDotBuf < fVectorDotMin)
				fVectorDotMin = fVectorDotBuf;
			break;
		}
	}

	Vector3 vDestPoint = vVectorDiff * (fVectorDotMax / fMagnitude);
	//pSrc-> 방향벡터 쪽으로 최대값 가지는 정점 구하기
	fVectorDotMax = -FLT_MAX;
	fVectorDotBuf = 0.f;
	
	for (char i = 0; i < 8; ++i)
	{
		float fSign = 1.f;
		switch (i)
		{
		case 0:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x , _tSrcInfo.vLength.y , _tSrcInfo.vLength.z );
							
			break;
		case 1:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x , _tSrcInfo.vLength.y , _tSrcInfo.vLength.z * -1.f);
			
			break;
		case 2:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x , _tSrcInfo.vLength.y * -1.f, _tSrcInfo.vLength.z );
			fSign = -1.f;
			break;
		case 3:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x , _tSrcInfo.vLength.y * -1.f, _tSrcInfo.vLength.z * -1.f);
			fSign = -1.f;
			break;
		case 4:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x * -1.f, _tSrcInfo.vLength.y , _tSrcInfo.vLength.z );
			
			break;
		case 5:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x * -1.f, _tSrcInfo.vLength.y , _tSrcInfo.vLength.z * -1.f);
			
			break;
		case 6:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x * -1.f, _tSrcInfo.vLength.y * -1.f, _tSrcInfo.vLength.z );
			fSign = -1.f;
			break;
		case 7:
			vVectorBuf = Vector3(_tSrcInfo.vLength.x * -1.f, _tSrcInfo.vLength.y * -1.f, _tSrcInfo.vLength.z *-1.f);
			fSign = -1.f;
		}

		vVectorDirBuf = Vector3::Zero;
		vVectorDirBuf += _tDestInfo.vAxis[AXIS_X] * vVectorBuf.x;
		vVectorDirBuf += _tDestInfo.vAxis[AXIS_Y] * vVectorBuf.y;
		vVectorDirBuf += _tDestInfo.vAxis[AXIS_Z] * vVectorBuf.z;

		fVectorDotBuf = vVectorDiff.Dot(vVectorDirBuf);

		if (fVectorDotBuf > fVectorDotMax)
		{
			fVectorDotMax = fVectorDotBuf;
			vMaxPoint = vVectorDirBuf;
			vMaxPointTangent = vMaxPoint;
			vMaxPointTangent = _tDestInfo.vAxis[AXIS_Y] * (2.f * fSign);
		}
	}

	Vector3 vSrcPoint = vVectorDiff * (fVectorDotMax / fMagnitude);

	//두 정점 사이를 0으로 만드는 벡터값 구하기
	float fSigned = fVectorDotMax + fVectorDotMin;
	Vector3 vVelocity = m_pMovePointer->GetWorldPos() - m_vPrevWorldPos;
	float fVelocityScale = vVelocity.Length();
	//vRes = vVectorDiff * (-abs(fSigned));
	vRes = vVectorDiff;
	vRes.y = 0.f;
	float fBias = 0.225f;
	if (m_iState &PSTATUS_VAULT)
		fBias = 2.75f;
	vRes *= (fVelocityScale + abs(fSigned)) * fBias;
	vRes -= vVelocity * 0.375f;

	if (vRes != Vector3::Zero)
		m_cMoveCheckFlag |= 2;
	
	//그만큼 밀기
	if (m_bNaviOn)
		PlayerMove_CheckNav(vRes);
	else
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

	if (pDestCol->GetColliderID() == UCI_DUCT)
	{
		if (m_iState & PSTATUS_DUCT)
			m_iState ^= PSTATUS_DUCT;

		if (!PUN::CInput::GetInst()->KeyPush("Ctrl"))
		{
			if (m_iState & PSTATUS_CROUCHED)
			{
				m_iState |= PSTATUS_CROUCHING;
				m_iState ^= PSTATUS_CROUCHED;
			}
		}
	}

	std::vector<PUN::CColliderOBB3D*>::iterator Itr = m_vecCollidingGeom.begin();
	std::vector<PUN::CColliderOBB3D*>::iterator ItrEnd = m_vecCollidingGeom.end();
	
	for (; Itr != ItrEnd; ++Itr)
	{
		if (*Itr == pDestCol)
		{
			m_vecCollidingGeom.erase(Itr);
			break;
		}
	}

	//std::cout << "OBB Exit call" << std::endl;

}

void CHuman_Player::PlayerMove_CheckNav(Vector3 vMove) 
{
	

	Vector3 vPos = m_pMovePointer->GetWorldPos();
	int iRes = Move(vMove);

	if (m_cMoveCheckFlag & 2)
	{
		
		if (iRes > 1)
		{
			m_pMovePointer->SetWorldPos(m_vPrevWorldPos);
			return;
		}
	}
	Vector3 vAfterPos = m_pMovePointer->GetWorldPos();
	Vector3 vRealMove = vAfterPos - vPos;
	
	if (vRealMove == Vector3::Zero)
		return;

	PlayerMove(vRealMove);
}