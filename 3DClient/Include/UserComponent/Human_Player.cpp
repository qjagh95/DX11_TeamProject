#include "../ClientHeader.h"

#include "Human_Player.h"
#include <Input.h>
#include <PathManager.h>
#include <Resource/ResourcesManager.h>
#include <Component/Animation.h>
#include <Component/BoneSocket.h>
#include <Component/Renderer.h>
#include <Component/Transform.h>
#include <Component/SoundSource.h>
#include <Scene/Scene.h>
#include <Component/Camera.h>
#include <Component/ColliderOBB3D.h>
#include <Component/Arm.h>
#include <Device.h>
#include "Inventory.h"

#include "Handycam.h"

using namespace PUN;

CHuman_Player::CHuman_Player():
	m_pAnimation(nullptr),
	m_pSound(nullptr),
	m_fTimerBuf(0.f),
	m_iState(1),
	m_fTimerBuf_State(0.f),
	m_pHandSocketObj(nullptr),
	m_pHeadObj(nullptr),
	m_fViewMaxAngleY(90.f),
	m_fViewMinAngleY(-90.f),
	m_fGunMaxAngleY(80.f),
	m_fGunMinAngleY(-80.f),
	pCamEffManager(nullptr),
	m_vCamWorldOffset(0.f, -7.5f, -5.f),
	m_vCamLocalOffset(0.f, 2.75f, -1.6f),
	m_fItemTakeTimerBuf(0.f),
	m_fCamTakeTime(0.8f),
	m_fGunTakeTime(1.2f)
{
}

CHuman_Player::CHuman_Player(const CHuman_Player & player):
	PUN::CUserComponent(player)
{
	m_bLoadedOnce		   = player.m_bLoadedOnce;
	m_iState = 1;
	
	m_fHideTime = player.m_fHideTime;
	m_fDoorEffectDelay = player.m_fDoorEffectDelay;
	m_fFWalkSpeed = player.m_fFWalkSpeed;
	m_fRWalkSpeed = player.m_fRWalkSpeed;
	m_fLWalkSpeed = player.m_fLWalkSpeed;
	m_fBackWalkSpeed = player.m_fBackWalkSpeed;
	m_fSprintSpeed = player.m_fSprintSpeed;
	m_fCrouchToStandSpeed	  = player.m_fCrouchToStandSpeed;
	m_fStandToCrouchSpeed = player.m_fStandToCrouchSpeed;
	m_fCrouchHeight = player.m_fCrouchHeight;
	
	m_fCrouchForwardSpd = player.m_fCrouchForwardSpd;
	m_fCrouchBackSpd = player.m_fCrouchBackSpd;
	m_fCrouchLSpd = player.m_fCrouchLSpd;
	m_fCrouchRSpd = player.m_fCrouchRSpd;
	m_fViewMaxAngleY = player.m_fViewMaxAngleY;
	m_fViewMinAngleY = player.m_fViewMinAngleY;
	m_fGunMaxAngleY = player.m_fGunMaxAngleY;
	m_fGunMinAngleY = player.m_fGunMinAngleY;
	m_vCamWorldOffset = player.m_vCamWorldOffset;

	pCamEffManager = nullptr;
	m_pHeadObj = nullptr; m_pAnimation = nullptr; m_pHandSocketObj = nullptr; m_pSound = nullptr;
	m_fTimerBuf = 0.f;
	m_fTimerBuf_State = 0.f;
}

CHuman_Player::~CHuman_Player()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pSound);
	SAFE_RELEASE(m_pHandSocketObj);
	SAFE_RELEASE(m_pHeadObj);
	SAFE_RELEASE(m_pInven);
	SAFE_RELEASE(m_pHandycam);
}

bool CHuman_Player::Init()
{
	pCamEffManager = CCameraEff::GetInst();
	pCamEffManager->Init();
	PUN::CTransform *pCamera = m_pScene->GetMainCameraTransform();
	pCamEffManager->SetCamTransform(pCamera);
	SAFE_RELEASE(pCamera);
	
	
	m_pTransform->SetLocalRotY(180.f);
	m_pHeadObj = PUN::CGameObject::CreateObject("Player_Daegari", m_pLayer);
	m_pHeadObj->SetFrustrumCullUse(false);
	m_pObject->SetFrustrumCullUse(false);
	
	PUN::CTransform*	pTr = m_pHeadObj->GetTransform();
	pTr->SetWorldScale(0.1f, 0.1f, 0.1f);
	
	PUN::CRenderer*	pHeadRenderer = m_pHeadObj->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pHeadRenderer->SetMesh("head", TEXT("Head.FBX"), MESH_PATH);
	pHeadRenderer->DontRenderMat(true);
	pHeadRenderer->SetDecalEnable(false);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pHeadRenderer);

	//Hero - L - Hand, player_cam_hand, 10, 0, 0.0, 0, 0, 0
	//GET_SINGLE(PUN::CInput)->BindAxis("MoveH", this, &CHuman_Player::Forward);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_W, 1.f);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_S, 2.f);
	PUN::CInput *_Input = PUN::CInput::GetInst();

	_Input->AddKey("W", 'W');
	_Input->AddKey("S", 'S');
	_Input->AddKey("D", 'D');
	_Input->AddKey("A", 'A');
	_Input->AddKey("E", 'E');
	_Input->AddKey("Shift", VK_SHIFT);
	_Input->AddKey("Space", VK_SPACE);
	_Input->AddKey("I", 'I');
	_Input->AddKey("G", 'G');
	_Input->AddKey("Ctrl", VK_CONTROL);

	AfterClone();


	
	// Inventory
	PUN::CGameObject *pInvObj = CGameObject::CreateObject("Inven", m_pLayer);

	m_pInven = pInvObj->AddComponent<CInventory>("Inven");
	m_pInven->SetInvenMax(19);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial = pInvObj->AddComponent<CMaterial>("Inven");
	pMaterial->SetDiffuseTex(0, "Inventory", TEXT("UI/Inven/InvenBack.png"));

	SAFE_RELEASE(pInvObj);
	SAFE_RELEASE(pMaterial);
	
	// Handycam
	PUN::CGameObject *pHandycamObj = CGameObject::CreateObject("Handycam", m_pLayer);

	m_pHandycam = pHandycamObj->AddComponent<CHandycam>("Handycam");
	m_pHandycam->Release();
	CTransform*	pHandycamTr = pHandycamObj->GetTransform();
	pHandycamTr->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pHandycamTr);
	SAFE_RELEASE(pHandycamObj);
	
		
	return true;
}

void CHuman_Player::AfterClone()
{
	
}

int CHuman_Player::Input(float fTime)
{

	m_iState |= PSTATUS_STOPMOVE;

	bool bBlend = false;

	PUN::CInput *_Input = PUN::CInput::GetInst();


	//Other Keys
	
	if (_Input->KeyPress("I"))
	{
		if (m_iState & PSTATUS_ITEM)
		{
			m_iState ^= (PSTATUS_ITEM | PSTATUS_INACTIVE);
			_Input->ShowMouse(false);
			Close_Item(fTime);
		}
		else
		{
			m_iState |= (PSTATUS_ITEM | PSTATUS_INACTIVE);
			_Input->ShowMouse(true);
			Open_Item(fTime);
		}
	}

	if (m_iState & PSTATUS_INACTIVE)
	{
		PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

		Vector3 vCamPos = pHeadTrans->GetWorldPos() + m_vCamWorldOffset;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamLocalOffset.x;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamLocalOffset.y;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamLocalOffset.z;

		pCamTrans->SetWorldPos(vCamPos);

		SAFE_RELEASE(pCamTrans);
		SAFE_RELEASE(pHeadTrans);


		return 1; //Inactive�� ���� ���� 1��ȯ
	}
	if (_Input->ShowMouse())
		_Input->ShowMouse(false);

	if (_Input->KeyPress("G"))
	{

		if (m_iState ^ PSTATUS_CAM_ON)
		{
			
			m_fItemTakeTimerBuf = 0.f;
			m_iState |= PSTATUS_CAM_TAKING_ON;
			m_iState |= PSTATUS_CAM_ON;

		}
		else
		{
			if (m_iState ^ PSTATUS_CAM_TAKING_OFF)
			{
				m_fItemTakeTimerBuf = 0.f;
			}
			
			m_iState |= PSTATUS_CAM_TAKING_OFF;
		}
	}

	if (m_iState & PSTATUS_CAM_ON)
	{
		if (m_iState & PSTATUS_CAM_TAKING_ON)
		{
			if (m_fItemTakeTimerBuf < m_fCamTakeTime)
			{
				m_fItemTakeTimerBuf += fTime;
				HandyCam_On(fTime);
			}
			else
			{
				m_fItemTakeTimerBuf = 0.f;
				m_iState ^= PSTATUS_CAM_TAKING_ON;
				HandyCam_Start();
			}
		}
		else if (m_iState & PSTATUS_CAM_TAKING_OFF)
		{
			if (m_fItemTakeTimerBuf < m_fCamTakeTime)
			{
				m_fItemTakeTimerBuf += fTime;
				HandyCam_Off(fTime);
			}
			else
			{
				m_fItemTakeTimerBuf = 0.f;
				m_iState ^= (PSTATUS_CAM_TAKING_OFF | PSTATUS_CAM_ON);
				HandyCam_End();
			}
		}
		else
		{
			HandyCam_On(fTime);
		}
		
	}
	

	//Movement
	
	if (_Input->KeyPress("Shift"))
	{

		m_iState |= PSTATUS_SPRINT;
	}
	else if (_Input->KeyRelease("Shift"))
	{
		m_iState &= ~PSTATUS_SPRINT;
	}

	if (_Input->KeyPress("Ctrl"))
	{
		m_iState |= PSTATUS_CROUCHING;
		
		m_pAnimation->ChangeClip("player_stand_to_crouch");
		m_fTimerBuf_State = 0.f;
	}
	else if (_Input->KeyRelease("Ctrl"))
	{
		m_iState &= ~PSTATUS_CROUCHED;
		m_iState |= PSTATUS_CROUCHING;
		m_pAnimation->ChangeClip("player_crouch_to_stand");
	}
	
	if (_Input->KeyPush("Ctrl"))
	{
		m_iState |= PSTATUS_CROUCHED;
		m_iState &= ~PSTATUS_SPRINT;
	}
		
	if (_Input->KeyPush("W"))
	{
		//std::cout << "W" << std::endl;
		m_iState &= ~PSTATUS_STOPMOVE;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_forward");
			Move(PUN::AXIS_Z, m_fCrouchForwardSpd, fTime);			
		}
		else
		{
			if (m_iState & PSTATUS_SPRINT)
			{
				//std::cout << "run" << endl;
				m_pAnimation->ChangeClip("player_run_forward");
				Move(PUN::AXIS_Z, m_fSprintSpeed, fTime);
			}
			else
			{
				m_pAnimation->ChangeClip("player_walk_forward");
				Move(PUN::AXIS_Z, m_fFWalkSpeed, fTime);
			}
		}
		
	}
	else if (_Input->KeyPush("S"))
	{
		m_iState &= ~PSTATUS_STOPMOVE;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_backward");
			Move(PUN::AXIS_Z, -m_fCrouchBackSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_backward");
			Move(PUN::AXIS_Z, -m_fBackWalkSpeed, fTime);
		}
		
	}

	if (_Input->KeyPush("D"))
	{
		
		if (m_iState & PSTATUS_CROUCHING)
			bBlend = true;
		else
			bBlend = false;

		if (_Input->KeyPush("W") || _Input->KeyPush("S"))

			bBlend = true;
		else
			bBlend ? bBlend : false;

		m_iState &= ~PSTATUS_STOPMOVE;

		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_strafe_right");
			Move(PUN::AXIS_X, m_fCrouchRSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_strafe_right");
			Move(PUN::AXIS_X, m_fRWalkSpeed, fTime);
		}
		
	}
	else if (_Input->KeyPush("A"))
	{

		if (m_iState & PSTATUS_CROUCHING)
			bBlend = true;
		else
			bBlend = false;

		if (_Input->KeyPush("W") || _Input->KeyPush("S"))

			bBlend = true;
		else
			bBlend ? bBlend : false;
		
		m_iState &= ~PSTATUS_STOPMOVE;
		if (m_iState & PSTATUS_CROUCHED)
		{
			m_pAnimation->ChangeClip("player_crouch_strafe_left");
			Move(PUN::AXIS_X, -m_fCrouchLSpd, fTime);
		}
		else
		{
			m_pAnimation->ChangeClip("player_walk_strafe_left");
			Move(PUN::AXIS_X, -m_fLWalkSpeed, fTime);
		}
	}

	if (_Input->KeyPress("Space"))
	{
		//m_iState |= PSTATUS_JUMPED;


		if (m_iState & PSTATUS_STOPMOVE)
		{
			m_pAnimation->ChangeClip("player_jump_from_idle");
		}
		else if (m_iState & PSTATUS_SPRINT)
		{
			m_pAnimation->ChangeClip("player_jump_over_from_run");
		}
		else
		{
			m_pAnimation->ChangeClip("player_jump_over_from_walk");
		}
	}


	//ȸ��
	if ((m_iState & PSTATUS_INACTIVE) == 0)
	{
		PUN::CTransform *pCamTrans = m_pScene->GetMainCameraTransform();
		PUN::CTransform *pHeadTrans = m_pHeadObj->GetTransform();

		Vector3 vCamPos = pHeadTrans->GetWorldPos() + m_vCamWorldOffset;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_X) * m_vCamLocalOffset.x;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Y) * m_vCamLocalOffset.y;
		vCamPos += pHeadTrans->GetWorldAxis(PUN::AXIS_Z) * m_vCamLocalOffset.z;

		pCamTrans->SetWorldPos(vCamPos);

		pCamEffManager->FirstPersonView(m_fViewMaxAngleY, m_fViewMinAngleY, m_pTransform);
		
		SAFE_RELEASE(pCamTrans);
		SAFE_RELEASE(pHeadTrans);
				
	}
	
	m_pAnimation->KeepBlendSet(bBlend);


	if (m_iState & PSTATUS_STOPMOVE)
	{
		
		if (m_iState & PSTATUS_CROUCHED)
		{
			if ((m_iState & PSTATUS_CROUCHING) == 0)
			{
				m_pAnimation->ChangeClip("player_crouch_idle");
			}
			
		}
		else
		{
			m_pAnimation->ChangeClip("player_stand_idle");
		}
		
	}

	return 0;
}

int CHuman_Player::Update(float fTime)
{
	
	if (m_iState & PSTATUS_CROUCHING)
	{
		if (m_iState & PSTATUS_CROUCHED)
		{
			if (m_fTimerBuf_State < m_fStandToCrouchSpeed)
			{
				m_fTimerBuf_State += fTime;

				float fLerp = m_fTimerBuf_State / m_fStandToCrouchSpeed;

				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y += m_fCrouchHeight * fLerp;

				m_pTransform->SetLocalPos(vLocPos);
			}
			else
			{
				m_iState &= ~PSTATUS_CROUCHING;
				m_fTimerBuf_State = 0.f;
				
				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y += m_fCrouchHeight;
				m_pTransform->SetLocalPos(vLocPos);
			}
		}
		else
		{
			if (m_fTimerBuf_State < m_fCrouchToStandSpeed)
			{
				m_fTimerBuf_State += fTime;

				m_fTimerBuf_State += fTime;

				float fLerp = m_fTimerBuf_State / m_fCrouchToStandSpeed;

				Vector3 vLocPos = m_vInitLocalPos;
				vLocPos.y -= m_fCrouchHeight * fLerp;

				m_pTransform->SetLocalPos(vLocPos);
			}
			else
			{
				m_iState &= ~PSTATUS_CROUCHING;
				m_fTimerBuf_State = 0.f;

				m_pTransform->SetLocalPos(m_vInitLocalPos);
			}
		}
		
	}

	pCamEffManager->Update(fTime);


	
	return 0;
}

int CHuman_Player::LateUpdate(float fTime)
{
	/*
	PUN::CTransform *pCam = m_pScene->GetMainCameraTransform();
	PUN::CTransform *pTr = m_pHandSocketObj->GetTransform();

	pCam->SetWorldPos(pTr->GetWorldPos());
	pCam->SetWorldRot(pTr->GetWorldRot());

	SAFE_RELEASE(pCam);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pTr);
	*/
	
	pCamEffManager->LateUpdate(fTime);
	return 0;
}

void CHuman_Player::Collision(float fTime)
{
}

CHuman_Player * CHuman_Player::Clone()
{
	return new CHuman_Player(*this);
}

//LoadData : Local Transform ������ ��� ������ �����ϼ���
bool CHuman_Player::LoadData(const TCHAR * dataPath)
{
	FILE *pFile = nullptr;
	
	m_vInitLocalPos = m_pTransform->GetLocalPos();

	std::wstring strPath = PUN::CPathManager::GetInst()->FindPath(DATA_PATH);
	strPath += dataPath;

	_wfopen_s(&pFile, strPath.c_str(), TEXT("rt"));

	if (!pFile)
		return false;

	PUN::CRenderer *pRenderer = m_pObject->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	if (!pRenderer)
		pRenderer = AddComponent<PUN::CRenderer>("Renderer");

	m_pAnimation = m_pObject->FindComponentFromType<PUN::CAnimation>(PUN::CT_ANIMATION);
	if(!m_pAnimation)
		m_pAnimation = AddComponent<PUN::CAnimation>("Animation");

	
	m_pSound = m_pObject->FindComponentFromTag<PUN::CSoundSource>("Sound");
	if(!m_pSound)
		m_pSound = AddComponent<PUN::CSoundSource>("Sound");

	char strBuf[1024] = {};
	char strDataBuf[512] = {};
	char strDataBuf_1[512] = {};
	char strDataBuf_2[512] = {};
	char strDataBuf_3[512] = {};

	int iBufIdx = 0;

	std::vector<std::string> vecSoundKey;
	std::vector<std::wstring> vecSoundPath;

	//���� offset ����
	Vector3 vOffset;
	Vector3 vRot;

	while (fgets(strBuf, 1024, pFile))
	{
		int iSteps = 0;
		int iType = 0;
		iBufIdx = 0; //0������ ��ǥ ��ȣ ��������
		//Parse by ','
		bool bCloseLoop = false;
		int iLast = (int)strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if(strBuf[iLast + 1] == 0)
			strBuf[iLast + 1] = ',';
		for (int i = 0; i < strnlen_s(strBuf, 1024); ++i)
		{
			
			if (strBuf[i] == ',')
			{
				memset(strDataBuf, 0, sizeof(char) * 512);
				int iDataSize = i - iBufIdx; // +1 �ϸ� ��ǥ���� �پ����
				memcpy_s(strDataBuf, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);

				if (iSteps == 0)
				{
					//iStep 0 : �������� ������ ��Ÿ����
					if (!strcmp(strDataBuf, "int"))
					{
						iType = (int)PDT_INT;
					}

					else if (!strcmp(strDataBuf, "float"))
					{
						iType = (int)PDT_FLOAT;
					}

					else if (!strcmp(strDataBuf, "string"))
					{
						iType = (int)PDT_STRING;
					}

					else if (!strcmp(strDataBuf, "sound"))
					{
						iType = (int)PDT_SOUND;
					}

					else if (!strcmp(strDataBuf, "mesh"))
					{
						iType = (int)PDT_MESH;
					}

					else if (!strcmp(strDataBuf, "anim"))
					{
						iType = (int)PDT_ANIM;
					}

					else if (!strcmp(strDataBuf, "bone"))
					{
						iType = (int)PDT_BONE;
					}

					else if (!strcmp(strDataBuf, "socket"))
					{
						iType = (int)PDT_SOCKET;
					}
				}
				else
				{
					switch (iType)
					{
					case PDT_NONE:
						break;
					case PDT_INT:
					{
						//Todo : Int Ÿ�� �����͵� ������ ����
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							
						}
					}
						break;
					case PDT_FLOAT:
					{
						//Todo : float Ÿ�� �����͵� ������ ����
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							if (!strcmp(strDataBuf_1, "HideTime"))
							{
								m_fHideTime = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "DoorInteractDelay"))
							{
								m_fDoorEffectDelay = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkFSpeed"))
							{
								m_fFWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkBSpeed"))
							{
								m_fBackWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkLSpeed"))
							{
								m_fLWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "WalkRSpeed"))
							{
								m_fRWalkSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "SprintSpeed"))
							{
								m_fSprintSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchToStandSpeed"))
							{
								m_fCrouchToStandSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fStandToCrouchSpeed"))
							{
								m_fStandToCrouchSpeed = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchHeight"))
							{
								m_fCrouchHeight = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchForwardSpd"))
							{
								m_fCrouchForwardSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchBackSpd"))
							{
								m_fCrouchBackSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchLSpd"))
							{
								m_fCrouchLSpd = std::stof(strDataBuf);
							}

							else if (!strcmp(strDataBuf_1, "m_fCrouchRSpd"))
							{
								m_fCrouchRSpd = std::stof(strDataBuf);
							}
						}
						
					}
						break;
					case PDT_STRING:
					{
						//Todo : string(char*) Ÿ�� �����͵� ������ ����
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							if (!strcmp(strDataBuf_1, "DefaultAnimClip"))
							{
								if (m_pAnimation)
								{
									m_pAnimation->ChangeClip(strDataBuf);
								}
							}
						}
					}
						break;
					case PDT_SOUND:
					{
						//Todo : ����
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
						}

					}
						break;
					case PDT_MESH:
					{
						if (iSteps == 1)
						{
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							pRenderer->SetMesh(strDataBuf_1, widePath);
							//m_pTransform->SetWorldPivot(Vector3::Zero);
						}
					}
						break;
					case PDT_ANIM:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnimation->Load(widePath);
						}
						
					}
						break;
					case PDT_BONE:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnimation->LoadBone(widePath);
						}
					}
						break;
					case PDT_SOCKET:
					{
						if (iSteps == 1)
						{
							//���� �� �̸�
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							//���� �̸�
							memset(strDataBuf_3, 0, sizeof(char) * 512);
							if (!m_pAnimation->AddSocket(strDataBuf_1, strDataBuf))
							{
								m_pAnimation->SetSocketObject(strDataBuf_1, strDataBuf, nullptr);
							}

							memcpy_s(strDataBuf_3, sizeof(char) * 512, strDataBuf, sizeof(char) * 512);
						}
						else if (iSteps > 2)
						{
							switch (iSteps)
							{
							case 3:
								vOffset.x = std::stof(strDataBuf);
								break;
							case 4:
								vOffset.y = std::stof(strDataBuf);
								break;
							case 5:
								vOffset.z = std::stof(strDataBuf);
								break;
							case 6:
								vRot.x = std::stof(strDataBuf);
								break;
							case 7:
								vRot.y = std::stof(strDataBuf);
								break;
							case 8:
								vRot.z = std::stof(strDataBuf);
								{
									m_pAnimation->SetSocketOffset(strDataBuf_1, strDataBuf_3, vOffset);
									m_pAnimation->SetSocketRotation(strDataBuf_1, strDataBuf_3, vRot);

									//test
									//m_pAnimation->SetSocketObject(strDataBuf_1, strDataBuf_3, m_pHandSocketObj);
									bCloseLoop = true;
								}
								break;
							default:
								break;
							}

						}
					}
						break;
					default:
						break;
					}
				}
				
				++iSteps;
				iBufIdx = i + 1;
			}

			if (bCloseLoop)
				break;
		}
	}

	fclose(pFile);

	SAFE_RELEASE(pRenderer);

	m_pAnimation->SetSocketObject("Hero-Neck", "daegari", m_pHeadObj);

	//m_pAnimation->SetSocketOffset("Hero-L-Hand", "player_cam_hand", Vector3(5.0f, 10.0f, 0.0f));

	/*
	PUN::CCamera *pCamera = m_pScene->GetMainCamera();
	pCamera->SetTarget(m_pHandSocketObj);
	
<<<<<<< .mine
	PUN::CArm*	pArm = pCamera->AddComponent<PUN::CArm>("CameraArm");

	PUN::CTransform *pTR = m_pHandSocketObj->GetTransform();

	SAFE_RELEASE(pCamera);
	pArm->SetTarget(pTR);
	pArm->EnableMouse();

	*/
	
	
	return true;
}

void CHuman_Player::Move(PUN::AXIS axis, float fSpeed, float fTime)
{
	m_pTransform->Move(axis, fSpeed, fTime);
}


