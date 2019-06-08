#include "ClientHeader.h"
#include "Core.h"
#include "SceneComponent/TutorialScene.h"
#include "SceneComponent/Stage1Scene.h"
#include "SceneComponent/JBH_Stage3.h"
#include "SceneComponent/TestScene-YH.h"
#include "SceneComponent/Stage2Scene.h"
#include "SceneComponent/LogoScene.h"
#include "CameraEff.h"
#include "CommonSoundLoader.h"
#include <Rendering/ShaderManager.h>
#include <Resource/ResourcesManager.h>
#include "GameManager.h"

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
		DESTROY_SINGLE(CGameManager);
		return 0;
	}	


	//자동전체화면
	//keybd_event(VK_LMENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
	//keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY, 0);
	//keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
	//keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

	GET_SINGLE(CGameManager)->PostInit();

	GET_SINGLE(CSceneManager)->AddScene<CLogoScene>("Start", "Start");
	GET_SINGLE(CSceneManager)->AddScene<CTutorialScene>("TutorialScene", "Tutorial");
	GET_SINGLE(CSceneManager)->AddScene<CStage1Scene>("Stage1", "Stage1");
	//GET_SINGLE(CSceneManager)->AddScene<CStage2Scene>("Stage2", "Stage2");

	GET_SINGLE(CGameManager)->Init();

	GET_SINGLE(CSceneManager)->Access();

	GET_SINGLE(CGameManager)->AfterInit();

    int iRet = CCore::GetInst()->Run();

	DESTROY_SINGLE(CGameManager);
	CCore::DestroyInst();
	CCameraEff::DestroyInst();
	CCommonSoundLoader::DestroyInst();

	return iRet;
}