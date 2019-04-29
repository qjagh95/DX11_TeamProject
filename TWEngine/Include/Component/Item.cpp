#include "../EngineHeader.h"
#include "Item.h"

PUN_USING

CItem::CItem()
{
}


CItem::CItem(const CItem & item)	:
	CComponent(item)
{
}

CItem::~CItem()
{
}

void CItem::Start()
{
}

void CItem::AfterClone()
{
}

bool CItem::Init()
{
	return true;
}

int CItem::Input(float fTime)
{
	return 0;
}

int CItem::Update(float fTime)
{
	return 0;
}

int CItem::LateUpdate(float fTime)
{
	return 0;
}

void CItem::Collision(float fTime)
{
}

void CItem::Render(float fTime)
{
}
