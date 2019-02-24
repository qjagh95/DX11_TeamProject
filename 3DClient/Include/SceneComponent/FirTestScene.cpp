#include "FirTestScene.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/Material.h"
#include "Component/Stage2D.h"
#include "Component/UIBar.h"
#include "NavigationManager.h"
#include "Component/Sound.h"
#include "SoundManager.h"
#include "Component/Light.h"
#include "Input.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Field.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
CFirTestScene::CFirTestScene()
{
}


CFirTestScene::~CFirTestScene()
{
}

bool CFirTestScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");

	pRenderer->SetMesh("BlackCow", TEXT("Monster4.msh"));

	CAnimation* pAnimation = pObject->AddComponent<CAnimation>("Animation");

	pAnimation->LoadBone("Monster4.bne");
	pAnimation->Load("Monster4.anm");

	SAFE_RELEASE(pAnimation);
	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Field", pDefaultLayer, true);

	CField*	pField = pObject->AddComponent<CField>("Field");

	SAFE_RELEASE(pField);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("TestObject", pDefaultLayer, true);
	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(0.f, 300.f, -300.f);
	pTransform->SetWorldRotX(45.f);
	pCamera->SetTarget(pObject);
	m_pTr = pTransform;

	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pTransform);

	CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");

	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pObject);



	/*CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, 1.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("ColorSphere");

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);*/


	SAFE_RELEASE(pCamera);

	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);
	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldRot(-90.f, 0.f, 0.f);
	pTransform->SetWorldPos(0.f, -1.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::Red, Vector4::White, Vector4::Blue);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(10.f);
	pLight->SetAngle(60.f, 90.f);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);

	pTransform = pLandScapeObj->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, -2.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
		TEXT("LandScape/Terrain_Cliff_11.dds"), "LandScape/height1_24.bmp");

	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLandScapeObj);


	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CFirTestScene::Update(float fTime)
{
	return 0;
}

void CFirTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CFirTestScene::CreatePrototype()
{
}

void CFirTestScene::NextScene(float fTime)
{
}
