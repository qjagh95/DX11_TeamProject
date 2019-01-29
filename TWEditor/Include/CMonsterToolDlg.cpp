// CMonsterToolDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TWEditor.h"
#include "CMonsterToolDlg.h"
#include "afxdialogex.h"


// CMonsterToolDlg 대화 상자

IMPLEMENT_DYNAMIC(CMonsterToolDlg, CDialogEx)

CMonsterToolDlg::CMonsterToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MOSTERTOOL, pParent)
{

}

CMonsterToolDlg::~CMonsterToolDlg()
{
}

void CMonsterToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonsterToolDlg, CDialogEx)
END_MESSAGE_MAP()


// CMonsterToolDlg 메시지 처리기
