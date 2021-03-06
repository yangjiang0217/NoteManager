#pragma once


// CUpdateKeyDlg 对话框

class CUpdateKeyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateKeyDlg)

public:
	CUpdateKeyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUpdateKeyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UPDATE_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    CString GetOldKey();
    CString GetNewKey();

private:
    CString m_strOldKey;
    CString m_strNewKey;
};
