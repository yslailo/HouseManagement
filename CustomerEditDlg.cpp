// CustomerEditDlg.cpp: Customer Edit Dialog Implementation
//

#include "pch.h"
#include "framework.h"
#include "HouseManagementNEW.h"
#include "CustomerEditDlg.h"
#include "CustomerDBHelper.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCustomerEditDlg Dialog

IMPLEMENT_DYNAMIC(CCustomerEditDlg, CDialogEx)

CCustomerEditDlg::CCustomerEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUSTOMER_EDIT, pParent)
	, m_bIsEdit(FALSE)
	// , m_pDoc(nullptr)
{
}

CCustomerEditDlg::~CCustomerEditDlg()
{
}

void CCustomerEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_ID, m_customerInfo.CustomerID);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_NAME, m_customerInfo.Name);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_IDCARD, m_customerInfo.IDCard);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_PHONE, m_customerInfo.Phone);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_EMAIL, m_customerInfo.Email);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_ADDRESS, m_customerInfo.Address);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_OCCUPATION, m_customerInfo.Occupation);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_INCOME, m_customerInfo.Income);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_PREFERENCE, m_customerInfo.Preference);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_STATUS, m_customerInfo.Status);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER_REMARK, m_customerInfo.Remark);
}

BEGIN_MESSAGE_MAP(CCustomerEditDlg, CDialogEx)
END_MESSAGE_MAP()

// CCustomerEditDlg Message Handlers

BOOL CCustomerEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set window title
	if (m_bIsEdit)
	{
		SetWindowText(_T("Edit Customer Information"));
	}
	else
	{
		SetWindowText(_T("Add Customer Information"));
	}

	// Disable customer ID edit box in edit mode
	if (m_bIsEdit)
	{
		GetDlgItem(IDC_EDIT_CUSTOMER_ID)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CCustomerEditDlg::SetCustomerInfo(const CustomerInfo& customerInfo)
{
	m_customerInfo = customerInfo;
}

CustomerInfo CCustomerEditDlg::GetCustomerInfo() const
{
	return m_customerInfo;
}

BOOL CCustomerEditDlg::ValidateInput()
{
	UpdateData(TRUE);

	// Validate customer ID
	if (m_customerInfo.CustomerID.IsEmpty())
	{
		MessageBox(_T("Customer ID cannot be empty!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_CUSTOMER_ID)->SetFocus();
		return FALSE;
	}

	// Validate customer name
	if (m_customerInfo.Name.IsEmpty())
	{
		MessageBox(_T("Customer name cannot be empty!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_CUSTOMER_NAME)->SetFocus();
		return FALSE;
	}

	// Validate ID card format (simple validation)
	if (!m_customerInfo.IDCard.IsEmpty())
	{
		if (m_customerInfo.IDCard.GetLength() != 18)
		{
			MessageBox(_T("ID card number must be 18 digits!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_CUSTOMER_IDCARD)->SetFocus();
			return FALSE;
		}
	}

	// Validate phone number format (simple validation)
	if (!m_customerInfo.Phone.IsEmpty())
	{
		if (m_customerInfo.Phone.GetLength() != 11)
		{
			MessageBox(_T("Phone number must be 11 digits!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_CUSTOMER_PHONE)->SetFocus();
			return FALSE;
		}
	}

	// Validate email format (simple validation)
	if (!m_customerInfo.Email.IsEmpty())
	{
		if (m_customerInfo.Email.Find(_T("@")) < 0)
		{
			MessageBox(_T("Invalid email format!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_CUSTOMER_EMAIL)->SetFocus();
			return FALSE;
		}
	}

	// Check customer ID uniqueness
	if (!m_bIsEdit) // 仅新增时校验
	{
		CCustomerDBHelper dbHelper;
		if (dbHelper.ConnectDatabase())
		{
			if (dbHelper.FindCustomer(m_customerInfo.CustomerID) != nullptr)
			{
				MessageBox(_T("Customer ID already exists!"), _T("Validation Failed"), MB_OK | MB_ICONWARNING);
				GetDlgItem(IDC_EDIT_CUSTOMER_ID)->SetFocus();
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CCustomerEditDlg::OnOK()
{
	if (!ValidateInput())
	{
		return;
	}

	CDialogEx::OnOK();
} 