#include "../ClientHeader.h"
#include "EventMessage.h"

CEventMessage::CEventMessage()
{
}

CEventMessage::CEventMessage(const CEventMessage & eventmsg)	:
	CUserComponent(eventmsg)
{
}

CEventMessage::~CEventMessage()
{
}

void CEventMessage::AfterClone()
{
}

bool CEventMessage::Init()
{
	return true;
}

int CEventMessage::Input(float fTime)
{
	return 0;
}

int CEventMessage::Update(float fTime)
{
	return 0;
}

int CEventMessage::LateUpdate(float fTime)
{
	return 0;
}

void CEventMessage::Collision(float fTime)
{
}

void CEventMessage::Render(float fTime)
{
}

CEventMessage * CEventMessage::Clone()
{
	return new CEventMessage(*this);
}

void CEventMessage::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CEventMessage::Out(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
