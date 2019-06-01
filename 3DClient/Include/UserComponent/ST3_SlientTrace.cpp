#include "../ClientHeader.h"
#include "ST3_SlientTrace.h"

ST3_SlientTrace::ST3_SlientTrace()
{
	m_Attack = 0;
}

ST3_SlientTrace::ST3_SlientTrace(const ST3_SlientTrace & CopyData)
	:NPCBase(CopyData)
{

}

ST3_SlientTrace::~ST3_SlientTrace()
{
}

bool ST3_SlientTrace::Init()
{
	NPCBase::Init();

	/*
	1. ÂÑ¾Æ¿À´Â ¸÷.
	2. Mesh : Cannibal
	3. bne, anm : Cannibal

	Idle : idle_chase
	Move(Run) : move_forword_fase
	Locker search : search_locker
	¹®ÄçÄç¿­±â : bash_door

	*/

	m_AniName[STS_IDLE] = "idle_search_forward";
	m_AniName[STS_TRACE] = "move_forword_fast";
	m_AniName[STS_BASH_DOOR] = "bash_door_loop";
	m_AniName[STS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[STS_NORMAL_DOOR_OPEN_LEFT] = "open_door_left_push_close";
	m_AniName[STS_NORMAL_DOOR_OPEN_RIGHT] = "open_door_right_push_close";
	m_AniName[STS_BED_FIND] = "grap_bed_left";
	m_AniName[STS_DESK_FIND] = "grap_desk";
	//m_AniName[STS_DESK_FIND] = "grap_desk";
	m_AniName[STS_LOCKER_FIND] = "grap_locker";
	m_AniName[STS_HOOK] = "attack_right";
	m_AniName[STS_JAP] = "attack_left";
	m_AniName[STS_HEAD_ATTACK] = "attack_middle";

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Renderer->SetSelectBoneName("NPCMedium-Pelvis");
	m_Renderer->SetCheckStart(true);

	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("Cannibal.anm");

	//CreateBT("Slient");
	ChangeState(STS_BED_FIND, m_AniName);
	return true;
}

int ST3_SlientTrace::Input(float fTime)
{
	NPCBase::Input(fTime);

	return 0;
}

int ST3_SlientTrace::Update(float fTime)
{
	NPCBase::Update(fTime);

	switch (m_State)
	{
	case STS_BED_FIND:
		FS_BED_FIND(fTime);
		break;
	case STS_IDLE:
		FS_IDLE(fTime);
		break;
	}

	return 0;
}

int ST3_SlientTrace::LateUpdate(float fTime)
{
	return 0;
}

void ST3_SlientTrace::Collision(float fTime)
{
}

void ST3_SlientTrace::Render(float fTime)
{
}

ST3_SlientTrace * ST3_SlientTrace::Clone()
{
	return new ST3_SlientTrace(*this);
}

void ST3_SlientTrace::FS_IDLE(float DeltaTime)
{

}

void ST3_SlientTrace::FS_BED_FIND(float DeltaTime)
{
	if (m_Animation->IsCurAnimEnd() == true)
	{
		Vector3 GetRot = m_Renderer->GetDiffrent();

		m_pTransform->RotationY(GetRot.x);
		//ChangeState(STS_IDLE, m_AniName);
	}
}

void ST3_SlientTrace::FS_DESK_FIND(float DeltaTime)
{

}

