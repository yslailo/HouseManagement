// HouseSearchDlg.cpp: 房屋多条件搜索对话框实现
#include "pch.h"
#include "HouseSearchDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(HouseSearchDlg, CDialogEx)

HouseSearchDlg::HouseSearchDlg(CWnd* pParent)
    : CDialogEx(IDD_HOUSE_SEARCH, pParent)
{
}

HouseSearchDlg::~HouseSearchDlg() {}

void HouseSearchDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SEARCH_ID, m_id);
    DDX_Text(pDX, IDC_EDIT_SEARCH_ADDRESS, m_address);
    DDX_Text(pDX, IDC_EDIT_SEARCH_TYPE, m_type);
    DDX_Text(pDX, IDC_EDIT_SEARCH_OWNER, m_owner);
    DDX_CBString(pDX, IDC_COMBO_SEARCH_STATUS, m_status);
}

BEGIN_MESSAGE_MAP(HouseSearchDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL HouseSearchDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SEARCH_STATUS);
    if (pCombo) {
        pCombo->AddString(_T(""));
        pCombo->AddString(_T("出租"));
        pCombo->AddString(_T("出售"));
        pCombo->AddString(_T("自住"));
        pCombo->AddString(_T("空置"));
        pCombo->SetCurSel(0);
    }
    return TRUE;
} 