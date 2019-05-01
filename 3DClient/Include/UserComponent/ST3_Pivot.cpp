#include "../ClientHeader.h"
#include "ST3_Pivot.h"

ST3_Pivot::ST3_Pivot()
{
}

ST3_Pivot::ST3_Pivot(const ST3_Pivot & CopyData)
	:NPCBase(CopyData)
{
}

ST3_Pivot::~ST3_Pivot()
{
}

bool ST3_Pivot::Init()
{
	NPCBase::Init();

	CreateBT("SlientTrace");
	return true;
}

int ST3_Pivot::Input(float fTime)
{
	NPCBase::Input(fTime);

	return 0;
}

int ST3_Pivot::Update(float fTime)
{
	NPCBase::Update(fTime);

	return 0;
}

int ST3_Pivot::LateUpdate(float fTime)
{
	return 0;
}

void ST3_Pivot::Collision(float fTime)
{
}

void ST3_Pivot::Render(float fTime)
{
}

ST3_Pivot * ST3_Pivot::Clone()
{
	return new ST3_Pivot(*this);
}
