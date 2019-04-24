#include "../ClientHeader.h"
#include "Human_Player.h"
#include "Handycam.h"
#include "Component/Renderer.h"

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
	
	pRenderer->DontRenderMat(false);
	
	SAFE_RELEASE(pRenderer);
	m_pHandycam->SetVisible();
}

void CHuman_Player::HandyCam_On(float fTime)
{
	
}
void CHuman_Player::HandyCam_Using(float fTime)
{

}
void CHuman_Player::HandyCam_Off(float fTime)
{
	
}