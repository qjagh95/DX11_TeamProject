#include "../ClientHeader.h"
#include "ST3_Slient.h"

#include "../SceneComponent/JBH_Stage3.h"

#include <Component/SoundSource.h>

ST3_Slient::ST3_Slient()
{
	m_Attack = 1;
	m_TraceDist = 5.0f;
	m_BashCount = 3;
	m_MoveSpeed = 20.0f;
	m_WateTime = 2.0f; 
	m_WateTimeVar = 0.0f;

	m_3DSoundObject = NULLPTR;
	m_3DSound = NULLPTR;
}

ST3_Slient::ST3_Slient(const ST3_Slient & CopyData)
	:NPCBase(CopyData)
{
}

ST3_Slient::~ST3_Slient()
{
	SAFE_RELEASE(m_3DSoundObject);
	SAFE_RELEASE(m_3DSound);
}

bool ST3_Slient::Init()
{
	NPCBase::Init();

	/*
	1. �Ѿƿ��� ��.
	2. Mesh : Cannibal
	3. bne, anm : Cannibal
	*/

	m_AniName[STS_IDLE] = "idle_search_forward";
	m_AniName[STS_CAN_WATE] = "idle_search_forward";
	m_AniName[STS_SUPRISE_TRACE] = "move_forward_fast";
	m_AniName[STS_USER_TRACE] = "move_forward_fast";
	m_AniName[STS_CAN_TRACE] = "move_forward_fast";
	m_AniName[STS_BASH_DOOR] = "bash_door_loop";
	m_AniName[STS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[STS_NORMAL_DOOR_OPEN_LEFT] = "open_door_left_push_close";
	m_AniName[STS_NORMAL_DOOR_OPEN_RIGHT] = "open_door_right_push_close";
	m_AniName[STS_BED_FIND] = "grap_bed_left";
	m_AniName[STS_DESK_FIND] = "grap_desk";
	m_AniName[STS_LOCKER_FIND] = "grap_locker";
	m_AniName[STS_HOOK] = "attack_right";
	m_AniName[STS_JAP] = "attack_left";
	m_AniName[STS_HEAD_ATTACK] = "attack_middle";

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Renderer->SetSelectBoneName("NPCMedium-Pelvis");

	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("enem_def_2.anm");

	ChangeState(STS_IDLE, m_AniName);

	m_3DSoundObject = CGameObject::CreateObject("3DCanSound", m_pLayer);
	m_3DSound = m_3DSoundObject->AddComponent<PUN::CSoundSource>("3DCanSound");
	m_3DSound->LoadSound("Engry1", L"enemies\\Enemies_def_angry1.wav");
	m_3DSound->LoadSound("Engry2", L"enemies\\Enemies_def_angry2.wav");
	m_3DSound->LoadSound("Engry3", L"enemies\\Enemies_def_angry3.wav");
	m_3DSound->LoadSound("Engry4", L"enemies\\Enemies_def_angry4.wav");
	m_3DSound->LoadSound("Engry5", L"enemies\\Enemies_def_angry5.wav");
	m_3DSound->GetTransformNonCount()->SetWorldPos(Vector3(67.0f, 0.0f, 155.0f));

	return true;
}

int ST3_Slient::Input(float DeltaTime)
{
	NPCBase::Input(DeltaTime);

	return 0;
}

int ST3_Slient::Update(float DeltaTime)
{
	NPCBase::Update(DeltaTime);
	m_3DSoundObject->GetTransformNonCount()->SetWorldPos(m_pTransform->GetWorldPos());

	switch (m_State)
	{
	case STS_IDLE:
		FS_IDLE(DeltaTime);
		break;
	case STS_CAN_WATE:
		FS_CAN_WATE(DeltaTime);
		break;
	case STS_USER_TRACE:
		FS_USER_TRACE(DeltaTime);
		break;
	case STS_CAN_TRACE:
		FS_CAN_TRACE(DeltaTime);
		break;
	case STS_SUPRISE_TRACE:
		FS_SUPRISE_TRACE(DeltaTime);
		break;
	case STS_BED_FIND:
		FS_BED_FIND(DeltaTime);
		break;
	case STS_DESK_FIND:
		FS_DESK_FIND(DeltaTime);
		break;
	case STS_LOCKER_FIND:
		FS_LOCKER_FIND(DeltaTime);
		break;
	case STS_HOOK:
		FS_HOOK(DeltaTime);
		break;
	case STS_JAP:
		FS_JAP(DeltaTime);
		break;
	case STS_HEAD_ATTACK:
		FS_HEAD_ATTACK(DeltaTime);
		break;
	case STS_BASH_DOOR:
		FS_BASH_DOOR(DeltaTime);
		break;
	case STS_BASH_DOOR_OPEN:
		FS_BASH_DOOR_OPEN(DeltaTime);
		break;
	case STS_NORMAL_DOOR_OPEN_LEFT:
		FS_NORMAL_DOOR_OPEN_LEFT(DeltaTime);
		break;
	case STS_NORMAL_DOOR_OPEN_RIGHT:
		FS_NORMAL_DOOR_OPEN_RIGHT(DeltaTime);
		break;
	}

	return 0;
}

int ST3_Slient::LateUpdate(float DeltaTime)
{
	return 0;
}

void ST3_Slient::Collision(float DeltaTime)
{
}

void ST3_Slient::Render(float DeltaTime)
{
}

ST3_Slient * ST3_Slient::Clone()
{
	return new ST3_Slient(*this);
}

void ST3_Slient::FS_IDLE(float DeltaTime)
{
	//������ �ѹ��ϰ� �ȵ���
	if (JBH_Stage3::m_SlientMode == true)
	{
		m_MovingPos = m_pLayer->FindObjectNonCount("Suprise")->GetTransformNoneCount()->GetWorldPos();
		m_MovingPos.y = 0.0f;
		ChangeState(STS_SUPRISE_TRACE, m_AniName);
	}
}

void ST3_Slient::FS_USER_TRACE(float DeltaTime)
{
}

void ST3_Slient::FS_CAN_TRACE(float DeltaTime)
{
	if (m_PathFind == false)
	{
		m_PathFind = true;

		CNavigationMesh* pMesh = CNavigationManager3D::GetInst()->FindNavMesh(m_pScene, m_pObject->GetTransformNonCount()->GetWorldPos());
		pMesh->FindPath(m_pTransform->GetWorldPos(), m_MovingPos);

		m_PathList.clear();
		m_PathList = pMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3	Dir = m_MovePos - m_MovingPos;
	Dir.Normalize();
	Dir.y = 0.0f;

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		m_MovePos = m_PathList.front();
		m_PathList.pop_front();
	}

	//m_pTransform->LookAtY(m_MovePos * -1.0f);
	m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);

	if (m_TargetDistance < 55.0f)
	{
		int RandNum = rand() % 5;
		RandNum += 1;

		string SoundKey = "Engry";

		switch (RandNum)
		{
		case 1:
			SoundKey += "1";
			break;
		case 2:
			SoundKey += "2";
			break;
		case 3:
			SoundKey += "3";
			break;
		case 4:
			SoundKey += "4";
			break;
		case 5:
			SoundKey += "5";
			break;
		}

		if (m_3DSound->GetClipState(SoundKey) != PLAYING)
			m_3DSound->Play(SoundKey);
	}
	else
	{
		m_3DSound->StopClip(0);
		m_3DSound->StopClip(1);
		m_3DSound->StopClip(2);
		m_3DSound->StopClip(3);
		m_3DSound->StopClip(4);
	}

	float Distance = m_pTransform->GetWorldPos().GetDistance(m_MovingPos);

	if (Distance < 5.0f)
	{
		m_pTransform->SetWorldRotY(0.0f);
		ChangeState(STS_BASH_DOOR, m_AniName);
	}
}

void ST3_Slient::FS_SUPRISE_TRACE(float DeltaTime)
{
	CGameObject* Suprise = m_pLayer->FindObjectNonCount("Suprise");

	if (m_PathFind == false)
	{
		m_PathFind = true;  

		CNavigationMesh* pMesh = CNavigationManager3D::GetInst()->FindNavMesh(m_pScene, m_pObject->GetTransformNonCount()->GetWorldPos());
		pMesh->FindPath(m_pTransform->GetWorldPos(), m_MovingPos);

		m_PathList.clear();
		m_PathList = pMesh->GetPathList();

		m_MovePos = m_PathList.front();
	}

	Vector3	Dir = Suprise->GetTransformNonCount()->GetWorldPos() - m_MovePos;
	Dir.Normalize();
	Dir.y = 0.0f;

	if (m_PathList.empty())
		m_PathFind = false;
	else
	{
		m_MovePos = m_PathList.front();
		m_PathList.pop_front();
	}

	m_pTransform->LookAtY(m_MovePos * -1.0f);
	m_pTransform->Move(Dir, m_MoveSpeed, DeltaTime);

	if (m_TargetDistance < 55.0f)
	{
		int RandNum = rand() % 5;
		RandNum += 1;

		string SoundKey = "Engry";

		switch (RandNum)
		{
		case 1:
			SoundKey += "1";
			break;
		case 2:
			SoundKey += "2";
			break;
		case 3:
			SoundKey += "3";
			break;
		case 4:
			SoundKey += "4";
			break;
		case 5:
			SoundKey += "5";
			break;
		}

		if (m_3DSound->GetClipState(SoundKey) != PLAYING)
			m_3DSound->Play(SoundKey);
	}
	else
	{
		m_3DSound->StopClip(0);
		m_3DSound->StopClip(1);
		m_3DSound->StopClip(2);
		m_3DSound->StopClip(3);
		m_3DSound->StopClip(4);
	}

	if (JBH_Stage3::m_isCanDrop == true)
	{
		CSoundManager::GetInst()->SoundPlay("CanSound");
		ChangeState(STS_CAN_WATE, m_AniName);
	}
}

void ST3_Slient::FS_HOOK(float DeltaTime)
{
}

void ST3_Slient::FS_JAP(float DeltaTime)
{
}

void ST3_Slient::FS_HEAD_ATTACK(float DeltaTime)
{
}

void ST3_Slient::FS_CAN_WATE(float DeltaTime)
{
	m_WateTimeVar += DeltaTime;

	if (m_WateTime >= m_WateTimeVar)
	{
		m_MovingPos = Vector3(39.0f, 0.0f, 55.0f);
		m_PathFind = false;
		ChangeState(STS_CAN_TRACE, m_AniName);
		m_WateTimeVar = 0.0f;
	}
}

void ST3_Slient::FS_BED_FIND(float DeltaTime)
{
}

void ST3_Slient::FS_DESK_FIND(float DeltaTime)
{
}

void ST3_Slient::FS_LOCKER_FIND(float DeltaTime)
{
}

void ST3_Slient::FS_NORMAL_DOOR_OPEN_LEFT(float DeltaTime)
{
}

void ST3_Slient::FS_NORMAL_DOOR_OPEN_RIGHT(float DeltaTime)
{
}

void ST3_Slient::FS_BASH_DOOR(float DeltaTime)
{
}

void ST3_Slient::FS_BASH_DOOR_OPEN(float DeltaTime)
{

}
