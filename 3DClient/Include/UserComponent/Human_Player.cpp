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

CHuman_Player::CHuman_Player():
	m_pAnimation(nullptr),
	m_pSound(nullptr),
	m_fTimerBuf(0.f),
	m_iState(1)
{
}

CHuman_Player::CHuman_Player(const CHuman_Player & player):
	PUN::CUserComponent(player)
{
}

CHuman_Player::~CHuman_Player()
{
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pSound);
}

bool CHuman_Player::Init()
{
	//GET_SINGLE(PUN::CInput)->BindAxis("MoveH", this, &CHuman_Player::Forward);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_W, 1.f);
	//GET_SINGLE(PUN::CInput)->AddKeyScale("MoveH", DIK_S, 2.f);

	GET_SINGLE(PUN::CInput)->AddKey("FrontMove", 'W');
	GET_SINGLE(PUN::CInput)->AddKey("BackMove", 'S');
	GET_SINGLE(PUN::CInput)->AddKey("YRotationP", 'D');
	GET_SINGLE(PUN::CInput)->AddKey("YRotationM", 'A');

	AfterClone();
	return true;
}

void CHuman_Player::AfterClone()
{

}

int CHuman_Player::Update(float fTime)
{
	if (PUN::CInput::GetInst()->KeyPush("FrontMove"))
		Forward(1.0f, fTime);
	else if (PUN::CInput::GetInst()->KeyPush("BackMove"))
		Forward(2.0f, fTime);
	if (PUN::CInput::GetInst()->KeyPush("YRotationP"))
		m_pTransform->RotationY(90.0f, fTime);
	else if (PUN::CInput::GetInst()->KeyPush("YRotationM"))
		m_pTransform->RotationY(-90.0f, fTime);


	return 0;
}

int CHuman_Player::LateUpdate(float fTime)
{
	m_pAnimation->ChangeClip("player_stand_idle");

	
	return 0;
}

void CHuman_Player::Collision(float fTime)
{
}

CHuman_Player * CHuman_Player::Clone()
{
	return new CHuman_Player(*this);
}

bool CHuman_Player::LoadData(const TCHAR * dataPath)
{
	FILE *pFile = nullptr;

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

	//소켓 offset 설정
	Vector3 vOffset;
	Vector3 vRot;

	while (fgets(strBuf, 1024, pFile))
	{
		int iSteps = 0;
		int iType = 0;
		iBufIdx = 0; //0번부터 쉼표 번호 이전까지
		//Parse by ','
		bool bCloseLoop = false;
		int iLast = strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if(strBuf[iLast + 1] == 0)
			strBuf[iLast + 1] = ',';
		for (int i = 0; i < strnlen_s(strBuf, 1024); ++i)
		{
			
			if (strBuf[i] == ',')
			{
				memset(strDataBuf, 0, sizeof(char) * 512);
				int iDataSize = i - iBufIdx; // +1 하면 쉼표까지 붙어버림
				memcpy_s(strDataBuf, sizeof(char) * iDataSize, &(strBuf[iBufIdx]), sizeof(char) * iDataSize);

				if (iSteps == 0)
				{
					//iStep 0 : 데이터의 종류를 나타내자
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
						//Todo : Int 타입 데이터들 변수를 설정
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
						//Todo : float 타입 데이터들 변수를 설정
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

							if (!strcmp(strDataBuf_1, "WalkFSpeed"))
							{
								m_fFWalkSpeed = std::stof(strDataBuf);
							}

							if (!strcmp(strDataBuf_1, "WalkBSpeed"))
							{
								m_fBackWalkSpeed = std::stof(strDataBuf);
							}

							if (!strcmp(strDataBuf_1, "WalkLSpeed"))
							{
								m_fLWalkSpeed = std::stof(strDataBuf);
							}

							if (!strcmp(strDataBuf_1, "WalkRSpeed"))
							{
								m_fRWalkSpeed = std::stof(strDataBuf);
							}

							if (!strcmp(strDataBuf_1, "SprintSpeed"))
							{
								m_fSprintSpeed = std::stof(strDataBuf);
							}
						}
						
					}
						break;
					case PDT_STRING:
					{
						//Todo : string(char*) 타입 데이터들 변수를 설정
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
						//Todo : 사운드
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
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, strlen(strDataBuf), widePath, strlen(strDataBuf));
							pRenderer->SetMesh(strDataBuf_1, widePath);
						}
					}
						break;
					case PDT_ANIM:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, strlen(strDataBuf), widePath, strlen(strDataBuf));
							m_pAnimation->Load(widePath);
						}
						
					}
						break;
					case PDT_BONE:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, strlen(strDataBuf), widePath, strlen(strDataBuf));
							m_pAnimation->LoadBone(widePath);
						}
					}
						break;
					case PDT_SOCKET:
					{
						if (iSteps == 1)
						{
							//소켓 뼈 이름
							memset(strDataBuf_1, 0, sizeof(char) * 512);
							size_t iSiz = sizeof(char) * strnlen_s(strDataBuf, 512);
							memcpy_s(strDataBuf_1, iSiz, strDataBuf, iSiz);
						}
						else if (iSteps == 2)
						{
							//소켓 이름
							memset(strDataBuf_3, 0, sizeof(char) * 512);
							m_pAnimation->AddSocket(strDataBuf_1, strDataBuf);

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


	PUN::CGameObject* pWeaponObj = PUN::CGameObject::CreateObject("Weapon", m_pLayer);
	PUN::CTransform*	pTr = pWeaponObj->GetTransform();
	pTr->SetWorldScale(0.2f, 0.2f, 0.2f);
	pTr->SetLocalRotY(-90.f);

	PUN::CRenderer*	pWeaponRenderer = pWeaponObj->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pWeaponRenderer->SetMesh("chair", TEXT("chair.msh"));
	pWeaponRenderer->SetDecalEnable(false);

	m_pAnimation->SetSocketObject("Hero-L-Hand", "player_cam_hand", pWeaponObj);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pWeaponRenderer);
	SAFE_RELEASE(pWeaponObj);

	pWeaponObj = PUN::CGameObject::CreateObject("Weapon", m_pLayer);
	pTr = pWeaponObj->GetTransform();
	pTr->SetWorldScale(0.2f, 0.2f, 0.2f);
	pTr->SetLocalRotY(-90.f);

	pWeaponRenderer = pWeaponObj->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pWeaponRenderer->SetMesh("chair", TEXT("chair.msh"));
	pWeaponRenderer->SetDecalEnable(false);

	m_pAnimation->SetSocketObject("Hero-Head", "daegari", pWeaponObj);

	
	SAFE_RELEASE(pWeaponRenderer);
	SAFE_RELEASE(pWeaponObj);

	PUN::CCamera *pCam = m_pScene->GetMainCamera();
	
	pCam->SetTarget(pTr);

	SAFE_RELEASE(pCam);
	SAFE_RELEASE(pTr);
	return true;
}

void CHuman_Player::MoveL(float fTime)
{
}

void CHuman_Player::MoveR(float fTime)
{
}

void CHuman_Player::Forward(float fScale, float fTime)
{
	
	unsigned int iDir = (unsigned int)fScale;
	if (iDir & 1)
	{
		m_pAnimation->ChangeClip("player_walk_forward");
		m_pTransform->Move(PUN::AXIS_Z, m_fFWalkSpeed, fTime);
	}
	else if (iDir & 2)
	{
		m_pAnimation->ChangeClip("player_walk_backward");
		m_pTransform->Move(PUN::AXIS_Z, -m_fBackWalkSpeed, fTime);
	}
}

void CHuman_Player::Back(float fTime)
{
	
}

void CHuman_Player::Sprint(float fTime)
{
	m_pAnimation->ChangeClip("player_run_forward");
}

void CHuman_Player::Open(float fTime)
{
}

void CHuman_Player::Close(float fTime)
{
}

void CHuman_Player::Hide_Locker(float fTime)
{
}

void CHuman_Player::Fire_Weapon(float fTime)
{
}
