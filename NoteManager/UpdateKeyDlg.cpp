// UpdateKeyDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "UpdateKeyDlg.h"
#include "afxdialogex.h"


// CUpdateKeyDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateKeyDlg, CDialogEx)

CUpdateKeyDlg::CUpdateKeyDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_UPDATE_KEY, pParent)
    , m_strOldKey("")
    , m_strNewKey("")
{

}

CUpdateKeyDlg::~CUpdateKeyDlg()
{
}

void CUpdateKeyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateKeyDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CUpdateKeyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUpdateKeyDlg 消息处理程序


void CUpdateKeyDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    CString strKey = _T("");
    GetDlgItemText(IDC_EDIT_OLD_KEY, m_strOldKey);
    GetDlgItemText(IDC_EDIT_NEW_KEY, m_strNewKey);
    GetDlgItemText(IDC_EDIT_VERIFY_NEW_KEY, strKey);
    if (strKey != m_strNewKey)
    {
        MessageBox(_T("两次输入的新密钥不同，请重新输入"));
        return;
    }
    else if (m_strNewKey == m_strOldKey)
    {
        MessageBox(_T("新密钥和旧密钥相同，请重新输入"));
        return;
    }
    CDialogEx::OnOK();
}


CString CUpdateKeyDlg::GetOldKey()
{
    return m_strOldKey;
}

CString CUpdateKeyDlg::GetNewKey()
{
    return m_strNewKey;
}

