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
	, m_strName(_T(""))
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_fPlayTime(0)
	, m_pAnimObj(nullptr)
	, m_pAnimation(nullptr)
	, m_pAnimRenderer(nullptr)
	, m_bClipDivide(FALSE)
{

}

CEditorForm::~CEditorForm()
{
	SAFE_RELEASE(m_pAnimObj);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pAnimRenderer);
}

void CEditorForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_vRot.x);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_vRot.y);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_vRot.z);
	DDX_Text(pDX, IDC_EDIT_POSX, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POSY, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT_ANIMNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_STARTFRAME, m_iStartFrame);
	DDX_Text(pDX, IDC_EDIT_ENDFRAME, m_iEndFrame);
	DDX_Text(pDX, IDC_EDIT__ANIMPLAYTIME, m_fPlayTime);
	DDX_Control(pDX, IDC_LIST_ANIMLIST, m_ClipList);
	DDX_Control(pDX, IDC_COMBO_ANIMOPTION, m_OptionCombo);
	DDX_Check(pDX, IDC_CHECK_CLIPDIVIDE, m_bClipDivide);
}


BEGIN_MESSAGE_MAP(CEditorForm, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMOPTION, &CEditorForm::OnCbnSelchangeComboAnimCombo)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, &CEditorForm::OnEnChangeEditScalex)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, &CEditorForm::OnEnChangeEditScaley)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, &CEditorForm::OnEnChangeEditScalez)
	ON_EN_CHANGE(IDC_EDIT_ROTX, &CEditorForm::OnEnChangeEditRotx)
	ON_EN_CHANGE(IDC_EDIT_ROTY, &CEditorForm::OnEnChangeEditRoty)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, &CEditorForm::OnEnChangeEditRotz)
	ON_EN_CHANGE(IDC_EDIT_POSX, &CEditorForm::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSY, &CEditorForm::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &CEditorForm::OnEnChangeEditPosz)
	ON_EN_CHANGE(IDC_EDIT_ANIMNAME, &CEditorForm::OnEnChangeEditAnimname)
	ON_EN_CHANGE(IDC_EDIT_STARTFRAME, &CEditorForm::OnEnChangeEditStartframe)
	ON_EN_CHANGE(IDC_EDIT_ENDFRAME, &CEditorForm::OnEnChangeEditEndframe)
	ON_EN_CHANGE(IDC_EDIT__ANIMPLAYTIME, &CEditorForm::OnEnChangeEdit)
	ON_LBN_SELCHANGE(IDC_LIST_ANIMLIST, &CEditorForm::OnLbnSelchangeListAnimlist)
	ON_BN_CLICKED(IDC_BUTTON_SAVECLIP, &CEditorForm::OnBnClickedButtonSaveclip)
	ON_BN_CLICKED(IDC_BUTTON_LOADCLIP, &CEditorForm::OnBnClickedButtonLoadclip)
	ON_BN_CLICKED(IDC_BUTTON_SAVEMESH, &CEditorForm::OnBnClickedButtonSavemesh)
	ON_BN_CLICKED(IDC_BUTTON_LOADMESH, &CEditorForm::OnBnClickedButtonLoadmesh)
	ON_BN_CLICKED(IDC_BUTTON_ANIMADD, &CEditorForm::OnBnClickedButtonAnimadd)
	ON_BN_CLICKED(IDC_BUTTON_ANIMMODIFY, &CEditorForm::OnBnClickedButtonAnimmodify)
	ON_BN_CLICKED(IDC_BUTTON__LOADFBX, &CEditorForm::OnBnClickedButtonLoadFbx)	
	ON_BN_CLICKED(IDC_CHECK_CLIPDIVIDE, &CEditorForm::OnBnClickedCheckClipdivide)
	ON_BN_CLICKED(IDC_BUTTON_SENDCLIENT, &CEditorForm::OnBnClickedButtonSendclient)
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

int CEditorForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEditorForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}


void CEditorForm::OnCbnSelchangeComboAnimCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnEnChangeEditScalex()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vScale = pTr->GetWorldScale();
	vScale.x = m_vScale.x;

	pTr->SetWorldScale(vScale);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditScaley()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vScale = pTr->GetWorldScale();
	vScale.y = m_vScale.y;

	pTr->SetWorldScale(vScale);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditScalez()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vScale = pTr->GetWorldScale();
	vScale.z = m_vScale.z;

	pTr->SetWorldScale(vScale);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditRotx()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	pTr->SetWorldRotX(m_vRot.x);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditRoty()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	pTr->SetWorldRotY(m_vRot.y);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditRotz()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	pTr->SetWorldRotZ(m_vRot.z);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditPosx()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vPos = pTr->GetWorldPos();
	vPos.x = m_vPos.x;
	pTr->SetWorldPos(vPos);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditPosy()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vPos = pTr->GetWorldPos();
	vPos.y = m_vPos.y;
	pTr->SetWorldPos(vPos);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditPosz()
{
	UpdateData(TRUE);

	CTransform*	pTr = m_pAnimObj->GetTransform();

	Vector3 vPos = pTr->GetWorldPos();
	vPos.z = m_vPos.z;
	pTr->SetWorldPos(vPos);

	SAFE_RELEASE(pTr);
}


void CEditorForm::OnEnChangeEditAnimname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnEnChangeEditStartframe()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnEnChangeEditEndframe()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnEnChangeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnLbnSelchangeListAnimlist()
{
	if (m_ClipList.GetCurSel() == -1)
	{
		AfxMessageBox(TEXT("클립을 선택하세요"));
		return;
	}

	int	iIndex = m_ClipList.GetCurSel();

	CString	strName;
	m_ClipList.GetText(iIndex, strName);

	char	strOriginName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, strName.GetString(), -1,
		strOriginName, strName.GetLength(), 0, 0);

	PANIMATIONCLIP	pClip = m_pAnimation->FindClip(strOriginName);

	if (!pClip)
		return;

	m_strName = strName;
	m_iStartFrame = pClip->iStartFrame;
	m_iEndFrame = pClip->iEndFrame;
	m_fPlayTime = pClip->fPlayTime;
	m_OptionCombo.SetCurSel(pClip->eOption);

	m_pAnimation->ChangeClip(strOriginName);

	UpdateData(FALSE);
}


void CEditorForm::OnBnClickedButtonSaveclip()
{
	if (!m_pAnimation)
		return;

	const TCHAR*	pFilter = TEXT("ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||");
	CFileDialog	dlg(FALSE, TEXT(".anm"), nullptr, OFN_OVERWRITEPROMPT, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다.
		CString	strPath = dlg.GetPathName();

		m_pAnimation->SaveFromFullPath(strPath.GetString());
	}
}


void CEditorForm::OnBnClickedButtonLoadclip()
{
	const TCHAR*	pFilter = TEXT("ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||");
	CFileDialog	dlg(TRUE, TEXT(".anm"), nullptr, OFN_HIDEREADONLY, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다
		CString strPath = dlg.GetPathName();

		if (!m_pAnimObj)
		{
			CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
			CLayer*	pLayer = pScene->FindLayer("Default");

			m_pAnimObj = CGameObject::CreateObject("AnimObj",
				pLayer);

			SAFE_RELEASE(pScene);
			SAFE_RELEASE(pLayer);
		}


		if (!m_pAnimation)
			m_pAnimation = m_pAnimObj->AddComponent<CAnimation>("Animation");

		int	iLength = strPath.GetLength();

		if (strPath[iLength - 3] == 'f' &&
			strPath[iLength - 2] == 'b' &&
			strPath[iLength - 1] == 'x')
			m_pAnimation->AddClip(strPath.GetString());

		else
			m_pAnimation->LoadBoneAndAnimationFullPath(strPath);

		const list<string>*	pClipNameList = m_pAnimation->GetAddClipName();

		PANIMATIONCLIP	pClip = m_pAnimation->GetCurrentClip();

		m_strName = CA2CT(pClip->strName.c_str());
		m_iStartFrame = pClip->iStartFrame;
		m_iEndFrame = pClip->iEndFrame;
		m_fPlayTime = 1.f;

		m_OptionCombo.SetCurSel(pClip->eOption);

		list<string>::const_iterator	iter;
		list<string>::const_iterator	iterEnd = pClipNameList->end();

		for (iter = pClipNameList->begin(); iter != iterEnd; ++iter)
		{
			CString	strClipName = CA2CT((*iter).c_str());
			m_ClipList.AddString(strClipName);
		}

		UpdateData(FALSE);
	}
}


void CEditorForm::OnBnClickedButtonSavemesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditorForm::OnBnClickedButtonLoadmesh()
{
	const TCHAR*	pFilter = TEXT("MeshFile(*.msh)|*.msh|모든파일(*.*)|*.*||");
	CFileDialog	dlg(TRUE, TEXT(".msh"), nullptr, OFN_HIDEREADONLY, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다.
		CString	strPath = dlg.GetPathName();

		// GameObject를 생성하고 fbx를 로드해서 출력한다.
		if (!m_pAnimObj)
		{
			CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
			CLayer*	pLayer = pScene->FindLayer("Default");

			m_pAnimObj = CGameObject::CreateObject("AnimObj",
				pLayer);

			SAFE_RELEASE(pScene);
			SAFE_RELEASE(pLayer);
		}

		if (!m_pAnimRenderer)
			m_pAnimRenderer = m_pAnimObj->AddComponent<CRenderer>("AnimRenderer");

		m_pAnimRenderer->SetMeshFromFullPath("AnimObj", strPath.GetString());

	}
}


void CEditorForm::OnBnClickedButtonAnimadd()
{
	if (!m_pAnimation)
		return;

	if (m_bClipDivide)
	{
		UpdateData(TRUE);

		char	strOriginName[256] = {};

		WideCharToMultiByte(CP_ACP, 0, m_strName.GetString(), -1,
			strOriginName, m_strName.GetLength(), 0, 0);

		m_pAnimation->AddClip(strOriginName,
			(ANIMATION_OPTION)m_OptionCombo.GetCurSel(),
			m_iStartFrame, m_iEndFrame, m_fPlayTime, m_vecDivideKeyFrame);

		m_ClipList.AddString(m_strName);
	}
}


void CEditorForm::OnBnClickedButtonAnimmodify()
{
	if (m_ClipList.GetCurSel() == -1)
	{
		AfxMessageBox(TEXT("클립을 선택하세요"));
		return;
	}

	else if (!m_pAnimation)
		return;

	if (m_bClipDivide)
	{
		int	iIndex = m_ClipList.GetCurSel();

		CString	strName;
		m_ClipList.GetText(iIndex, strName);

		UpdateData(TRUE);

		char	strOriginName[256] = {};

		WideCharToMultiByte(CP_ACP, 0, strName.GetString(), -1,
			strOriginName, strName.GetLength(), 0, 0);

		char	strChangeName[256] = {};

		WideCharToMultiByte(CP_ACP, 0, m_strName.GetString(), -1,
			strChangeName, m_strName.GetLength(), 0, 0);

		m_pAnimation->ModifyClip(strOriginName, strChangeName,
			(ANIMATION_OPTION)m_OptionCombo.GetCurSel(),
			m_iStartFrame, m_iEndFrame, m_fPlayTime, m_vecDivideKeyFrame);

		// ListBox의 이름을 변경해주면 된다.
		m_ClipList.DeleteString(iIndex);
		m_ClipList.AddString(m_strName);
	}
}



void CEditorForm::OnBnClickedButtonLoadFbx()
{
	const TCHAR*	pFilter = TEXT("FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||");
	CFileDialog	dlg(TRUE, TEXT(".fbx"), nullptr, OFN_HIDEREADONLY, pFilter);

	if (dlg.DoModal() == IDOK)
	{
		// 전체 경로를 얻어온다.
		CString	strPath = dlg.GetPathName();

		// GameObject를 생성하고 fbx를 로드해서 출력한다.
		SAFE_RELEASE(m_pAnimObj);
		SAFE_RELEASE(m_pAnimation);
		SAFE_RELEASE(m_pAnimRenderer);

		CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
		CLayer*	pLayer = pScene->FindLayer("Default");

		m_pAnimObj = CGameObject::CreateObject("AnimObj",
			pLayer);

		m_pAnimRenderer = m_pAnimObj->AddComponent<CRenderer>("AnimRenderer");

		m_pAnimRenderer->SetMeshFromFullPath("AnimObj", strPath.GetString());

		m_pAnimation = m_pAnimObj->FindComponentFromType<CAnimation>(CT_ANIMATION);

		PANIMATIONCLIP	pClip = m_pAnimation->GetCurrentClip();

		m_strName = CA2CT(pClip->strName.c_str());
		m_iStartFrame = pClip->iStartFrame;
		m_iEndFrame = pClip->iEndFrame;
		m_fPlayTime = 1.f;

		m_OptionCombo.SetCurSel(pClip->eOption);

		m_ClipList.AddString(m_strName);

		UpdateData(FALSE);

		SAFE_RELEASE(pScene);
		SAFE_RELEASE(pLayer);
	}
}

void CEditorForm::OnBnClickedCheckClipdivide()
{
	if (!m_pAnimation)
	{
		m_bClipDivide = FALSE;

		UpdateData(FALSE);
		return;
	}

	UpdateData(TRUE);

	if (m_bClipDivide)
	{
		m_pAnimation->GetCurrentKeyFrame(m_vecDivideKeyFrame);
	}

	else
	{
		Safe_Delete_VecList(m_vecDivideKeyFrame);
	}
}

void CEditorForm::OnBnClickedButtonSendclient()
{
	ShellExecuteA(nullptr, "open", "EditorCopy.bat", nullptr,
		nullptr, SW_SHOW);
}
