#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Inventory.h"
#include "Component/ColliderOBB3D.h"
#include "Resource/Mesh.h"
#include "Scene/SceneManager.h"
#include "Door.h"
#include "Locker.h"
#include "../GameManager.h"

static float fx;
static float fy;
static float fz;


bool CHuman_Player::Init_Interact()
{
	CRenderer* pRD = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	CColliderOBB3D* pOBB = m_pObject->AddComponent<CColliderOBB3D>("Player_Interact");
	pOBB->SetMyTypeName("Player_Interact");
	pOBB->SetContinueTypeName("MouseRay");

	pOBB->SetColliderID(UCI_PLAYER_INTERACT);

	CMesh* pMesh = pRD->GetMesh();

	Vector3 vScale = pMesh->GetLength();

	Vector3 vMax = pMesh->GetMax();
	Vector3 vMin = pMesh->GetMin();

	Vector3 vLength;

	vLength.x = 1.0f;
	vLength.y = fabs(vMax.y - vMin.y);
	vLength.z = fabs(vMax.z - vMin.z);

	pOBB->SetInfo(Vector3(0.0f, 0.0f, 20.0f), Vector3::Axis, vLength * 0.05f);
	pOBB->SetCollisionCallback(CCT_ENTER, this, &CHuman_Player::InteractCallBackEnter);
	pOBB->SetCollisionCallback(CCT_STAY, this, &CHuman_Player::InteractCallBackStay);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CHuman_Player::InteractCallBackLeave);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pOBB);
	SAFE_RELEASE(pRD);

	GET_SINGLE(CInput)->AddKey("KEY_1", '1');
	GET_SINGLE(CInput)->AddKey("KEY_2", '2');
	GET_SINGLE(CInput)->AddKey("KEY_3", '3');
	GET_SINGLE(CInput)->AddKey("KEY_4", '4');
	GET_SINGLE(CInput)->AddKey("KEY_5", '5');
	GET_SINGLE(CInput)->AddKey("KEY_6", '6');
	GET_SINGLE(CInput)->AddKey("KEY_7", '7');

	fx = 0.0f;
	fy = 0.0f;
	fz = 0.0f;

	m_pCameraObj = CSceneManager::GetInst()->GetMainCameraObj();
	m_pCameraTr = m_pCameraObj->GetTransform();

	if (!m_pCameraObj)
		return false;
	if (!m_pCameraTr)
		return false;

	m_pAnimation->SetClipUseBoneTransform("player_enter_bed_left_stand", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_enter_bed_right_stand", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_exit_bed_left", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_exit_bed_right", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_exit_bed_left_crouch", "Hero-Pelvis");
	m_pAnimation->SetClipUseBoneTransform("player_exit_bed_right_crouch", "Hero-Pelvis");
	
	return true;
}


void CHuman_Player::OnDestroyInteract()
{
	SAFE_RELEASE(m_pCameraTr);
	SAFE_RELEASE(m_pCameraObj);

}

void CHuman_Player::Interact(float fTime) 
{
	
}

void CHuman_Player::Open_Door_Normal(float fTime)
{
	
}
void CHuman_Player::Close_Door_Normal(float fTime)
{

}
void CHuman_Player::Open_Door_Fast(float fTime)
{

}
void CHuman_Player::Close_Door_Fast(float fTime)
{

}
void CHuman_Player::Hide_Locker(float fTime)
{
	if (m_pAnimation->GetCurrentClip()->strName == "player_locker_inside_idle")
	{
		HidingMotionEnd(fTime);
	}
}
void CHuman_Player::Hiding_Locker(float fTime)
{
	m_pAnimation->ChangeClip("player_locker_inside_idle");
}
void CHuman_Player::Exit_Locker(float fTime)
{
	m_pAnimation->ChangeClip("player_locker_exit");
	if (m_pAnimation->GetCurrentClip()->strName == "player_locker_exit")
	{
		if (m_pAnimation->GetCurrentClip()->fTimeLength - m_pAnimation->GetCurrentClipTime() < 0.3f)
		{
			HidingMotionEnd(fTime);
		}
	}
}
void CHuman_Player::Hide_Bed(float fTime)
{
	//m_eTempPlayerState = (PLAYER_STATUS)m_iState;
	//m_iState = PSTATUS_NONE;

	float fAngle = m_pTransform->GetWorldRot().y - m_vTargetDir.y;

	if (fAngle > 0.f && fAngle <= 180.0f)
	{
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_enter_bed_left");
		}
		else
		{
			m_pAnimation->ChangeClip("player_enter_bed_left_stand");
		}
		
		m_iRotDir = -1;
	}
	else if (fAngle <= .0f && fAngle > -180.0f)
	{
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_enter_bed_right");
		}
		else
		{
			m_pAnimation->ChangeClip("player_enter_bed_right_stand");
		}
		
		m_iRotDir = 1;
	}
	else
	{
		//m_iState = m_eTempPlayerState; //이거 누구냐....
		return;
	}
	
	//m_fDestRotY = fAngle;
	//m_fDestRotX = m_pCameraTr->GetWorldRot().x;
	//m_fAccRotX = 0.0f;
	//m_fAccRotY = 0.0f;

	if (m_pAnimation->GetCurrentClip()->strName.find("player_enter_bed") != std::string::npos)
	{
		if (m_pAnimation->GetCurrentClipTime() > 0.8f)
		{
			HidingMotionEnd(fTime);
		}
	}
	
	//PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();
	//m_fAnimPlayTime = 1.0f / pClip->fPlayTime;
	
}


void CHuman_Player::Hiding_Bed(float fTime)
{
	m_pAnimation->ChangeClip("player_bed_idle");
	
	if (m_iState & PSTATUS_CROUCHED)
	{
		m_iState ^= PSTATUS_CROUCHED;
		m_iState |= PSTATUS_CROUCHING;
	}
}
void CHuman_Player::Exit_Bed(float fTime) 
{
	PUN::CInput *_Input = PUN::CInput::GetInst();
	if (m_iRotDir == 1) //left
	{
		if (_Input->KeyPush("Ctrl"))
		{
			m_pAnimation->ChangeClip("player_exit_bed_right_crouch"); //들어온 데로 나와야 되는데 방향이 반대다?

		}
		else
		{

			m_pAnimation->ChangeClip("player_exit_bed_right"); //들어온 데로 나와야 되는데 방향이 반대다?
		}
	}
	else if (m_iRotDir == -1)
	{
		if (_Input->KeyPush("Ctrl"))
		{
			m_pAnimation->ChangeClip("player_exit_bed_left_crouch"); //들어온 데로 나와야 되는데 방향이 반대다?

		}
		else
		{
			m_pAnimation->ChangeClip("player_exit_bed_left");
		}
		
	}
	//m_pAnimation->ChangeClip("player_exit_bed_left");
	//m_fDestRotY = fAngle;
	//m_fAccRotY = 0.0f;

	if (m_pAnimation->GetCurrentClip()->strName.find("player_exit_bed") != std::string::npos)
	{
		if (m_pAnimation->GetCurrentClipTime() > 0.8f)
		{
			HidingMotionEnd(fTime);
		}
	}
	//m_fViewMaxAngleY = 90;
	//m_fViewMinAngleY = -90;
}

void CHuman_Player::CheckInteractState(float fTime)
{
	switch (m_eInteractState)
	{
	case IS_HIDE_IN_BED:
		StateHideInBed(fTime);
		break;

	case IS_HIDING_BED:
		StateHidingBed(fTime);
		break;

	case IS_GET_OUT_BED:
		StateGetOutBed(fTime);
		break;
	}
}
void CHuman_Player::ChangeInteractState(int iState, float fTime)
{
	m_eInteractState = (INTERACT_STATE)iState;
	switch(m_eInteractState)
	{
	case IS_HIDE_IN_BED:
		Hide_Bed(fTime);
		break;

	case IS_HIDING_BED:
		Hiding_Bed(fTime);
		break;

	case IS_GET_OUT_BED:
		Exit_Bed(fTime);
		break;
	}
}


void CHuman_Player::StateHideInBed(float fTime)
{
	if (m_pAnimation->IsCurAnimEnd("player_enter_bed_left_stand") || m_pAnimation->IsCurAnimEnd("player_enter_bed_right_stand"))
	{
		m_pCameraTr->SetWorldRotX(0.0f);
		Vector3 vRot = m_pCameraTr->GetWorldRot();
		m_pCameraTr->SetEnable(false);
		m_pTransform->SetWorldRot(vRot);
		ChangeInteractState(IS_HIDING_BED, fTime);
	}
	else
	{
		float fDeltaRotX = m_fDestRotX * m_fAnimPlayTime * fTime;
		float fDeltaRotY = m_fDestRotY * m_fAnimPlayTime * fTime;

		m_fAccRotX += fDeltaRotX;
		m_fAccRotY += fDeltaRotY;
		
		if(fabs(m_fAccRotX) < fabs(m_fDestRotX))
			m_pCameraTr->RotationX(fDeltaRotX * -1);
		if(fabs(m_fAccRotY) < fabs(m_fDestRotY))
			m_pCameraTr->RotationY(fDeltaRotY * m_iRotDir);
	}
}

void CHuman_Player::StateHidingBed(float fTime)
{
	if (m_pAnimation->IsCurAnimEnd("player_bed_idle"))
	{
	}

	//if (KEYDOWN("E"))
	//	ChangeInteractState(IS_GET_OUT_BED, fTime);
}

void CHuman_Player::StateGetOutBed(float fTime)
{
	if (m_pAnimation->IsCurAnimEnd("player_exit_bed_left") || m_pAnimation->IsCurAnimEnd("player_exit_bed_right"))
	{
		m_iState = PSTATUS_IDLE;
		ChangeInteractState(IS_NONE, fTime);
		m_pAnimation->ChangeClip("player_stand_idle");
	}
	else
	{
		
	}
}


int CHuman_Player::Input_Interact(float fTime)
{
	


	//if (KEYDOWN("KEY_1"))
	//{
	//	m_pAnimation->ChangeClip("player_enter_bed_left_stand");
	//	
	//}
	//else if (KEYDOWN("KEY_2"))
	//	m_pAnimation->ChangeClip("player_enter_bed_right_stand");
	//else if (KEYDOWN("KEY_3"))
	//	m_pAnimation->ChangeClip("player_bed_idle");
	//else if (KEYDOWN("KEY_4"))
	//	m_pAnimation->ChangeClip("player_bed_look_left_right");
	//else if (KEYDOWN("KEY_5"))
	//	m_pAnimation->ChangeClip("player_exit_bed_left_crouch");
	//else if (KEYDOWN("KEY_6"))
	//	m_pAnimation->ChangeClip("player_exit_bed_right_crouch");

	return 0;
}

int CHuman_Player::InteractUpdate(float fTime)
{
	
	//Vector3 vTestPos = m_pTransform->GetWorldPos();
	//std::cout << "test position : (" << vTestPos.x << ", " << vTestPos.y << ", " << vTestPos.z << ")" << std::endl;
	if (m_iState & PSTATUS_LOCKER)
	{
		if (m_iState & PSTATUS_HIDEINTERACT)
		{

		}
	}
	

	return 0;
}



int CHuman_Player::InteractLateUpdate(float fTime)
{
	CheckInteractState(fTime);

	//CTransform* pTr = m_pHeadObj->GetTransform();
	//Vector3 vMove = pTr->GetWorldMove();

	//fx += vMove.x;
	//fy += vMove.y;
	//fz += vMove.z;

	//SAFE_RELEASE(pTr);

	//if (m_pAnimation->IsCurAnimEnd("player_enter_bed_left_stand"))
	//{
	//	int a = 2;

	//	//m_pTransform->SetWorldPos(fx, fy, fz);
	//	m_pAnimation->ChangeClip("player_bed_idle");

	//	fx = 0.0f;
	//	fy = 0.0f;
	//	fz = 0.0f;
	//}

	return 0;
}

void CHuman_Player::HidingMotionEnd(float fTime) 
{
	if (m_iState & PSTATUS_HIDEINTERACT) 
	{
		m_iState ^= PSTATUS_HIDEINTERACT;
	}
	else if (m_iState & PSTATUS_HIDEINTERACT_OUT)
	{
		m_iState ^= PSTATUS_HIDEINTERACT_OUT;
		if (m_iState & PSTATUS_BED)
		{
			
			m_iState ^= PSTATUS_BED;
		}
		else if (m_iState & PSTATUS_LOCKER)
		{
			if ((m_iState & PSTATUS_CAMOUT) == 0)
			{
				if (m_pScene)
				{
					PUN::CTransform *pCamTr = m_pScene->GetMainCameraTransform();

					Vector3 vCamRot = pCamTr->GetWorldRot();
					Vector3 vWorldRot = m_pTransform->GetWorldRot();

					vCamRot.y = vWorldRot.y;

					pCamTr->SetWorldRot(vCamRot);

					SAFE_RELEASE(pCamTr);
				}
			}
			m_iState ^= PSTATUS_LOCKER;
		}
	}
};

void CHuman_Player::InputRot_Interact(float fTime)
{
	//락커나 침대 안에 있는 경우
	if (m_iState & PSTATUS_BED)
	{
		//각도 버퍼 얻어오기
		Vector3 vRot = m_pTransform->GetWorldRot();
		
		float vYdiff = vRot.y - m_vTargetDir.y;

		if (m_iState & PSTATUS_HIDEINTERACT)
		{
			if (vYdiff > m_fMaxHideBedAngleX)
			{
				float fSpeed = m_fMaxHideBedAngleX * fTime * 128.f;
				if (vRot.y + fSpeed > m_vTargetDir.y + m_fMaxHideBedAngleX)
				{
					fSpeed = m_vTargetDir.y + m_fMaxHideBedAngleX - vRot.y;
				}
				vYdiff = fSpeed + m_fMaxHideBedAngleX;
			}
			else if (vYdiff < -m_fMaxHideBedAngleX)
			{
				float fSpeed = -m_fMaxHideBedAngleX * fTime * 128.f;
				if (vRot.y + fSpeed < m_vTargetDir.y - m_fMaxHideBedAngleX)
				{
					fSpeed = m_vTargetDir.y - m_fMaxHideBedAngleX - vRot.y;
				}
				vYdiff = fSpeed - m_fMaxHideBedAngleX;
			}

			vRot.y = m_vTargetDir.y + vYdiff;

			m_pTransform->SetWorldRot(vRot);
		}
		else
		{
			if (vYdiff > m_fMaxHideBedAngleX)
			{
				vYdiff = m_fMaxHideBedAngleX;
				vRot.y = m_vTargetDir.y + vYdiff;

				m_pTransform->SetWorldRot(vRot);
			}
			else if (vYdiff < -m_fMaxHideBedAngleX)
			{
				vYdiff = -m_fMaxHideBedAngleX;
				vRot.y = m_vTargetDir.y + vYdiff;

				m_pTransform->SetWorldRot(vRot);
			}

			
		}
		
		PUN::CTransform *pCamera = m_pScene->GetMainCameraTransform();
		Vector3 vCamRot = pCamera->GetWorldRot();
		if (vCamRot.x < -m_fMaxHideBedAngleY)
		{
			float fSpeed = m_fMaxHideBedAngleY * fTime * 16.f;

			if (vCamRot.x + fSpeed > -m_fMaxHideBedAngleY)
			{
				vCamRot.x = -m_fMaxHideBedAngleY;
			}
			else
			{
				vCamRot.x += fSpeed;
			}
		}
		else if (vCamRot.x > m_fMaxHideBedAngleY)
		{
			float fSpeed = m_fMaxHideBedAngleY * fTime * 16.f;

			if (vCamRot.x - fSpeed < m_fMaxHideBedAngleY)
			{
				vCamRot.x = m_fMaxHideBedAngleY;
			}
			else
			{
				vCamRot.x -= fSpeed;
			}
		}

		vCamRot.y = vRot.y;
		pCamera->SetWorldRot(vCamRot);


		SAFE_RELEASE(pCamera);
	}
	else if (m_iState & PSTATUS_LOCKER)
	{
		if (m_iState & (PSTATUS_HIDEINTERACT | PSTATUS_HIDEINTERACT_OUT))
		{
			int aa = 0;
		}
		else
		{
			//각도 버퍼 얻어오기
			Vector3 vRot = m_pTransform->GetWorldRot();

			float vYdiff = vRot.y - m_vTargetDir.y;
			if (vYdiff > m_fMaxHideBedAngleX)
			{
				vYdiff = m_fMaxHideBedAngleX;
				vRot.y = m_vTargetDir.y + vYdiff;

				m_pTransform->SetWorldRot(vRot);
			}
			else if (vYdiff < -m_fMaxHideBedAngleX)
			{
				vYdiff = -m_fMaxHideBedAngleX;
				vRot.y = m_vTargetDir.y + vYdiff;

				m_pTransform->SetWorldRot(vRot);
			}
			PUN::CTransform *pCamera = m_pScene->GetMainCameraTransform();
			Vector3 vCamRot = pCamera->GetWorldRot();
			if (vCamRot.x < -m_fMaxHideBedAngleY)
			{
				float fSpeed = m_fMaxHideBedAngleY * fTime * 16.f;

				if (vCamRot.x + fSpeed > -m_fMaxHideBedAngleY)
				{
					vCamRot.x = -m_fMaxHideBedAngleY;
				}
				else
				{
					vCamRot.x += fSpeed;
				}
			}
			else if (vCamRot.x > m_fMaxHideBedAngleY)
			{
				float fSpeed = m_fMaxHideBedAngleY * fTime * 16.f;

				if (vCamRot.x - fSpeed < m_fMaxHideBedAngleY)
				{
					vCamRot.x = m_fMaxHideBedAngleY;
				}
				else
				{
					vCamRot.x -= fSpeed;
				}
			}

			vCamRot.y = vRot.y;
			pCamera->SetWorldRot(vCamRot);


			SAFE_RELEASE(pCamera);
		}
		
	}
}

void CHuman_Player::SetInteractRotationVector(const Vector3& vec)
{
	m_vTargetDir = vec;
}

void  CHuman_Player::Interact_With_Locker(CLocker *pLocker, float fTime) 
{
	//this >> 존내 잘 들어온다
	PUN::CInput *_Input = PUN::CInput::GetInst();
	PUN::CTransform *pLockerTr = pLocker->GetTransform();
	if (m_iState & PSTATUS_LOCKER)
	{
		
		if (m_iState & (PSTATUS_HIDEINTERACT | PSTATUS_HIDEINTERACT_OUT))
		{
			if (m_iState & PSTATUS_HIDEINTERACT)
			{
				if (m_pAnimation->GetCurrentClip()->strName == "player_locker_hide")
				{
					if (m_pAnimation->GetCurrentClip()->fPlayTime + 0.25f < m_pAnimation->GetCurrentClip()->fEndTime)
					{
						Vector3 vLockerAxisZ = pLockerTr->GetWorldAxis(PUN::AXIS_Z);
						Vector3 vOpenPos = pLockerTr->GetWorldPos() + (vLockerAxisZ * 2.75f);
						m_pTransform->SetWorldPos(vOpenPos);
					}
				}
				
			}
			//pLocker->OpenDoor();
		}
		else
		{

			if (m_pAnimation->GetCurrentClip()->strName == "player_locker_inside_idle")
			{
				
				Vector3 vLockerPos = pLockerTr->GetWorldPos();

				Vector3 vLockerZ = pLockerTr->GetWorldAxis(PUN::AXIS_Z);
				vLockerPos += (vLockerZ * (-1.05f));

				m_pTransform->SetWorldPos(vLockerPos);

				
			}
			
		}
				
	}
	else
	{
		float fLockerRotY = pLockerTr->GetWorldRot().y;
		fLockerRotY += 180.f;

		if (fLockerRotY > 180.f)
			fLockerRotY -= 360.f;

		Vector3 vBodyRot = m_pTransform->GetWorldRot();

		float fAngleDiff = vBodyRot.y - fLockerRotY;

		if (fAngleDiff > 180.f)
			fAngleDiff -= 360.f;
		else if (fAngleDiff < -180.f)
			fAngleDiff += 360.f;

		if (fabsf(fAngleDiff) <= 45.f)
		{
			
			if (_Input->KeyRelease("E"))
			{
				GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
				if (m_iState & PSTATUS_LOCKER)
				{
					
				}
				else
				{
					pLocker->OpenDoor();
					m_pHidingLocker = pLocker;
					m_iState |= (PSTATUS_HIDEINTERACT | PSTATUS_LOCKER);

					if (m_iState & PSTATUS_CROUCHED)
					{
						m_iState ^= PSTATUS_CROUCHED;
					}

					if (m_iState & PSTATUS_CROUCHING)
					{
						m_iState ^= PSTATUS_CROUCHING;
					}
					//
					m_pAnimation->ChangeClip("player_locker_hide");
					m_pAnimation->SetDefaultClip("player_locker_inside_idle");

				}

			}
			else
			{
				CGameManager::GetInst()->ChangeNoticeClip("Button_E_Hide");
			}
		}

		
	}
	

	SAFE_RELEASE(pLockerTr);
}

void CHuman_Player::Interact_Exit_Locker(CLocker *pLocker, float fTime)
{
	CGameManager::GetInst()->ChangeNoticeClip("Button_Empty");
}
void CHuman_Player::Interact_With_Door(CDoor *pDoor, float fTime)
{
	PUN::CGameObject *pDoorObj = pDoor->GetGameObject();

	if (!pDoorObj->GetParent())
	{
		CGameManager::GetInst()->ChangeNoticeClip("Button_E_Door");
	}

	SAFE_RELEASE(pDoorObj);
}
void CHuman_Player::Interact_Exit_Door(CDoor *pDoor, float fTime)
{
	CGameManager::GetInst()->ChangeNoticeClip("Button_Empty");
}

void CHuman_Player::Interact_With_Bed(class CBed *pBed, float fTime)
{
	if((m_iState & PSTATUS_BED) == 0)
		CGameManager::GetInst()->ChangeNoticeClip("Button_E_Hide");
}
void CHuman_Player::Interact_Exit_Bed(class CBed *pBed, float fTime)
{
	CGameManager::GetInst()->ChangeNoticeClip("Button_Empty");
}

void CHuman_Player::Geometry_Push(CCollider *pSrc, CCollider *pDest, float fTime)
{
	CHuman_Player *pHuman = pDest->FindComponentFromType<CHuman_Player>((PUN::COMPONENT_TYPE)UT_PLAYER);
	if (pHuman)
	{
		SAFE_RELEASE(pHuman);
		return;
	}

	if (m_iState & PSTATUS_BED)
		return;
	if (m_iState & PSTATUS_LOCKER)
		return;

	PUN::CColliderOBB3D * pSrcCol = dynamic_cast<PUN::CColliderOBB3D*>(pSrc);
	PUN::CColliderOBB3D * pDestCol = dynamic_cast<PUN::CColliderOBB3D*>(pDest);

	if (!pDestCol)
		return;
	if (!pSrcCol)
		return;

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
	float fMagnitude = 1.f;
	if(vVectorDiff.x != 0.f)
		fMagnitude = vVectorDiffUnNorm.x / vVectorDiff.x;
	else if(vVectorDiff.y != 0.f)
		fMagnitude = vVectorDiffUnNorm.y / vVectorDiff.y;
	else if(vVectorDiff.z != 0.f)
		fMagnitude = vVectorDiffUnNorm.z / vVectorDiff.z;
	//pDest-> 방향벡터 쪽으로 최소값 가지는 정점 구하기

	float fVectorDotMax = FLT_MAX;
	float fVectorDotBuf = 0.f;
	
	for (char i = 0; i < 8; ++i)
	{
		switch(i)
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

	Vector3 vDestPoint = vVectorDiffUnNorm * (fVectorDotMax / fMagnitude);
	//pSrc-> 방향벡터 쪽으로 최대값 가지는 정점 구하기
	fVectorDotMax = -FLT_MAX;
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

	Vector3 vSrcPoint = (vVectorDiffUnNorm * (fVectorDotMax / fMagnitude));

	//두 정점 사이를 0으로 만드는 벡터값 구하기
	Vector3 vRes = vDestPoint - vSrcPoint;
	vRes.y = 0.f;

	vRes /= fMagnitude;
	//그만큼 밀기

	PlayerMove(vRes);
}