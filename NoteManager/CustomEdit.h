#pragma once
#include <afxwin.h>

const unsigned int WM_CUSTOM_EDIT_KILL_FOCUS = WM_USER + 0x201; // �Զ���༭��ʧȥ�����¼�
/*
*   �Զ���༭��
*/
class CCustomEdit :
    public CEdit
{
    DECLARE_DYNAMIC(CCustomEdit)

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
};

