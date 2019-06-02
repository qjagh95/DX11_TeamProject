#include "../ClientHeader.h"
#include "ST3_Suprise.h"

#include "../GameManager.h"
#include "Door.h"

#include <BehaviorTree.h>

ST3_Suprise::ST3_Suprise()
{
	m_FindDist = 10.0f;
	m_isDoorOpenMode = false;
	m_Axis = AX_X;
	m_Offset = 6.0f;
	m_YOffset = 5.0f;

	m_3DSoundObject = NULLPTR;
	m_3DSound = NULLPTR;
}

ST3_Suprise::ST3_Suprise(const ST3_Suprise & CopyData)
	:NPCBase(CopyData)
{
}

ST3_Suprise::~ST3_Suprise()
{
	SAFE_RELEASE(m_3DSoundObject);
	SAFE_RELEASE(m_3DSound);
}

bool ST3_Suprise::Init()
{
	/*
	//Ã¹¹øÂ°¸÷.
	Patient20.msh
	NpcMediumDeathPoses-01_AS.bne
	NpcMediumDeathPoses-01_AS.anm
	
	ClipName
	NPCMediumHanging-Looping-01
	
	Sound/music/MUS_Demo_ending.wav
	*/

	NPCBase::Init();
	m_Renderer->SetMesh("Patient20", TEXT("Patient20.msh"));
	m_Animation->LoadBone("patient20.bne");
	m_Animation->Load("NpcMediumDeathPoses-01_AS.anm");

	m_MoveSpeed = 200.0f;

	m_AniName[SS_FIND] = "NPCMediumHanging-Looping-01";
	m_AniName[SS_IDLE] = "NPCMediumHanging-Looping-01";
	m_AniName[SS_DOWN] = "NPCMediumHanging-Looping-01";

	ChangeState(SS_FIND, m_AniName);
	SetOBBCollider(Vector3(1.0f, 5.0f, 1.0f));
	CSoundManager::Get()->CreateSoundEffect("Suprise", L"music/10-AI NPC CHASE INTRO 3.wav");

	m_3DSoundObject = CGameObject::CreateObject("3DSound", m_pLayer);
	m_3DSound = m_3DSoundObject->AddComponent<CSoundSource>("3DSound");

	vector<string> soundNames;
	soundNames.push_back("Wood1");
	soundNames.push_back("Wood2");
	soundNames.push_back("Wood3");
	soundNames.push_back("WoodCrash");

	vector<wstring> FileNames;
	FileNames.push_back(L"ambient/Wood_Creak_3D_01.wav");
	FileNames.push_back(L"ambient/Wood_Creak_3D_02.wav");
	FileNames.push_back(L"ambient/Wood_Creak_3D_03.wav");
	FileNames.push_back(L"wood_brake.wav");
	m_3DSound->LoadSounds(soundNames, FileNames);

	return true;
}

int ST3_Suprise::Input(float DeltaTime)
{
	return 0;
}

int ST3_Suprise::Update(float DeltaTime)
{
	NPCBase::Update(DeltaTime);

	switch (m_State)
	{
	case SS_IDLE:
		FS_IDLE(DeltaTime);
		break;
	case SS_DOWN:
		FS_DOWN(DeltaTime);
		break;
	case SS_FIND:
		FS_FIND(DeltaTime);
		break;
	}

	return 0;
}

int ST3_Suprise::LateUpdate(float DeltaTime)
{
	return 0;
}

void ST3_Suprise::Collision(float DeltaTime)
{
}

void ST3_Suprise::Render(float DeltaTime)
{
}

ST3_Suprise * ST3_Suprise::Clone()
{
	return new ST3_Suprise(*this);
}

void ST3_Suprise::FS_IDLE(float DeltaTime)
{
	if (m_TargetDistance > 10.0f)
		return;

	if (m_Animation->IsCurAnimEnd() == true)
	{
		m_3DSoundObject->GetTransformNonCount()->SetWorldPos(m_pTransform->GetWorldPos());

		int RandNum = rand() % 3;
		RandNum += 1;

		string Temp = "Wood";

		switch (RandNum)
		{
		case 1:
			Temp += "1";
			break;
		case 2:
			Temp += "2";
			break;
		case 3:
			Temp += "3";
			break;
		}
		m_3DSound->Play(Temp);
	}
}

void ST3_Suprise::FS_DOWN(float DeltaTime)
{
	if (m_pTransform->GetWorldPos().y <= m_YOffset)
	{
		m_pTransform->SetWorldPos(Vector3(m_pTransform->GetWorldPos().x, m_YOffset, m_pTransform->GetWorldPos().z));
		CSoundManager::Get()->SoundPlay("WoodCrash");
		CSoundManager::Get()->SoundPlay("Suprise");
		ChangeState(SS_IDLE, m_AniName);
	}
	else
		m_pTransform->Move(AXIS_Y, -m_MoveSpeed, DeltaTime);
}

void ST3_Suprise::FS_FIND(float DeltaTime)
{
	if (m_isDoorOpenMode == false)
	{
		if (m_Axis & AX_X)
		{
			if (m_pTransform->GetWorldPos().x >= m_TargetTransform->GetWorldPos().x - m_Offset)
				ChangeState(SS_DOWN, m_AniName);
		}
		else if (m_Axis & AX_Z)
		{
			if (m_pTransform->GetWorldPos().z >= m_TargetTransform->GetWorldPos().z - m_Offset)
				ChangeState(SS_DOWN, m_AniName);
		}
	}
	else
	{
		if (CGameManager::GetInst()->FindDoor("Second", m_DoorName)->IsOpenFinished() == true)
			ChangeState(SS_DOWN, m_AniName);
	}
}
