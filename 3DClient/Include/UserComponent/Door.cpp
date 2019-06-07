#include "../ClientHeader.h"
#include "Input.h"
#include "Door.h"
#include "SoundManager.h"
#include "Human_Player.h"
#include "Scene/SceneManager.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/SoundSource.h"
#include "Component/ColliderOBB3D.h"
#include "../GameManager.h"

//�������� ���� ������ ���� ������ ������ �������� ���� �����̼��� ��ġ

PUN_USING

#define OPEN_SOUND		0
#define CLOSE_SOUND		1
#define FAST_OPEN_SOUND	2
#define LOCK_SOUND		3
#define UNLOCK_SOUND	4


CDoor::CDoor() :
	m_fAccTime(0.0f),
	m_fFastOpenTime(0.5f),
	m_fOpenTime(1.5f),
	m_iState(1),
	m_pSndComp(0),
	m_bLock(false),
	m_bFastOpen(false),
	m_bOpen(false),
	m_bNeedKey(false),
	m_iDir(0),
	m_fOpenRot(90.0f),
	m_fRot(0.0f),
	m_fAccRot(0.0f),
	m_fCloseRot(0.0f),
	m_strKeyName(""),
	m_strTargetDoorKey(""),
	m_strTargetSceneKey("")
{
	m_iState = DOOR_CLOSE;
	m_eDoorType = DOOR_NORMAL;
	m_iSection = SST_END;
	m_eComType = (COMPONENT_TYPE)UT_DOOR;
	m_bisPlayerColl = false;
	m_bAutoClose = false;
	m_fAutoCloseTime = 0.0f; // ���� �� �ڵ����� ���� ����?
	m_fAutoCloseTimeVar = 0.0f; 
}

CDoor::CDoor(const CDoor & door) :
	PUN::CUserComponent(door)
{
	m_pSndComp = 0;

}

CDoor::~CDoor()
{
	SAFE_RELEASE(m_Renderer);
	if (m_pSndComp != 0)
	{
		PUN::CSoundSource* pSnd = (PUN::CSoundSource*)m_pSndComp;
		pSnd->Release();
	}
}

bool CDoor::Init()
{
	//m_pTransform->SetWorldPivot(0.25f, -0.5f, 0.0f);
	//m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);

	m_Renderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	if (!m_Renderer)
	{
		m_Renderer = m_pObject->AddComponent<CRenderer>("DoorRenderer");
		m_Renderer->SetMesh("Door_Wood_Right-01", TEXT("Door_Wood_Right-01.msh"));
	}


	CColliderOBB3D* pOBB = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

	if (!pOBB)
	{
		pOBB = m_pObject->AddComponent<CColliderOBB3D>("DoorBody");
		/*m_pTransform->SetWorldPivot(0.25f, -0.5f, 0.0f);
		m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
		m_pTransform->SetLocalRotY(90.0f);*/
		float fLocalY = m_pTransform->GetLocalRot().y;

		Vector3 vMeshLength = m_Renderer->GetMeshLength();
		Vector3 vScale = vMeshLength * GetWorldScale();
		Vector3 vCenter;

		vScale.x = vScale.x * 0.5f;

		if (fLocalY > 1.0f)
		{
			vCenter.x = vMeshLength.z * -0.5f;
			vCenter.z = vMeshLength.x * 0.0f;
		}
		else
		{
			vCenter.x = vMeshLength.z * 0.5f;
			vCenter.z = vMeshLength.x * 0.0f;
		}


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
	}

	pOBB->SetCollisionCallback(CCT_STAY, this, &CDoor::Interact);
	pOBB->SetCollisionCallback(CCT_LEAVE, this, &CDoor::InteractRelease);

	SAFE_RELEASE(pOBB);

	AfterClone();

	m_iState = 1;
	m_fOpenRot = 90.0f;
	m_fOpenTime = 1.0f;
	m_fFastOpenTime = 0.1f;

	m_fFastOpenSpeed = 1.0f / m_fFastOpenTime;
	m_fOpenSpeed = 1.0f / m_fOpenTime;

	string names[5];
	names[0] = "wood_Door_Open1";
	names[1] = "wood_Door_Close1";
	names[2] = "wood_Door_Bash1";
	names[3] = "";
	names[4] = "";

	TCHAR* strPaths[5];
	strPaths[0] = (TCHAR*)TEXT("WoodenDoor_OPENING_01.wav");
	strPaths[1] = (TCHAR*)TEXT("WoodenDoor_CLOSING_03.wav");
	strPaths[2] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");
	strPaths[3] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");
	strPaths[4] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");

	SetSounds(names, (const TCHAR**)strPaths);

	return true;
}

int CDoor::Update(float fTime)
{
	if (m_iState == DOOR_OPEN)
		int a = 2;

	Bumho();

	if (m_eDoorType != DOOR_HEAVY)
		OnAct(fTime);

	AutoTimeClose(fTime);

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
	PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
	pSnd->StopClip(UNLOCK_SOUND);
	pSnd->Play(UNLOCK_SOUND);

	GET_SINGLE(CGameManager)->AddChangedListDoor(this);
}

void CDoor::UnLock(const Vector3 & vAxis)
{
	if (m_bNeedKey)
	{
		/*
		�κ��丮�� unordered_map���� m_strKeyName�� �־ bool���� ã��
		���ϵ� bool���� Ʈ���� Ű �������� ȹ���� ������ �����ϰ�
		�������.
		*/
		//�ϴ� �ӽ÷� GameManager�� �Լ� ����� ����. ��ȯ�� �߰��Ǹ� �װɷ� �ٲ� ����
		if (GET_SINGLE(CGameManager)->FindKey(m_strKeyName))
		{
			m_bLock = false;
			//������� ���� ���
			UnLock();
		}
		else
		{
			//��� ���� ���
			PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
			pSnd->StopClip(LOCK_SOUND);
			pSnd->Play(LOCK_SOUND);
		}
	}
	else
	{
		Vector3 vDoorAxis = GetWorldAxis(AXIS_Z);

		vDoorAxis *= m_iLockDir;

		float fAngle = vDoorAxis.Angle(vAxis);

		if (isnan(fAngle))
			fAngle = 0.0f;

		if (fAngle < 90.0f)
		{
			UnLock();

			if (m_eDoorType == DOOR_STAGE)
			{
				CDoor* pTargetDoor = GET_SINGLE(CGameManager)->FindDoor(m_strTargetSceneKey, m_strTargetDoorKey);

				pTargetDoor->UnLock();
			}
		}
		else
		{
			//��� ���� ���
			PUN::CSoundSource *pSnd = (PUN::CSoundSource*)m_pSndComp;
			pSnd->StopClip(LOCK_SOUND);
			pSnd->Play(LOCK_SOUND);
		}
	}
}

void CDoor::Lock(bool bNeedKey, const string & strKeyName)
{
	/*
	��� �Լ��� ���谡 �־�� �� �� �ִ� �������,
	�ƴϸ� �ݴ��ʿ��� ����־ �ݴ��� ���� ��� �����Ǵ� ���������
	bNeedKey������ �־��༭ ó���Ѵ�.

	bNeedKey�� true��� (����θ� ���� �� �� �ִ� ���̶��)
	Key���� �ݵ�� ��������Ѵ�.
	*/

	m_bLock = true;
	m_bNeedKey = bNeedKey;
	m_strKeyName = strKeyName;
}

void CDoor::LockIn(int iDir)
{
	m_bLock = true;
	m_bNeedKey = false;
	m_iLockDir = iDir;
}

void CDoor::ChangeObjectSection(CGameObject* Object)
{
	Object->SetStageSection(m_iSection);
}

bool CDoor::IsOpenFinished() const
{
	return m_bOpen;
}

void CDoor::SetFastOpen(bool bFastOpen)
{
	m_bFastOpen = bFastOpen;
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
	//������ ���� �ɸ��� �ð��� �����Ѵ�.
	m_fOpenTime = time;

	//�ð��� ����� �ӵ��� ���س��´�.
	m_fOpenSpeed = 1.0f / m_fOpenTime;
}

void CDoor::SetCloseTime(float fTime)
{
}

void CDoor::SetFastOpenTime(float fTime)
{
	//������ ������ �� �ɸ��� �� �ð��� �����Ѵ�.
	m_fFastOpenTime = fTime;

	//������ ������ �ð��� ����� ������ �ӵ��� ���س��´�.
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

	vecKey.resize(5);
	vecPath.resize(5);

	for (char i = 0; i < 5; ++i)
	{
		vecKey[i] = strNames[i];
		vecPath[i] = strFilePaths[i];
	}

	pSource->LoadSounds(vecKey, vecPath);

	for (char i = 0; i < 5; ++i)
	{
		pSource->SetVolume(vecKey[i], 3.0f);
	}

	return true;
}

bool CDoor::SetOpenSound(const std::string& strKey, const TCHAR* pFileName)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	pSource->LoadSound(OPEN_SOUND, strKey, pFileName);

	return true;
}

bool CDoor::SetCloseSound(const std::string& strKey, const TCHAR* pFileName)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	pSource->LoadSound(CLOSE_SOUND, strKey, pFileName);

	return true;
}

bool CDoor::SetFastOpenSound(const string& strKey, const TCHAR* pFileName)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	pSource->LoadSound(FAST_OPEN_SOUND, strKey, pFileName);

	return true;
}

bool CDoor::SetLockSound(const string& strKey, const TCHAR* pFileName)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	pSource->LoadSound(LOCK_SOUND, strKey, pFileName);

	return true;
}

bool CDoor::SetUnLockSound(const string& strKey, const TCHAR* pFileName)
{
	PUN::CSoundSource *pSource = (PUN::CSoundSource*)m_pSndComp;

	pSource->LoadSound(UNLOCK_SOUND, strKey, pFileName);

	return true;
}

void CDoor::OnAct(float fTime)
{
	m_fAccTime += fTime;

	if (m_iState & DOOR_ONACT)
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
		}
	}
}

void CDoor::OnActNormal(float fTime)
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

			m_bOpen = true;

			m_fAccTime = 0.0f;
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

			m_bOpen = false;
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
		//���� ������ ������ �ٲپ� ���
		fOpenTime = m_fFastOpenTime;
		fOpenSpeed = m_fFastOpenSpeed;
	}

	//�����Ӵ� ������ ���� �����ְ�
	float fDeltaRot = m_fOpenRot * fTime * fOpenSpeed;

	//���� ������ ���� ���� ������ �����ش�.
	m_fAccRot += fDeltaRot;
	m_fRot += fDeltaRot * m_iDir;

	//���� ������ ������ �ִ� ������ ������ �� �� ���� ���� ���밢���� ���� ȸ����Ų��.
	if (m_fAccRot < m_fOpenRot)
		m_pTransform->SetWorldRotY(m_fCloseRot + m_fRot);

	//���� ������ �ִ� ������ �Ǿ��ٸ�
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
	else
	{
		if (!m_bOpen)
			m_bOpen = true;
	}
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
	if (m_fAccTime < 1.0f)
		return;

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

		if (fAngle < 90.0f)
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
	float fLocalY = m_pTransform->GetLocalRot().y;

	float fAngle = vDirZ.Angle(vDir);
	bool bChangeStage = false;

	if (fAngle < 90.0f)
		bChangeStage = true;

	if (bChangeStage)
	{
		//�������� ��ȯ
		GET_SINGLE(CSceneManager)->ChangeScene(m_strTargetSceneKey);

		//��� �� ����
		CDoor*		pTargetDoor = GET_SINGLE(CGameManager)->FindDoor(m_strTargetSceneKey, m_strTargetDoorKey);

		Vector3 vTargetPos = pTargetDoor->GetWorldPos(); // �̵��� ����� Position

		Vector3 vTargetRot = pTargetDoor->GetWorldRot();
		Vector3 vInvDir = pTargetDoor->GetWorldAxis(AXIS_Z) * -1.0f;

		pTargetDoor->Open(vInvDir);

		if (m_bFastOpen)
		{
			pTargetDoor->SetFastOpen(m_bFastOpen);
			m_bFastOpen = false;
		}

		Vector3 vDiffRot;
		vDiffRot.x = vTargetRot.x - GetWorldRot().x;
		vDiffRot.y = vTargetRot.y - GetWorldRot().y;
		vDiffRot.z = vTargetRot.z - GetWorldRot().z;

		//�÷��̾ ������ �ִϸ��̼� �ϰ� ������ ���ư��� ���� �ڵ����� ��������

		CTransform* pTr = GET_SINGLE(CGameManager)->GetPlayerTr();

		Vector3 vPos = GetWorldPos();
		float fDist = vPos.Distance(pTr->GetWorldPos());
		vPos.y = 0.f;

		Vector3 vDir = pTr->GetWorldPos() - vPos;
		vDir.Normalize();
		vDir.y = 0.0f;
		
		Matrix matWorld;
		matWorld.RotationY(m_pTransform->GetWorldRot().y);
		matWorld.RotationY(vDiffRot.y);
		vDir = vDir.TransformNormal(matWorld);
		//if (vTargetRot.y < 45.0f && vTargetRot.y > -45.0f)
		//	vDir.x *= -1.0f;

		//vDir.z *= -1.0f;
		//vDir.x *= -1.0f;
		//if (fLocalY > 0.0f)
		//	vDir.x *= -1.0f;
		vDir *= -fDist;
		pTr->SetWorldPos(vTargetPos + vDir);
		//pTr->SetWorldRot(vTargetRot);
		if(vDiffRot.y != 180.f && vDiffRot.y != -180.f && vDiffRot.y != 0.f)
		pTr->RotationY(-vDiffRot.y);
		else if (vDiffRot.y == 0.f)
			pTr->RotationY(180.f);

		pTr = GET_SINGLE(CSceneManager)->GetMainCameraTransform();

		if (vDiffRot.y != 180.f && vDiffRot.y != -180.f && vDiffRot.y != 0.f)
			pTr->RotationY(-vDiffRot.y);
		else if(vDiffRot.y == 0.f)
		pTr->RotationY(180.f);

		SAFE_RELEASE(pTr);
	}
	else
	{
		if (fLocalY < 0.0f)
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

void CDoor::OpenLocker(const Vector3 & vDir)
{
	m_iDir = -1;
	m_fAccTime = 0.0f;
	m_fAccRot = 0.0f;
	m_fRot = 0.0f;
	m_iState = DOOR_ONACT | DOOR_OPEN;
	m_fCloseRot = GetWorldRot().y;
}

void CDoor::OpenFast(const Vector3& vDir)
{
	m_bFastOpen = true;
	Open(vDir);
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

	switch (m_eDoorType)
	{
	case DOOR_NORMAL:
	case DOOR_STAGE:
		SetOpenRot(90.0f);
		break;

	case DOOR_LOCKER:
		SetDoorTypeLocker();
		SetOpenRot(90.0f);
		break;

	case DOOR_HEAVY:
		m_iDir = 1;
		SetOpenRot(60.0f);
		break;
	}
}

void CDoor::SetDoorTypeLocker()
{
	CRenderer* pRD = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);

	if (!pRD)
		pRD = m_pObject->AddComponent<CRenderer>("DoorRenderer");

	pRD->SetMesh("Large_Locker_Door", TEXT("Large_Locker_Door.msh"));

	SAFE_RELEASE(pRD);

	CColliderOBB3D* pOBB = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

	if (pOBB)
	{
		pOBB->SetEnable(false);

		SAFE_RELEASE(pOBB);
	}


}

void CDoor::SetTargetDoor(const string & strSceneKey, const string & strDoorKey)
{ 
	m_strTargetSceneKey = strSceneKey;
	m_strTargetDoorKey = strDoorKey;
}

void CDoor::SetLeftRight(bool bLeft)
{
	CRenderer* pRD = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (bLeft)
	{
		CColliderOBB3D* pOBB = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

		Vector3 vMeshLength = pRD->GetMeshLength();
		Vector3 vScale = vMeshLength * GetWorldScale();
		Vector3 vCenter;
		vCenter.x = vMeshLength.z * -0.5f;
		vCenter.y = vMeshLength.y * 0.0f;
		vCenter.z = vMeshLength.x * 0.0f;

		Vector3 vAxis[3];
		Matrix matLocalRot = m_pTransform->GetLocalRotMatrix();

		vScale = vScale * 0.5f;

		for (int i = 0; i < 3; ++i)
		{
			vAxis[i] = Vector3::Axis[(AXIS)i];
			vAxis[i] = vAxis[i].TransformNormal(matLocalRot);
		}

		pOBB->SetInfo(vCenter, vAxis, vScale);

		SAFE_RELEASE(pOBB);
	}
	else
	{
		CColliderOBB3D* pOBB = m_pObject->FindComponentFromType<CColliderOBB3D>(CT_COLLIDER);

		if (!pOBB)
			pOBB = m_pObject->AddComponent<CColliderOBB3D>("DoorBody");

		Vector3 vMeshLength = pRD->GetMeshLength();
		Vector3 vScale = vMeshLength * GetWorldScale();
		Vector3 vCenter;
		vCenter.x = vMeshLength.z * 0.5f;
		vCenter.y = vMeshLength.y * 0.0f;
		vCenter.z = vMeshLength.x * 0.0f;

		Vector3 vAxis[3];
		Matrix matLocalRot = m_pTransform->GetLocalRotMatrix();

		vScale = vScale * 0.5f;

		for (int i = 0; i < 3; ++i)
		{
			vAxis[i] = Vector3::Axis[(AXIS)i];
			vAxis[i] = vAxis[i].TransformNormal(matLocalRot);
		}

		pOBB->SetInfo(vCenter, vAxis, vScale);

		SAFE_RELEASE(pOBB);
	}

	SAFE_RELEASE(pRD);
}

const bool CDoor::IsOpened() const
{
	return m_iState & DOOR_OPEN;
}

const bool CDoor::IsOnAction() const
{
	return m_iState & DOOR_ONACT;
}

void CDoor::AutoTimeClose(float fTime)
{
	if (m_bAutoClose)
	{
		if (m_iState == DOOR_OPEN)
		{
			m_fAutoCloseTimeVar += fTime;

			if (m_fAutoCloseTime <= m_fAutoCloseTimeVar)
			{
				m_fAutoCloseTimeVar = 0.0f;
				Close();
			}
		}
	}
}

void CDoor::Interact(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bisPlayerColl = true;

		Vector3 vDir = pDest->GetWorldAxis(AXIS_Z);

		bool bOnAct = m_iState & DOOR_ONACT;

		if (!bOnAct)
		{
			//ȭ�� �� �ؽ�Ʈ UI ���
			if (m_iState & DOOR_CLOSE)
			{
				if (m_fAccTime > 1.0f)
				{
					//ex) E : ������
				}
			}
			else if (m_iState & DOOR_OPEN)
			{
				//ex) E : ���ݱ�
			}
		}
		//�� Ÿ���� DOOR_HEAVY�� �ƴ϶��
		if (m_eDoorType != DOOR_HEAVY)
		{
			if (CInput::GetInst()->KeyPress("E"))
			{
				//����մ� ���¶��
				if (m_bLock)
					UnLock(vDir);
				//������� �ʴٸ�
				else
				{
					bool bAct = m_iState & DOOR_ONACT;
					//�����ų� ������ ���� �ƴ϶��
					if (!bAct)
					{
						if (m_iState & DOOR_CLOSE)
							Open(vDir);
						else if (m_iState & DOOR_OPEN)
							Close();
					}
				}
			}
		}
		//�� Ÿ���� DOOR_HEAVY���
		else
		{			
			if(KEYHOLD("E"))
			{
				//���� �����ִ� ���¶��
				if (m_iState & DOOR_CLOSE)
				{
					//������ �ִ� ���̶�� �ٲ��� �� 
					m_iState = DOOR_OPEN | DOOR_ONACT;
					m_fCloseRot = GetWorldRot().y;
				}
				//���ݾ� ���� ����
				OnActHeavy(fTime);
			}
		}
	}
}

void CDoor::InteractRelease(PUN::CCollider * pSrc, PUN::CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_INTERACT)
	{
		m_bisPlayerColl = false;
	}
}

CDoor * CDoor::Clone()
{
	return new CDoor(*this);
}

void CDoor::Bumho()
{
	m_MeshLength = m_Renderer->GetMeshLength() * m_pTransform->GetWorldScale();

	Matrix Temp = m_pTransform->GetPosDelta();
	m_WorldPos.x = Temp._41;
	m_WorldPos.y = Temp._42;
	m_WorldPos.z = Temp._43;

	Vector3 LengthHelf = m_MeshLength * 0.5f;

	m_FrontCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z - LengthHelf.z);
	m_FrontCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z - LengthHelf.z);
	m_FrontCenterRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z - LengthHelf.z);
	m_FrontUpCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z - LengthHelf.z);
	m_FrontUpLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z - LengthHelf.z);
	m_FrontUpRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z - LengthHelf.z);
	m_FrontDownCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y, m_WorldPos.z - LengthHelf.z);
	m_FrontDownLeft = m_WorldPos;
	m_FrontDownRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y, m_WorldPos.z - LengthHelf.z);

	m_CenterCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_CenterCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_CenterCenterRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_CenterUpCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + LengthHelf.z);
	m_CenterUpLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + LengthHelf.z);
	m_CenterUpRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + LengthHelf.z);
	m_CenterDownCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y, m_WorldPos.z + LengthHelf.z);
	m_CenterDownLeft = Vector3(m_WorldPos.x, m_WorldPos.y, m_WorldPos.z + LengthHelf.z);
	m_CenterDownRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y, m_WorldPos.z + LengthHelf.z);

	m_BackCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_BackCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_BackCenterRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + LengthHelf.y, m_WorldPos.z + LengthHelf.z);
	m_BackUpCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + m_MeshLength.z);
	m_BackUpCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + m_MeshLength.z);
	m_BackUpCenterRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y + m_MeshLength.y, m_WorldPos.z + m_MeshLength.z);
	m_BackDownCenter = Vector3(m_WorldPos.x + LengthHelf.x, m_WorldPos.y, m_WorldPos.z + m_MeshLength.z);
	m_BackDownCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y, m_WorldPos.z + m_MeshLength.z);
	m_BackDownCenterRight = Vector3(m_WorldPos.x + m_MeshLength.x, m_WorldPos.y, m_WorldPos.z + m_MeshLength.z);
}
