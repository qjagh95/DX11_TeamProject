#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Inventory.h"
#include "Component/ColliderOBB3D.h"
#include "Resource/Mesh.h"
#include "Scene/SceneManager.h"

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
	pOBB->SetCollisionCallback(CCT_STAY, this, &CHuman_Player::InteractCallBackStay);

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

}
void CHuman_Player::Hiding_Locker(float fTime)
{

}
void CHuman_Player::Exit_Locker(float fTime)
{

}
void CHuman_Player::Hide_Bed(float fTime)
{
	m_eTempPlayerState = (PLAYER_STATUS)m_iState;
	m_iState = PSTATUS_NONE;

	Vector3 vAxisZ = m_pCameraTr->GetWorldAxis(AXIS_Z);
	float fAngle = vAxisZ.Angle(m_vTargetDir);

	if (isnan(fAngle))
		fAngle = 1.0f;

	Vector3 vAxisY = vAxisZ.Cross(m_vTargetDir);

	if (vAxisY.y < 0.0f)
		fAngle *= -1.0f;

	if (fAngle > 45.0f && fAngle < 135.0f)
	{
		m_pAnimation->ChangeClip("player_enter_bed_right_stand");
		m_iRotDir = -1;
	}
	else if (fAngle < -45.0f && fAngle > -135.0f)
	{
		m_pAnimation->ChangeClip("player_enter_bed_left_stand");
		m_iRotDir = 1;
	}
	else
	{
		m_iState = m_eTempPlayerState;
		return;
	}
	
	m_fDestRotY = fAngle;
	m_fDestRotX = m_pCameraTr->GetWorldRot().x;
	m_fAccRotX = 0.0f;
	m_fAccRotY = 0.0f;


	PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();
	m_fAnimPlayTime = 1.0f / pClip->fPlayTime;
}

void CHuman_Player::Hiding_Bed(float fTime)
{
	m_pAnimation->ChangeClip("player_bed_idle");
	m_fViewMaxAngleY = 10;
	m_fViewMinAngleY = -10;
}
void CHuman_Player::Exit_Bed(float fTime) 
{
	Vector3 vAxisZ = GetWorldAxis(AXIS_Z);
	float fAngle = vAxisZ.Angle(m_vTargetDir);

	if (isnan(fAngle))
		fAngle = 1.0f;

	Vector3 vAxisY = vAxisZ.Cross(m_vTargetDir);

	if (vAxisY.y < 0.0f)
		fAngle *= -1.0f;

	if (fAngle > 0.0f)
	{
		m_pAnimation->ChangeClip("player_exit_bed_right");
		m_iRotDir = 1;
	}
	else if (fAngle < 0.0f)
	{
		m_pAnimation->ChangeClip("player_exit_bed_left");
		m_iRotDir = -1;
	}

	m_fDestRotY = fAngle;
	m_fAccRotY = 0.0f;

	m_fViewMaxAngleY = 90;
	m_fViewMinAngleY = -90;
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
;		float fDeltaRotY = m_fDestRotY * m_fAnimPlayTime * fTime;

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

	if (KEYDOWN("E"))
		ChangeInteractState(IS_GET_OUT_BED, fTime);
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