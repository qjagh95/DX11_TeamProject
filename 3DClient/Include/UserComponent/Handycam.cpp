#include "../ClientHeader.h"
#include "Handycam.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Rendering/RenderManager.h"
#include "Component/UIBar.h"
#include "BatteryIcon.h"
#include "Device.h"
#include <SoundManager.h>

CHandycam::CHandycam()
{
	m_eComType = (COMPONENT_TYPE)UT_HANDYCAM;
	m_iFlag = -1;
	m_bVisible = false;
	m_bBatteryOut = false;
	m_bChangeFlag = false;
	m_bChange = false;
	m_bCantUse = false;

	m_fBatteryTime = 0.8f;
}

CHandycam::CHandycam(const CHandycam & handycam)	:
	CUserComponent(handycam)
{
}

CHandycam::~CHandycam()
{
	SAFE_RELEASE(m_pBarObj);
	SAFE_RELEASE(m_pBar);
}

void CHandycam::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;

	m_pObject->SetEnable(m_bVisible);

	if (m_bVisible)
		m_iFlag = -1;
	else if (!m_bVisible)
		m_iFlag = 1;
}

void CHandycam::SetVisible()
{
	if (m_iFlag == -1)
	{
		if (m_bCantUse)
		{
			CSoundManager::GetInst()->SoundPlay("Cam_Start", 0.f);
			CSoundManager::GetInst()->SoundPlay("NV_Start", 0.f);
			m_bVisible = true;
			GET_SINGLE(CRenderManager)->SetOnOff(0.00001f);
			GET_SINGLE(CRenderManager)->SetStarLightScope(-1);
		}
		else
		{
			CSoundManager::GetInst()->SoundPlay("Cam_Start");
			CSoundManager::GetInst()->SoundPlay("NV_Start");
			m_bVisible = true;
			GET_SINGLE(CRenderManager)->SetOnOff(1.f);
			GET_SINGLE(CRenderManager)->SetStarLightScope(1);
			m_pObject->SetEnable(true);
			m_pBarObj->SetEnable(true);
		}
	}

	else if (m_iFlag == 1)
	{
		if(m_bCantUse)
			CSoundManager::GetInst()->SoundPlay("Cam_Stop", 0.f);

		else
			CSoundManager::GetInst()->SoundPlay("Cam_Stop");

		m_bVisible = false;
		m_pObject->SetEnable(false);
		m_pBarObj->SetEnable(false);
		GET_SINGLE(CRenderManager)->SetStarLightScope(-1);
	}

	m_iFlag *= -1;
}

void CHandycam::SetOutFlag(bool bFlag)
{
	m_bBatteryOut = bFlag;
}

void CHandycam::SetChangeFlag(bool bFlag)
{
	m_bChangeFlag = bFlag;
}

void CHandycam::SetCantUse(bool bCantUse)
{
	m_bCantUse = bCantUse;
}

void CHandycam::DisableHandycam()
{
	m_iFlag = 1;
}

void CHandycam::SetBatteryTime(float fBatteryTime)
{
	m_fBatteryTime = fBatteryTime;
}

void CHandycam::ChangeBattery(bool bFlag)
{
	m_bChangeFlag = bFlag;

	if (m_bChangeFlag)
	{
		m_pBar->SetValue(120.f);

		m_bChangeFlag = true;
		m_bCantUse = false;
	}
}

bool CHandycam::GetBatteryOut() const
{
	return m_bBatteryOut;
}

bool CHandycam::GetBatteryChange() const
{
	return m_bChange;
}

bool CHandycam::GetVisible() const
{
	return m_bVisible;
}

bool CHandycam::GetCantUse() const
{
	return m_bCantUse;
}

void CHandycam::AfterClone()
{
}

bool CHandycam::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("Cam_Start", TEXT("Bip_Cam_START.wav"));
	CSoundManager::GetInst()->CreateSoundEffect("NV_Start", TEXT("NV_Start_HFq.wav"));
	CSoundManager::GetInst()->CreateSoundEffect("Cam_Stop", TEXT("Bip_Cam_STOP.wav"));
	CSoundManager::GetInst()->CreateSoundEffect("Low_Battery", TEXT("CAM_Low_Battery_Clue.wav"));
	CSoundManager::GetInst()->CreateSoundEffect("Reload_Battery", TEXT("CAM_Reload_Battery.wav"));

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("HandycamRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Handycam", TEXT("UI/CameraHud.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();

	pTransform->SetWorldScale((float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 1.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	m_pBarObj = CGameObject::CreateObject("HandycamBar", pUILayer, true);	   

	m_pBar = m_pBarObj->AddComponent<CUIBar>("HandycamBar");

	m_pBar->SetScale(145.f, 20.f, 1.f);
	m_pBar->SetBarDir(BD_RIGHT);
	m_pBar->SetMinMaxValue(0.f, 120);
	m_pBar->SetValue(120.f);

	CTransform*	pBarTr = m_pBarObj->GetTransform();

	pBarTr->SetWorldPos(_RESOLUTION.iWidth * (1.f - (1 - 1152.f / 1280.f)) + 7.f, _RESOLUTION.iHeight * (1.f - (1 - 629.f / 720.f)), 0.f);
	pBarTr->SetWorldScale(93.f, 13.f, 1.f);

	m_pBarObj->SetRenderGroup(RG_UI);
	SAFE_RELEASE(pBarTr);

	SAFE_RELEASE(pUILayer);

	m_pBarObj->SetEnable(false);
	m_pObject->SetEnable(false);

	return true;
}

int CHandycam::Input(float fTime)
{
	return 0;
}

int CHandycam::Update(float fTime)
{
	if (m_pObject->GetEnable() == true)
	{
		m_pBar->AddValue(-0.01f * fTime);
	}

	float fValue = m_pBar->GetValue();

	if (fValue < 60.f)
	{

		m_fBatteryTime += fTime;

		m_pBar->SetVisible(m_fBatteryTime);

		if (m_fBatteryTime > 1.f)
		{
			CSoundManager::GetInst()->SoundPlay("Low_Battery");
			m_fBatteryTime = 0.f;
		}
	}	

	else if (fValue > 30.f)
	{
		m_pBar->SetColor(Vector4::White);
	}

	if (m_pBar->GetValue() == 0.f && m_bBatteryOut == false)
	{
		m_bBatteryOut = true;
	}

	else
		m_bBatteryOut = false;

	if (m_bChangeFlag)
	{
		CSoundManager::GetInst()->SoundPlay("Reload_Battery");
		m_bChange = true;
	}

	else
		m_bChange = false;

	return 0;
}

int CHandycam::LateUpdate(float fTime)
{
	return 0;
}

void CHandycam::Collision(float fTime)
{
}

void CHandycam::Render(float fTime)
{
}

CHandycam * CHandycam::Clone()
{
	return new CHandycam(*this);
}
