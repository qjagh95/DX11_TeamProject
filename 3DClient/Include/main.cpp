#include "ClientHeader.h"
#include "Core.h"
#include "SceneComponent/MainScene.h"
#include "SceneComponent/TutorialScene.h"
#include "SceneComponent/FirTestScene.h"
#include "SceneComponent/Stage1Scene.h"
#include "SceneComponent/JBH_Stage3.h"
#include "SceneComponent/TestScene.h"
#include "SceneComponent/TestScene-YH.h"
#include "SceneComponent/LogoScene.h"
#include "SceneComponent/RandScapeTestScene.h"
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

	//FPS 카메라와 카메라 흔들림 이펙트를 구현하는 매니저 구동

	GET_SINGLE(CGameManager)->PostInit();

	GET_SINGLE(CSceneManager)->AddScene<CTutorialScene>("First", "Test");
	//GET_SINGLE(CSceneManager)->AddScene<CStage1Scene>("Stage1", "Stage1");

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