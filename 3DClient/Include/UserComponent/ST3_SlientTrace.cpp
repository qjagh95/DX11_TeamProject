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

	m_Renderer->SetMesh("Cannibal", TEXT("Cannibal.msh"));
	m_Animation->LoadBone("Cannibal.bne");
	m_Animation->Load("Cannibal.anm");

	CreateBT("Slient");
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

