// ModifyPasswordDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "ModifyPasswordDlg.h"
#include "afxdialogex.h"


// CModifyPasswordDlg 对话框

IMPLEMENT_DYNAMIC(CModifyPasswordDlg, CDialogEx)

CModifyPasswordDlg::CModifyPasswordDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_MODIFY_PASSWORD, pParent)
    , m_strOldPassword("")
    , m_strNewPassword("")
{

}

CModifyPasswordDlg::~CModifyPasswordDlg()
{
}

void CModifyPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModifyPasswordDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CModifyPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyPasswordDlg 消息处理程序


void CModifyPasswordDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    CString strPassword = "";
    GetDlgItemText(IDC_EDIT_OLD_PASSWORD, m_strOldPassword);
    GetDlgItemText(IDC_EDIT_NEW_PASSWORD, m_strNewPassword);
    GetDlgItemText(IDC_EDIT_VERIFY_NEW_PASSWORD, strPassword);
    if (strPassword != m_strNewPassword)
    {
        MessageBox("两次输入的新密码不同，请重新输入");
        return;
    }
    CDialogEx::OnOK();
}


CString CModifyPasswordDlg::GetOldPassword()
{
    return m_strOldPassword;
}

CString CModifyPasswordDlg::GetNewPassword()
{
    return m_strNewPassword;
}

