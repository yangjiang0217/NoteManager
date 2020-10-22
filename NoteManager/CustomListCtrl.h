#pragma once
#include <afxcmn.h>
#include "CustomEdit.h"

// CCustomListCtrl

const unsigned int WM_CUSTOM_LIST_ITEM_CHANGE = WM_USER + 0x301;    // 自定义列表项发生改变消息
const unsigned int WM_CUSTOM_KILL_EDIT_FOCUS = WM_USER + 0x302;     // 自定义列表让编辑框失去焦点消息

/*
*   自定义列表
*/
class CCustomListCtrl :
    public CListCtrl
{
    DECLARE_DYNAMIC(CCustomListCtrl)

public:
    CCustomListCtrl();

protected:
    DECLARE_MESSAGE_MAP()
public:
    // 绘制项
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    // 左键双击
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
    // 
    virtual void PreSubclassWindow();
    // 编辑框失去焦点响应函数
    afx_msg LRESULT OnEditKillFocus(WPARAM wParam, LPARAM lParam);
    // 让编辑框失去焦点消息响应函数
    afx_msg LRESULT OnCustomKillEditFocus(WPARAM wParam, LPARAM lParam);

private:
    int m_nItem;            // 主项
    int m_nSubItem;         // 子项
    CCustomEdit m_editItem; // 自定义编辑框
};

