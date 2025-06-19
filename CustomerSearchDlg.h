// CustomerSearchDlg.h: Customer Search Dialog Interface
//

#pragma once
#include "HouseManagementNEWDoc.h"

// CCustomerSearchDlg Dialog
class CCustomerSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomerSearchDlg)

public:
	CCustomerSearchDlg(CWnd* pParent = nullptr);   // Standard constructor
	virtual ~CCustomerSearchDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMER_SEARCH };
#endif

	// Search conditions
	CString m_strSearchKeyword;
	std::vector<CustomerInfo> m_searchResults;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	// Execute search
	void DoSearch();
	// Get search results
	std::vector<CustomerInfo> GetSearchResults() const { return m_searchResults; }

	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonClear();
}; 