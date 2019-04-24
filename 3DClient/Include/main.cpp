#include "ClientHeader.h"
#include "Core.h"
#include "SceneComponent/MainScene.h"
#include "SceneComponent/TutorialScene.h"
#include "SceneComponent/FirTestScene.h"
#include "SceneComponent/JBH_Stage3.h"
#include "SceneComponent/TestScene.h"
#include "SceneComponent/TestScene-YH.h"
#include "SceneComponent/LogoScene.h"
#include "SceneComponent/RandScapeTestScene.h"
#include "CameraEff.h"
#include "CommonSoundLoader.h"

PUN_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	srand((unsigned int)time(0));
	
	if (!CCore::GetInst()->Init(hInstance, 1280, 720,
		TEXT("OutLast - 1"), TEXT("OutLast - 1"), IDI_ICON1, IDI_ICON1,
		true))
	{
		CCore::DestroyInst();
		return 0;
	}

	//FPS 카메라와 카메라 흔들림 이펙트를 구현하는 매니저 구동
	CCameraEff::GetInst()->Init();

	//wstring RootPath = CPathManager::GetInst()->FindPath(ROOT_PATH);
	//SHELLEXECUTEINFO sei;
	//ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	//sei.cbSize = sizeof(SHELLEXECUTEINFO);

	//wstring FullPath;
	//FullPath = RootPath + L"FBXBineryExtractor64.exe";

	//sei.lpFile = FullPath.c_str();
	//sei.lpParameters = L"";
	//sei.nShow = SW_SHOW;
	//sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	//sei.lpVerb = L"open";

	////여기서 해당 프로세스가 종료될때까지 대기시킴.
	//DWORD result = ShellExecuteEx(&sei);
	//WaitForSingleObject(sei.hProcess, INFINITE);

	// MainScene 추가
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CStartScene>("StartScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CMainScene>("MainScene");

	CCommonSoundLoader::GetInst()->LoadSoundCSVList(TEXT("CommonSound.csv"));
	CCommonSoundLoader::GetInst()->LoadSoundRandomSeedCnt(TEXT("CommonSound_SoundCnt.csv"));

#ifdef _DEBUG
	GET_SINGLE(CSceneManager)->AddScene<CLogoScene>("First", "LogoScene");
	GET_SINGLE(CSceneManager)->AddScene<CTestSceneYH>("Second", "YH_TEST");
	//GET_SINGLE(CSceneManager)->AddScene<CFirTestScene>("Third", "FirTestScene");

#else
	GET_SINGLE(CSceneManager)->AddSceneComponent<CTutorialScene>("TutorialScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CLogoScene>("LogoScene");
#endif

	//GET_SINGLE(CSceneManager)->AddSceneComponent<CTestScene>("TestScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CFirTestScene>("Fir");

    int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();
	CCameraEff::DestroyInst();
	CCommonSoundLoader::DestroyInst();

	return iRet;
}