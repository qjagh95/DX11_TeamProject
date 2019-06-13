#include "../ClientHeader.h"
#include "TestScene-YH.h"
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
#include <SoundManager.h>
#include <Component/Animation.h>
#include "Component/Gizmo.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Ghost.h"
#include "../UserComponent/Human_Player.h"
#include "../UserComponent/Battery.h"
#include "../UserComponent/BatteryIcon.h"
#include "../CameraEff.h"
#include "../UserComponent/Enemies.h"
#include "../UserComponent/Bed.h"
#include "../UserComponent/Locker.h"
#include "../UserComponent/Parkour.h"

CTestSceneYH::CTestSceneYH():
	m_fAmb1SndTime(10.5f),
	m_fAmb1Timer(0.f),
	m_bInitUpdate(false)
{
	m_fPlayTime = 0.f;
	m_bGhostOn = false;
	m_bIncrease = false;
	m_fX = 0.f;
	m_fY = 0.f;
}

CTestSceneYH::~CTestSceneYH()
{
	SAFE_RELEASE(m_pUILayer);

	if (m_bGhostOn)
		SAFE_RELEASE(m_pGhostTr);

	SAFE_RELEASE(m_pAmb);
}

bool CTestSceneYH::Init()
{
	//CCameraEff::GetInst()->SetFirstPersonViewEnable();
	//PUN::CSoundManager::GetInst()->PlayBgm(TEXT("Zipper_Open.WAV"), true);

	PUN::CSoundManager::GetInst()->CreateSoundEffect("bgm001", TEXT("APINK.wav"));
	PUN::CSoundManager::GetInst()->CreateSoundEffect("bgm002", TEXT("orig_sudden.wav"));

	PUN::CSoundManager::GetInst()->SetTransitionTime(1.5f);

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);
	pCamera->SetNear(0.03f);

	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");
	m_pUILayer = m_pScene->FindLayer("UI");
	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	std::vector<std::string> vecAmb;

	vecAmb.push_back("AMB_Ext_loop");
	vecAmb.push_back("AMB_Fence_wind");

	PUN::CGameObject *pObj = PUN::CGameObject::CreateObject("ambient", pDefaultLayer);
	m_pAmb = pObj->AddComponent<PUN::CSoundSource>("sound");
	m_pAmb->LoadSounds(vecAmb);
	m_pAmb->SetVolume(1, 0.3f);

	SAFE_RELEASE(pObj);

	m_pAmb->Play(0, true);

	CLayer* pLightLayer = m_pScene->FindLayer("Light");

	pObject = CGameObject::CreateObject("SpotLight", pLightLayer);

	CLight* pLight = pObject->AddComponent<CLight>("Light");
	pLight->SetLightDiffuse(Vector4::Red);
	pLight->SetLightType(LT_SPOT);
	pLight->SetLightRange(75.0f);
	pLight->SetAngle(45.0f, 60.0f);

	pTransform = pObject->GetTransform();
	pTransform->RotationX(90.0f);
	pTransform->SetWorldPos(Vector3(50.f, 75.f, 0.f));
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLightLayer);
	
	
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


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	/*
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


		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pObject);
	}
	*/
	
	/*
	///////////////////////XCorner 배치////////////////////////////////////
	pObject = CGameObject::CreateObject("Terrain", pDefaultLayer);

	pRenderer = pObject->AddComponent<CRenderer>("TerrainRenderer");

	pRenderer->SetMesh("Corridor_3m3m_Corner", L"Corridor_3mx3m_Corner.msh");

	SAFE_RELEASE(pRenderer);

	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(240.f, 0.f, 0.f);
	pTransform->SetWorldRot(0.f, 90.f, 0.f);
	pTransform->SetWorldScale(0.2f);


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


	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	////////////////////////////////////////////////////////////////////////

	pObject = CGameObject::CreateObject("Door", pDefaultLayer);
	pTransform = pObject->GetTransform();

	pTransform->SetWorldPos(20.f, 0.f, 25.f);
	pTransform->SetWorldScale(0.1f, .1f, 0.1f);

	pTransform->RotationY(-90.f);
	CDoor* pDoor = pObject->AddComponent<CDoor>("door");

	pTransform->SetWorldPivot(0.5f, .0f, .0f);
	pDoor->SetOpenTime(1.5f);
	//pDoor->SetCloseTime(1.5f);

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

	pObject = PUN::CGameObject::CreateObject("FxxChair", pDefaultLayer);

	PUN::CTransform*	pTr = pObject->GetTransform();
	pTr->SetWorldScale(0.2f, 0.2f, 0.2f);
	pTr->SetWorldPos(0.f, 0.f, 25.f);
	//pTr->SetLocalRotY(-90.f);

	PUN::CRenderer*	pWeaponRenderer = pObject->AddComponent<PUN::CRenderer>("WeaponRenderer");
	pWeaponRenderer->SetMesh("chair", TEXT("chair.msh"));
	pWeaponRenderer->SetDecalEnable(false);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pWeaponRenderer);
	SAFE_RELEASE(pObject);

	*/
	

	//pObject = PUN::CGameObject::CreateObject("hPlayer", pDefaultLayer, true);
	//pTransform = pObject->GetTransform();
	//pTransform->SetWorldScale(0.0375f, .0375f, .0375f);
	//pTransform->SetLocalRotY(180.f);
	//CHuman_Player *pHPlayer = pObject->AddComponent<CHuman_Player>("Player");
	
	//SAFE_RELEASE(pHPlayer);

	//SAFE_RELEASE(pTransform);
	//SAFE_RELEASE(pObject);

	/*
	CGameObject*	pBatteryObj = CGameObject::CreateObject("Battery", pDefaultLayer);

	CBattery*	pBattery = pBatteryObj->AddComponent<CBattery>("Battery");

	CTransform*	pBatteryTr = pBatteryObj->GetTransform();

	pBatteryTr->SetWorldPos(-10.f, 5.f, 30.f);

	SAFE_RELEASE(pBatteryTr);
	SAFE_RELEASE(pBattery);
	SAFE_RELEASE(pBatteryObj);
	
	pObject = Enemies::GetInstance("enem1", 0, pDefaultLayer, false);

	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.05f, .05f, .05f);

	pTransform->SetWorldPos(0.f, 0.f, 5.f);

	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pTransform);

	PUN::CAnimation *pAnim = nullptr;

	pObject = Enemies::GetInstance("enem2", 1, pDefaultLayer, false);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.05f, .05f, .05f);
	pTransform->SetWorldPos(-5.f, 0.f, 5.f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);
	
	
	
	pObject = Enemies::GetInstance("enem3", 2, pDefaultLayer, false);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.05f, .05f, .05f);
	pTransform->SetWorldPos(-10.f, 0.f, 5.f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);

	pObject = Enemies::GetInstance("enem4", 3, pDefaultLayer, false);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.05f, .05f, .05f);
	pTransform->SetWorldPos(-15.f, 0.f, 5.f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pAnim);
	SAFE_RELEASE(pObject);
	
	
	pObject = PUN::CGameObject::CreateObject("gunfire", pDefaultLayer);
	pRenderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	pRenderer->SetMesh("pew", TEXT("glock_Fire.FBX"), MESH_PATH);
	SAFE_RELEASE(pRenderer);
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(0.05f, .05f, .05f);
	pTransform->SetWorldPos(-15.f, 0.f, 3.5f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObject);
	
	*/
	
/*
//파티클 만들기
	CGameObject*	pParticleObj = CGameObject::CreateObject("BloodParticle", pDefaultLayer);
	CParticle*	pParticle = pParticleObj->AddComponent<CParticle>("Particle");
	SAFE_RELEASE(pParticle);

	pRenderer = pParticleObj->FindComponentFromType<CRenderer>(PUN::CT_RENDERER);
	pRenderer->SetMesh("Particle_GreenMat");
	SAFE_RELEASE(pRenderer);
	CMaterial*	pParticleMtrl = pParticleObj->FindComponentFromType<CMaterial>(CT_MATERIAL);
	vector<const TCHAR*>	vecExplosionName;
	vector<Clip2DFrame>	vecFrame;
	for (int i = 1; i <= 14; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(pFileName, TEXT("Particles\\Blood1\\Blood%d.png"), i);
		vecExplosionName.push_back(pFileName);

		Clip2DFrame	tFrame = {};
		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(1.f, 1.f);
		vecFrame.push_back(tFrame);
	}
	pParticleMtrl->SetDiffuseTex(4, "Gun_Blood", vecExplosionName);
	pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pParticleMtrl);
	CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
	pParticleAnimation->AddClip("Idle", A2D_FRAME, AO_LOOP,
		0.333334f, vecFrame, "Gun_Blood", vecExplosionName);

	SAFE_RELEASE(pParticleAnimation);
	for (size_t i = 0; i < vecExplosionName.size(); ++i)
	{
		SAFE_DELETE_ARRAY(vecExplosionName[i]);
	}
	vecExplosionName.clear();

	pTransform = pParticleObj->GetTransform();
	pTransform->SetWorldPos(-50.f, 15.f, 5.f);
	pTransform->SetWorldScale(6.f, 6.f, 1.f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pParticleObj);


	//먼지 파티클
	pParticleObj = CGameObject::CreateObject("DustParticle", pDefaultLayer);
	pParticle = pParticleObj->AddComponent<CParticle>("Particle");
	SAFE_RELEASE(pParticle);

	pRenderer = pParticleObj->FindComponentFromType<CRenderer>(PUN::CT_RENDERER);
	pRenderer->SetMesh("Particle_GreenMat");
	//pRenderer->SetMesh("Particle_BlackMat");
	SAFE_RELEASE(pRenderer);
	pParticleMtrl = pParticleObj->FindComponentFromType<CMaterial>(CT_MATERIAL);
	vecExplosionName.clear();
	vecFrame.clear();
	for (int i = 1; i <= 179; ++i)
	{
		int idx = i < 90 ? i : 180 - i;
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(pFileName, TEXT("Particles\\Dust_Interior_Loop\\dust%d.png"), idx);
		vecExplosionName.push_back(pFileName);

		Clip2DFrame	tFrame = {};
		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(1.f, 1.f);
		vecFrame.push_back(tFrame);
	}
	pParticleMtrl->SetDiffuseTex(4, "Dust_Loop", vecExplosionName);
	pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);
	SAFE_RELEASE(pParticleMtrl);
	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
	pParticleAnimation->AddClip("Idle", A2D_FRAME, AO_LOOP,
		6.f, vecFrame, "Interior_Dust", vecExplosionName);

	SAFE_RELEASE(pParticleAnimation);
	for (size_t i = 0; i < vecExplosionName.size(); ++i)
	{
		SAFE_DELETE_ARRAY(vecExplosionName[i]);
	}
	vecExplosionName.clear();

	pTransform = pParticleObj->GetTransform();
	pTransform->SetWorldPos(0.f, 4.f, 0.f);
	pTransform->SetWorldScale(32.f, 18.f, 1.f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pParticleObj);
*/
	
	
	
	//숨을 침대
	pObj = CGameObject::CreateObject("bed_test", pDefaultLayer);
	
	pTransform = pObj->GetTransform();
	pTransform->SetWorldPos(15.f, 0.f, 10.f);
	pTransform->SetWorldRot(0.f, 45.f, 0.f);
	SAFE_RELEASE(pTransform);
	CBed *pBed = pObj->AddComponent<CBed>("bed");

	SAFE_RELEASE(pBed);

	SAFE_RELEASE(pObj);
	
	

	
	//Locker
	pObj = CGameObject::CreateObject("locker_test", pDefaultLayer);
	pTransform = pObj->GetTransform();
	pTransform->SetWorldPos(Vector3(10.f, 0.f, 15.f));
	//pTransform->SetWorldRot(0.f, 90.f, 0.f);
	CLocker* pLocker = pObj->AddComponent<CLocker>("locker");

	SAFE_RELEASE(pLocker);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObj);
	
	
	//넘을 책상
	pObj = CGameObject::CreateObject("vault_desk", pDefaultLayer);
	pTransform = pObj->GetTransform();
	pTransform->SetWorldPos(Vector3(5.f, 0.f, -15.f));
	pTransform->SetWorldRot(Vector3(0.f, 15.f, 0.f));
	CParkour *pParkour = pObj->AddComponent<CParkour>("vault");
	

	SAFE_RELEASE(pParkour);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObj);

	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CTestSceneYH::Update(float fTime)
{
	if (PUN::CInput::GetInst()->KeyPress("NumPad1"))
	{
		PUN::CSoundManager::GetInst()->PlayBgm("bgm001");
	}
	if (PUN::CInput::GetInst()->KeyPress("NumPad2"))
	{
		PUN::CSoundManager::GetInst()->PlayBgm("bgm002");
	}

	if (!m_bInitUpdate)
	{
		if (_PLAYER)
		{
			PUN::CTransform *pPlayerTR = _PLAYER->GetTransform();
			pPlayerTR->SetWorldPos(Vector3::Zero);
			SAFE_RELEASE(pPlayerTR);
			m_bInitUpdate = true;
		}
	}

	static bool bPush = false;

	PUN::CTransform *pTr = m_pAmb->GetTransform();
	PUN::CTransform * pCamera = m_pScene->GetMainCameraTransform();
	pTr->SetWorldPos(pCamera->GetWorldPos());
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pTr);

	if (m_fAmb1Timer >= m_fAmb1SndTime)
	{
		if (m_pAmb->GetClipState(0) != PLAYING)
		{
			m_pAmb->Play(1, false);
		}
		m_fAmb1Timer = 0.f;
		
	}
	
	m_fAmb1Timer += fTime;

	if (GetAsyncKeyState('P') & 0x8000)
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

void CTestSceneYH::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CTestSceneYH::CreatePrototype()
{
}

void CTestSceneYH::NextScene(float fTime)
{
}
