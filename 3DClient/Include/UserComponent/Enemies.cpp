#include "../ClientHeader.h"
#include "Enemies.h"
#include "../CommonSoundLoader.h"
#include <PathManager.h>

std::wstring Enemies::m_strEnemDataPath[5] = {
	L"Enem1.csv",
	L"Enem2.csv",
	L"Enem3.csv",
	L"Enem4.csv",
	L"Enem5.csv"

};

Enemies::Enemies():
	m_fVisionDistance    (0.f),
	m_fHearingDistance   (0.f),
	m_fWalkSpeed		 (0.f),
	m_fStrafeSpeed		 (0.f),
	m_fSprintSpeed		 (0.f),
	m_fSprintTimeBuf	 (0.f),
	m_fAttackTime		 (0.f),
	m_fAttackTimeBuf	 (0.f),
	m_fSprintTime		 (0.f),
	m_fAtk				 (0.f),
	m_fHP				 (0.f),
	m_fCurrHp			 (0.f),
	m_fHPLowThreshold	 (0.f),
	m_fDyingAnimSpeed	 (0.f),
	m_iEnemState		 (0),
	m_pAnim				 (nullptr),
	m_pSound			 (nullptr),
	m_eWeap				 (EWT_NONE)
{
}

Enemies::Enemies(const Enemies & enem):
	PUN::CUserComponent(enem)
{
}

Enemies::~Enemies()
{
	SAFE_RELEASE(m_pAnim);
	SAFE_RELEASE(m_pSound);
}

bool Enemies::Init()
{
	return true;
}

int Enemies::Update(float fTime)
{
	return 0;
}

int Enemies::LateUpdate(float fTime)
{
	return 0;
}

void Enemies::AfterClone()
{
}

Enemies * Enemies::Clone()
{
	return new Enemies(*this);
}

PUN::CGameObject * Enemies::GetInstance(const std::string& strName, int iType, class PUN::CLayer* pLayer, bool bDestroy)
{
	PUN::CGameObject *pGameObject = PUN::CGameObject::CreateObject(strName, pLayer, bDestroy);

	Enemies *pComp = pGameObject->AddComponent<Enemies>("Enemies");
	pComp->LoadData(m_strEnemDataPath[iType]);
	SAFE_RELEASE(pComp);

	return nullptr;
}

bool Enemies::LoadData(const std::wstring strPath)
{
	FILE *pFile = nullptr;

	std::wstring wstrPath = PUN::CPathManager::GetInst()->FindPath(DATA_PATH);
	wstrPath += strPath;

	_wfopen_s(&pFile, wstrPath.c_str(), TEXT("rt"));

	if (!pFile)
		return false;

	PUN::CRenderer *pRenderer = m_pObject->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	if (!pRenderer)
		pRenderer = AddComponent<PUN::CRenderer>("Renderer");

	m_pAnim = m_pObject->FindComponentFromType<PUN::CAnimation>(PUN::CT_ANIMATION);
	if (!m_pAnim)
		m_pAnim = AddComponent<PUN::CAnimation>("Animation");

	//m_pAnimation->SetAnimationComplex(true);

	m_pSound = m_pObject->FindComponentFromTag<PUN::CSoundSource>("Sound");
	if (!m_pSound)
		m_pSound = AddComponent<PUN::CSoundSource>("Sound");

	char strBuf[1024] = {};
	char strDataBuf[512] = {};
	char strDataBuf_1[512] = {};
	char strDataBuf_2[512] = {};
	char strDataBuf_3[512] = {};

	int iBufIdx = 0;

	std::vector<std::string> vecSoundKey;


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
		int iLast = (int)strlen(strBuf) - 1;
		if (strBuf[iLast] == '\n')
			strBuf[iLast] = ',';
		if (strBuf[iLast + 1] == 0)
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
						iType = (int)EDT_INT;
					}

					else if (!strcmp(strDataBuf, "float"))
					{
						iType = (int)EDT_FLOAT;
					}

					else if (!strcmp(strDataBuf, "string"))
					{
						iType = (int)EDT_STRING;
					}

					else if (!strcmp(strDataBuf, "sound"))
					{
						iType = (int)EDT_SOUND;
					}

					else if (!strcmp(strDataBuf, "mesh"))
					{
						iType = (int)EDT_MESH;
					}

					else if (!strcmp(strDataBuf, "anim"))
					{
						iType = (int)EDT_ANIM;
					}

					else if (!strcmp(strDataBuf, "bone"))
					{
						iType = (int)EDT_BONE;
					}

					else if (!strcmp(strDataBuf, "socket"))
					{
						iType = (int)EDT_SOCKET;
					}
				}
				else
				{
					switch (iType)
					{
					case EDT_NONE:
						break;
					case EDT_INT:
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
							if (!strcmp(strDataBuf_1, "m_eWeap"))
							{
								m_eWeap = (ENEMY_WEAPON_TYPE)std::stoi(strDataBuf);
							}
						}
					}
					break;
					case EDT_FLOAT:
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
							//m_fVisionDistance
							if (!strcmp(strDataBuf_1, "m_fVisionDistance"))
							{
								m_fVisionDistance = std::stof(strDataBuf);
							}
							//m_fHearingDistance
							else if (!strcmp(strDataBuf_1, "m_fHearingDistance"))
							{
								m_fHearingDistance = std::stof(strDataBuf);
							}
							//m_fWalkSpeed
							else if (!strcmp(strDataBuf_1, "m_fWalkSpeed"))
							{
								m_fWalkSpeed = std::stof(strDataBuf);
							}
							//m_fStrafeSpeed
							else if (!strcmp(strDataBuf_1, "m_fStrafeSpeed"))
							{
								m_fStrafeSpeed = std::stof(strDataBuf);
							}
							//m_fSprintSpeed
							else if (!strcmp(strDataBuf_1, "m_fSprintSpeed"))
							{
								m_fSprintSpeed = std::stof(strDataBuf);
							}
							//m_fAttackTime
							else if (!strcmp(strDataBuf_1, "m_fAttackTime"))
							{
								m_fAttackTime = std::stof(strDataBuf);
							}
							//m_fSprintTime
							else if (!strcmp(strDataBuf_1, "m_fSprintTime"))
							{
								m_fSprintTime = std::stof(strDataBuf);
							}
							//m_fAtk
							else if (!strcmp(strDataBuf_1, "m_fAtk"))
							{
								m_fAtk = std::stof(strDataBuf);
							}
							//m_fHP
							else if (!strcmp(strDataBuf_1, "m_fHP"))
							{
								m_fHP = std::stof(strDataBuf);
								m_fCurrHp = m_fHP;
							}
							//m_fHPLowThreshold
							else if (!strcmp(strDataBuf_1, "m_fHPLowThreshold"))
							{
								m_fHPLowThreshold = std::stof(strDataBuf);
							}
							//m_fDyingAnimSpeed
							else if (!strcmp(strDataBuf_1, "m_fDyingAnimSpeed"))
							{
								m_fDyingAnimSpeed = std::stof(strDataBuf);
							}

						}

					}
					break;
					case EDT_STRING:
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
								if (m_pAnim)
								{
									m_pAnim->ChangeClip(strDataBuf);
								}
							}
							else if (!strcmp(strDataBuf_1, "m_strIdleSndKey"))
							{
								m_strIdleSndKey = strDataBuf;
							}
							//m_strIdleSndKey
							else if (!strcmp(strDataBuf_1, "m_strAngerSndKey"))
							{
								m_strAngerSndKey = strDataBuf;
							}
							//m_strAngerSndKey
							else if (!strcmp(strDataBuf_1, "m_strAttackSndKey"))
							{
								m_strAttackSndKey = strDataBuf;
							}
							//m_strAttackSndKey
							else if (!strcmp(strDataBuf_1, "m_strHitSndKey"))
							{
								m_strHitSndKey = strDataBuf;
							}
							//m_strHitSndKey
							else if (!strcmp(strDataBuf_1, "m_strLostSndKey"))
							{
								m_strLostSndKey = strDataBuf;
							}
							//m_strLostSndKey
							else if (!strcmp(strDataBuf_1, "m_strDeathSndKey"))
							{
								m_strDeathSndKey = strDataBuf;
							}
							//m_strDeathSndKey
						}
					}
					break;
					case EDT_SOUND:
					{
						//Todo : 사운드
						if (iSteps == 1)
						{
							std::string strData(strDataBuf);
							vecSoundKey.push_back(strDataBuf);
						}
					}
					break;
					case EDT_MESH:
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
					case EDT_ANIM:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnim->Load(widePath);
						}

					}
					break;
					case EDT_BONE:
					{
						if (iSteps == 1)
						{
							WCHAR widePath[512] = {};
							MultiByteToWideChar(CP_ACP, 0, strDataBuf, (int)strlen(strDataBuf), widePath, (int)strlen(strDataBuf));
							m_pAnim->LoadBone(widePath);
						}
					}
					break;
					case EDT_SOCKET:
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
							if (!m_pAnim->AddSocket(strDataBuf_1, strDataBuf))
							{
								m_pAnim->SetSocketObject(strDataBuf_1, strDataBuf, nullptr);
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
									m_pAnim->SetSocketOffset(strDataBuf_1, strDataBuf_3, vOffset);
									m_pAnim->SetSocketRotation(strDataBuf_1, strDataBuf_3, vRot);

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

	if (vecSoundKey.size() > 0)
		m_pSound->LoadSounds(vecSoundKey);


	return true;
}

int Enemies::GetState() const
{
	return m_iEnemState;
}

bool Enemies::IsIdle() const
{
	return m_iEnemState & EAS_IDLE;
}

bool Enemies::IsAlerted() const
{
	return m_iEnemState & EAS_ALERTED;
}

bool Enemies::InCombat() const
{
	return m_iEnemState & EAS_COMBAT;
}

bool Enemies::IsFleeing() const
{
	return m_iEnemState & EAS_FLEE;
}

bool Enemies::GotTarget() const
{
	return m_iEnemState & EAS_HAVETARGET;
}

bool Enemies::HasLostTarget() const
{
	return m_iEnemState & EAS_LOSTTARGET;
}

bool Enemies::IsSearchingPlayer() const
{
	return m_iEnemState & EAS_SEARCHING;
}

bool Enemies::IsHealthEnough()
{
	if (m_fCurrHp > m_fHPLowThreshold)
		m_iEnemState |= EAS_HP_NORMAL;
	else
	{
		m_iEnemState ^= EAS_HP_NORMAL;
		m_iEnemState |= EAS_HP_LOW;
	}
		
	return m_iEnemState & EAS_HP_NORMAL;
}

bool Enemies::IsHealthLow()
{
	if (m_fCurrHp <= m_fHPLowThreshold)
	{
		m_iEnemState ^= EAS_HP_NORMAL;
		m_iEnemState |= EAS_HP_LOW;
	}
		
	else
	{
		m_iEnemState ^= EAS_HP_LOW;
		m_iEnemState |= EAS_HP_NORMAL;
	}
	return m_iEnemState & EAS_HP_LOW;
}

bool Enemies::HasWeapon() const
{
	return m_iEnemState & EAS_HAVE_WEAPON;
}

void Enemies::FindEnemy(float fTime)
{

}

void Enemies::OnIdle(float fTime)
{
}

void Enemies::OnCombatStart()
{
}

void Enemies::OnCombat(float fTime)
{
}

void Enemies::OnSearchStart()
{
}

void Enemies::OnSearching(float fTime)
{
}

void Enemies::OnSearchFail()
{
}

void Enemies::OnSearchSuccess()
{
}

void Enemies::GetHit(float fDamage)
{
}

void Enemies::OnGettingHit(float fTime)
{
}

void Enemies::Attack(float fDamage)
{
}

void Enemies::OnAttacking(float fTime)
{
}
