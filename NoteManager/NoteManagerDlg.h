
// NoteManagerDlg.h: 头文件
//

#pragma once
#include <memory>
#include <map>
#include "CustomListCtrl.h"
/*
*   便签数据
*/
class CNoteData
{
public:
    CNoteData(int nNoteID, const char * pPlatform, const char * pAccount, const char * pPassword)
        : m_nNoteID(nNoteID)
        , m_strPlatform(pPlatform)
        , m_strAccount(pAccount)
        , m_strPassword(pPassword)
    {
    }
    ~CNoteData() {}

public:
    int m_nNoteID;
    std::string m_strPlatform;
    std::string m_strAccount;
    std::string m_strPassword;
};
// 便签ID，便签数据
using MAP_NOTE_DATA = std::map<int, std::shared_ptr<CNoteData>>;

// CNoteManagerDlg 对话框
class CNoteManagerDlg : public CDialogEx
{
// 构造
public:
    CNoteManagerDlg(CWnd* pParent = nullptr); // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_NOTEMANAGER_DIALOG };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
    virtual void OnOK();

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedReload();
    afx_msg void OnBnClickedUpdateKey();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonDelete();

protected:
    afx_msg LRESULT OnLoadData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCustomListItemChange(WPARAM wParam, LPARAM lParam);

private:
    // 查询便签回调函数
    static int QueryNoteResult(int nNoteID, const char * pPlatform, const char * pAccount, const char * pPassword, void * pUser);
    // 便签添加到列表
    int AddNoteToMap(int nNoteID, const char * pPlatform, const char * pAccount, const char * pPassword);

private:
    CCustomListCtrl m_listNote; // 便签列表控件
    MAP_NOTE_DATA m_mapNote;    // 便签列表，缓存从数据库查询到要显示到界面的数据
};
