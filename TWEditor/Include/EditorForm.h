#pragma once


// CEditorForm 폼 뷰
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Renderer.h"
#include "afxwin.h"

PUN_USING

class CEditorForm : public CFormView
{
	DECLARE_DYNCREATE(CEditorForm)

public:
	CEditorForm();   // 표준 생성자입니다.
	virtual ~CEditorForm();

// 대화 상자 데이터입니다.
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CString m_strTag;
	Vector3	m_vScale;
	Vector3	m_vRot;
	Vector3	m_vPos;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();

	afx_msg void OnCbnSelchangeComboAnimCombo();
	afx_msg void OnEnChangeEditScalex();
	afx_msg void OnEnChangeEditScaley();
	afx_msg void OnEnChangeEditScalez();
	afx_msg void OnEnChangeEditRotx();
	afx_msg void OnEnChangeEditRoty();
	afx_msg void OnEnChangeEditRotz();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditPosz();
	CString m_strName;
	afx_msg void OnEnChangeEditAnimname();
	int m_iStartFrame;
	afx_msg void OnEnChangeEditStartframe();
	int m_iEndFrame;
	afx_msg void OnEnChangeEditEndframe();
	float m_fPlayTime;
	afx_msg void OnEnChangeEdit();
	CListBox m_ClipList;
	afx_msg void OnLbnSelchangeListAnimlist();
	CComboBox m_OptionCombo;

	CGameObject*	m_pAnimObj;
	CAnimation*		m_pAnimation;
	CRenderer*		m_pAnimRenderer;
	afx_msg void OnBnClickedButtonSaveclip();
	afx_msg void OnBnClickedButtonLoadclip();
	afx_msg void OnBnClickedButtonSavemesh();
	afx_msg void OnBnClickedButtonLoadmesh();
	afx_msg void OnBnClickedButtonAnimadd();
	afx_msg void OnBnClickedButtonAnimmodify();
	afx_msg void OnBnClickedButtonLoadFbx();
};
