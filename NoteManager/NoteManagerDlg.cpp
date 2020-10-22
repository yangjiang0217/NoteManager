
// NoteManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NoteManager.h"
#include "NoteManagerDlg.h"
#include "afxdialogex.h"
#include <string>
#include "KeyDlg.h"
#include "ModifyPasswordDlg.h"
#include "AddDlg.h"
#include "../VerifyApi/VerifyApi.h"
#include "../CryptoApi/CryptoApi.h"
#include "../DatabaseApi/DatabaseApi.h"
#ifdef _DEBUG
#pragma comment(lib,"../lib/Debug/CryptoApi.lib")
#pragma comment(lib,"../lib/Debug/VerifyApi.lib")
#pragma comment(lib,"../lib/Debug/DatabaseApi.lib")
#else
#pragma comment(lib,"../lib/Release/CryptoApi.lib")
#pragma comment(lib,"../lib/Release/VerifyApi.lib")
#pragma comment(lib,"../lib/Release/DatabaseApi.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const unsigned int WM_LOAD＿DATA = WM_USER + 0x101;  // 加载数据消息

const unsigned int NOTE_COLUMN_ID = 0;
const unsigned int NOTE_COLUMN_PLATFORM = 1;
const unsigned int NOTE_COLUMN_ACCOUNT = 2;
const unsigned int NOTE_COLUMN_PASSWORD = 3;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNoteManagerDlg 对话框



CNoteManagerDlg::CNoteManagerDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_NOTEMANAGER_DIALOG, pParent)
    , m_strKey("")
    , m_strUser("")
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNoteManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_NOTE, m_listNote);
}

BEGIN_MESSAGE_MAP(CNoteManagerDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_MESSAGE(WM_LOAD＿DATA, &CNoteManagerDlg::OnLoadData)
    ON_MESSAGE(WM_CUSTOM_LIST_ITEM_CHANGE, &CNoteManagerDlg::OnCustomListItemChange)
    ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CNoteManagerDlg::OnBnClickedButtonReload)
    ON_BN_CLICKED(IDC_BUTTON_MODIFY_PASSWORD, &CNoteManagerDlg::OnBnClickedButtonModifyPassword)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CNoteManagerDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CNoteManagerDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CNoteManagerDlg 消息处理程序

BOOL CNoteManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // 初始化列表框
    RECT rcBtn = { 0 };
    CWnd *pWnd = GetDlgItem(IDC_BUTTON_RELOAD);
    if (pWnd)
    {
        pWnd->GetWindowRect(&rcBtn);
    }
    ScreenToClient(&rcBtn);
    RECT rcClient = { 0 };
    GetClientRect(&rcClient);
    rcClient.top += rcBtn.bottom + rcBtn.top;
    m_listNote.MoveWindow(&rcClient);
    DWORD dwExStyle = m_listNote.GetExtendedStyle();
    dwExStyle |= LVS_EX_GRIDLINES;
    m_listNote.SetExtendedStyle(dwExStyle);
    RECT rcList = { 0 };
    m_listNote.GetClientRect(&rcList);
    int nWidth = rcList.right - rcList.left - 25;
    m_listNote.InsertColumn(NOTE_COLUMN_ID, "ID", LVCFMT_LEFT, int(nWidth*0.1));
    m_listNote.InsertColumn(NOTE_COLUMN_PLATFORM, "平台", LVCFMT_LEFT, int(nWidth*0.2));
    m_listNote.InsertColumn(NOTE_COLUMN_ACCOUNT, "账号", LVCFMT_LEFT, int(nWidth*0.35));
    m_listNote.InsertColumn(NOTE_COLUMN_PASSWORD, "密码", LVCFMT_LEFT, int(nWidth*0.35));
    // 连接数据库
    int nRet = DBM_Connect();
    if (nRet != 0)
    {
        MessageBox("连接数据库失败");
        PostMessage(WM_QUIT);
        return TRUE;
    }
    // 发送加载数据消息
    PostMessage(WM_LOAD＿DATA);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNoteManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
    CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNoteManagerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNoteManagerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CNoteManagerDlg::OnLoadData(WPARAM wParam, LPARAM lParam)
{
    DBM_QueryNote(QueryNoteResult, this);
    if (m_strKey.IsEmpty())
    {
        CKeyDlg dlgKey;
        dlgKey.DoModal();
        m_strKey = dlgKey.GetKey();
    }
    int nItem = 0;
    char szTmp[128] = { 0 };
    m_listNote.LockWindowUpdate();
    m_listNote.DeleteAllItems();
    for (auto itr : m_mapNote)
    {
        m_listNote.InsertItem(nItem, std::to_string(itr.first).c_str());
        m_listNote.SetItemText(nItem, NOTE_COLUMN_PLATFORM, itr.second->m_strPlatform.c_str());
        memset(szTmp, 0, sizeof(szTmp));
        CRYPTO_DecryptString(itr.second->m_strAccount.c_str(), m_strKey.GetBuffer(), szTmp, sizeof(szTmp));
        m_listNote.SetItemText(nItem, NOTE_COLUMN_ACCOUNT, szTmp);
        memset(szTmp, 0, sizeof(szTmp));
        CRYPTO_DecryptString(itr.second->m_strPassword.c_str(), m_strKey.GetBuffer(), szTmp, sizeof(szTmp));
        m_listNote.SetItemText(nItem, NOTE_COLUMN_PASSWORD, szTmp);
        nItem++;
    }
    m_mapNote.clear();
    m_listNote.UnlockWindowUpdate();
    return 0;
}


// 查询便签回调函数
int CNoteManagerDlg::QueryNoteResult(int nNoteID, const char * pPlatform, const char * pAccount, const char * pPassword, void * pUser)
{
    CNoteManagerDlg *pThis = static_cast<CNoteManagerDlg*>(pUser);
    if (NULL != pThis)
    {
        pThis->AddNoteToMap(nNoteID, pPlatform, pAccount, pPassword);
    }
    return 0;
}


// 便签添加到列表
int CNoteManagerDlg::AddNoteToMap(int nNoteID, const char * pPlatform, const char * pAccount, const char * pPassword)
{
    if (NULL == pPlatform || NULL == pAccount || NULL == pPassword)
    {
        return 0;
    }
    auto pNote = std::make_shared<CNoteData>(nNoteID, pPlatform, pAccount, pPassword);
    if (NULL == pNote)
    {
        return 0;
    }
    m_mapNote.insert(std::make_pair(nNoteID, pNote));
    return 0;
}


void CAboutDlg::OnClose()
{
    // 断开数据库连接
    DBM_Disconnect();

    CDialogEx::OnClose();
}

void CNoteManagerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // 调整列表框
    if (NULL == m_listNote.GetSafeHwnd())
    {
        return;
    }
    RECT rcClient = { 0 };
    GetClientRect(&rcClient);
    RECT rcBtn = { 0 };
    CWnd *pWnd = GetDlgItem(IDC_BUTTON_RELOAD);
    if (pWnd)
    {
        pWnd->GetWindowRect(&rcBtn);
    }
    ScreenToClient(&rcBtn);
    rcClient.top += rcBtn.bottom + rcBtn.top;
    m_listNote.MoveWindow(&rcClient);
    int nWidth = rcClient.right - rcClient.left - 25;
    m_listNote.SetColumnWidth(NOTE_COLUMN_ID, int(nWidth*0.1));
    m_listNote.SetColumnWidth(NOTE_COLUMN_PLATFORM, int(nWidth*0.2));
    m_listNote.SetColumnWidth(NOTE_COLUMN_ACCOUNT, int(nWidth*0.35));
    m_listNote.SetColumnWidth(NOTE_COLUMN_PASSWORD, int(nWidth*0.35));
}


afx_msg LRESULT CNoteManagerDlg::OnCustomListItemChange(WPARAM wParam, LPARAM lParam)
{
    CString strData = m_listNote.GetItemText(wParam, lParam);
    int nNoteID = atoi(m_listNote.GetItemText(wParam, NOTE_COLUMN_ID));
    char szTmp[128] = { 0 };
    // 平台字段不加密
    if (lParam != NOTE_COLUMN_PLATFORM)
    {
        CRYPTO_EncryptString(strData.GetBuffer(), m_strKey.GetBuffer(), szTmp, sizeof(szTmp));
    }
    int nRet = 0;
    switch (lParam)
    {
    case NOTE_COLUMN_PLATFORM:
        nRet = DBM_UpdatePlatform(nNoteID, strData.GetBuffer());
        break;
    case NOTE_COLUMN_ACCOUNT:
        nRet = DBM_UpdateAccount(nNoteID, szTmp);
        break;
    case NOTE_COLUMN_PASSWORD:
        nRet = DBM_UpdatePassword(nNoteID, szTmp);
        break;
    default:
        break;
    }
    if (nRet != 0)
    {
        MessageBox("数据保存失败", "数据保存", MB_OK | MB_ICONWARNING);
    }
    return 0;
}


void CNoteManagerDlg::OnBnClickedButtonReload()
{
    PostMessage(WM_LOAD＿DATA);
}

void CNoteManagerDlg::SetUser(CString strUser)
{
    m_strUser = strUser;
}

void CNoteManagerDlg::OnBnClickedButtonModifyPassword()
{
    CModifyPasswordDlg dlg;
    int nRet = dlg.DoModal();
    if (nRet != IDOK)
    {
        return;
    }
    CString strOldPassword = dlg.GetOldPassword();
    CString strNewPassword = dlg.GetNewPassword();
    nRet = Verify_ModifyPassword(m_strUser.GetBuffer(), strOldPassword.GetBuffer(), strNewPassword.GetBuffer());
    if (nRet != 0)
    {
        MessageBox("密码修改失败", "密码修改", MB_OK | MB_ICONWARNING);
        return;
    }
    MessageBox("密码修改成功", "密码修改", MB_OK | MB_ICONINFORMATION);
}


void CNoteManagerDlg::OnOK()
{
    // 让列表编辑框失去焦点
    m_listNote.PostMessage(WM_CUSTOM_KILL_EDIT_FOCUS, 0, 0);

    // 取消父类调用，屏蔽回车退出程序
    // CDialogEx::OnOK();
}


void CNoteManagerDlg::OnBnClickedButtonAdd()
{
    CAddDlg dlgAdd;
    int nRet = dlgAdd.DoModal();
    if (nRet != IDOK)
    {
        return;
    }

    CString strPlatform = dlgAdd.GetPlatform();
    CString strAccount = dlgAdd.GetAccount();
    CString strPassword = dlgAdd.GetPassword();
    char szAccount[128] = { 0 };
    char szPassword[128] = { 0 };
    CRYPTO_EncryptString(strAccount.GetBuffer(), m_strKey.GetBuffer(), szAccount, sizeof(szAccount));
    CRYPTO_EncryptString(strPassword.GetBuffer(), m_strKey.GetBuffer(), szPassword, sizeof(szPassword));
    nRet = DBM_AddNote(strPlatform.GetBuffer(), szAccount, szPassword);
    if (nRet != 0)
    {
        MessageBox("数据保存失败", "数据保存", MB_OK | MB_ICONWARNING);
        return;
    }
    PostMessage(WM_LOAD＿DATA);
}


void CNoteManagerDlg::OnBnClickedButtonDelete()
{
    // 获取选中便签的ID
    int nSelectItem = m_listNote.GetNextItem(-1, LVIS_SELECTED);
    if (nSelectItem == CB_ERR)
    {
        MessageBox("请选中一条进行删除");
        return;
    }
    CString strNoteID = m_listNote.GetItemText(nSelectItem, NOTE_COLUMN_ID);
    int nNoteID = atoi(strNoteID.GetBuffer());
    int nRet = DBM_DeleteNote(nNoteID);
    if (nRet != 0)
    {
        MessageBox("数据删除失败", "删除保存", MB_OK | MB_ICONWARNING);
        return;
    }
    PostMessage(WM_LOAD＿DATA);
}
