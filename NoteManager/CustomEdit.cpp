#include "pch.h"
#include "CustomEdit.h"

// CCustomEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CCustomEdit 消息处理程序

void CCustomEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

    // 向父控件发送 编辑框失去焦点事件
    CWnd* pParent = GetParent();
    if (pParent)
    {
        ::SendMessage(pParent->GetSafeHwnd(), WM_CUSTOM_EDIT_KILL_FOCUS, 0, 0);
    }
}
