// KeyDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "KeyDlg.h"
#include "afxdialogex.h"


// CKeyDlg 对话框

IMPLEMENT_DYNAMIC(CKeyDlg, CDialogEx)

CKeyDlg::CKeyDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_KEY, pParent)
    , m_strKey("")
{

}

CKeyDlg::~CKeyDlg()
{
}

void CKeyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeyDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CKeyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKeyDlg 消息处理程序


void CKeyDlg::OnBnClickedOk()
{
    GetDlgItemText(IDC_EDIT_KEY, m_strKey);

    CDialogEx::OnOK();
}

CString CKeyDlg::GetKey()
{
    return m_strKey;
}
