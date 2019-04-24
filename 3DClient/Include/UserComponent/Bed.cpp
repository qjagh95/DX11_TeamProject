#include "../ClientHeader.h"
#include "Bed.h"

CBed::CBed()
{
}

CBed::CBed(const CBed & battery)
{
}


CBed::~CBed()
{
}

void CBed::AfterClone()
{
}

bool CBed::Init()
{
	return true;
}

int CBed::Input(float fTime)
{
	return 0;
}

int CBed::Update(float fTime)
{
	return 0;
}

int CBed::LateUpdate(float fTime)
{
	return 0;
}

void CBed::Collision(float fTime)
{
}

void CBed::Render(float fTime)
{
}

CBed * CBed::Clone()
{
	return nullptr;
}
