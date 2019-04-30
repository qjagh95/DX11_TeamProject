#include "../ClientHeader.h"
#include "JBH_Stage3.h"

#include <Component/Camera.h>
#include <Component/FreeCamera.h>
#include <Component/LandScape.h>

#include "../UserComponent/Human_Player.h"
#include "../CameraEff.h"

JBH_Stage3::JBH_Stage3()
{
}

JBH_Stage3::~JBH_Stage3()
{
}

bool JBH_Stage3::Init()
{
	CCamera* pCamera = m_pScene->GetMainCameraNonCount();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);
	CCameraEff::GetInst()->SetFirstPersonViewEnable();

	//pCamera->GetTransformNonCount()->SetLocalPos(Vector3(0.f, 5.0f, 5.0f));
	//pCamera->SetCameraInfo(CT_PERSPECTIVE, 1280.0f, 720.0f, 90.0f, 0.03f, 1000.0f);
	//CFreeCamera* FreeCam = pCamera->AddComponent<CFreeCamera>("FreeCam");

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "JBH_Stage3.dat";
	m_pScene->Load(Path);

	CGameObject* NavLandObject = CGameObject::CreateObject("Land", pDefaultLayer);
	CLandScape* Land = NavLandObject->AddComponent< CLandScape>("Land");
	
	Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	Path += "Stage3Nav.nav";
	Land->LoadLandScape(Path);

	CGameObject* PlayerObject = CGameObject::CreateObject("Player", pDefaultLayer, true);
	CHuman_Player* newPlayer = PlayerObject->AddComponent<CHuman_Player>("Player");
	newPlayer->GetTransformNonCount()->SetWorldPos(Vector3(231.0f, 0.0f, 68.0f));
	newPlayer->GetTransformNonCount()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	newPlayer->GetTransformNonCount()->SetLocalRot(Vector3(0.0f, 180.0f, 0.0f));

	//SAFE_RELEASE(FreeCam);
	SAFE_RELEASE(newPlayer);
	SAFE_RELEASE(PlayerObject);
	SAFE_RELEASE(Land);
	SAFE_RELEASE(NavLandObject);
	SAFE_RELEASE(pBackLayer);
	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);

	CInput::GetInst()->AddKey("SummonChair", 'G');
	CInput::GetInst()->AddKey("ChangeScene", VK_SPACE);

	return true;
}

int JBH_Stage3::Update(float DeltaTime)
{
	if (CInput::GetInst()->KeyPress("SummonChair"))
	{
		CGameObject* newChair = CGameObject::CreateObject("Chair", m_pScene->FindLayer("Default"));
		CRenderer* newRenderer = newChair->AddComponent<CRenderer>("Chair");
		newRenderer->SetMesh("Chair", TEXT("Chair.msh"));

		SAFE_RELEASE(newChair);
		SAFE_RELEASE(newRenderer);
	}
	else if (CInput::GetInst()->KeyPress("ChangeScene"))
	{
		CSceneManager::GetInst()->ChangeScene("Third");
	}

	return 0;
}

int JBH_Stage3::LateUpdate(float DeltaTime)
{
	return 0;
}

void JBH_Stage3::Collision(float DeltaTime)
{
}

void JBH_Stage3::Render(float DeltaTime)
{
}
