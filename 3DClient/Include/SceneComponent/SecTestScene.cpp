#include "SecTestScene.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/Material.h"
#include "Component/Stage2D.h"
#include "Component/UIBar.h"
#include "NavigationManager.h"
#include "Component/Sound.h"
#include "SoundManager.h"
#include "Component/Light.h"
#include "Input.h"
#include "Component/Animation.h"


CSecTestScene::CSecTestScene()
{
}


CSecTestScene::~CSecTestScene()
{
}

bool CSecTestScene::Init()
{
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(3.f, 2.f, 3.f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");

	pRenderer->SetMesh("Player", TEXT("girl1.fbx"));

	//CAnimation* pAnimation = pObject->AddComponent<CAnimation>("Animation");
	//pAnimation->Load("Monster4.anm");

	//SAFE_RELEASE(pAnimation);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pDefaultLayer);
	return true;
}

int CSecTestScene::Update(float fTime)
{
	return 0;
}

void CSecTestScene::Move(float fScale, float fTime)
{
}

void CSecTestScene::CreatePrototype()
{
}

void CSecTestScene::NextScene(float fTime)
{
}
