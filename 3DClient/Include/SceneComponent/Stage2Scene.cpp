#include "../ClientHeader.h"
#include "Stage2Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/LandScape.h"
#include "Component/Arm.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Field.h"
#include "Component/Gizmo.h"
#include "Component/ParkourTest.h"
#include <NavigationMesh.h>
#include "../UserComponent/Human_Player.h"
#include "Rendering/RenderManager.h"
#include "Component/FreeCamera.h"
#include "../GameManager.h"

CStage2Scene::CStage2Scene()
{
}

CStage2Scene::~CStage2Scene()
{
}

bool CStage2Scene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"SC.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);

	CGameObject* pObj = nullptr;
	CTransform* pTr = nullptr;
	CLight* pLight = nullptr;

	CLayer* pLayer = m_pScene->FindLayer("Light");

	pObj = CGameObject::CreateObject("PointLight", pLayer);
	pLight = pObj->AddComponent<CLight>("Light");
	pTr = pObj->GetTransform();

	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(30.0f);
	pLight->SetAngle(15.0f, 30.0f);
	pLight->EnableLightVolume();

	pTr->SetWorldPos(Vector3(0.0f, 20.0f, 0.0f));
	pTr->RotationX(90.0f);

	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLight);

	pObj = CGameObject::CreateObject("PointLight", pLayer);
	pLight = pObj->AddComponent<CLight>("Light");
	pTr = pObj->GetTransform();

	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(30.0f);
	pLight->SetAngle(15.0f, 30.0f);
	pLight->EnableLightVolume();

	pTr->SetWorldPos(0.0f, 20.0f, 30.0f);
	pTr->RotationX(90.0f);

	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLight);

	SAFE_RELEASE(pLayer);

	return true;
}

void CStage2Scene::AfterInit()
{
	//GET_SINGLE(CGameManager)->PlayerSpon(Vector3(120.0f, 0.0f, 177.5f), Vector3::Zero);
	//CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S2_S1_1");

	//pDoor->SetDoorType(DOOR_STAGE);
	//pDoor->SetTargetDoor("Stage1", "Door_S1_S2_1");
	//pDoor->SetLeftRight(true);
}

int CStage2Scene::Update(float fTime)
{

	return 0;
}
