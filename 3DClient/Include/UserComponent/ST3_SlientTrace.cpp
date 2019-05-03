#include "../ClientHeader.h"
#include "ST3_SlientTrace.h"

ST3_SlientTrace::ST3_SlientTrace()
{
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
	m_AniName[STS_WATE] = "idle_search_forward";
	m_AniName[STS_RUN] = "move_forword_fast";
	m_AniName[STS_TRACE] = "move_forword_fast";
	m_AniName[STS_BASH_DOOR] = "bash_door_loop";
	m_AniName[STS_BASH_DOOR_OPEN] = "bash_door_end";
	m_AniName[STS_NORMAL_DOOR_OPEN_LEFT] = "open_door_left_push_close";
	m_AniName[STS_NORMAL_DOOR_OPEN_RIGHT] = "open_door_right_push_close";

	m_AniName[STS_BED_FIND] = "";
	m_AniName[STS_DESK_FIND] = "";
	m_AniName[STS_LOCKER_FIND] = "";
	m_AniName[STS_HOOK] = "";
	m_AniName[STS_JAP] = "";
	m_AniName[STS_HEAD_ATTACK] = "";

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("Cannibal.anm");

	CreateBT("Slient");
	ChangeState(STS_IDLE, m_AniName);

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

