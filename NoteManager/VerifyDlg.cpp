// VerifyDlg.cpp: 实现文件
//

#include "pch.h"
#include "NoteManager.h"
#include "VerifyDlg.h"
#include "afxdialogex.h"
#include "../VerifyApi/VerifyApi.h"

// CVerifyDlg 对话框

IMPLEMENT_DYNAMIC(CVerifyDlg, CDialogEx)

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_VERIFY, pParent)
{

}

CVerifyDlg::~CVerifyDlg()
{
}

void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CVerifyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVerifyDlg 消息处理程序


void CVerifyDlg::OnBnClickedOk()
{
    CString strUserKey = _T("");
    GetDlgItemText(IDC_EDIT_USER_KEY, strUserKey);
    theApp.m_strKey = CStringA(strUserKey).GetBuffer();
    int nVerify = Verify_Key(theApp.m_strKey.c_str());
    if (nVerify != 0)
    {
        MessageBox(_T("密钥错误！"), _T("密钥验证"), MB_ICONSTOP | MB_OK);
        return;
    }
    CDialogEx::OnOK();
}

