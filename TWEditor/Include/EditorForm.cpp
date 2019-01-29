// EditorForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "TWEditor.h"
#include "EditorForm.h"
#include "afxdialogex.h"
#include "TWEditorDoc.h"
#include "GameObject.h"
#include "Component/Stage2D.h"
#include "Component/Tile2D.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourcesManager.h"
#include "Input.h"

// CEditorForm

IMPLEMENT_DYNCREATE(CEditorForm, CFormView)

CEditorForm::CEditorForm()
	: CFormView(IDD_DIALOG_EDIT)
	, m_strTag(_T(""))
	, m_vScale(0)
	, m_vRot(0)
	, m_vPos(0)
	, m_iTileNumX(0)
	, m_iTileNumY(0)
	, m_iTileSizeX(0)
	, m_iTileSizeY(0)
	, m_pStageObj(nullptr)
	, m_pStage(nullptr)
	, m_pStageTr(nullptr)
{

}

CEditorForm::~CEditorForm()
{
	SAFE_RELEASE(m_pStageObj);
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pStageTr);/*
	SAFE_RELEASE(m_pTileObj);
	SAFE_RELEASE(m_pTile);
	SAFE_RELEASE(m_pTileTr);*/
}

void CEditorForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTag);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_vRot.x);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_vRot.y);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_vRot.z);
	DDX_Text(pDX, IDC_EDIT_POSX, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POSY, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_vPos.z);
	DDX_Control(pDX, IDC_COMBO_TILETYPE, m_TileTypeCombo);
	DDX_Text(pDX, IDC_EDIT_TILENUMX, m_iTileNumX);
	DDX_Text(pDX, IDC_EDIT_TILENUMY, m_iTileNumY);
	DDX_Text(pDX, IDC_EDIT_TILESIZEX, m_iTileSizeX);
	DDX_Text(pDX, IDC_EDIT_TILESIZEY, m_iTileSizeY);
	DDX_Control(pDX, IDC_COMBO_TILEOPTION, m_TileOptionCombo);
	DDX_Control(pDX, IDC_COMBO_TILESHAPE, m_TileShapeCombo);
	//DDX_Control(pDX, IDC_STATIC, m_pPicture);
	DDX_Control(pDX, IDC_PIC, m_pPic);
}


BEGIN_MESSAGE_MAP(CEditorForm, CFormView)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CEditorForm::OnEnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, &CEditorForm::OnEnChangeEditScalex)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, &CEditorForm::OnEnChangeEditScaley)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, &CEditorForm::OnEnChangeEditScalez)
	ON_EN_CHANGE(IDC_EDIT_ROTX, &CEditorForm::OnEnChangeEditRotx)
	ON_EN_CHANGE(IDC_EDIT_ROTY, &CEditorForm::OnEnChangeEditRoty)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, &CEditorForm::OnEnChangeEditRotz)
	ON_EN_CHANGE(IDC_EDIT_POSX, &CEditorForm::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSY, &CEditorForm::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &CEditorForm::OnEnChangeEditPosz)
	ON_BN_CLICKED(IDC_BUTTON_CREATESTAGE, &CEditorForm::OnBnClickedButtonCreatestage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEditorForm::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditorForm::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_TILESHAPE, &CEditorForm::OnCbnSelchangeComboTileshape)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TILE, &CEditorForm::OnBnClickedButtonSelectTile)
	ON_BN_CLICKED(IDC_PIC, &CEditorForm::OnBnClickedPic)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CEditorForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditorForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
// CEditorForm 메시지 처리기

void CEditorForm::OnEnChangeEditName()
{
}

void CEditorForm::OnEnChangeEditScalex()
{
}

void CEditorForm::OnEnChangeEditScaley()
{
}

void CEditorForm::OnEnChangeEditScalez()
{
}

void CEditorForm::OnEnChangeEditRotx()
{
}

void CEditorForm::OnEnChangeEditRoty()
{
}

void CEditorForm::OnEnChangeEditRotz()
{
}

void CEditorForm::OnEnChangeEditPosx()
{
}

void CEditorForm::OnEnChangeEditPosy()
{
}

void CEditorForm::OnEnChangeEditPosz()
{
}

int CEditorForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CEditorForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TileTypeCombo.SetCurSel(0);

	m_TileOptionCombo.AddString(TEXT("일반타일"));
	m_TileOptionCombo.AddString(TEXT("이동불가타일"));

	m_TileShapeCombo.AddString(TEXT("CDTile1"));
	m_TileShapeCombo.AddString(TEXT("CDTile2"));
	m_TileShapeCombo.AddString(TEXT("CDTile3"));
	m_TileShapeCombo.AddString(TEXT("CDTile4"));
	m_TileShapeCombo.AddString(TEXT("CDTile5"));
	m_TileShapeCombo.AddString(TEXT("CDTile6"));
	m_TileShapeCombo.AddString(TEXT("CDTile7"));
	m_TileShapeCombo.AddString(TEXT("CDTile8"));
	m_TileShapeCombo.AddString(TEXT("CDTile9"));
	m_TileShapeCombo.AddString(TEXT("CDTile10"));
	m_TileShapeCombo.AddString(TEXT("CDTile11"));
	m_TileShapeCombo.AddString(TEXT("CDTile12"));
	m_TileShapeCombo.AddString(TEXT("CDTile13"));
	m_TileShapeCombo.AddString(TEXT("CDTile14"));
	m_TileShapeCombo.AddString(TEXT("CDTile15"));
	m_TileShapeCombo.AddString(TEXT("CDTile16"));
	
	m_TileShapeCombo.AddString(TEXT("CDXTile1"));
	m_TileShapeCombo.AddString(TEXT("CDXTile2"));

	m_TileOptionCombo.SetCurSel(0);
	m_TileShapeCombo.SetCurSel(0);
}

void CEditorForm::OnBnClickedButtonCreatestage()
{
	if (m_pStageObj)
		return;

	UpdateData(TRUE);

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer*	pStageLayer = pScene->FindLayer("Stage_Front");

	m_pStageObj = CGameObject::CreateObject("StageObj", pStageLayer);

	CRenderer* pRenderer = m_pStageObj->AddComponent<CRenderer>("StageRenderer");

	pRenderer->SetMesh("TexRect");

	SAFE_RELEASE(pRenderer);

	CMaterial* pMaterial = m_pStageObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Stage3", TEXT("ChristMasDungeon.png"));

	SAFE_RELEASE(pMaterial);

	m_pStage = m_pStageObj->AddComponent<CStage2D>("Stage");

	int	iTileType = m_TileTypeCombo.GetCurSel();

	Vector3	vTileScale = Vector3((float)m_iTileSizeX, (float)m_iTileSizeY, 1.f);

	m_pStage->CreateTile(m_iTileNumX, m_iTileNumY, Vector3::Zero, vTileScale,
		(STAGE2D_TILE_TYPE)iTileType);

	m_pStageTr = m_pStageObj->GetTransform();	

	SAFE_RELEASE(pStageLayer);
	SAFE_RELEASE(pScene);

	UpdateData(FALSE);
}

void CEditorForm::SaveStage(const CString & strFullPath)
{
	// string으로 변환한다
	string	strPath = CT2CA(strFullPath);

	FILE*	pFile = nullptr;

	fopen_s(&pFile, strPath.c_str(), "wb");

	if (!pFile)
		return;

	m_pStage->Save(pFile);

	fclose(pFile);
}

void CEditorForm::LoadStage(const CString & strFullPath)
{
	// string으로 변환한다.
	string	strPath = CT2CA(strFullPath);

	FILE*	pFile = nullptr;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
		return;

	if (m_pStageObj)
		m_pStageObj->Die();

	SAFE_RELEASE(m_pStageObj);
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pStageTr);

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CLayer*	pStageLayer = pScene->FindLayer("Stage");

	m_pStageObj = CGameObject::CreateObject("StageObj", pStageLayer);

	CRenderer* pRenderer = m_pStageObj->AddComponent<CRenderer>("StageRenderer");

	pRenderer->SetMesh("TexRect");

	SAFE_RELEASE(pRenderer);

	CMaterial* pMaterial = m_pStageObj->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Stage3", TEXT("ChristMasDungeon.png"));

	SAFE_RELEASE(pMaterial);

	m_pStage = m_pStageObj->AddComponent<CStage2D>("Stage");

	m_pStage->Load(pFile);

	m_pStageTr = m_pStageObj->GetTransform();

	SAFE_RELEASE(pStageLayer);
	SAFE_RELEASE(pScene);

	m_TileTypeCombo.SetCurSel(m_pStage->GetTileType());

	m_iTileNumX = m_pStage->GetTileNumX();
	m_iTileNumY = m_pStage->GetTileNumY();
	m_iTileSizeX = m_pStage->GetTileScale().x;
	m_iTileSizeY = m_pStage->GetTileScale().y;

	UpdateData(FALSE);

	fclose(pFile);
}

void CEditorForm::OnBnClickedButtonLoad()
{
	const TCHAR*	pFilter = TEXT("StageFile(*.stg)|*.stg|모든파일(*.*)|*.*||");
	CFileDialog	dlg(TRUE, TEXT(".stg"), nullptr, OFN_HIDEREADONLY, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다.
		CString	strPath = dlg.GetPathName();

		LoadStage(strPath);
	}
}


void CEditorForm::OnBnClickedButtonSave()
{
	if (!m_pStageObj)
	{
		AfxMessageBox(TEXT("스테이지가 없다."));
		return;
	}

	const TCHAR*	pFilter = TEXT("StageFile(*.stg)|*.stg|모든파일(*.*)|*.*||");
	CFileDialog	dlg(FALSE, TEXT(".stg"), nullptr, OFN_OVERWRITEPROMPT, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다.
		CString	strPath = dlg.GetPathName();

		SaveStage(strPath);
	}
}

void CEditorForm::OnCbnSelchangeComboTileshape()
{

}

void CEditorForm::OnDraw(CDC* pDC)
{
	CImage  Img;
	
	if (m_TileShapeCombo.GetCurSel() == 0)
	{
		auto r = Img.Load(L"MapTile1.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 1)
	{
		auto r = Img.Load(L"MapTile2.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 2)
	{
		auto r = Img.Load(L"MapTile3.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 3)
	{
		auto r = Img.Load(L"MapTile4.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 4)
	{
		auto r = Img.Load(L"MapTile5.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 5)
	{
		auto r = Img.Load(L"MapTile6.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 6)
	{
		auto r = Img.Load(L"MapTile7.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 7)
	{
		auto r = Img.Load(L"MapTile8.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 8)
	{
		auto r = Img.Load(L"MapTile9.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 9)
	{
		auto r = Img.Load(L"MapTile10.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 10)
	{
		auto r = Img.Load(L"MapTile11.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 11)
	{
		auto r = Img.Load(L"MapTile12.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 12)
	{
		auto r = Img.Load(L"MapTile13.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 13)
	{
		auto r = Img.Load(L"MapTile14.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 14)
	{
		auto r = Img.Load(L"MapTile15.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
	else if (m_TileShapeCombo.GetCurSel() == 15)
	{
		auto r = Img.Load(L"MapTile16.png");
		Img.Draw(pDC->m_hDC, 340, 430);
	}
}

void CEditorForm::OnBnClickedButtonSelectTile()
{
	CDC* pDC;
	pDC = GetDC();
	OnDraw(pDC);
}


void CEditorForm::OnBnClickedPic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
