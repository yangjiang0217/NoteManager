#pragma once


// CModifyPasswordDlg 对话框

class CModifyPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPasswordDlg)

public:
	CModifyPasswordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CModifyPasswordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODIFY_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    CString GetOldPassword();
    CString GetNewPassword();

private:
    CString m_strOldPassword;
    CString m_strNewPassword;
};
