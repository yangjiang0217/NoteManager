// LoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "../VerifyApi/VerifyApi.h"
#ifdef _DEBUG
#pragma comment(lib,"../lib/Debug/VerifyApi.lib")
#else
#pragma comment(lib,"../lib/Release/VerifyApi.lib")
#endif


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_LOGIN, pParent)
    , m_strUser("")
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
    CString strPassword = "";
    GetDlgItemText(IDC_EDIT_USER, m_strUser);
    GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);
    int nVerify = Verify_User(m_strUser.GetBuffer(), strPassword.GetBuffer());
    if (nVerify != 0)
    {
        MessageBox("用户名密码错误！", "登录失败", MB_ICONSTOP | MB_OK);
        return;
    }
    CDialogEx::OnOK();
}

CString CLoginDlg::GetUser()
{
    return m_strUser;
}


