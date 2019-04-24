#include "../ClientHeader.h"
#include "Locker.h"

CLocker::CLocker()
{
}

CLocker::CLocker(const CLocker & battery)
{
}

CLocker::~CLocker()
{
}

void CLocker::AfterClone()
{
}

bool CLocker::Init()
{
	return true;
}

int CLocker::Input(float fTime)
{
	return 0;
}

int CLocker::Update(float fTime)
{
	return 0;
}

int CLocker::LateUpdate(float fTime)
{
	return 0;
}

void CLocker::Collision(float fTime)
{
}

void CLocker::Render(float fTime)
{
}

CLocker * CLocker::Clone()
{
	return nullptr;
}
