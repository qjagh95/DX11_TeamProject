#include "EngineHeader.h"
#include "EditorScene.h"
#include "Component/Camera.h"
#include "Component/Animation.h"
#include "Component/Light.h"
#include "Component/LandScape.h"


PUN_USING

EditorScene::EditorScene()
{
}


EditorScene::~EditorScene()
{
}

bool EditorScene::Init()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	pCameraTr->SetWorldPos(Vector3(0.f, 0.0f, -200.0f));
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

	SAFE_RELEASE(pCamera);

	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);
	pTransform = pLightObj->GetTransform();

	pTransform->SetWorldRot(45.f , 0.f, 0.f);
	pTransform->SetWorldPos(0.f, -1.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White , Vector4::White, Vector4::White);
	pLight->SetLightType(LT_DIR);
	
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);

	pTransform = pLandScapeObj->GetTransform();

	pTransform->SetWorldScale(3.f, 1.f, 3.f);
	pTransform->SetWorldPos(0.f, -2.f, 0.f);

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
		TEXT("LandScape/Terrain_Cliff_11.dds"),
		TEXT("LandScape/Terrain_Cliff_11_NRM.bmp"),
		TEXT("LandScape/Terrain_Cliff_11_SPEC.bmp"),
		"LandScape/height1.bmp");

	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLandScapeObj);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int EditorScene::Update(float fTime)
{
	return 0;
}

void EditorScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void EditorScene::CreatePrototype()
{
}
