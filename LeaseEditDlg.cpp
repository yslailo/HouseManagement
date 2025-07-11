#include "pch.h"
#include "LeaseEditDlg.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(LeaseEditDlg, CDialogEx)

LeaseEditDlg::LeaseEditDlg(CWnd* pParent)
    : CDialogEx(IDD_LEASE_EDIT, pParent), m_bIsEdit(FALSE)
{
}

LeaseEditDlg::~LeaseEditDlg() {}

void LeaseEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // 这里可用 DDX/DDV 绑定控件
}

BEGIN_MESSAGE_MAP(LeaseEditDlg, CDialogEx)
    // 可添加控件事件
END_MESSAGE_MAP()

BOOL LeaseEditDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    LoadHouseIDs();
    LoadCustomerIDs();
    // 初始化其他控件
    if (m_bIsEdit)
    {
        // 设置控件为当前合同信息
    }
    return TRUE;
}

void LeaseEditDlg::LoadHouseIDs()
{
    // 加载所有房屋编号到下拉框
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HOUSE_ID);
    if (!pCombo) return;
    pCombo->ResetContent();
    std::vector<HouseInfo> houses = HouseDBHelper::GetAllHouses();
    for (const auto& h : houses)
    {
        CString strID;
        strID.Format(_T("%d"), h.id);
        pCombo->AddString(strID);
    }
}

void LeaseEditDlg::LoadCustomerIDs()
{
    // 加载所有客户编号到下拉框
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOMER_ID);
    if (!pCombo) return;
    pCombo->ResetContent();
    CCustomerDBHelper dbHelper;
    if (!dbHelper.ConnectDatabase()) return;
    std::vector<CustomerInfo> customers = dbHelper.GetAllCustomers();
    for (const auto& c : customers)
    {
        pCombo->AddString(c.CustomerID);
    }
}

void LeaseEditDlg::OnOK()
{
    // 1. 获取房屋编号
    CComboBox* pHouseCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HOUSE_ID);
    CString strHouseID;
    pHouseCombo->GetWindowText(strHouseID);
    m_leaseInfo.houseID = _ttoi(strHouseID);

    // 2. 获取客户编号
    CComboBox* pCustomerCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOMER_ID);
    pCustomerCombo->GetWindowText(m_leaseInfo.customerID);

    // 3. 获取起租日期
    CString strStartDate;
    GetDlgItem(IDC_EDIT_START_DATE)->GetWindowText(strStartDate);
    m_leaseInfo.startDate.ParseDateTime(strStartDate);

    // 4. 获取退租日期
    CString strEndDate;
    GetDlgItem(IDC_EDIT_END_DATE)->GetWindowText(strEndDate);
    m_leaseInfo.endDate.ParseDateTime(strEndDate);

    // 5. 获取租金
    CString strRent;
    GetDlgItem(IDC_EDIT_RENT)->GetWindowText(strRent);
    m_leaseInfo.rent = _ttof(strRent);

    // 6. 获取押金
    CString strDeposit;
    GetDlgItem(IDC_EDIT_DEPOSIT)->GetWindowText(strDeposit);
    m_leaseInfo.deposit = _ttof(strDeposit);

    // 7. 获取状态
    CComboBox* pStatusCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STATUS);
    pStatusCombo->GetWindowText(m_leaseInfo.status);

    // 8. 获取备注
    GetDlgItem(IDC_EDIT_REMARK)->GetWindowText(m_leaseInfo.remark);

    // 9. 校验（可选）

    CDialogEx::OnOK();
} 