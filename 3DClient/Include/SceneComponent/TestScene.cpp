#include "../ClientHeader.h"
#include "TestScene.h"
#include "Component/Arm.h"
#include "Component/Light.h"
#include "Component/Decal.h"
#include "Component/Camera.h"
#include "Component/Particle.h"
#include "Component/VolumeFog.h"
#include "Component/LandScape.h"
#include "Component/Animation2D.h"
#include "Component/ColliderOBB3D.h"
#include <Component/Renderer.h>
#include <Component/Material.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "Component/Gizmo.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"

CTestScene::CTestScene()
{
	m_fPlayTime = 0.f;
	m_bGhostOn = false;
	m_bIncrease = false;
	m_fX = 0.f;
	m_fY = 0.f;
}

CTestScene::~CTestScene()
{
	SAFE_RELEASE(m_pUILayer);

	if (m_bGhostOn)
		SAFE_RELEASE(m_pGhostTr);
}

bool CTestScene::Init()
{
	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->PlayBgm("bgm1");
	//
	//_SMgr->SetAudioCoordSize(16.f);

	//PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	//_SMgr->CreateSoundEffect("bgm1", TEXT("SurgeonAttack.wav"));
	//_SMgr->SoundPlay("bgm1", ST_BGM);


	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	m_pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	CLayer* pLightLayer = m_pScene->FindLayer("Light");

	pObject = CGameObject::CreateObject("SpotLight", pLightLayer);

	CLight* pLight = pObject->AddComponent<CLight>("Light");
	pLight->SetLightDiffuse(Vector4::Red);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(100.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(50.0f, 70.0f, 0.0f);

	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLightLayer);


	//CGameObject* pFogObj = CGameObject::CreateObject("FogObj", pDefaultLayer);
	//pTransform = pFogObj->GetTransform();

	//pTransform->SetWorldScale(50, 50, 50);
	//pTransform->SetWorldPos(3, 4, 3);

	//SAFE_RELEASE(pTransform);

	//CVolumeFog* pFog = pFogObj->AddComponent<CVolumeFog>("Fog");
	//pFog->SetFogColor(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	//SAFE_RELEASE(pFog);
	//SAFE_RELEASE(pFogObj);


	//pObject = CGameObject::CreateObject("Field", pDefaultLayer, true);

	//CField*	pField = pObject->AddComponent<CField>("Field");

	//SAFE_RELEASE(pField);

	//SAFE_RELEASE(pObject);

	
	//pObject = CGameObject::CreateObject("TestObject", pDefaultLayer, true);
	//pTransform = pObject->GetTransform();

	////pTransform->SetWorldPos(0.f, 0.f, 0.f);
	//pTransform->SetWorldPos(1.f, 1.f, 1.f);
	//pCamera->SetTarget(pObject);
	//m_pTr = pTransform;



	//CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");

	//SAFE_RELEASE(pTransform);
	//SAFE_RELEASE(pPlayer);
	//SAFE_RELEASE(pObject);	

	//pObject = CGameObject::CreateObject("Minion", pDefaultLayer);


	////PUN::CSoundSource *pSoundSrc = pObject->AddComponent<PUN::CSoundSource>("Ssource");
	////const char *pStrName = "heeeeeik";
	////const TCHAR *pFileName = TEXT("evillaugh.wav");
	////pSoundSrc->LoadSounds(&pStrName, &pFileName, 1);
	//////pSoundSrc->SetRadius(1500.f);
	////pSoundSrc->SetPitch(0, -0.2f);
	////pSoundSrc->Play(0, true);

	////SAFE_RELEASE(pSoundSrc);

	//CMinion3D*	pMinion = pObject->AddComponent<CMinion3D>("Minion");

	//pMinion->SetTargetTransform(m_pTr);

	//SAFE_RELEASE(pMinion);

	//SAFE_RELEASE(pObject);

	//pObject = CGameObject::CreateObject("HandGun", pDefaultLayer, true);
	//PUN::CRenderer* renderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	//pTransform = pObject->GetTransform();
	//pTransform->SetWorldScale(1.f, 1.f, 1.f);
	//pTransform->SetWorldPos(0.f, 0.f, 15.f);
	//pTransform->SetWorldRotX(-90.f);
	//renderer->SetMesh("gun", TEXT("glock_OnlyFire.FBX"), MESH_PATH);

	//SAFE_RELEASE(pTransform);
	//SAFE_RELEASE(renderer);
	//SAFE_RELEASE(pObject);



	/*CGameObject*	pDecalObj = CGameObject::CreatePrototype("Decal");

	CDecal*	pDecal = pDecalObj->AddComponent<CDecal>("Decal");

	SAFE_RELEASE(pDecal);

	CColliderOBB3D*	pDecalSphere = pDecalObj->AddComponent<CColliderOBB3D>("DecalSphere");

	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));

	SAFE_RELEASE(pDecalSphere);

	CMaterial*	pDecalMtrl = pDecalObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pDecalMtrl->SetDiffuseTex(0, "DecalDif", TEXT("Decal/Shout24674-perfil3_COLOR.png"));
	pDecalMtrl->SetNormalTex(1, "DecalNrm", TEXT("Decal/Shout24674-perfil3_NRM.png"));
	pDecalMtrl->SetSpecularTex(2, "DecalSpc", TEXT("Decal/Shout24674-perfil3_SPEC.png"));

	pDecalMtrl->SetSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pDecalMtrl);

	SAFE_RELEASE(pDecalObj);

	pDecalObj = CGameObject::CreatePrototype("DecalCannon");

	pDecal = pDecalObj->AddComponent<CDecal>("Decal");

	SAFE_RELEASE(pDecal);

	pDecalSphere = pDecalObj->AddComponent<CColliderOBB3D>("DecalSphere");

	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(4.f, 0.75f, 4.f));

	SAFE_RELEASE(pDecalSphere);

	pDecalMtrl = pDecalObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pDecalMtrl->SetDiffuseTex(0, "DecalCannonDif", TEXT("Decal/Decal.png"));
	pDecalMtrl->SetNormalTex(1, "DecalCannonNrm", TEXT("Decal/Decal_NRM.png"));
	pDecalMtrl->SetSpecularTex(2, "DecalCannonSpc", TEXT("Decal/Decal_SPEC.png"));

	pDecalMtrl->SetSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	pDecalMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pDecalMtrl);

	SAFE_RELEASE(pDecalObj);

	pDecalObj = CGameObject::CreateClone("Decal", "Decal",
		pDefaultLayer);

	CTransform*	pDecalTr = pDecalObj->GetTransform();

	pDecalTr->SetWorldPos(3.f, 2.f, 3.f);
	pDecalTr->SetWorldScale(5.f, 3.f, 5.f);

	SAFE_RELEASE(pDecalTr);

	SAFE_RELEASE(pDecalObj);

	pDecalObj = CGameObject::CreateClone("DecalCannon", "DecalCannon",
		pDefaultLayer);

	pDecalTr = pDecalObj->GetTransform();

	pDecalTr->SetWorldPos(10.f, 1.5f, 3.f);
	pDecalTr->SetWorldScale(8.f, 1.5f, 8.f);

	SAFE_RELEASE(pDecalTr);

	SAFE_RELEASE(pDecalObj);*/


	//CGameObject* pLightObj = CGameObject::CreateObject("GlobalLight", pDefaultLayer, true);

	//CLight* pLight = pLightObj->AddComponent<CLight>("GlobalLight");
	//pLight->SetLightColor(Vector4::White, Vector4::White, Vector4::White);
	//pLight->SetLightType(LT_DIR);
	//pLight->SetLightDirection(Vector3(0.0f, -1.0f, 0.0f));

	//SAFE_RELEASE(pLight);
	//SAFE_RELEASE(pLightObj);

	//CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);
	//pTransform = pLandScapeObj->GetTransform();

	//SAFE_RELEASE(pTransform);

	//CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	//pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
	//	TEXT("LandScape/Terrain_Cliff_11.dds"),
	//	TEXT("LandScape/Terrain_Cliff_11_NRM.bmp"),
	//	TEXT("LandScape/Terrain_Cliff_11_SPEC.bmp"),
	//	"LandScape/height2.bmp");

	////GET_SINGLE(CNavigationManager3D)->CreateNavMesh(m_pScene,
	////	"Nav.nav");

	//vector<const TCHAR*> vecSplatName;

	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));

	//pLandScape->AddSplatDifTexture("LandScapeSplatDif",	vecSplatName);

	//vecSplatName.clear();
	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));

	//pLandScape->AddSplatNrmTexture("LandScapeSplatNrm", vecSplatName);

	//vecSplatName.clear();
	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPC.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));

	//pLandScape->AddSplatSpcTexture("LandScapeSplatSpc", vecSplatName);

	//vecSplatName.clear();
	//vecSplatName.push_back(TEXT("LandScape/RoadAlpha.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/StonAlpha.bmp"));
	//vecSplatName.push_back(TEXT("LandScape/FlowerAlpha.bmp"));

	//pLandScape->AddSplatAlphaTexture("LandScapeSplatAlpha", vecSplatName);

	//SAFE_RELEASE(pLandScape);

	//SAFE_RELEASE(pLandScapeObj);

	//CGameObject*	pParticleObj = CGameObject::CreatePrototype("ExplosionParticle");

	//CParticle*	pParticle = pParticleObj->AddComponent<CParticle>("Particle");

	//SAFE_RELEASE(pParticle);

	//CColliderOBB3D*	pParticleSphere = pParticleObj->AddComponent<CColliderOBB3D>("DecalSphere");

	//pParticleSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.f, 1.f, 1.f));

	//SAFE_RELEASE(pParticleSphere);

	//CMaterial*	pParticleMtrl = pParticleObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	//vector<const TCHAR*>	vecExplosionName;
	//for (int i = 1; i <= 89; ++i)
	//{
	//	TCHAR*	pFileName = new TCHAR[MAX_PATH];
	//	memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

	//	wsprintf(pFileName, TEXT("Explosion/Explosion%d.png"), i);
	//	vecExplosionName.push_back(pFileName);
	//}
	//pParticleMtrl->SetDiffuseTex(4, "Explosion", vecExplosionName);

	//pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);

	//SAFE_RELEASE(pParticleMtrl);

	//CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");

	//vector<Clip2DFrame>	vecFrame;
	//for (int i = 1; i <= 89; ++i)
	//{
	//	Clip2DFrame	tFrame = {};
	//	tFrame.vLT = Vector2(0.f, 0.f);
	//	tFrame.vRB = Vector2(1.f, 1.f);
	//	vecFrame.push_back(tFrame);
	//}

	//pParticleAnimation->AddClip("Idle", A2D_FRAME, AO_LOOP,
	//	1.f, vecFrame, "Explosion", vecExplosionName);

	//SAFE_RELEASE(pParticleAnimation);

	//for (size_t i = 0; i < vecExplosionName.size(); ++i)
	//{
	//	SAFE_DELETE_ARRAY(vecExplosionName[i]);
	//}

	//vecExplosionName.clear();

	//SAFE_RELEASE(pParticleObj);

	//pParticleObj = CGameObject::CreateClone("ExplosionParticle",
	//	"ExplosionParticle", pDefaultLayer);

	//pTransform = pParticleObj->GetTransform();

	//pTransform->SetWorldPos(5.f, 2.f, 5.f);
	//pTransform->SetWorldScale(6.f, 6.f, 1.f);

	//SAFE_RELEASE(pTransform);

	//SAFE_RELEASE(pParticleObj);

//////////////////////////////////////////Terrain 배치 //////////////////////////////////////////////////
///////////////////////////////X Tile 배치///////////////////////////////////////////////////////

	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(20.f, 0.f, 0.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	pObject->SetRenderGroup(RG_LANDSCAPE);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);


	for (int i = 1; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(i * 40.f, 0.f, 0.f);
		pTransform->SetWorldRot(0.f, 0.f, 0.f);
		pTransform->SetWorldScale(0.2f);

		pObject->SetRenderGroup(RG_LANDSCAPE);

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	///////////////////////XCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(240.f, 0.f, 0.f);
	pTransform->SetWorldRot(0.f, 90.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	pObject->SetRenderGroup(RG_LANDSCAPE);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);
	//////////////////////////////////Z배치//////////////////////////////////

	for (int i = 0; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(240.f, 0.f, i * 40.f + 40.f);
		pTransform->SetWorldRot(0.f, 90.f, 0.f);
		pTransform->SetWorldScale(0.2f);

		pObject->SetRenderGroup(RG_LANDSCAPE);

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	///////////////////////ZCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(240.f, 0.f, 280.f);
	pTransform->SetWorldRot(0.f, 0.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	pObject->SetRenderGroup(RG_LANDSCAPE);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	///////////////////////////////X Tile 배치///////////////////////////////////////////////////////
	for (int i = 0; i < 6; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(200.f - (i * 40.f), 0.f, 280.f);
		pTransform->SetWorldRot(0.f, 180.f, 0.f);
		pTransform->SetWorldScale(0.2f);

		pObject->SetRenderGroup(RG_LANDSCAPE);

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}

	///////////////////////XCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(-30.f, 0.f, 280.f);
	pTransform->SetWorldRot(0.f, 270.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	pObject->SetRenderGroup(RG_LANDSCAPE);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	///////////////////////////////Z Tile 배치///////////////////////////////////////////////////////
	for (int i = 0; i < 5; ++i)
	{
		pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

		pRenderer->SetMesh("Corridor_3mx2m", L"Corridor_3mx2m.msh");

		SAFE_RELEASE(pRenderer);

		pTransform = pObject->GetTransform();

		pTransform->SetWorldPos(-30.f, 0.f, 240.f - (i * 40.f));
		pTransform->SetWorldRot(0.f, 90.f, 0.f);
		pTransform->SetWorldScale(0.2f);

		pObject->SetRenderGroup(RG_LANDSCAPE);

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx2m.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(-30.f, 0.f, 50.f);
	pTransform->SetWorldRot(0.f, 90.f, 0.f);
	pTransform->SetWorldScale(0.2f);

	pObject->SetRenderGroup(RG_LANDSCAPE);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	////////////////////////////////////////////////////////////////////////

	pObject = CGameObject::CreateObject("Door", pDefaultLayer);
	pTransform = pObject->GetTransform();

	CDoor* pDoor = pObject->AddComponent<CDoor>("door");


	pTransform->SetWorldPos(0.f, 0.f, 5.f);
	pTransform->SetWorldScale(0.1f, .1f, 0.1f);

	pTransform->RotationY(-90.f);
	
	pTransform->SetWorldPivot(0.5f, .0f, .0f);
	pDoor->SetOpenTime(1.5f);
	pDoor->SetCloseTime(1.5f);
	
	std::string names[3];
	names[0] = "wood_Door_Open1";
	names[1] = "wood_Door_Close1";
	names[2] = "wood_Door_Bash1";

	TCHAR *strPaths[3]; 
	strPaths[0] = (TCHAR*)TEXT("WoodenDoor_OPENING_01.wav");
	strPaths[1] = (TCHAR*)TEXT("WoodenDoor_CLOSING_03.wav");
	strPaths[2] = (TCHAR*)TEXT("SFX_WoodenDoor_Bash_01.wav");

	pDoor->SetSounds(names, (const TCHAR**)strPaths);

	PUN::CRenderer* renderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	renderer->SetMesh("wdoor_1", TEXT("wood_door.FBX"), MESH_PATH);

	
	SAFE_RELEASE(pDoor);

	SAFE_RELEASE(renderer);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = PUN::CGameObject::CreateObject("hPlayer", pDefaultLayer, true);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.1f, .1f, .1f);
	CHuman_Player *pHPlayer = pObject->AddComponent<CHuman_Player>("Player");
	pHPlayer->LoadData(TEXT("PlayerData.csv"));

	SAFE_RELEASE(pHPlayer);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	//SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CTestScene::Update(float fTime)
{
	static bool bPush = false;
	if (GetAsyncKeyState('G') & 0x8000)
	{
		bPush = true;
	}
	else if (bPush)
	{
		bPush = false;
		m_bGhostOn = true;
		m_bIncrease = true;

		CGameObject*	pGhostObj = CGameObject::CreateObject("Ghost", m_pUILayer);

		CGhost*	pGhost = pGhostObj->AddComponent<CGhost>("Ghost");

		m_pGhostTr = pGhostObj->GetTransform();

		//SAFE_RELEASE(pGhostTr);
		SAFE_RELEASE(pGhost);
		SAFE_RELEASE(pGhostObj);
	}

	if (m_bIncrease)
	{
		m_fPlayTime += fTime;

		if (m_fX <= 4096.f && m_fY <= 4096.f)
		{
			m_fX += 9000 * fTime;
			m_fY += 9000 * fTime;
			m_pGhostTr->SetWorldScale(m_fX, m_fY, 1.f);
		}
	}

	if (m_fPlayTime >= 2.f)
	{
		m_bIncrease = false;
		m_fPlayTime = 0.f;
		m_fX = 1.f;
		m_fY = 1.f;
	}

	return 0;
}

void CTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CTestScene::CreatePrototype()
{
}

void CTestScene::NextScene(float fTime)
{
}
