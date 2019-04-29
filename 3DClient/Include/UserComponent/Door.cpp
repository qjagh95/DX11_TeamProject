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

	//�⺻ ������ �ð��� ������ �ӵ��� ����
	float fOpenTime = m_fOpenTime;
	float fOpenSpeed = m_fOpenSpeed;

	//���� ���� ���Ⱑ ���̸�
	if (m_bFastOpen)
	{
		//���� ������ ���� ����
		fOpenTime = m_fFastOpenTime;
		fOpenSpeed = m_fFastOpenSpeed;
	}

	//���� ������ �ӵ� �����ְ�
	float fDeltaRot = m_fOpenRot * fTime * fOpenSpeed;

	//���� ������ ���� ���� ������ �����ش�.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//���� ������ 90���� �� �� ���� ���� ���밢���� ���� ȸ����Ų��.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//���� ������ 90���� �Ǿ��ٸ�
	else
	{
		//XOR �������� ONACT ���¸� ����
		m_iState ^= DOOR_ONACT;

		//���� ���¶��
		if (m_iState & DOOR_OPEN)
		{
			//���� ���� ��忴���� Ȯ���ؼ� ���̸� ���� ���� ��带 FALSE
			if (m_bFastOpen)
				m_bFastOpen = false;

			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
		}
		//���� ���¶��
		else
		{
			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//������ ���带 ����Ѵ�.
				PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
				pSnd->StopClip(CLOSE_SOUND);
				pSnd->Play(CLOSE_SOUND);
			}
		}
	}
}

void CDoor::OnActStage(float fTime)
{
	//�⺻ ������ �ð��� ������ �ӵ��� ����
	float fOpenTime = m_fOpenTime;
	float fOpenSpeed = m_fOpenSpeed;

	//���� ���� ���Ⱑ ���̸�
	if (m_bFastOpen)
	{
		//���� ������ ���� ����
		fOpenTime = m_fFastOpenTime;
		fOpenSpeed = m_fFastOpenSpeed;
	}

	//���� ������ �ӵ� �����ְ�
	float fDeltaRot = m_fOpenRot * fTime * fOpenSpeed;

	//���� ������ ���� ���� ������ �����ش�.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//���� ������ 90���� �� �� ���� ���� ���밢���� ���� ȸ����Ų��.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//���� ������ 90���� �Ǿ��ٸ�
	else
	{
		//XOR �������� ONACT ���¸� ����
		m_iState ^= DOOR_ONACT;

		//���� ���¶��
		if (m_iState & DOOR_OPEN)
		{
			//���� ���� ��忴���� Ȯ���ؼ� ���̸� ���� ���� ��带 FALSE
			if (m_bFastOpen)
				m_bFastOpen = false;

			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
			Close();
		}
		//���� ���¶��
		else
		{
			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//������ ���带 ����Ѵ�.
				PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
				pSnd->StopClip(CLOSE_SOUND);
				pSnd->Play(CLOSE_SOUND);
			}
		}
	}
}

void CDoor::OnActLocker(float fTime)
{
	//���� ������ �ӵ� �����ְ�
	float fDeltaRot = m_fOpenRot * fTime * m_fOpenSpeed;

	//���� ������ ���� ���� ������ �����ش�.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//���� ������ 90���� �� �� ���� ���� ���밢���� ���� ȸ����Ų��.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//���� ������ �ִ�ġ�� �Ǿ��ٸ�
	else
	{
		//XOR �������� ONACT ���¸� ����
		m_iState ^= DOOR_ONACT;

		//���� ���¶��
		if (m_iState & DOOR_OPEN)
		{
			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_fRot = m_fCloseRot + m_fOpenRot * m_iDir;
			m_pTransform->SetWorldRotY(m_fRot);
			Close();
		}
		//���� ���¶��
		else
		{
			//���� ���� ���� �� �ֱ⶧���� ���� �����ִ� ������ ��� ȸ������
			//���� �����̼� ���� �����Ѵ�.
			m_pTransform->SetWorldRotY(m_fCloseRot);

			if (m_pSndComp)
			{
				//������ ���带 ����Ѵ�.
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

	//���� ������ ���� ���� ������ �����ش�.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//���� ������ 90���� �� �� ���� ���� ���밢���� ���� ȸ����Ų��.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);
}

void CDoor::Open(const Vector3& vDir)
{
	//�� Ÿ���� Ȯ���Ͽ� �� Ÿ�� �� �Լ��� ȣ�����ش�.
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

	//�⺻ ������ ���� �ε����� �������ش�.
	int iSoundIndex = OPEN_SOUND;

	//���࿡ SHIFTŰ�� �����ִٸ�
	if (CInput::GetInst()->KeyPush("Shift"))
	{
		//���� ���� ��� TRUE, ���� �ε����� ���� ����� �������ش�.
		m_bFastOpen = true;
		iSoundIndex = FAST_OPEN_SOUND;
	}

	if (m_pSndComp)
	{
		//������ ������ ���带 ���
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
		//�������� ��ȯ
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
		//ȭ�� �� �ؽ�Ʈ UI ���
		if (m_iState & DOOR_CLOSE)
		{
			//ex) E : ������
		}
		else if (m_iState & DOOR_OPEN)
		{
			//ex) E : ���ݱ�
		}

		if (CInput::GetInst()->KeyPress("E"))
		{
			if (m_eDoorType != DOOR_HEAVY)
			{
				if (m_bLock)
				{
					//GameManager���� Ű�� ������ �մ��� Ȯ���ϰ�
					//�ִٸ� ������� �� ������� ���� ���.
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
