#pragma once


// CEditorForm 폼 뷰
#include "GameObject.h"
#include "Component/Stage2D.h"
#include "Component/Transform.h"
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
	afx_msg void OnEnChangeEditName();
	afx_msg void OnEnChangeEditScalex();
	afx_msg void OnEnChangeEditScaley();
	afx_msg void OnEnChangeEditScalez();
	afx_msg void OnEnChangeEditRotx();
	afx_msg void OnEnChangeEditRoty();
	afx_msg void OnEnChangeEditRotz();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditPosz();
	afx_msg void OnBnClickedButtonCreatestage();
	int m_iTileNumX;	
	int m_iTileNumY;
	int m_iTileSizeX;
	int m_iTileSizeY;
	CComboBox		m_TileTypeCombo;
	CGameObject*	m_pStageObj;
	CStage2D*		m_pStage;
	CTransform*		m_pStageTr;
	CGameObject*	m_pTileObj;
	CTile2D*		m_pTile;
	CTransform*		m_pTileTr;
	CImage			m_Image;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();

public:
	CGameObject*	GetStageObj()	const
	{
		return m_pStageObj;
	}

	CStage2D*	GetStage()	const
	{
		return m_pStage;
	}

	CTransform* GetStageTr()	const
	{
		return m_pStageTr;
	}

	CComboBox m_TileOptionCombo;

	int GetTileOption()	const
	{
		return m_TileOptionCombo.GetCurSel();
	}

	CComboBox m_TileShapeCombo;

	int GetTileShape()	const
	{
		return m_TileShapeCombo.GetCurSel();
	}

public:
	void SaveStage(const CString& strFullPath);
	void LoadStage(const CString& strFullPath);

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCbnSelchangeComboTileshape();
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnBnClickedButtonSelectTile();
	afx_msg void OnBnClickedPic();
	CStatic m_pPic;
};
