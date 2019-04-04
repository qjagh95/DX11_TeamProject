#include "../ClientHeader.h"
#include "LogoScene.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Renderer.h"
#include "StartScene.h"
#include "FirTestScene.h"

CLogoScene::CLogoScene()
{
	m_fChangeTime = 0.f;
}


CLogoScene::~CLogoScene()
{
}


bool CLogoScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_ORTHO);
	pCamera->SetNear(0.f);

	CLayer* pUILayer = m_pScene->FindLayer("UI");

	
	CGameObject* pGameObject = CGameObject::CreateObject("Logo", pUILayer);
	pGameObject->SetRenderGroup(RG_UI);
	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("LogoRenderer");
	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	CMaterial* pMaterial = pGameObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "LogoTex", L"WelcomeLogo.png");

	pMaterial->SetSampler(0, SAMPLER_LINEAR);
	CTransform* pLogoTr = pGameObject->GetTransform();

	pLogoTr->SetWorldPos(640.f, 360.f , 0.f);
	pLogoTr->SetWorldPivot(Vector3(0.5f, 0.5f, 0.f));
	pLogoTr->SetWorldScale(756.f, 156.f, 1.f);

	SAFE_RELEASE(pUILayer);

	SAFE_RELEASE(pLogoTr);

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pGameObject);

	SAFE_RELEASE(pCamera);

	return true;
}

int CLogoScene::Update(float fTime)
{
	m_fChangeTime += fTime;

	if (m_fChangeTime >= 3.f)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->AddSceneComponent<CStartScene>("StartScene", false);
	}
	return 0;
}

void CLogoScene::CreatePrototype()
{
}

void CLogoScene::NextScene(float fTime)
{
	
}