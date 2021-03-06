#include "pch.h"
#include "CustomListCtrl.h"

// CCustomListCtrl

IMPLEMENT_DYNAMIC(CCustomListCtrl, CListCtrl)

CCustomListCtrl::CCustomListCtrl()
    : m_nItem(0)
    , m_nSubItem(0)
{
}

BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
    ON_WM_LBUTTONDBLCLK()
    ON_MESSAGE(WM_CUSTOM_EDIT_KILL_FOCUS, OnEditKillFocus)
    ON_MESSAGE(WM_CUSTOM_KILL_EDIT_FOCUS, &CCustomListCtrl::OnCustomKillEditFocus)
END_MESSAGE_MAP()


// CCustomListCtrl 消息处理程序

void CCustomListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if (NULL == pDC)
    {
        return;
    }
    LVITEM lvi = { 0 };
    lvi.mask = LVIF_STATE;
    lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
    lvi.iItem = lpDrawItemStruct->itemID;
    BOOL bGet = GetItem(&lvi);

    BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) ||
        ((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));

    CRect rcBack = lpDrawItemStruct->rcItem;

    if (bHighlight) // 高亮显示
    {
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
        pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
        pDC->FillRect(rcBack, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
    }
    else
    {
        pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
        pDC->FillRect(rcBack, &CBrush(::GetSysColor(COLOR_WINDOW)));
    }
    // 绘制文本
    CString strText = _T("");
    CRect rcItem;
    if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
    {
        for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
        {
            if (!GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem))
            {
                continue;
            }
            rcItem.left += 3;
            strText = GetItemText(lpDrawItemStruct->itemID, i);
            pDC->DrawText(strText, strText.GetLength(), &rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_BOTTOM);
        }
    }
}

void CCustomListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // 命中测试
    LVHITTESTINFO lvhti = { point };
    m_nItem = SubItemHitTest(&lvhti);
    if (m_nItem == -1)
    {
        return;
    }
    m_nSubItem = lvhti.iSubItem;
    // 第一列不允许编辑
    if (m_nSubItem == 0)
    {
        return CListCtrl::OnLButtonDblClk(nFlags, point);
    }
    // 获取列表框位置
    CRect rcCtrl = { 0 };
    GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rcCtrl);
    // 编辑框未创建，创建编辑框
    if (m_editItem.GetSafeHwnd() == NULL)
    {
        // 创建编辑框
        DWORD dwStyle = ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER;
        m_editItem.Create(dwStyle, rcCtrl, this, 99);
        m_editItem.ShowWindow(SW_HIDE);
        // 设置字体
        CFont *pFont = GetFont();
        m_editItem.SetFont(pFont);
        // 编辑框纵向居中
        TEXTMETRIC tm = { 0 };
        CDC *pDc = m_editItem.GetDC();
        if (pDc)
        {
            pDc->GetTextMetrics(&tm);
        }
        int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
        int nOffY = (rcCtrl.Height() - nFontHeight) / 2;
        ::OffsetRect(&rcCtrl, nOffY, nOffY);
        ::SendMessage(m_editItem.GetSafeHwnd(), EM_SETRECT, 0, (LPARAM)&rcCtrl);
    }
    CString strItem = GetItemText(m_nItem, m_nSubItem);
    m_editItem.MoveWindow(&rcCtrl);
    m_editItem.SetWindowText(strItem);  // 设置文本
    m_editItem.ShowWindow(SW_SHOW); // 显示窗口
    m_editItem.SetFocus();      // 设置焦点
    m_editItem.SetSel(0, -1);   // 全选文本

    CListCtrl::OnLButtonDblClk(nFlags, point);
}
void CCustomListCtrl::PreSubclassWindow()
{
    // 指定窗口所有者将在报表视图中绘制项
    ModifyStyle(0, LVS_OWNERDRAWFIXED);

    CListCtrl::PreSubclassWindow();
}

LRESULT CCustomListCtrl::OnEditKillFocus(WPARAM wParam, LPARAM lParam)
{
    CString strText = _T("");
    m_editItem.GetWindowText(strText);
    CString strTextOld = GetItemText(m_nItem, m_nSubItem);
    m_editItem.ShowWindow(SW_HIDE);
    // 编辑框内容发生改变，改变列表项内容，向父窗口发送改变消息
    if (strTextOld != strText)
    {
        SetItemText(m_nItem, m_nSubItem, strText);
        CWnd *pParent = GetParent();
        if (pParent)
        {
            ::PostMessage(pParent->GetSafeHwnd(), WM_CUSTOM_LIST_ITEM_CHANGE, m_nItem, m_nSubItem);
        }
    }
    return 0;
}

afx_msg LRESULT CCustomListCtrl::OnCustomKillEditFocus(WPARAM wParam, LPARAM lParam)
{
    // 让编辑框失去焦点
    if (m_editItem.GetSafeHwnd())
    {
        m_editItem.PostMessage(WM_KILLFOCUS, 0, 0);
    }
    return 0;
}
