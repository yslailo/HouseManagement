// HouseSearchDlg.h: 房屋多条件搜索对话框
#pragma once
#include <afxwin.h>

class HouseSearchDlg : public CDialogEx
{
    DECLARE_DYNAMIC(HouseSearchDlg)
public:
    HouseSearchDlg(CWnd* pParent = nullptr);
    virtual ~HouseSearchDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_HOUSE_SEARCH };
#endif

    CString m_id;
    CString m_address;
    CString m_type;
    CString m_owner;
    CString m_status;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
}; 