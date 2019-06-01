#include "../ClientHeader.h"
#include "Event.h"

CEvent::CEvent()
{
	m_eComType = (COMPONENT_TYPE)UT_EVENT;
	m_pAnimation = nullptr;
}

CEvent::CEvent(const CEvent & event)	:
	CUserComponent(event)
{
	m_pAnimation = nullptr;
}

CEvent::~CEvent()
{
	SAFE_RELEASE(m_pAnimation);
}

void CEvent::AfterClone()
{
}

bool CEvent::Init()
{
	return true;
}

int CEvent::Input(float fTime)
{
	return 0;
}

int CEvent::Update(float fTime)
{
	return 0;
}

int CEvent::LateUpdate(float fTime)
{
	return 0;
}

void CEvent::Collision(float fTime)
{
}

void CEvent::Render(float fTime)
{
}

CEvent * CEvent::Clone()
{
	return new CEvent(*this);
}

void CEvent::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CEvent::Out(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
