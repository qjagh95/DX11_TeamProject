#include "../ClientHeader.h"
#include "Input.h"
#include "Door.h"
#include <Component/SoundSource.h>
#include <Component/Transform.h>
#include <SoundManager.h>
#include "Component/Collider.h"

PUN_USING

CDoor::CDoor() :
	fCurrTimer(0.f),
	m_fCloseTime(1.5f),
	m_fOpenTime(1.5f),
	m_iState(1),
	m_pSndComp(0),
	m_bLock(false)
{
	m_vRotClosed = Vector3(0.f, 0.f, 0.f);
	m_vRotOpened = Vector3(0.f, 90.f, 0.f);
	m_eDoorType = DOOR_NORMAL;
}

CDoor::CDoor(const CDoor & door) :
	PUN::CUserComponent(door)
{
	m_pSndComp = 0;

}

CDoor::~CDoor()
{
	if (m_pSndComp != 0)
	{
		PUN::CSoundSource* pSnd = (PUN::CSoundSource*)m_pSndComp;
		pSnd->Release();
	}

}

bool CDoor::Init()
{
	AfterClone();
	return true;
}

int CDoor::Update(float fTime)
{

	return 0;
}

int CDoor::LateUpdate(float fTime)
{
	return 0;
}

void CDoor::AfterClone()
{
	PUN::CSoundSource *pSound = m_pObject->FindComponentFromTag<PUN::CSoundSource>("sound");
	if (!pSound)
	{
		pSound = AddComponent<CSoundSource>("sound");
	}

	m_pSndComp = (ptrdiff_t)pSound;

	CInput::GetInst()->AddKey("Door_Interact", 'E');
}

void CDoor::Open()
{
	//부서진 건 하는거 아냐..
	if (m_iState & DOOR_DESTROYED)
		return;

	//만약 뭔가 되고 있다면
	if (m_iState & DOOR_ONACT)
	{
		if (m_iState & DOOR_OPEN)
			return;

		if (m_iState & DOOR_DESTROYED)
			return;

		if (m_fCloseTime != 0.f)
		{
			float fRatioInv = fCurrTimer / m_fCloseTime;
			fCurrTimer = m_fOpenTime * (1.f - fRatioInv);
		}
	}
	else if (m_iState & DOOR_OPEN)
		return;
	else
		fCurrTimer = 0.f;
	m_iState = DOOR_ONACT | DOOR_OPEN;

	if (m_pSndComp)
	{
		PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
		pSnd->StopClip(0);
		pSnd->Play(0);
	}
}

void CDoor::Close()
{
	//부서진 건 하는거 아냐..
	if (m_iState & DOOR_DESTROYED)
		return;

	if (m_iState & DOOR_ONACT)
	{
		if (m_iState & DOOR_CLOSE)
			return;
		if (m_iState & DOOR_DESTROYED)
			return;

		if (m_fOpenTime != 0.f)
		{
			float fRatioInv = fCurrTimer / m_fOpenTime;
			fCurrTimer = m_fCloseTime * (1.f - fRatioInv);
		}
	}
	else if (m_iState & DOOR_CLOSE)
		return;
	else
		fCurrTimer = 0.f;
	m_iState = DOOR_ONACT | DOOR_CLOSE;

	if (m_pSndComp)
	{
		PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
		pSnd->StopClip(1);
		pSnd->Play(1);
	}
}

void CDoor::Destroy()
{

	fCurrTimer = 0.f;
	m_iState = DOOR_ONACT | DOOR_DESTROYED;
}

bool CDoor::IsLock() const
{
	return m_bLock;
}

void CDoor::UnLock()
{
	m_bLock = false;
}

int CDoor::GetState() const
{
	return 0;
}

void CDoor::SetState(int state)
{
}

float CDoor::GetOpenTime() const
{
	return m_fOpenTime;
}

float CDoor::GetCloseTime() const
{
	return m_fCloseTime;
}

float CDoor::GetDestroyTime() const
{
	return m_fDestroyBurstTime;
}

void CDoor::SetOpenTime(float time)
{
	m_fOpenTime = time;
}

void CDoor::SetCloseTime(float time)
{
	m_fCloseTime = time;
}

void CDoor::SetDestroyTime(float time)
{
	m_fDestroyBurstTime = time;
}

bool CDoor::SetSounds(const std::string strNames[], const TCHAR ** strFilePaths)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	std::vector<std::string> vecKey;
	std::vector<std::wstring> vecPath;

	vecKey.resize(3);
	vecPath.resize(3);

	for (char i = 0; i < 3; ++i)
	{
		vecKey[i] = strNames[i];
		vecPath[i] = strFilePaths[i];
	}

	pSource->LoadSounds(vecKey, vecPath);

	return true;
}

bool CDoor::SetOpenSound(const std::string strKey)
{
	return true;
}

bool CDoor::SetCloseSound(const std::string strKey)
{
	return true;
}

bool CDoor::SetDestroySound(const std::string strKey)
{
	return true;
}

void CDoor::OnAct(float fTime)
{
	switch (m_eDoorType)
	{
	case DOOR_NORMAL:
		OnActNormal(fTime);
		break;

	case DOOR_STAGE:
		OnActStage(fTime);
		break;

	case DOOR_LOCKER:
		OnActLocker(fTime);
		break;

	case DOOR_HEAVY:
		OnActHeavy(fTime);
		break;
	}
}

void CDoor::OnActNormal(float fTime)
{
	if (m_iState & DOOR_ONACT)
	{
		if (m_iState & DOOR_OPEN)
		{
			if (fCurrTimer <= m_fOpenTime)
			{
				if (m_fOpenTime > 0.f)
				{
					float fRatio = fCurrTimer / m_fOpenTime;

					Vector3 vRot = m_vRotClosed + ((m_vRotOpened - m_vRotClosed) * fRatio);

					m_pTransform->SetLocalRot(vRot);
				}

				fCurrTimer += fTime;
			}
			else
			{
				m_iState ^= DOOR_ONACT; //XOR 연산으로 ONACT 종료
				fCurrTimer = 0.f;
			}
		}

		if (m_iState & DOOR_CLOSE)
		{
			if (fCurrTimer <= m_fCloseTime)
			{
				if (m_fCloseTime > 0.f)
				{
					float fRatio = fCurrTimer / m_fCloseTime;

					Vector3 vRot = m_vRotOpened + ((m_vRotClosed - m_vRotOpened) * fRatio);

					m_pTransform->SetLocalRot(vRot);
				}

				fCurrTimer += fTime;
			}
			else
			{
				m_iState ^= DOOR_ONACT; //XOR 연산으로 ONACT 종료
				fCurrTimer = 0.f;
			}
		}

		if (m_iState & DOOR_DESTROYED)
		{
			if (fCurrTimer <= m_fDestroyBurstTime)
			{

				fCurrTimer += fTime;
			}
			else
			{
				m_iState ^= DOOR_ONACT; //XOR 연산으로 ONACT 종료
				fCurrTimer = 0.f;
			}
		}
	}
}

void CDoor::OnActStage(float fTime)
{
}

void CDoor::OnActLocker(float fTime)
{
}

void CDoor::OnActHeavy(float fTime)
{
}

DOOR_TYPE CDoor::GetDoorType() const
{
	return m_eDoorType;
}

void CDoor::SetDoorType(int iType)
{
	m_eDoorType = (DOOR_TYPE)iType;
}

const bool CDoor::IsOpened() const
{
	return m_iState & DOOR_OPEN;
}

const bool CDoor::IsOnAction() const
{
	return m_iState & DOOR_ONACT;
}


void CDoor::Interact(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CInput::GetInst()->KeyPress("Door_Interact"))
	{
		if (m_iState & DOOR_CLOSE)
			Open();

		else if (m_iState & DOOR_OPEN)
			Close();
	}
}

CDoor * CDoor::Clone()
{
	if (CInput::GetInst()->KeyPress("Door_Interact"))
	{
		if (m_iState & DOOR_CLOSE)
			Open();

		else if (m_iState & DOOR_OPEN)
			Close();
	}

	return new CDoor(*this);
}
