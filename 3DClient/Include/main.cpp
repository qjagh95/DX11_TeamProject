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
#include <Rendering/ShaderManager.h>
#include <Resource/ResourcesManager.h>

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

	char* pEntry[3] = {};
	pEntry[ST_VERTEX] = (char*)"ParticleVS";
	pEntry[ST_PIXEL] = (char*)"ParticlePS_GreenMat";
	pEntry[ST_GEOMETRY] = (char*)"ParticleGS";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_GreenMat", TEXT("Particle.fx"), pEntry))
		return false;

	Vector3	vParticle;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_GreenMat", "Particle_GreenMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pEntry[ST_PIXEL] = (char*)"ParticlePS_BlueMat";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_BlueMat", TEXT("Particle.fx"), pEntry))
		return false;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_BlueMat", "Particle_BlueMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pEntry[ST_PIXEL] = (char*)"ParticlePS_BlackMat";
	if (!GET_SINGLE(CShaderManager)->LoadShader("Particle_BlackMat", TEXT("Particle.fx"), pEntry))
		return false;
	GET_SINGLE(CResourcesManager)->CreateMesh("Particle_BlackMat", "Particle_BlackMat", POS_LAYOUT,
		&vParticle, 1, sizeof(Vector3), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	CCommonSoundLoader::GetInst()->LoadSoundCSVList(TEXT("CommonSound.csv"));
	CCommonSoundLoader::GetInst()->LoadSoundRandomSeedCnt(TEXT("CommonSound_SoundCnt.csv"));
	GET_SINGLE(CSceneManager)->AddScene<CTestSceneYH>("First", "Test");

#ifdef _DEBUG
	//GET_SINGLE(CSceneManager)->AddScene<CTutorialScene>("First", "TutorialScene");
	//GET_SINGLE(CSceneManager)->AddScene<CTestSceneYH>("First", "TutorialScene");
	//GET_SINGLE(CSceneManager)->AddScene<CLogoScene>("First", "LogoScene"); 
	//GET_SINGLE(CSceneManager)->AddScene<CTestSceneYH>("Second", "YH_TEST");
	//GET_SINGLE(CSceneManager)->AddScene<CFirTestScene>("Third", "FirTestScene");

#else
	//GET_SINGLE(CSceneManager)->AddScene<CTutorialScene>("First", "TutorialScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CTutorialScene>("TutorialScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CLogoScene>("LogoScene");
#endif

	GET_SINGLE(CSceneManager)->Access();

	//GET_SINGLE(CSceneManager)->AddSceneComponent<CTestScene>("TestScene");
	//GET_SINGLE(CSceneManager)->AddSceneComponent<CFirTestScene>("Fir");
	

    int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();
	CCameraEff::DestroyInst();
	CCommonSoundLoader::DestroyInst();

	return iRet;
}