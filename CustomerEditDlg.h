// CustomerEditDlg.h: Customer Edit Dialog Interface
//

#pragma once
#include "CustomerDBHelper.h"

// CCustomerEditDlg Dialog
class CCustomerEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomerEditDlg)

public:
	CCustomerEditDlg(CWnd* pParent = nullptr);   // Standard constructor
	virtual ~CCustomerEditDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMER_EDIT };
#endif

	// Customer Information
	CustomerInfo m_customerInfo;
	BOOL m_bIsEdit; // TRUE for edit mode, FALSE for add mode
	// CHouseManagementNEWDoc* m_pDoc; // 不再需要文档对象引用

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	// Set customer information
	void SetCustomerInfo(const CustomerInfo& customerInfo);
	// Get customer information
	CustomerInfo GetCustomerInfo() const;
	// Validate input data
	BOOL ValidateInput();
	virtual void OnOK();
}; 