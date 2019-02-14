#include "stdafx.h"
#include "EditScene.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderPixel.h"
#include "Component/Material.h"
#include "PathManager.h"
#include "Component/Sound.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Core.h"
#include "../MainFrm.h"
#include "../EditorForm.h"
#include "Component/Tile2D.h"
#include "Input.h"

CEditScene::CEditScene()
{
	m_bTile = false;
}

CEditScene::~CEditScene()
{
	if (m_bTile)
	{
		SAFE_RELEASE(m_pTileObj);
		//SAFE_RELEASE(m_pTile);
		SAFE_RELEASE(m_pTileTr);
	}
}

bool CEditScene::Init()
{

	CCamera*	pCamera = m_pScene->GetMainCamera();

	pCamera->SetCameraType(CT_ORTHO);
	pCamera->SetNear(0.f);
	//pCamera->SetWorldSize(Vector3(8960.f, 5040.f, 0.f));

	SAFE_RELEASE(pCamera);	
		
	return true;
}

int CEditScene::Input(float fTime)
{
	return 0;
}

int CEditScene::Update(float fTime)
{
	return 0;
}
