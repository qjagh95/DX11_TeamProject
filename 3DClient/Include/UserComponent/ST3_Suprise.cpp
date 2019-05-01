#include "../ClientHeader.h"
#include "ST3_Suprise.h"

#include <BehaviorTree.h>

ST3_Suprise::ST3_Suprise()
{
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
	NPCBase::Init();

	m_AniName[SS_IDLE] = "";
	m_AniName[SS_TURN] = "";
	m_AniName[SS_MOVE] = "";

	ChangeState(SS_IDLE);

	CreateBT("Suprise");

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