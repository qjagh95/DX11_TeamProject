#include "../ClientHeader.h"
#include "TestScene.h"
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
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/Battery.h"
#include "../UserComponent/BatteryIcon.h"

CTestScene::CTestScene()
{
	m_fPlayTime = 0.f;
	m_bGhostOn = false;
	m_bIncrease = false;
	m_fX = 0.f;
	m_fY = 0.f;
}

CTestScene::~CTestScene()
{
	SAFE_RELEASE(m_pUILayer);

	if (m_bGhostOn)
		SAFE_RELEASE(m_pGhostTr);
}

bool CTestScene::Init()
{
	wstring wstr = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstr += L"SC.dat";
	string filePath = CW2A(wstr.c_str());
	m_pScene->Load(filePath);
	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->PlayBgm("bgm1");
	//
	//_SMgr->SetAudioCoordSize(16.f);

	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->SoundPlay("bgm1", ST_BGM);


	//CCamera* pCamera = m_pScene->GetMainCamera();
	//pCamera->SetCameraType(CT_PERSPECTIVE);
	//pCamera->SetNear(0.03f);



	//CTransform* pCameraTr = pCamera->GetTransform();
	//
	//CFreeCamera* pFreeCam = pCamera->AddComponent<CFreeCamera>("FreeCamera");

	//SAFE_RELEASE(pFreeCam);
	//SAFE_RELEASE(pCameraTr);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");
	m_pUILayer = m_pScene->FindLayer("UI");

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
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(100.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(0.0f, 70.0f, 0.0f);


	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("GlobalLight1", pLightLayer);
	pTransform = pObject->GetTransform();
	pLight = pObject->AddComponent<CLight>("GlobalLight1");
	pLight->SetLightType(LT_DIR);


	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("GlobalLight1", pLightLayer);
	pTransform = pObject->GetTransform();
	pLight = pObject->AddComponent<CLight>("GlobalLight1");
	pLight->SetLightType(LT_DIR);

	pTransform->RotationY(90.0f);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("GlobalLight1", pLightLayer);
	pTransform = pObject->GetTransform();
	pLight = pObject->AddComponent<CLight>("GlobalLight1");
	pLight->SetLightType(LT_DIR);

	pTransform->RotationY(180.0f);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("GlobalLight1", pLightLayer);
	pTransform = pObject->GetTransform();
	pLight = pObject->AddComponent<CLight>("GlobalLight1");
	pLight->SetLightType(LT_DIR);

	pTransform->RotationY(-90.0f);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("GlobalLight1", pLightLayer);
	pTransform = pObject->GetTransform();
	pLight = pObject->AddComponent<CLight>("GlobalLight1");
	pLight->SetLightType(LT_DIR);

	pTransform->RotationX(-90.0f);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	SAFE_RELEASE(pLightLayer);

	pObject = CGameObject::CreateObject("Door", pDefaultLayer);
	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(20.f, 0.f, 25.f);
	pTransform->SetWorldScale(0.1f, .1f, 0.1f);

	CDoor* pDoor = pObject->AddComponent<CDoor>("door");

	pTransform->SetWorldPivot(0.5f, .0f, .0f);
	pDoor->SetOpenTime(1.5f);
	pDoor->SetCloseTime(1.5f);

	std::string names[3];
	names[0] = "wood_Door_Open1";
	names[1] = "wood_Door_Close1";
	names[2] = "wood_Door_Bash1";

	TCHAR *strPaths[3];
	strPaths[0] = (TCHAR*)TEXT("WoodenDoor_OPENING_01.wav");
	strPaths[1] = (TCHAR*)TEXT("WoodenDoor_CLOSING_03.wav");
	strPaths[2] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");

	pDoor->SetSounds(names, (const TCHAR**)strPaths);

	PUN::CRenderer* renderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	renderer->SetMesh("wdoor_1", TEXT("wood_door.FBX"), MESH_PATH);


	SAFE_RELEASE(pDoor);

	SAFE_RELEASE(renderer);
	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	pObject = PUN::CGameObject::CreateObject("hPlayer", pDefaultLayer, true);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.035f, 0.035f, 0.035f);
	pTransform->SetWorldPos(0.0f, 0.0f, 0.0f);
	CHuman_Player *pHPlayer = pObject->AddComponent<CHuman_Player>("Player");

	SAFE_RELEASE(pHPlayer);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CTestScene::Update(float fTime)
{
	static bool bPush = false;
	if (GetAsyncKeyState('P') & 0x8000)
	{
		bPush = true;
	}
	else if (bPush)
	{
		bPush = false;
		m_bGhostOn = true;
		m_bIncrease = true;

		CGameObject*	pGhostObj = CGameObject::CreateObject("Ghost", m_pUILayer);

		CGhost*	pGhost = pGhostObj->AddComponent<CGhost>("Ghost");

		m_pGhostTr = pGhostObj->GetTransform();

		//SAFE_RELEASE(pGhostTr);
		SAFE_RELEASE(pGhost);
		SAFE_RELEASE(pGhostObj);
	}

	if (m_bIncrease)
	{
		m_fPlayTime += fTime;

		if (m_fX <= 4096.f && m_fY <= 4096.f)
		{
			m_fX += 9000 * fTime;
			m_fY += 9000 * fTime;
			m_pGhostTr->SetWorldScale(m_fX, m_fY, 1.f);
		}
	}

	if (m_fPlayTime >= 2.f)
	{
		m_bIncrease = false;
		m_fPlayTime = 0.f;
		m_fX = 1.f;
		m_fY = 1.f;
	}

	return 0;
}

void CTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CTestScene::CreatePrototype()
{
}

void CTestScene::NextScene(float fTime)
{
}
