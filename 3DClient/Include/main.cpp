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

	// MainScene Ãß°¡
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CStartScene>("StartScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CMainScene>("MainScene");
	GET_SINGLE(CSceneManager)->AddSceneComponent<CFirTestScene>("FirTestScene");

	int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();

	return iRet;
}