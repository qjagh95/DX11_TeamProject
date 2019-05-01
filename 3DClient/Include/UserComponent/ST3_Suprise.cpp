#include "../ClientHeader.h"
#include "ST3_Suprise.h"
#include <DirectXCollision.h>

#include <BehaviorTree.h>

ST3_Suprise::ST3_Suprise()
{
	m_FindDist = 10.0f;
	m_isDoorOpenMode = false;
	m_Axis = AX_X;
	m_Offset = 6.0f;
}

ST3_Suprise::ST3_Suprise(const ST3_Suprise & CopyData)
	:NPCBase(CopyData)
{
}

ST3_Suprise::~ST3_Suprise()
{
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
	m_Animation->LoadBone("NpcMediumDeathPoses-01_AS.bne");
	m_Animation->Load("NpcMediumDeathPoses-01_AS.anm");

	m_MoveSpeed = 200.0f;

	m_AniName[SS_FIND] = "NPCMediumHanging-Looping-01";
	m_AniName[SS_IDLE] = "NPCMediumHanging-Looping-01";
	m_AniName[SS_DOWN] = "NPCMediumHanging-Looping-01";

	ChangeState(SS_FIND, m_AniName);
	SetOBBCollider(Vector3(1.0f, 10.0f, 1.0f));
	CSoundManager::Get()->CreateSoundEffect("Suprise", L"music/MUS_Demo_ending.wav");

	return true;
}

int ST3_Suprise::Input(float fTime)
{
	NPCBase::Input(fTime);

	return 0;
}

int ST3_Suprise::Update(float fTime)
{
	NPCBase::Update(fTime);

	switch (m_State)
	{
	case SS_IDLE:
		FS_IDLE(fTime);
		break;
	case SS_DOWN:
		FS_DOWN(fTime);
		break;
	case SS_FIND:
		FS_FIND(fTime);
		break;
	}

	return 0;
}

int ST3_Suprise::LateUpdate(float fTime)
{
	return 0;
}

void ST3_Suprise::Collision(float fTime)
{
}

void ST3_Suprise::Render(float fTime)
{
}

ST3_Suprise * ST3_Suprise::Clone()
{
	return new ST3_Suprise(*this);
}

void ST3_Suprise::FS_IDLE(float DeltaTime)
{

}

void ST3_Suprise::FS_DOWN(float DeltaTime)
{
	if (m_pTransform->GetWorldPos().y <= 5.0f)
	{
		m_pTransform->SetWorldPos(Vector3(m_pTransform->GetWorldPos().x, 5.0f , m_pTransform->GetWorldPos().z));
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
	}
}
