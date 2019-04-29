#include "../ClientHeader.h"
#include "Input.h"
#include "Door.h"
#include <Component/SoundSource.h>
#include <Component/Transform.h>
#include <SoundManager.h>
#include "Component/ColliderOBB3D.h"

PUN_USING

#define OPEN_SOUND 0
#define CLOSE_SOUND 1
#define FAST_OPEN_SOUND	2


CDoor::CDoor() :
	m_fAccTime(0.0f),
	m_fFastOpenTime(0.5f),
	m_fOpenTime(1.5f),
	m_iState(1),
	m_pSndComp(0),
	m_bLock(false),
	m_iDir(0),
	m_bFastOpen(false),
	m_fOpenRot(90.0f),
	m_fRot(0.0f),
	m_fAccRot(0.0f),
	m_fCloseRot(0.0f)
{
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
	CRenderer* pRD = m_pObject->AddComponent<CRenderer>("DoorRenderer");
	CColliderOBB3D* pOBB = AddComponent<CColliderOBB3D>("DoorBody");

	pRD->SetMesh("Large_Locker_Door", TEXT("Large_Locker_Door.msh"));

	pRD->SetShader(STANDARD_BUMP_SHADER);
	m_pTransform->SetWorldPivot(0.5f, -0.5f, 0.0f);
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetLocalRot(90.0f);

	Vector3 vMeshLength = pRD->GetMeshLength();
	Vector3 vScale = vMeshLength * GetWorldScale();

	Vector3 vCenter;
	vCenter.x =vMeshLength.z * 0.5f;
	vCenter.y =vMeshLength.y * -0.085f;
	vCenter.z =vMeshLength.x * 0.0f;


	//vCenter.x = 30.0f;

	Vector3 vAxis[3];
	Matrix matLocalRot = m_pTransform->GetLocalRotMatrix();

	//Vector3 vCenter = vScale ; 

	vScale = vScale * 0.5f;

	for (int i = 0; i < 3; ++i)
	{
		vAxis[i] = Vector3::Axis[(AXIS)i];
		vAxis[i] = vAxis[i].TransformNormal(matLocalRot);
	}

	pOBB->SetInfo(vCenter, vAxis, vScale);
	pOBB->SetColliderID((COLLIDER_ID)UCI_DOOR);
	pOBB->SetCollisionCallback(CCT_STAY, this, &CDoor::Interact);

	SAFE_RELEASE(pRD);
	SAFE_RELEASE(pOBB);

	AfterClone();

	m_iState = 1;

	m_fOpenRot = 90.0f;
	m_fOpenTime = 1.0f;
	m_fFastOpenTime = 0.1f;

	m_fFastOpenSpeed = 1.0f / m_fFastOpenTime;
	m_fOpenSpeed = 1.0f / m_fOpenTime;

	SetDoorType(DOOR_HEAVY);

	return true;
}

int CDoor::Update(float fTime)
{
	OnAct(fTime);

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

float CDoor::GetDestroyTime() const
{
	return m_fDestroyBurstTime;
}

void CDoor::SetOpenRot(float fRot)
{
	m_fOpenRot = fRot;
}

void CDoor::SetOpenTime(float time)
{
	m_fOpenTime = time;
	m_fOpenSpeed = 1.0f / m_fOpenTime;
}

void CDoor::SetFastOpenTime(float fTime)
{
	m_fFastOpenTime = fTime;

	m_fFastOpenSpeed = m_fOpenTime * m_fOpenTime / m_fFastOpenTime;
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
	if (m_iState & DOOR_ONACT)
	{
		switch (m_eDoorType)
		{
		case DOOR_NORMAL:
			OnActNormal(fTime);
			break;

		case DOOR_LOCKER:
			OnActLocker(fTime);
			break;

		case DOOR_HEAVY:

			break;
		}
	}
}

void CDoor::OnActNormal(float fTime)
{
	m_fAccTime += fTime;

	//기본 열리는 시간과 열리는 속도를 설정
	float fOpenTime = m_fOpenTime;
	float fOpenSpeed = m_fOpenSpeed;

	//만약 빨리 열기가 참이면
	if (m_bFastOpen)
	{
		//빨리 열리는 값을 설정
		fOpenTime = m_fFastOpenTime;
		fOpenSpeed = m_fFastOpenSpeed;
	}

	//최종 열리는 속도 구해주고
	float fDeltaRot = m_fOpenRot * fTime * fOpenSpeed;

	//누적 각도와 실제 적용 각도를 구해준다.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//누적 각도가 90도가 될 때 까지 실제 적용각도로 문을 회전시킨다.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//누적 각도가 90도가 되었다면
	else
	{
		//XOR 연산으로 ONACT 상태를 제거
		m_iState ^= DOOR_ONACT;

		//열린 상태라면
		if (m_iState & DOOR_OPEN)
		{
			//빨리 열기 모드였는지 확인해서 참이면 빨리 열기 모드를 FALSE
			if (m_bFastOpen)
				m_bFastOpen = false;

			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
		}
		//닫힌 상태라면
		else
		{
			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//닫히는 사운드를 재생한다.
				PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
				pSnd->StopClip(CLOSE_SOUND);
				pSnd->Play(CLOSE_SOUND);
			}
		}
	}
}

void CDoor::OnActStage(float fTime)
{
	//기본 열리는 시간과 열리는 속도를 설정
	float fOpenTime = m_fOpenTime;
	float fOpenSpeed = m_fOpenSpeed;

	//만약 빨리 열기가 참이면
	if (m_bFastOpen)
	{
		//빨리 열리는 값을 설정
		fOpenTime = m_fFastOpenTime;
		fOpenSpeed = m_fFastOpenSpeed;
	}

	//최종 열리는 속도 구해주고
	float fDeltaRot = m_fOpenRot * fTime * fOpenSpeed;

	//누적 각도와 실제 적용 각도를 구해준다.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//누적 각도가 90도가 될 때 까지 실제 적용각도로 문을 회전시킨다.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//누적 각도가 90도가 되었다면
	else
	{
		//XOR 연산으로 ONACT 상태를 제거
		m_iState ^= DOOR_ONACT;

		//열린 상태라면
		if (m_iState & DOOR_OPEN)
		{
			//빨리 열기 모드였는지 확인해서 참이면 빨리 열기 모드를 FALSE
			if (m_bFastOpen)
				m_bFastOpen = false;

			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
			Close();
		}
		//닫힌 상태라면
		else
		{
			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//닫히는 사운드를 재생한다.
				PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
				pSnd->StopClip(CLOSE_SOUND);
				pSnd->Play(CLOSE_SOUND);
			}
		}
	}
}

void CDoor::OnActLocker(float fTime)
{
	//최종 열리는 속도 구해주고
	float fDeltaRot = m_fOpenRot * fTime * m_fOpenSpeed;

	//누적 각도와 실제 적용 각도를 구해준다.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//누적 각도가 90도가 될 때 까지 실제 적용각도로 문을 회전시킨다.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//누적 각도가 최대치가 되었다면
	else
	{
		//XOR 연산으로 ONACT 상태를 제거
		m_iState ^= DOOR_ONACT;

		//열린 상태라면
		if (m_iState & DOOR_OPEN)
		{
			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
			Close();
		}
		//닫힌 상태라면
		else
		{
			//오차 값이 있을 수 있기때문에 기존 닫혀있는 상태의 상대 회전으로
			//월드 로테이션 값을 설정한다.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//닫히는 사운드를 재생한다.
				PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
				pSnd->StopClip(CLOSE_SOUND);
				pSnd->Play(CLOSE_SOUND);
			}
		}
	}
}

void CDoor::OnActHeavy(float fTime)
{

	float fDeltaRot = m_fOpenRot * fTime * m_fOpenSpeed;

	//누적 각도와 실제 적용 각도를 구해준다.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//누적 각도가 90도가 될 때 까지 실제 적용각도로 문을 회전시킨다.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);
}

void CDoor::Open(const Vector3& vDir)
{
	//문 타입을 확인하여 각 타입 별 함수를 호출해준다.
	switch (m_eDoorType)
	{
	case DOOR_NORMAL:
		OpenNormal(vDir);
		break;

	case DOOR_STAGE:
		OpenStage(vDir);
		break;

	case DOOR_LOCKER:
		OpenLocker(vDir);
		break;
	}

	//기본 열리는 사운드 인덱스로 설정해준다.
	int iSoundIndex = OPEN_SOUND;

	//만약에 SHIFT키가 눌려있다면
	if (CInput::GetInst()->KeyPush("Shift"))
	{
		//빨리 열기 모드 TRUE, 사운드 인덱스를 빨리 열기롤 설정해준다.
		m_bFastOpen = true;
		iSoundIndex = FAST_OPEN_SOUND;
	}

	if (m_pSndComp)
	{
		//위에서 정해진 사운드를 재생
		PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
		pSnd->StopClip(iSoundIndex);
		pSnd->Play(iSoundIndex);
	}
}

void CDoor::Close()
{
	if (m_iState & DOOR_OPEN)
	{
		m_iState = DOOR_ONACT | DOOR_CLOSE;
		m_fAccTime = 0.0f;
		m_fAccRot = 0.0f;
		m_fRot = m_fOpenRot * m_iDir;
		m_iDir *= -1;
	}
}

void CDoor::OpenNormal(const Vector3 & vDir)
{
	if (m_iState & DOOR_CLOSE)
	{
		Vector3 vDirZ = GetWorldAxis(AXIS_Z);

		float fAngle = vDirZ.Angle(vDir);

		if (fAngle > 90.0f)
			m_iDir = 1;
		else
			m_iDir = -1;

		m_fAccTime = 0.0f;
		m_fAccRot = 0.0f;
		m_fRot = 0.0f;
		m_iState = DOOR_ONACT | DOOR_OPEN;
		m_fCloseRot = GetWorldRot().y;
	}
}

void CDoor::OpenStage(const Vector3 & vDir)
{
	Vector3 vDirZ = GetWorldAxis(AXIS_Z);

	float fAngle = vDirZ.Angle(vDir);

	if (fAngle > 90.0f)
		m_iDir = -1;
	else
		m_iDir = 1;

	if (m_iDir < 0)
	{
		//스테이지 전환
	}
	else if (m_iDir > 0)
	{
		m_fAccTime = 0.0f;
		m_fAccRot = 0.0f;
		m_fRot = 0.0f;
		m_iState = DOOR_ONACT | DOOR_OPEN;

		m_fCloseRot = GetWorldRot().y;
	}
}

void CDoor::OpenLocker(const Vector3 & vDir)
{
	Vector3 NSADFJASLFNvDir = GetWorldAxis(AXIS_Z);

	m_iDir = -1;
	m_fAccTime = 0.0f;
	m_fAccRot = 0.0f;
	m_fRot = 0.0f;
	m_iState = DOOR_ONACT | DOOR_OPEN;

	m_fCloseRot = GetWorldRot().y;
}

void CDoor::SetMesh(const string & strMeshKey, const TCHAR * pFileName)
{
	CRenderer* pRD = FindComponentFromType<CRenderer>(CT_RENDERER);

	pRD->SetMesh(strMeshKey, pFileName);

	SAFE_RELEASE(pRD);
}

DOOR_TYPE CDoor::GetDoorType() const
{
	return m_eDoorType;
}

void CDoor::SetDoorType(int iType)
{
	m_eDoorType = (DOOR_TYPE)iType;

	if (m_eDoorType == DOOR_NORMAL)
	{
		SetOpenRot(90.0f);
	}
	else if (m_eDoorType == DOOR_STAGE)
	{
		SetOpenRot(90.0f);
	}

	else if (m_eDoorType == DOOR_LOCKER)
	{
		CRenderer* pRD = m_pObject->AddComponent<CRenderer>("DoorRenderer");
		CColliderOBB3D* pOBB = AddComponent<CColliderOBB3D>("DoorBody");

		pRD->SetMesh("Large_Locker_Door", TEXT("Large_Locker_Door.msh"));

		pRD->SetShader(STANDARD_BUMP_SHADER);

		Vector3 vMeshLength = pRD->GetMeshLength();
		Vector3 vScale = vMeshLength * GetWorldScale();

		Vector3 vCenter;
		vCenter.x = vMeshLength.z * 0.5f;
		vCenter.y = vMeshLength.y * -0.085f;
		vCenter.z = vMeshLength.x * 0.0f;


		//vCenter.x = 30.0f;

		Vector3 vAxis[3];
		Matrix matLocalRot = m_pTransform->GetLocalRotMatrix();

		//Vector3 vCenter = vScale ; 

		vScale = vScale * 0.5f;

		for (int i = 0; i < 3; ++i)
		{
			vAxis[i] = Vector3::Axis[(AXIS)i];
			vAxis[i] = vAxis[i].TransformNormal(matLocalRot);
		}

		pOBB->SetInfo(vCenter, vAxis, vScale);
		pOBB->SetColliderID((COLLIDER_ID)UCI_DOOR);
		pOBB->SetCollisionCallback(CCT_STAY, this, &CDoor::Interact);

		SAFE_RELEASE(pRD);
		SAFE_RELEASE(pOBB);

		SetOpenRot(90.0f);

		SAFE_RELEASE(pRD);

	}
	else if (m_eDoorType == DOOR_HEAVY)
	{
		m_iDir = 1;
		SetOpenRot(60.0f);
	}
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
	if (pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		//화면 상에 텍스트 UI 출력
		if (m_iState & DOOR_CLOSE)
		{
			//ex) E : 문열기
		}
		else if (m_iState & DOOR_OPEN)
		{
			//ex) E : 문닫기
		}

		if (CInput::GetInst()->KeyPress("E"))
		{
			if (m_eDoorType != DOOR_HEAVY)
			{
				if (m_bLock)
				{
					//GameManager에서 키를 가지고 잇는지 확인하고
					//있다면 잠금해제 후 잠금해제 사운드 재생.
				}
				else
				{
					bool bAct = m_iState & DOOR_ONACT;

					if (!bAct)
					{
						Vector3 vDir = pDest->GetWorldAxis(AXIS_Z);
						if (m_iState & DOOR_CLOSE)
						{
							Open(vDir);
						}
						else if (m_iState & DOOR_OPEN)
							Close();
					}
				}
			}
		}
		else if (KEYHOLD("E"))
		{
			if (m_eDoorType == DOOR_HEAVY)
			{
				if (!m_bLock)
				{
					if (m_iState & DOOR_CLOSE)
					{
						m_iState = DOOR_OPEN | DOOR_ONACT;
						m_fCloseRot = GetWorldRot().y;
					}
					OnActHeavy(fTime);
				}
			}
		}
	}
}

CDoor * CDoor::Clone()
{
	return new CDoor(*this);
}
