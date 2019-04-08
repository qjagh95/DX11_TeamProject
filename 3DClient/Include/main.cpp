#include "ClientHeader.h"
#include "Core.h"
#include "SceneComponent/MainScene.h"
#include "SceneComponent/StartScene.h"
#include "SceneComponent/FirTestScene.h"
#include "SceneComponent/TestScene.h"
#include "SceneComponent/LogoScene.h"

PUN_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	srand((unsigned int)time(0));

	if (!CCore::GetInst()->Init(hInstance, 1280, 720,
		TEXT("Welcome To Horror Game"), TEXT("Welcome To Horror Game"), IDI_ICON1, IDI_ICON1,
		true))
	{
		CCore::DestroyInst();
		return 0;
	}

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

	////���⼭ �ش� ���μ����� ����ɶ����� ����Ŵ.
	//DWORD result = ShellExecuteEx(&sei);
	//WaitForSingleObject(sei.hProcess, INFINITE);

	// MainScene �߰�
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CStartScene>("StartScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CMainScene>("MainScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CLogoScene>("LogoScene");
	GET_SINGLE(CSceneManager)->AddSceneComponent<CTestScene>("TestScene");

    int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();

	return iRet;
}