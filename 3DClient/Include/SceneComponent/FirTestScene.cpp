#include "../ClientHeader.h"
#include "FirTestScene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Field.h"

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

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Field", pDefaultLayer, true);

	CField*	pField = pObject->AddComponent<CField>("Field");

	SAFE_RELEASE(pField);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("TestObject", pDefaultLayer, true);
	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(0.f, 0.f, -300.f);
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


	SAFE_RELEASE(pCamera);

	CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);

	CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	pLight->SetLightType(LT_DIR);
	pLight->SetLightDirection(Vector3(1.0f, -1.0f, 1.0f));

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pLightObj);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);
	pTransform = pLandScapeObj->GetTransform();

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
		TEXT("LandScape/Terrain_Cliff_11.dds"),
		TEXT("LandScape/Terrain_Cliff_11_NRM.bmp"),
		TEXT("LandScape/Terrain_Cliff_11_SPEC.bmp"),
		"LandScape/height2.bmp");

	vector<const TCHAR*> vecSplatName;

	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));

	pLandScape->AddSplatDifTexture("LandScapeSplatDif",	vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));

	pLandScape->AddSplatNrmTexture("LandScapeSplatNrm", vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));

	pLandScape->AddSplatSpcTexture("LandScapeSplatSpc", vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/RoadAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/StonAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/FlowerAlpha.bmp"));

	pLandScape->AddSplatAlphaTexture("LandScapeSplatAlpha", vecSplatName);

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
