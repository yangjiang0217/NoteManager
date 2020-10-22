// AddDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "AddDlg.h"
#include "afxdialogex.h"


// CAddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ADD, pParent)
    , m_strPlatform("")
    , m_strAccount("")
    , m_strPassword("")
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddDlg 消息处理程序


void CAddDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDlgItemText(IDC_EDIT_PLATFORM, m_strPlatform);
    GetDlgItemText(IDC_EDIT_ACCOUNT, m_strAccount);
    GetDlgItemText(IDC_EDIT_PASSWORD, m_strPassword);
    CDialogEx::OnOK();
}


CString CAddDlg::GetPlatform()
{
    return m_strPlatform;
}
CString CAddDlg::GetAccount()
{
    return m_strAccount;
}
CString CAddDlg::GetPassword()
{
    return m_strPassword;
}