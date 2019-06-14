#include "../ClientHeader.h"
#include "Scene4_DG.h"
#include "../GameManager.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/ST3_Suprise.h"
#include <StrUtility.h>
#include <Component/Light.h>
#include <Component/FreeCamera.h>
#include <Component/ColliderOBB3D.h>

// 테스트
#define IS_TEST			false
#define IS_CONVERTFBX	false
#define IS_LIGHT_TEST	false

CScene4::CScene4() :
	m_pObjPlayer(nullptr),
	m_pPlayerTr(nullptr),
	m_pHumanPlayer(nullptr),
	m_pHeavyDoor(nullptr),



	m_pObjBSDecal(nullptr),
	m_pObjBBDecal(nullptr),
	m_pBSDecal(nullptr),
	m_pBBDecal(nullptr)
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


	SAFE_RELEASE(m_pObjBSDecal);
	SAFE_RELEASE(m_pObjBBDecal);
	SAFE_RELEASE(m_pBSDecal);
	SAFE_RELEASE(m_pBBDecal);
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
	
	// 전역광(Directional Light) 설정
	Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	Vector4 vTestColor  = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 vDarkColor  = Vector4(0.005f, 0.005f, 0.005f, 1.0f);
	CLayer* pLayer = m_pScene->FindLayer("Light");
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

	





	// 데칼(Screen Space Decal)
	//CreateMilestoneDecal();


	//CreateDecal();


	// 초기화
	{
		//SetDoorInit();
	}


	/*
	CreateBloodDecal();*/

	// y축 기준으로 데칼이 보인다.


	//CLayer* pLayer = m_pScene->FindLayer("Default");

	// BloodDecal_Small
	//CGameObject* pObjBSDecal = CGameObject::CreateObject("Decal", pLayer);
	//CTransform* pBSTr = pObjBSDecal->GetTransform();
	//pBSTr->SetWorldRotX(-90.f);
	//pBSTr->SetWorldScale(5.f, 5.f, 5.f);
	//pBSTr->SetWorldPos(10.f, 5.f, 8.f);
	//CDecal* pBSDecal = pObjBSDecal->AddComponent<CDecal>("Decal");
	//CRenderer* pRenderer = pObjBSDecal->FindComponentFromTag<CRenderer>("DecalRenderer");
	//pRenderer->SetRenderState(ALPHA_BLEND);
	//// SetDiffuseTex, NormalTex, SpecularTex() 호출 시 Texture를 만들어준다.
	//CMaterial* pBSMtrl = pObjBSDecal->FindComponentFromType<CMaterial>(CT_MATERIAL);
	//pBSMtrl->SetDiffuseTex(0, "BloodDecalD_Small", TEXT("Decal/ControlCenter.png"));
	///*pBSMtrl->SetNormalTex(1, "BloodDecalN_Small", TEXT("Decal/ControlCenter.png"));
	//pBSMtrl->SetSpecularTex(2, "BloodDecalS_Small", TEXT("Decal/ControlCenter.png"));*/
	//pBSMtrl->SetSampler(0, SAMPLER_LINEAR);
	////pBSMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	////pBSMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);


	/*CColliderOBB3D*	pDecalSphere = pObjBSDecal->AddComponent<CColliderOBB3D>("DecalSphere");
	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));
	SAFE_RELEASE(pDecalSphere);*/






	//SAFE_RELEASE(pLayer);

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
	m_pPlayerTr->SetWorldPos(Vector3::Zero);
	CRenderer* pRenderer = m_pObjPlayer->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDecalEnable(false);
	SAFE_RELEASE(pRenderer);

	// 스테이지 이동 문
	DoorInit();

	// 시체
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CGameObject* pObjCorpse1 = CGameObject::CreateObject("Suprise", pLayer);
	CTransform* pCropse1Tr = pObjCorpse1->GetTransform();
	ST3_Suprise* Suprise = pObjCorpse1->AddComponent<ST3_Suprise>("Suprise");

	CGameObject*	pPObj = CGameObject::FindObject("Player");

	Suprise->SetTarget(pPObj);

	SAFE_RELEASE(pPObj);

	SAFE_RELEASE(pCropse1Tr);
	SAFE_RELEASE(Suprise);
	SAFE_RELEASE(pObjCorpse1);
	SAFE_RELEASE(pLayer);

	CDoor* pDoor = GET_SINGLE(CGameManager)->FindDoor(m_pScene, "Door_S4_S1");

	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_S1_S4");
	pDoor->SetLeftRight(true);

#endif
}

int CScene4::Input(float _fTime)
{
	return 0;
}

int CScene4::Update(float _fTime)
{
	if (GET_SINGLE(CSceneManager)->GetChange())
	{
		static bool bAdd = true;

		if (bAdd)
		{
			bAdd = false;
			GET_SINGLE(CGameManager)->AddUILayer();
			GET_SINGLE(CGameManager)->SetPlayerNaviY(true);
		}
	}

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

void CScene4::DoorInit()
{
	// 스테이지 이동문
	CDoor* pDoor = CGameManager::GetInst()->FindDoor(m_pScene, "Door_S4_S1");
	pDoor->SetDoorType(DOOR_STAGE);
	pDoor->SetTargetDoor("Stage1", "Door_1");

	// 총이 있는 방문
	m_pHeavyDoor = CGameManager::GetInst()->FindDoor(m_pScene, "Gun_HeavyDoor");
	m_pHeavyDoor->SetOpenRot(75.f);
	m_pHeavyDoor->SetOpenTime(10.f);
	m_pHeavyDoor->SetDoorType(DOOR_HEAVY);
}

void CScene4::CreateMilestoneDecal()
{
	/*CLayer* pLayer = m_pScene->FindLayer("Default");
	CGameObject* pObjDecal = CGameObject::CreateObject("Decal", pLayer);
	CTransform* pTr = pObjDecal->GetTransform();
	pTr->SetWorldScale(5.f, 5.f, 5.f);
	pTr->SetWorldRotX(-90.f);
	pTr->SetWorldPos(10.f, 5.f, 8.f);
	CDecal* pDecal = pObjDecal->AddComponent<CDecal>("Decal");
	CRenderer* pRenderer = pObjDecal->FindComponentFromTag<CRenderer>("DecalRenderer");
	pRenderer->SetRenderState(ALPHA_BLEND);
	CMaterial* pMtrl = pObjDecal->FindComponentFromType<CMaterial>(CT_MATERIAL);
	pMtrl->SetDiffuseTex(0, "StageMileStone_D", TEXT("MainHall.png"));
	pMtrl->SetSampler(0, SAMPLER_LINEAR);
	CColliderOBB3D*	pDecalSphere = pObjDecal->AddComponent<CColliderOBB3D>("DecalSphere");
	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));
	SAFE_RELEASE(pDecalSphere);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pMtrl);
	SAFE_RELEASE(pDecal);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObjDecal);
	SAFE_RELEASE(pLayer);*/
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

void CScene4::InitDecal()
{
	// 핏 자국


}














//void CScene4::CreateDecal(const string& _strTag,   const string& _decalTag,
//						  const string& _difName,  const string& _difTexName, 
//						  const string& _norName,  const string& _norTexName,
//						  const string& _specName, const string& _specTexName,
//						  Vector3 _scale, Vector3 _rotate, Vector3 _pos)
//{
	//CLayer* pLayer = m_pScene->FindLayer("Default");

	//// BloodDecal_Small
	//m_pObjBSDecal = CGameObject::CreateObject("Decal", pLayer);
	//CTransform* pBSTr = m_pObjBSDecal->GetTransform();
	//pBSTr->SetWorldRotZ(-90.f);
	//pBSTr->SetWorldScale(5.f, 5.f, 5.f);
	//pBSTr->SetWorldPos(8.f, 5.f, 1.f);
	//m_pBSDecal = m_pObjBSDecal->AddComponent<CDecal>("Decal");
	//CRenderer* pRenderer = m_pObjBSDecal->FindComponentFromTag<CRenderer>("DecalRenderer");
	//pRenderer->SetRenderState(ALPHA_BLEND);
	//// SetDiffuseTex, NormalTex, SpecularTex() 호출 시 Texture를 만들어준다.
	//CMaterial* pBSMtrl = m_pObjBSDecal->FindComponentFromType<CMaterial>(CT_MATERIAL);
	//pBSMtrl->SetDiffuseTex(0,  "BloodDecalD_Small", TEXT("Decal/Shout24674-perfil3_COLOR.png"));
	//pBSMtrl->SetNormalTex(1,   "BloodDecalN_Small", TEXT("Decal/Shout24674-perfil3_NRM.png"));
	//pBSMtrl->SetSpecularTex(2, "BloodDecalS_Small", TEXT("Decal/Shout24674-perfil3_SPEC.png"));
	//pBSMtrl->SetSampler(0, SAMPLER_LINEAR);
	//pBSMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	//pBSMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);
//
//	CColliderOBB3D*	pDecalSphere = m_pObjBSDecal->AddComponent<CColliderOBB3D>("DecalSphere");
//	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));
//	SAFE_RELEASE(pDecalSphere);
//
//	SAFE_RELEASE(pBSMtrl);
//	SAFE_RELEASE(pRenderer);
//	SAFE_RELEASE(pBSTr);
//	SAFE_RELEASE(pLayer);
//
//	// BloodDecal_Big
//	//CMaterial* pBBMtrl = m_pObjBBDecal->FindComponentFromType<
//}

void CScene4::SetAmbientSoundPlay(const string& _soundName)
{
	// 환경(주변) 사운드 재생
}