#include "../ClientHeader.h"
#include "Scene4_DG.h"
#include "../GameManager.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST3_Suprise.h"
#include "../UserComponent/ST_Default.h"
#include <StrUtility.h>
#include <Component/Light.h>
#include <Component/LandScape.h>
#include <Component/FreeCamera.h>
#include <Component/ColliderOBB3D.h>

// 테스트
#define IS_TEST			false
#define IS_CONVERTFBX	false
#define IS_LIGHT_TEST	false

CScene4::CScene4() :
	m_fFade(0.f),
	m_bMouseOn(false),
	m_bMotion(false),
	m_isChangeBGM(false),
	m_isShot(false),
	m_pObjPlayer(nullptr),
	m_pPlayerTr(nullptr),
	m_pHumanPlayer(nullptr),
	m_pHeavyDoor(nullptr),
	m_pHandGun(nullptr),
	m_newMonsterObject(nullptr),
	m_newMonster(nullptr)
{
}

CScene4::~CScene4()
{
#if IS_TEST
	// 메모리 해제
	SAFE_RELEASE(m_pObjPlayer);
	SAFE_RELEASE(m_pHumanPlayer);

#endif
	SAFE_RELEASE(m_pObjPlayer);
	SAFE_RELEASE(m_pPlayerTr);
	SAFE_RELEASE(m_pHeavyDoor);
	SAFE_RELEASE(m_newMonsterObject);
	SAFE_RELEASE(m_newMonster);
	SAFE_RELEASE(m_pHandGun);
	SAFE_RELEASE(m_pHandGunTr);
}

void CScene4::Start()
{
}

bool CScene4::Init()
{
#if IS_CONVERTFBX
	// FBX 파일 로드
	this->ConvertCorridorFBXFiles();
#endif
	// 스테이지4 맵 데이터 파일 로드
	wstring wstrPath = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstrPath += L"Stage4_DG.dat";
	string strDataPath = CW2A(wstrPath.c_str());
	m_pScene->Load(strDataPath);

	// Navigation
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CGameObject* NavLandObject = CGameObject::CreateObject("Land", pLayer);
	CLandScape* Land = NavLandObject->AddComponent<CLandScape>("Land");
	string strPath = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);
	strPath += "Stage4_Nav_DG.nav";
	Land->LoadLandScape(strPath);
	SAFE_RELEASE(pLayer);

	// 사운드
	CSoundManager::GetInst()->CreateSoundEffect("Stage4BGM", L"music\\11-Surgeon Stealth 2 (MIX1-Strings).wav");
	CSoundManager::GetInst()->CreateSoundEffect("TraceBGM", L"music\\10-AI NPC CHASE LOOP 2 (Strings).wav");
	CSoundManager::GetInst()->SetTransitionTime(2.0f);

	// 전역광(Directional Light) 설정
	Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	Vector4 vTestColor  = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 vDarkColor  = Vector4(0.04f, 0.01f, 0.01f, 1.0f);
	pLayer = m_pScene->FindLayer("Light");
	list<CGameObject*>* pLightList = pLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pLightList->end();
	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CLight* pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
		#if IS_LIGHT_TEST
			pLight->SetLightColor(vTestColor, vTestColor, vTestColor);
		#else
			pLight->SetLightColor(vDarkColor, vDarkColor, vDarkColor);
		#endif
		SAFE_RELEASE(pLight);
	}
	SAFE_RELEASE(pLayer);

	// 키
	GET_SINGLE(CInput)->AddKey("Shot", VK_RBUTTON);

	return true;
}

void CScene4::AfterInit()
{
#if IS_TEST
	CLayer* pLayer = m_pScene->FindLayer("Default");
	m_pObjPlayer = CGameObject::CreateObject("Player", pLayer);
	CTransform* pPlayerTr = m_pObjPlayer->GetTransform();
	pPlayerTr->SetWorldPos(Vector3::Zero);
	m_pHumanPlayer = m_pObjPlayer->AddComponent<CHuman_Player>("UC_HumanPlayer");
	CRenderer* pRenderer = m_pObjPlayer->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDecalEnable(true);
	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);

#else
	// 플레이어
	m_pHumanPlayer = _PLAYER;
	m_pObjPlayer = _PLAYER->GetGameObject();
	m_pPlayerTr = m_pObjPlayer->GetTransform();
	m_pHumanPlayer->PlayerRot(Vector3(0.f, 90.f, 0.f));
	m_pPlayerTr->SetWorldPos(40.f, 0.f, 25.f);
	CRenderer* pRenderer = m_pObjPlayer->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDecalEnable(false);
	SAFE_RELEASE(pRenderer);

	// 몬스터
	CLayer* pLayer = m_pScene->FindLayer("Default");
	m_newMonsterObject = CGameObject::CreateObject("Suprise", pLayer);
	m_newMonster = m_newMonsterObject->AddComponent<ST_Default>("Suprise");
	m_newMonster->GetTransformNonCount()->SetWorldPos(Vector3(60.f, 0.f, 100.f));
	m_newMonster->AddPatrolPos(Vector3(60.f, 0.f, 100.f));
	m_newMonster->AddPatrolPos(Vector3(109.f, 0.f, 115.f));
	m_newMonster->AddPatrolPos(Vector3(54.f, 0.f, 82.f));
	m_newMonster->IamNotFirstStage();
	m_newMonster->SetPlayerFindDistance(35.f);
	m_newMonster->SetTarget(m_pObjPlayer);

	// 총
	m_pHandGun = PUN::CGameObject::CreateObject("Gun", pLayer);
	m_pHandGun->SetFrustrumCullUse(false);
	m_pHandGunTr = m_pHandGun->GetTransform();
	m_pHandGunTr->SetWorldPos(85.f, 4.5f, 99.f);
	m_pHandGunTr->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	m_pHandGunTr->SetWorldRotY(30.f);
	CRenderer *renderer = m_pHandGun->AddComponent<CRenderer>("renderer");
	renderer->SetMesh("gun", TEXT("glock_OnlyFire.msh"));
	SAFE_RELEASE(renderer);
	CAnimation* pAnim = m_pHandGun->AddComponent<CAnimation>("animation");
	pAnim->LoadBone(TEXT("glock_OnlyFire.bne"));
	pAnim->Load(TEXT("glock_idle.anm"));
	pAnim->LoadFileAsClip("fire", TEXT("glock_fire.anm"));
	pAnim->SetDefaultClip("Take 001");
	pAnim->SetClipOption("fire", PUN::AO_ONCE_RETURN);
	pAnim->SetBlendSkip(true);
	SAFE_RELEASE(pAnim);
	CColliderSphere* pBody = m_pHandGun->AddComponent<CColliderSphere>("GunBody");
	pBody->SetCollisionCallback(CCT_ENTER, this, &CScene4::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CScene4::MouseOut);
	pBody->SetColliderID((COLLIDER_ID)UCI_ITEM_BATTERY);
	pBody->SetInfo(Vector3::Zero, 1.f);
	SAFE_RELEASE(pBody);
	SAFE_RELEASE(pLayer);

	// 스테이지 이동 문
	DoorInit();
#endif
}

int CScene4::Input(float _fTime)
{
	return 0;
}

int CScene4::Update(float _fTime)
{
	/*if (GET_SINGLE(CSceneManager)->GetChange())
	{
		static bool bAdd = true;
		if (bAdd)
		{
			bAdd = false;
			GET_SINGLE(CGameManager)->AddUILayer();
			GET_SINGLE(CGameManager)->SetPlayerNaviY(true);
		}
	}*/

	// 행동에 따른 사운드 재생
	if (m_newMonster->GetState() == DS_USER_TRACE
		|| m_newMonster->GetState() == DS_HOOK
		|| m_newMonster->GetState() == DS_JAP
		|| m_newMonster->GetState() == DS_HEAD_ATTACK)
	{
		if (m_isChangeBGM == false)
		{
			CSoundManager::GetInst()->PlayBgm("TraceBGM");
			m_isChangeBGM = true;
		}
	}
	else
	{
		if (m_isChangeBGM = true)
		{
			CSoundManager::GetInst()->PlayBgm("Stage4BGM");
			m_isChangeBGM = false;
		}
	}

	if (m_isShot == false)
	{
		if (CInput::GetInst()->KeyPress("Shot") == true)
		{
			m_isShot = true;
		}
	}
	else
	{
		m_fFade += 2.f * _fTime;
		GET_SINGLE(CRenderManager)->SetFadeAmount(1.0f - m_fFade, _fTime);
	}
	if (m_fFade > 1.f)
	{
		m_fFade = 1.f;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_bMouseOn)
	{
		if (KEYUP("F"))
		{
			CGameObject*	pPlayerObj = CGameObject::FindObject("Player");
			CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
			pPlayer->ChangeRayAnim("AimOff");
			m_pHandGun->SetEnable(false);
			GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
			pPlayer->TakeItem();
			SAFE_RELEASE(pPlayer);
			SAFE_RELEASE(pPlayerObj);
		}
	}
	if (m_bMotion)
	{
		CGameObject*	pPlayerObj = CGameObject::FindObject("Player");
		CHuman_Player*	pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
		pPlayer->ChangeRayAnim("AimOff");
		GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_Empty");
		m_bMotion = false;
		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pPlayerObj);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	GET_SINGLE(CGameManager)->Update(_fTime);

	return 0;
}

int CScene4::LateUpdate(float _fTime)
{
	CGameManager::GetInst()->Update(_fTime);
	return 0;
}

void CScene4::Collision(float _fTime)
{
}

void CScene4::Render(float _fTime)
{
}

void CScene4::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	CGameObject* pPlayerObj = CGameObject::FindObject("Player");
	CHuman_Player* pPlayer = pPlayerObj->FindComponentFromType<CHuman_Player>((COMPONENT_TYPE)UT_PLAYER);
	CTransform*	pPlayerTr = pPlayerObj->GetTransform();
	Vector3 vPlayerPos = pPlayerTr->GetWorldPos();
	float fDist = m_pHandGun->GetTransformNonCount()->GetWorldPos().Distance(vPlayerPos);
	if (fDist < 10.f)
	{
		if (pDest->GetColliderID() == UCI_PLAYER_RAY)
		{
			m_bMouseOn = true;
			pPlayer->ChangeRayAnim("AimOn");
			GET_SINGLE(CGameManager)->ChangeNoticeClip("Button_F_Pickup");
		}
	}
	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pPlayerObj);
}

void CScene4::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetColliderID() == UCI_PLAYER_RAY)
	{
		m_bMouseOn = false;
		m_bMotion = true;
	}
}

void CScene4::DoorInit()
{
	// 스테이지 이동문
	CDoor* pDoor = CGameManager::GetInst()->FindDoor(m_pScene, "Door_S4_S1");
	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_1");

	// 총이 있는 방문
	m_pHeavyDoor = CGameManager::GetInst()->FindDoor(m_pScene, "Gun_HeavyDoor");
	m_pHeavyDoor->SetOpenRot(80.f);
	m_pHeavyDoor->SetOpenTime(2.f);
	m_pHeavyDoor->SetDoorType(DOOR_HEAVY);
}

void CScene4::ConvertCorridorFBXFiles()
{
	CStrUtility* pUtil = CStrUtility::GetInst();

	// .FBX 예외 파일 목록
	vector<wstring> vecException;
	vecException.push_back(L"Waylon2.FBX");
	vecException.push_back(L"shooter_anim.FBX");
	vecException.push_back(L"Enem_Cannibal.FBX");
	vecException.push_back(L"SE_handycam_HandyCam.FBX");
	vecException.push_back(L"Handycam.FBX");
	vecException.push_back(L"glock_OnlyFire.FBX");

	// MESH_PATH 폴더안에 .FBX 파일명을 모두 읽어온다.
	const TCHAR* path = CPathManager::GetInst()->FindPath(MESH_PATH);
	const TCHAR* meshDataPath = CPathManager::GetInst()->FindPath(MESH_DATA_PATH);
	string strPath = pUtil->TCHARToString(path);
	wstring wstrPath = L"";
	wstrPath.assign(strPath.begin(), strPath.end());
	vector<wstring> vecFBXList = pUtil->GetFilesInFolder(wstrPath, L"*.FBX*");

	// 읽어온 .FBX 파일명과 예외 .FBX 파일명이 일치하면 삭제한다.
	for (size_t i = 0; i < vecFBXList.size(); ++i)
	{
		for (size_t j = 0; j < vecException.size(); ++j)
		{
			if (vecFBXList[i] == vecException[j])
			{
				vecFBXList.erase(vecFBXList.begin() + i);
				break;
			}
		}
	}

	// .FBX 파일을 .MSH 파일로 변환한다.
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CGameObject* pObject = CGameObject::CreateObject("ConvertFBX", pLayer);
	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
	for (size_t i = 0; i < vecFBXList.size(); ++i)
	{
		std::wstring wstrFileName = vecFBXList[i];
		std::string strFileName = "";
		strFileName.assign(wstrFileName.begin(), wstrFileName.end());
		const TCHAR* fileName = pUtil->StringToTCHAR(strFileName);
		pRenderer->SetMesh(strFileName, fileName, MESH_PATH);

		// 변환된 .MSH 파일을 경로(MESH_DATA_PATH)에 이동시키는데, 이때 이미 있는 파일이면 삭제한다.
		char* context = NULLPTR;
		string strTok = strtok_s(const_cast<char*>(strFileName.c_str()), ".", &context);
		strTok += ".msh";
		wstring wstrExistingFileName = CA2W((pUtil->TCHARToString(path) + strTok).c_str());
		wstring wstrNewFileName = CA2W((pUtil->TCHARToString(meshDataPath) + strTok).c_str());
		BOOL isDeleteResult = DeleteFile(wstrNewFileName.c_str());

		// .MSH 파일을 임시폴더로 복사한다. (웹 하드용)
		/*wstring wstrCopyFolder = CA2W((pUtil->TCHARToString(L"D:\\TempMshFile\\") + strTok).c_str());
		CopyFile(wstrExistingFileName.c_str(), wstrCopyFolder.c_str(), false);*/

		// .MSH 파일 이동
		BOOL isMoveResult = MoveFile(wstrExistingFileName.c_str(), wstrNewFileName.c_str());
	}
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}