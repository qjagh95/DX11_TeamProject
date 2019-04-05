#include "../ClientHeader.h"
#include "FirTestScene.h"
#include "Component/Arm.h"
#include "Component/Light.h"
#include "Component/Decal.h"
#include "Component/Camera.h"
#include "Component/Particle.h"
#include "Component/VolumeFog.h"
#include "Component/LandScape.h"
#include "Component/Animation2D.h"
#include "Component/ColliderOBB3D.h"
#include "../UserComponent/Field.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Minion.h"
#include "../UserComponent/Field.h"
#include <Component/Renderer.h>
#include <Component/Material.h>
#include <Component/SoundSource.h>
#include <Component/Animation.h>
#include "Component/Gizmo.h"
CFirTestScene::CFirTestScene()
{
}

CFirTestScene::~CFirTestScene()
{
}

bool CFirTestScene::Init()
{
	PUN::CSoundManager *_SMgr = PUN::CSoundManager::GetInst();

	_SMgr->CreateSoundEffect("bgm1", TEXT("APINK.wav"));
	_SMgr->PlayBgm("bgm1");

	CCamera* pCamera = m_pScene->GetMainCamera();
	pCamera->SetCameraType(CT_PERSPECTIVE);

	CTransform* pCameraTr = pCamera->GetTransform();
	pCameraTr->SetLocalPos(Vector3(0.f, 5.f, 5.f));
	SAFE_RELEASE(pCameraTr);
	CLayer* pBackLayer = m_pScene->FindLayer("BackGround");
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");
	CLayer* pUILayer = m_pScene->FindLayer("UI");
	CLayer* pTileLayer = m_pScene->FindLayer("Tile");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pObject);

	CGameObject* pFogObj = CGameObject::CreateObject("FogObj", pDefaultLayer);
	pTransform = pFogObj->GetTransform();

	pTransform->SetWorldScale(50, 20, 50);
	pTransform->SetWorldPos(3, 4, 3);

	SAFE_RELEASE(pTransform);

	CVolumeFog* pFog = pFogObj->AddComponent<CVolumeFog>("Fog");
	pFog->SetFogColor(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	SAFE_RELEASE(pFog);
	SAFE_RELEASE(pFogObj);

	pObject = CGameObject::CreateObject("Field", pDefaultLayer, true);

	CField*	pField = pObject->AddComponent<CField>("Field");

	SAFE_RELEASE(pField);

	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("TestObject", pDefaultLayer, true);
	pTransform = pObject->GetTransform();

	//pTransform->SetWorldPos(0.f, 0.f, 0.f);
	pTransform->SetWorldPos(1.f, 1.f, 1.f);
	pCamera->SetTarget(pObject);
	m_pTr = pTransform;

	CArm*	pArm = pCamera->AddComponent<CArm>("CameraArm");

	pArm->EnableMouse();
	pArm->SetTarget(pTransform);

	SAFE_RELEASE(pArm);

	CPlayer*	pPlayer = pObject->AddComponent<CPlayer>("Player");

	//CGameObject* pGizmoObject = CGameObject::CreateObject("GizmoObj", pDefaultLayer);
	//CGizmo*     pGizmo = pGizmoObject->AddComponent<CGizmo>("Gizmo");
	//pGizmo->SetTargetTr(pTransform);
	//pGizmo->SetGizmoType(GT_X);
	//SAFE_RELEASE(pGizmo);
	//SAFE_RELEASE(pGizmoObject);

	//pGizmoObject = CGameObject::CreateObject("GizmoObj", pDefaultLayer);
	//pGizmo = pGizmoObject->AddComponent<CGizmo>("Gizmo");
	//pGizmo->SetTargetTr(pTransform);
	//pGizmo->SetGizmoType(GT_Y);
	//SAFE_RELEASE(pGizmo);
	//SAFE_RELEASE(pGizmoObject);

	//pGizmoObject = CGameObject::CreateObject("GizmoObj", pDefaultLayer);
	//pGizmo = pGizmoObject->AddComponent<CGizmo>("Gizmo");
	//pGizmo->SetTargetTr(pTransform);
	//pGizmo->SetGizmoType(GT_Z);
	//SAFE_RELEASE(pGizmo);
	//SAFE_RELEASE(pGizmoObject);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pObject);

	pObject = CGameObject::CreateObject("Minion", pDefaultLayer);

	PUN::CSoundSource *pSoundSrc = pObject->AddComponent<PUN::CSoundSource>("Ssource");
	const char *pStrName = "heeeeeik";
	const TCHAR *pFileName = TEXT("evillaugh.wav");
	pSoundSrc->LoadSounds(&pStrName, &pFileName, 1);
	//pSoundSrc->SetRadius(1500.f);
	pSoundSrc->SetPitch(0, -0.2f);
	pSoundSrc->Play(0, true);

	SAFE_RELEASE(pSoundSrc);

	CMinion3D*	pMinion = pObject->AddComponent<CMinion3D>("Minion");

	pMinion->SetTargetTransform(m_pTr);

	SAFE_RELEASE(pMinion);

	SAFE_RELEASE(pObject);
	
	pObject = CGameObject::CreateObject("HandGun", pDefaultLayer, true);
	PUN::CRenderer* renderer = pObject->AddComponent<PUN::CRenderer>("renderer");
	pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(1.f, 1.f, 1.f);
	pTransform->SetWorldPos(0.f, 0.f, 15.f);
	pTransform->SetWorldRotX(-90.f);
	renderer->SetMesh("gun", TEXT("glock_OnlyFire.FBX"), MESH_PATH);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(renderer);
	SAFE_RELEASE(pObject);
	
	
	CGameObject*	pDecalObj = CGameObject::CreatePrototype("Decal");

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

	SAFE_RELEASE(pDecalObj);

	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pDefaultLayer);
	pTransform = pLandScapeObj->GetTransform();

	SAFE_RELEASE(pTransform);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", 129, 129, "LandScapeDif",
		TEXT("LandScape/Terrain_Cliff_11.dds"),
		TEXT("LandScape/Terrain_Cliff_11_NRM.bmp"),
		TEXT("LandScape/Terrain_Cliff_11_SPEC.bmp"),
		"LandScape/height2.bmp");

	//GET_SINGLE(CNavigationManager3D)->CreateNavMesh(m_pScene,
	//	"Nav.nav");

	vector<const TCHAR*> vecSplatName;

	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));

	pLandScape->AddSplatDifTexture("LandScapeSplatDif",	vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));

	pLandScape->AddSplatNrmTexture("LandScapeSplatNrm", vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecSplatName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));

	pLandScape->AddSplatSpcTexture("LandScapeSplatSpc", vecSplatName);

	vecSplatName.clear();
	vecSplatName.push_back(TEXT("LandScape/RoadAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/StonAlpha.bmp"));
	vecSplatName.push_back(TEXT("LandScape/FlowerAlpha.bmp"));

	pLandScape->AddSplatAlphaTexture("LandScapeSplatAlpha", vecSplatName);

	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLandScapeObj);

	CGameObject*	pParticleObj = CGameObject::CreatePrototype("ExplosionParticle");

	CParticle*	pParticle = pParticleObj->AddComponent<CParticle>("Particle");

	SAFE_RELEASE(pParticle);

	CColliderOBB3D*	pParticleSphere = pParticleObj->AddComponent<CColliderOBB3D>("DecalSphere");

	pParticleSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(1.f, 1.f, 1.f));

	SAFE_RELEASE(pParticleSphere);

	CMaterial*	pParticleMtrl = pParticleObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	vector<const TCHAR*>	vecExplosionName;
	for (int i = 1; i <= 89; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Explosion/Explosion%d.png"), i);
		vecExplosionName.push_back(pFileName);
	}
	pParticleMtrl->SetDiffuseTex(4, "Explosion", vecExplosionName);

	pParticleMtrl->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pParticleMtrl);

	CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");

	vector<Clip2DFrame>	vecFrame;
	for (int i = 1; i <= 89; ++i)
	{
		Clip2DFrame	tFrame = {};
		tFrame.vLT = Vector2(0.f, 0.f);
		tFrame.vRB = Vector2(1.f, 1.f);
		vecFrame.push_back(tFrame);
	}

	pParticleAnimation->AddClip("Idle", A2D_FRAME, AO_LOOP,
		1.f, vecFrame, "Explosion", vecExplosionName);

	SAFE_RELEASE(pParticleAnimation);

	for (size_t i = 0; i < vecExplosionName.size(); ++i)
	{
		SAFE_DELETE_ARRAY(vecExplosionName[i]);
	}

	vecExplosionName.clear();

	SAFE_RELEASE(pParticleObj);

	pParticleObj = CGameObject::CreateClone("ExplosionParticle",
		"ExplosionParticle", pDefaultLayer);

	pTransform = pParticleObj->GetTransform();

	pTransform->SetWorldPos(5.f, 2.f, 5.f);
	pTransform->SetWorldScale(6.f, 6.f, 1.f);

	SAFE_RELEASE(pTransform);

	SAFE_RELEASE(pParticleObj);


	SAFE_RELEASE(pCamera);

	SAFE_RELEASE(pDefaultLayer);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pTileLayer);
	SAFE_RELEASE(pBackLayer);

	return true;
}

int CFirTestScene::Update(float fTime)
{
	return 0;
}

void CFirTestScene::Move(float fScale, float fTime)
{
	if (fScale == 0.f)
		return;

	m_pTr->Move(AXIS_Y, 2.f * fScale, fTime);
}

void CFirTestScene::CreatePrototype()
{
}

void CFirTestScene::NextScene(float fTime)
{
}
