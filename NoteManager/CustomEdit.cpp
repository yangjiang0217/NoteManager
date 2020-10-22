#include "pch.h"
#include "CustomEdit.h"

// CCustomEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CCustomEdit ��Ϣ�������

void CCustomEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

    // �򸸿ؼ����� �༭��ʧȥ�����¼�
    CWnd* pParent = GetParent();
    if (pParent)
    {
        ::SendMessage(pParent->GetSafeHwnd(), WM_CUSTOM_EDIT_KILL_FOCUS, 0, 0);
    }
}
