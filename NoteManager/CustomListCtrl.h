#pragma once
#include <afxcmn.h>
#include "CustomEdit.h"

// CCustomListCtrl

const unsigned int WM_CUSTOM_LIST_ITEM_CHANGE = WM_USER + 0x301;    // �Զ����б�����ı���Ϣ
const unsigned int WM_CUSTOM_KILL_EDIT_FOCUS = WM_USER + 0x302;     // �Զ����б��ñ༭��ʧȥ������Ϣ

/*
*   �Զ����б�
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
    // ������
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    // ���˫��
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
    // 
    virtual void PreSubclassWindow();
    // �༭��ʧȥ������Ӧ����
    afx_msg LRESULT OnEditKillFocus(WPARAM wParam, LPARAM lParam);
    // �ñ༭��ʧȥ������Ϣ��Ӧ����
    afx_msg LRESULT OnCustomKillEditFocus(WPARAM wParam, LPARAM lParam);

private:
    int m_nItem;            // ����
    int m_nSubItem;         // ����
    CCustomEdit m_editItem; // �Զ���༭��
};

