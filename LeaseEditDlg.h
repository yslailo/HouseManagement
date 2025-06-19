#pragma once
#include <afxwin.h>
#include "LeaseDBHelper.h"
#include "HouseDBHelper.h"
#include "CustomerDBHelper.h"

class LeaseEditDlg : public CDialogEx
{
    DECLARE_DYNAMIC(LeaseEditDlg)
public:
    LeaseEditDlg(CWnd* pParent = nullptr);
    virtual ~LeaseEditDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LEASE_EDIT };
#endif

    LeaseInfo m_leaseInfo;
    BOOL m_bIsEdit; // TRUE=编辑，FALSE=新增

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnOK();
    DECLARE_MESSAGE_MAP()

    void LoadHouseIDs();
    void LoadCustomerIDs();
}; 