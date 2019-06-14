#include "../ClientHeader.h"
#include "LogoScene.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Renderer.h"
#include "Component/UIButton.h"
#include "Component/EndButton.h"
#include "Component/ContinueButton.h"
#include "TutorialScene.h"
#include "../UserComponent/Human_Player.h"
#include "Device.h"
#include "Input.h"
#include "Core.h"
#include "../UserComponent/Katok.h"
#include "Rendering/RenderManager.h"

CLogoScene::CLogoScene()
{
	m_fChangeTime = 0.f;
	m_bNextScene = false;
	m_bStart = false;
}

CLogoScene::~CLogoScene()
{
	SAFE_RELEASE(m_pUILayer);

	if (m_bStart)
	{
		SAFE_RELEASE(m_pPMsgTr);
		SAFE_RELEASE(m_pMsgObj);
	}

	SAFE_RELEASE(m_pSButtonObj);
	SAFE_RELEASE(m_pCButtonObj);
	SAFE_RELEASE(m_pEButtonObj);
	SAFE_RELEASE(m_pPlayerObj);
}

void CLogoScene::AfterInit()
{
	m_pPlayerObj = CGameObject::FindObject("Player");

	m_pPlayerObj->SetEnable(false);

	_PLAYER->SetEnable(false);
	GET_SINGLE(CInput)->ShowMouse(true);
}

bool CLogoScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	m_pUILayer = m_pScene->FindLayer("UI");
	
	// BackGround
	CGameObject* pBackObj = CGameObject::CreateObject("BackGround", m_pUILayer);
	pBackObj->SetRenderGroup(RG_UI);

	CRenderer* pRenderer = pBackObj->AddComponent<CRenderer>("BGRenderer");
	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	CMaterial* pMaterial = pBackObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "BackGroundTex", L"Scene/StartScene.jpg");
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	CTransform* pBackTr = pBackObj->GetTransform();

	pBackTr->SetWorldScale((float)_RESOLUTION.iWidth, (float)_RESOLUTION.iHeight, 1.f);

	SAFE_RELEASE(pBackTr);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pBackObj);

	// Start Button
	m_pSButtonObj = CGameObject::CreateObject("StartButton", m_pUILayer);

	CUIButton*	pSButton = m_pSButtonObj->AddComponent<CUIButton>("StartButton");

	pSButton->SetCallback(&CLogoScene::StartBtnCallback, this);

	CTransform*	pSBTr = m_pSButtonObj->GetTransform();

	//pSBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2.0f), (float)(_RESOLUTION.iHeight / 2.0f - 250.f), 0.f);
	pSBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2.0f), (float)(_RESOLUTION.iHeight / 2.0f - 150.f), 0.f);
	pSBTr->SetWorldScale(258.f, 55.f, 1.f);

	SAFE_RELEASE(pSBTr);
	SAFE_RELEASE(pSButton);

	// Contine Button
	m_pCButtonObj = CGameObject::CreateObject("ContinueButton", m_pUILayer);

	CContinueButton*	pCButton = m_pCButtonObj->AddComponent<CContinueButton>("ContinueButton");

	pCButton->SetCallback(&CLogoScene::Continue, this);

	CTransform*	pCBTr = m_pCButtonObj->GetTransform();

	//pCBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2), (float)(_RESOLUTION.iHeight / 2.0f - 350.f), 0.f);
	pCBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2), (float)(_RESOLUTION.iHeight / 2.0f - 220.f), 0.f);
	pCBTr->SetWorldScale(258.f, 55.f, 1.f);

	SAFE_RELEASE(pCBTr);
	SAFE_RELEASE(pCButton);

	// End Button
	m_pEButtonObj = CGameObject::CreateObject("EndButton", m_pUILayer);

	CEndButton*	pEButton = m_pEButtonObj->AddComponent<CEndButton>("EndButton");

	pEButton->SetCallback(&CLogoScene::Exit, this);

	CTransform*	pEBTr = m_pEButtonObj->GetTransform();

	//pEBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2.0f), (float)(_RESOLUTION.iHeight / 2.0f - 450.f), 0.f);
	pEBTr->SetWorldPos((float)(_RESOLUTION.iWidth / 2.0f), (float)(_RESOLUTION.iHeight / 2.0f - 290.f), 0.f);
	pEBTr->SetWorldScale(258.f, 55.f, 1.f);

	SAFE_RELEASE(pEBTr);
	SAFE_RELEASE(pEButton);

	SAFE_RELEASE(pCamera);

	return true;
}

int CLogoScene::Update(float fTime)
{
	if (m_bNextScene)
	{
		m_fChangeTime += fTime;
		static bool	bPush = true;
		static bool bMove = true;

		m_pSButtonObj->SetEnable(false);
		m_pCButtonObj->SetEnable(false);
		m_pEButtonObj->SetEnable(false);
		
		if (bPush)
		{
			bPush = false;

			m_pMsgObj = CGameObject::CreateObject("PMessage", m_pUILayer);

			m_pMsgObj->SetRenderGroup(RG_UI);

			CKaTok*	pMsg = m_pMsgObj->AddComponent<CKaTok>("PMessage");

			m_pPMsgTr = m_pMsgObj->GetTransform();

			//m_pPMsgTr->SetWorldScale(1080.f, 1080.f, 1.f);
			//m_pPMsgTr->SetWorldPos(_RESOLUTION.iWidth - 550.f, _RESOLUTION.iHeight/2.f, 0.f);
			m_pPMsgTr->SetWorldScale(720.f, 720.f, 1.f);
			m_pPMsgTr->SetWorldPos(950.f, 350.f, 0.f);

			SAFE_RELEASE(pMsg);
		}

		if (bMove)
		{
			m_pPMsgTr->Move(Vector3(-1.f, 0.f, 0.f), 3000.f, fTime);
		}

		if (m_fChangeTime >= 0.1f && m_fChangeTime < 10.f)
		{
			bMove = false;
		}

		else if (m_fChangeTime >= 10.f)
		{
			GET_SINGLE(CRenderManager)->SetFadeAmount(0.001f, fTime);

			m_pMsgObj->Die();

			GET_SINGLE(CSceneManager)->ChangeScene("TutorialScene");

			m_pPlayerObj->SetEnable(true);
			_PLAYER->SetEnable(true);
			GET_SINGLE(CInput)->ShowMouse(false);

			CGameObject*	pPlayerObj = CGameObject::FindObject("Player");

			CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromTag<CHuman_Player>("Player");

			CTransform*	pTr = pPlayerObj->GetTransform();

			pTr->SetLocalRot(0.f, 180.f, 0.f);
			pTr->SetWorldScale(0.0375f, 0.0375f, 0.0375f);
			pTr->SetWorldPos(316.f, 20.f, 748.f);
			pPlayer->PlayerRot(Vector3(0.f, 180.f, 0.f));

			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pPlayer);
			SAFE_RELEASE(pPlayerObj);
		}
	}

	return 0;
}

void CLogoScene::NextScene(float fTime)
{	
}

void CLogoScene::StartBtnCallback(float fTime)
{	
	m_bNextScene = true;
	m_bStart = true;
}

void CLogoScene::Continue(float fTime)
{
}

void CLogoScene::Exit(float fTime)
{
	DestroyWindow(WINDOWHANDLE);
}
