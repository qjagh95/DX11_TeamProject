#include "EngineHeader.h"
#include "UserComponent.h"

PUN_USING

CUserComponent::CUserComponent()
{
}

CUserComponent::CUserComponent(const CUserComponent & com)	
	:CComponent(com)
{
}

CUserComponent::~CUserComponent()
{
}

void CUserComponent::Start()
{
}

void CUserComponent::AfterClone()
{
}

bool CUserComponent::Init()
{
	return true;
}

int CUserComponent::Input(float fTime)
{
	return 0;
}

int CUserComponent::Update(float fTime)
{
	return 0;
}

int CUserComponent::LateUpdate(float fTime)
{
	return 0;
}

void CUserComponent::Collision(float fTime)
{
}

void CUserComponent::Render(float fTime)
{
}
