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
	CMainFrame*	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEditorForm*	pForm = pMainFrame->GetEditorForm();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer*	pStageFrontLayer = pScene->FindLayer("Stage");

	CStage2D* pStage = pForm->GetStage();

	Vector2	vMouseWorld = GET_SINGLE(CInput)->GetMouseWorld();

	if (pStage)
	{
		pStage->SetWorldSize(Vector3(8960.f, 5040.f, 1.f));
		if (KEYPUSH("LButton"))
		{
			// 마우스의 월드위치를 얻어온다.

			pStage->SetTileOption(Vector3(vMouseWorld.x, vMouseWorld.y, 0.f),
				(TILE2D_OPTION)pForm->GetTileOption());
		}

		if (KEYPUSH("RButton"))
		{
			m_bTile = true;

			m_pTileObj = CGameObject::CreateObject("Tile2D", pStageFrontLayer);

			CRenderer* pRenderer = m_pTileObj->AddComponent<CRenderer>("TileRenderer");

			pRenderer->SetMesh("TexIso");

			SAFE_RELEASE(pRenderer);

			CMaterial* pMaterial = m_pTileObj->FindComponentFromType<CMaterial>(CT_MATERIAL);
		
			if (pForm->GetTileShape() == 0)
				pMaterial->SetDiffuseTex(0, "Tile1", TEXT("Tile/MapTile1.png"));
			else if (pForm->GetTileShape() == 1)
				pMaterial->SetDiffuseTex(0, "Tile2", TEXT("Tile/MapTile2.png"));
			else if (pForm->GetTileShape() == 2)
				pMaterial->SetDiffuseTex(0, "Tile3", TEXT("Tile/MapTile3.png"));
			else if (pForm->GetTileShape() == 3)
				pMaterial->SetDiffuseTex(0, "Tile4", TEXT("Tile/MapTile4.png"));
			else if (pForm->GetTileShape() == 4)
				pMaterial->SetDiffuseTex(0, "Tile5", TEXT("Tile/MapTile5.png"));
			else if (pForm->GetTileShape() == 5)
				pMaterial->SetDiffuseTex(0, "Tile6", TEXT("Tile/MapTile6.png"));
			else if (pForm->GetTileShape() == 6)
				pMaterial->SetDiffuseTex(0, "Tile7", TEXT("Tile/MapTile7.png"));
			else if (pForm->GetTileShape() == 7)
				pMaterial->SetDiffuseTex(0, "Tile8", TEXT("Tile/MapTile8.png"));
			else if (pForm->GetTileShape() == 8)
				pMaterial->SetDiffuseTex(0, "Tile9", TEXT("Tile/MapTile9.png"));
			else if (pForm->GetTileShape() == 9)
				pMaterial->SetDiffuseTex(0, "Tile10", TEXT("Tile/MapTile10.png"));
			else if (pForm->GetTileShape() == 10)
				pMaterial->SetDiffuseTex(0, "Tile11", TEXT("Tile/MapTile11.png"));
			else if (pForm->GetTileShape() == 11)
				pMaterial->SetDiffuseTex(0, "Tile12", TEXT("Tile/MapTile12.png"));
			else if (pForm->GetTileShape() == 12)
				pMaterial->SetDiffuseTex(0, "Tile13", TEXT("Tile/MapTile13.png"));
			else if (pForm->GetTileShape() == 13)
				pMaterial->SetDiffuseTex(0, "Tile14", TEXT("Tile/MapTile14.png"));
			else if (pForm->GetTileShape() == 14)
				pMaterial->SetDiffuseTex(0, "Tile15", TEXT("Tile/MapTile15.png"));
			else if (pForm->GetTileShape() == 15)
				pMaterial->SetDiffuseTex(0, "Tile16", TEXT("Tile/MapTile16.png"));
			
			SAFE_RELEASE(pMaterial);

			CTile2D*	pTarget = pStage->GetTile(Vector3(vMouseWorld.x, vMouseWorld.y, 0.f));

			if (!pTarget)
				return -1;			

			CTransform*	pTr = pTarget->GetTransform();
			Vector3 vTargetCenter = pTr->GetWorldPos() + pTr->GetWorldScale() * 0.5f;			
			
			m_pTileTr = m_pTileObj->GetTransform();
			m_pTileTr->SetWorldScale(128.f, 72.f, 1.f);
			//m_pTileTr->SetWorldPos(vMouseWorld.x, vMouseWorld.y, 0.f);
			m_pTileTr->SetWorldPos(vTargetCenter);
			m_pTileTr->SetWorldPivot(0.5f, 0.5f, 0.f);

		//	pStage->GetTile(Vector3(vMouseWorld.x, vMouseWorld.y, 0.f));

			/*CTransform*	pStageTr = pStage->GetTransform();

			pStageTr->GetPivot();*/
		}
	}

	SAFE_RELEASE(pStageFrontLayer);
	SAFE_RELEASE(pScene);

	return 0;
}
