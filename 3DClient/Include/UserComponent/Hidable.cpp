#include "../Client.h"
#include "Hidable.h"



CHidable::CHidable()
{
	m_eComType = (COMPONENT_TYPE)UT_HIDABLE;
	m_eType = HT_NONE;
}

CHidable::CHidable(const CHidable & battery)
{
}


CHidable::~CHidable()
{
}

HIDABLE_TYPE CHidable::GetHidableType() const
{
	return m_eType;
}

void CHidable::AfterClone()
{
}

bool CHidable::Init()
{
	return false;
}

int CHidable::Input(float fTime)
{
	return 0;
}

int CHidable::Update(float fTime)
{
	return 0;
}

int CHidable::LateUpdate(float fTime)
{
	return 0;
}

void CHidable::Collision(float fTime)
{
}

void CHidable::Render(float fTime)
{
}

CHidable * CHidable::Clone()
{
	return nullptr;
}
