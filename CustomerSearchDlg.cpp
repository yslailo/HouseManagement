// CustomerSearchDlg.cpp: Customer Search Dialog Implementation
//

#include "pch.h"
#include "framework.h"
#include "HouseManagementNEW.h"
#include "CustomerSearchDlg.h"
#include "CustomerDBHelper.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCustomerSearchDlg Dialog

IMPLEMENT_DYNAMIC(CCustomerSearchDlg, CDialogEx)

CCustomerSearchDlg::CCustomerSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUSTOMER_SEARCH, pParent)
{
}

CCustomerSearchDlg::~CCustomerSearchDlg()
{
}

void CCustomerSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH_KEYWORD, m_strSearchKeyword);
}

BEGIN_MESSAGE_MAP(CCustomerSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CCustomerSearchDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCustomerSearchDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()

// CCustomerSearchDlg Message Handlers

BOOL CCustomerSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("Customer Information Search"));

	return TRUE;
}

void CCustomerSearchDlg::DoSearch()
{
	UpdateData(TRUE);

	// Get document object
	CHouseManagementNEWDoc* pDoc = nullptr;
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->GetMainWnd();
	if (pFrame != nullptr)
	{
		pDoc = (CHouseManagementNEWDoc*)pFrame->GetActiveDocument();
	}
	
	if (pDoc == nullptr)
	{
		MessageBox(_T("Cannot get document object!"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	// Execute search using database helper
	CCustomerDBHelper dbHelper;
	if (!dbHelper.ConnectDatabase())
	{
		MessageBox(_T("Failed to connect to database!"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	// Execute search
	m_searchResults = dbHelper.SearchCustomers(m_strSearchKeyword);

	// Display search results
	CString strResult;
	strResult.Format(_T("Found %d customer records"), m_searchResults.size());
	SetDlgItemText(IDC_STATIC_RESULT, strResult);

	// Update list control
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_SEARCH_RESULTS);
	if (pList != nullptr)
	{
		pList->DeleteAllItems();

		for (int i = 0; i < m_searchResults.size(); i++)
		{
			const CustomerInfo& customer = m_searchResults[i];
			
			pList->InsertItem(i, customer.CustomerID);
			pList->SetItemText(i, 1, customer.Name);
			pList->SetItemText(i, 2, customer.Phone);
			pList->SetItemText(i, 3, customer.Address);
			pList->SetItemText(i, 4, customer.Status);
		}
	}
}

void CCustomerSearchDlg::OnBnClickedButtonSearch()
{
	DoSearch();
}

void CCustomerSearchDlg::OnBnClickedButtonClear()
{
	m_strSearchKeyword.Empty();
	SetDlgItemText(IDC_EDIT_SEARCH_KEYWORD, _T(""));
	SetDlgItemText(IDC_STATIC_RESULT, _T(""));
	
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_SEARCH_RESULTS);
	if (pList != nullptr)
	{
		pList->DeleteAllItems();
	}
	
	m_searchResults.clear();
} 