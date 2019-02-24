#include "../ClientHeader.h"
#include "TrdTestScene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Animation.h"

CTrdTestScene::CTrdTestScene()
{
}


CTrdTestScene::~CTrdTestScene()
{
}

bool CTrdTestScene::Init()
{
	CLayer* pDefaultLayer = m_pScene->FindLayer("Default");

	CGameObject* pObject = CGameObject::CreateObject("Pyramid", pDefaultLayer);
	CTransform*	pTransform = pObject->GetTransform();

	pTransform->SetWorldScale(0.3f, 0.3f, 0.1f);
	pTransform->SetWorldPos(0.f, 0.f, 0.f);

	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Render");

	pRenderer->SetMesh("Player", TEXT("girl1.fbx"));

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pDefaultLayer);

	return true;
}

int CTrdTestScene::Update(float fTime)
{
	return 0;
}

void CTrdTestScene::Move(float fScale, float fTime)
{
}

void CTrdTestScene::CreatePrototype()
{
}

void CTrdTestScene::NextScene(float fTime)
{
}

