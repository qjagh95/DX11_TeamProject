#include "../ClientHeader.h"
#include "JBH_Stage3.h"

#include <Component/Camera.h>

JBH_Stage3::JBH_Stage3()
{
}

JBH_Stage3::~JBH_Stage3()
{
}

bool JBH_Stage3::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");





	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);

	return true;
}

int JBH_Stage3::Update(float DeltaTime)
{
	return 0;
}

int JBH_Stage3::LateUpdate(float DeltaTime)
{
	return 0;
}

void JBH_Stage3::Collision(float DeltaTime)
{
}

void JBH_Stage3::Render(float DeltaTime)
{
}
