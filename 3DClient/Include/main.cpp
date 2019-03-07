#include "ClientHeader.h"
#include "Core.h"
#include "SceneComponent/MainScene.h"
#include "SceneComponent/StartScene.h"
#include "SceneComponent/FirTestScene.h"

PUN_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	srand((unsigned int)time(0));

	if (!CCore::GetInst()->Init(hInstance, 1280, 720,
		TEXT("TalesWeaver"), TEXT("TalesWeaver"), IDI_ICON1, IDI_ICON1,
		true))
	{
		CCore::DestroyInst();
		return 0;
	}

	wstring RootPath = CPathManager::GetInst()->FindPath(ROOT_PATH);
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

 	wstring FullPath;
	FullPath = RootPath + L"FBXBineryExtractor64.exe";

	sei.lpFile = FullPath.c_str();
	sei.lpParameters = L"";
	sei.nShow = SW_SHOW;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = L"open";

	//여기서 해당 프로세스가 종료될때까지 대기시킴.
	DWORD result = ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE);

	// MainScene 추가
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CStartScene>("StartScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CMainScene>("MainScene");
	GET_SINGLE(CSceneManager)->AddSceneComponent<CFirTestScene>("FirTestScene");

	int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();

	return iRet;
}