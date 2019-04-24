#include "../ClientHeader.h"
#include "TutorialScene.h"
#include "Component/Arm.h"
#include "Component/Light.h"
#include "Component/Decal.h"
#include "Component/Camera.h"
#include "Component/Particle.h"
#include "Component/VolumeFog.h"
#include "Component/LandScape.h"
#include "Component/Animation2D.h"
#include "Component/ColliderOBB3D.h"
#include <Component/Renderer.h>
#include <Component/Material.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "Component/Gizmo.h"
#include "Component/FreeCamera.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/Battery.h"
#include "../UserComponent/BatteryIcon.h"

CTutorialScene::CTutorialScene()
{
}

CTutorialScene::~CTutorialScene()
{
}

bool CTutorialScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"Tutorial.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	GET_SINGLE(CRenderManager)->SetHDRValue(0.02f, 5.7f);

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CTransform* pCameraTr = pCamera->GetTransform();

	CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");

	SAFE_RELEASE(pFreeCam);
	SAFE_RELEASE(pCameraTr);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	CLayer* pLightLayer = m_pScene->FindLayer("Light");

	pObject = CGameObject::CreateObject("SpotLight", pLightLayer);

	CLight* pLight = pObject->AddComponent<CLight>("Light");
	pLight->SetLightDiffuse(Vector4::Red);
	pLight->SetLightType(LT_DIR);
	pLight->SetLightRange(100.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(50.0f, 70.0f, 0.0f);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLightLayer);

	//////////////////////////////////////////Terrain 배치 //////////////////////////////////////////////////
	///////////////////////////////X Tile 배치///////////////////////////////////////////////////////

		/*pObject = CGameObject::CreateObject("Stair", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("StairRenderer");

		pRenderer->SetMesh("Stair", L"Stair.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(350.f, 40.f, 0.f);
		pTransform->SetWorldRot(0.f, 270.f, 0.f);
		pTransform->SetWorldScale(0.1f);

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);*/

	/*pObject = CGameObject::CreateObject("Ground", pDefaultLayer);

	CRenderer*	pRenderer = pObject->AddComponent<CRenderer>("GroundRenderer");

	pRenderer->SetMesh("Ground", L"Ext_GroundCap-01.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.1f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Ground2", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("Ground2Renderer");

	pRenderer->SetMesh("Ground2", L"Ext_GroundCap-06.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(75.f, 0.f, 200.f);
	pTransform->SetWorldScale(0.1f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Ground3", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("Ground3Renderer");

	pRenderer->SetMesh("Ground3", L"Ext_GroundCap-08.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(90.f, 7.f, 30.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.25f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Ground4", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("Ground4Renderer");

	pRenderer->SetMesh("Ground4", L"Ext_GroundCap-09.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(80.f, 5.f, -80.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.1f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Ground5", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("Ground5Renderer");

	pRenderer->SetMesh("Ground5", L"Ext_GroundCap-09.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(80.f, 5.f, 136.f);
	pTransform->SetWorldRot(0.f, 180.f, 14.f);
	pTransform->SetWorldScale(0.1f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);*/

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pUILayer);

	return true;
}

int CTutorialScene::Update(float fTime)
{
	return 0;
}
