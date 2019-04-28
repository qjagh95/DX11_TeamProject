#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Handycam.h"
#include "Component/Renderer.h"

bool CHuman_Player::Init_Cam()
{
	return true;
}

void CHuman_Player::OnDestroyCam()
{
	
}

void  CHuman_Player::HandyCam_Start()
{
	
	PUN::CGameObject *pHandyCamObj = m_pHandycam->GetGameObject();
	
	//pHandyCamObj->SetEnable(true);

	//m_pAnimation->SetSocketObject("Hero-L-Hand", "camhand", pHandyCamObj);

	PUN::CRenderer *pRenderer = m_pCamModelObj->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);

	pRenderer->DontRenderMat(true);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pHandyCamObj);

	m_pHandycam->SetVisible();
}
void CHuman_Player::HandyCam_End()
{

	PUN::CRenderer *pRenderer = m_pCamModelObj->FindComponentFromType<PUN::CRenderer>(PUN::CT_RENDERER);
	GET_SINGLE(CRenderManager)->SetOnOff(1.f);
	pRenderer->DontRenderMat(false);
	
	SAFE_RELEASE(pRenderer);
	m_pHandycam->SetVisible();
}

void CHuman_Player::HandyCam_On(float fTime)
{
	
}
void CHuman_Player::HandyCam_Using(float fTime)
{
	if (!m_pHandycam->GetVisible())
	{
		GET_SINGLE(CRenderManager)->SetOnOff(1.f);
	}
}
void CHuman_Player::HandyCam_Off(float fTime)
{
	
}

int CHuman_Player::CamTestAction(float fTime) 
{
	return 0;
}

int CHuman_Player::Input_Cam(float fTime) 
{
	return 0;
}
int CHuman_Player::CamUpdate(float fTime) 
{
	if (m_pHandycam->GetBatteryOut())
	{
		m_bBatteryOut = true;
	}

	else if (m_pHandycam->GetBatteryChange())
	{
		m_bBatteryChange = true;
	}

	if (m_iState & PSTATUS_CAM_ON)
	{
		if (m_bBatteryOut)
		{
			if (m_pHandycam->GetVisible())
			{
				m_pHandycam->SetVisible(false);
				m_pHandycam->SetCantUse(true);
				GET_SINGLE(CRenderManager)->SetOnOff(0.00001f);
				GET_SINGLE(CRenderManager)->SetStarLightScope(-1);
				m_bBatteryOut = false;
				m_pHandycam->SetOutFlag(m_bBatteryOut);
			}
		}
	}
	

	if (m_bBatteryChange)
	{
		m_pHandycam->SetChangeFlag(false);
		GET_SINGLE(CRenderManager)->SetOnOff(1.f);
		GET_SINGLE(CRenderManager)->SetStarLightScope(1);
		m_bBatteryChange = false;
	}
	return 0;
}
int CHuman_Player::CamLateUpdate(float fTime) 
{
	return 0;
}