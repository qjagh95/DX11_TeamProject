#pragma once


// CMonsterToolDlg 대화 상자

class CMonsterToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonsterToolDlg)

public:
	CMonsterToolDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMonsterToolDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MOSTERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
