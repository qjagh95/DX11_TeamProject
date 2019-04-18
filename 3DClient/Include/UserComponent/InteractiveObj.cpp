#include "../ClientHeader.h"
#include "InteractiveObj.h"

CInteractiveObj::CInteractiveObj() :
	m_pInteractiveCol(nullptr),
	m_bActivated(false)
{
}

CInteractiveObj::CInteractiveObj(const CInteractiveObj & obj):
	PUN::CUserComponent(obj)
{
	m_bActivated = false;
	m_pInteractiveCol = nullptr;
	
}

CInteractiveObj::~CInteractiveObj()
{
	SAFE_RELEASE(m_pInteractiveCol);
	
}

bool CInteractiveObj::Init()
{
	AfterClone();
	return true;
}

int CInteractiveObj::Update(float fTime)
{
	return 0;
}

int CInteractiveObj::LateUpdate(float fTime)
{
	return 0;
}

void CInteractiveObj::Collision(float fTime)
{
}

void CInteractiveObj::Render(float fTime)
{
}

void CInteractiveObj::AfterClone()
{
	
}

bool CInteractiveObj::Activate(bool bSwitch)
{
	return m_bActivated;
}

bool CInteractiveObj::IsActivated() const
{
	return m_bActivated;
}


const Vector3 & CInteractiveObj::GetInteractOffset() const
{
	return m_vInteractOffset;
}

void CInteractiveObj::SetInteractOffset(const Vector3 & offset)
{
	m_vInteractOffset = offset;
}
