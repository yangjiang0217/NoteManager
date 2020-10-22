#pragma once

// CKeyDlg 对话框

class CKeyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyDlg)

public:
	CKeyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKeyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString GetKey();

private:
    CString m_strKey;
};
