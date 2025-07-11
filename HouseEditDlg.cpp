// HouseEditDlg.cpp: 房屋编辑对话框类的实现
//

#include "pch.h"
#include "HouseManagementNEW.h"
#include "HouseEditDlg.h"
#include "HouseDBHelper.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHouseEditDlg 对话框

IMPLEMENT_DYNAMIC(CHouseEditDlg, CDialogEx)

CHouseEditDlg::CHouseEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOUSE_EDIT, pParent)
	, m_bIsEdit(FALSE)
{
}

CHouseEditDlg::~CHouseEditDlg()
{
}

void CHouseEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUSE_ID, m_houseInfo.id);
	DDX_Text(pDX, IDC_EDIT_HOUSE_ADDRESS, m_houseInfo.address);
	DDX_Text(pDX, IDC_EDIT_HOUSE_TYPE, m_houseInfo.type);
	DDX_Text(pDX, IDC_EDIT_HOUSE_AREA, m_houseInfo.area);
	DDX_Text(pDX, IDC_EDIT_HOUSE_PRICE, m_houseInfo.price);
	DDX_Text(pDX, IDC_EDIT_HOUSE_DESCRIPTION, m_houseInfo.description);
}

BEGIN_MESSAGE_MAP(CHouseEditDlg, CDialogEx)
END_MESSAGE_MAP()

// CHouseEditDlg 消息处理程序

BOOL CHouseEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化状态组合框
	CComboBox* pComboStatus = (CComboBox*)GetDlgItem(IDC_COMBO_HOUSE_STATUS);
	if (pComboStatus)
	{
		pComboStatus->AddString(_T("出租"));
		pComboStatus->AddString(_T("出售"));
		pComboStatus->AddString(_T("自住"));
		pComboStatus->AddString(_T("空置"));
		pComboStatus->SetCurSel(0); // 默认选择第一个
	}

	// 如果是编辑模式，设置房屋编号为只读
	if (m_bIsEdit)
	{
		GetDlgItem(IDC_EDIT_HOUSE_ID)->EnableWindow(FALSE);
		SetWindowText(_T("编辑房屋信息"));
	}
	else
	{
		SetWindowText(_T("添加房屋信息"));
	}

	// 设置状态组合框的值
	if (!m_houseInfo.status.IsEmpty())
	{
		if (pComboStatus)
		{
			int nIndex = pComboStatus->FindStringExact(-1, m_houseInfo.status);
			if (nIndex != CB_ERR)
			{
				pComboStatus->SetCurSel(nIndex);
			}
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHouseEditDlg::SetHouseInfo(const HouseInfo& houseInfo)
{
	m_houseInfo = houseInfo;
}

HouseInfo CHouseEditDlg::GetHouseInfo() const
{
	return m_houseInfo;
}

BOOL CHouseEditDlg::ValidateInput()
{
	UpdateData(TRUE);

	// 验证房屋编号
	if (m_houseInfo.id <= 0)
	{
		AfxMessageBox(_T("请输入有效的房屋编号（大于0）！"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_HOUSE_ID)->SetFocus();
		return FALSE;
	}

	// 唯一性校验（新增时不能重复，编辑时允许本身）
	if (!m_bIsEdit) // 仅新增时校验
	{
		CString strID;
		strID.Format(_T("%d"), m_houseInfo.id);
		
		if (HouseDBHelper::IsHouseIDExist(strID))
		{
			AfxMessageBox(_T("房屋编号已存在，请重新输入！"), MB_OK | MB_ICONERROR);
			GetDlgItem(IDC_EDIT_HOUSE_ID)->SetFocus();
			return FALSE;
		}
	}

	// 验证地址
	if (m_houseInfo.address.IsEmpty())
	{
		AfxMessageBox(_T("请输入房屋地址！"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_HOUSE_ADDRESS)->SetFocus();
		return FALSE;
	}

	// 验证类型
	if (m_houseInfo.type.IsEmpty())
	{
		AfxMessageBox(_T("请输入房屋类型！"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_HOUSE_TYPE)->SetFocus();
		return FALSE;
	}

	// 验证面积
	if (m_houseInfo.area <= 0)
	{
		AfxMessageBox(_T("请输入有效的面积（大于0）！"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_HOUSE_AREA)->SetFocus();
		return FALSE;
	}

	// 验证价格
	if (m_houseInfo.price < 0)
	{
		AfxMessageBox(_T("请输入有效的价格（大于等于0）！"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_HOUSE_PRICE)->SetFocus();
		return FALSE;
	}

	// 验证业主 - 数据库版本暂不支持业主字段
	// if (m_houseInfo.strOwner.IsEmpty())
	// {
	// 	AfxMessageBox(_T("请输入业主姓名！"), MB_OK | MB_ICONWARNING);
	// 	GetDlgItem(IDC_EDIT_HOUSE_OWNER)->SetFocus();
	// 	return FALSE;
	// }

	// 验证联系方式 - 数据库版本暂不支持联系方式字段
	// if (m_houseInfo.strContact.IsEmpty())
	// {
	// 	AfxMessageBox(_T("请输入联系方式！"), MB_OK | MB_ICONWARNING);
	// 	GetDlgItem(IDC_EDIT_HOUSE_CONTACT)->SetFocus();
	// 	return FALSE;
	// }

	// 获取状态组合框的值
	CComboBox* pComboStatus = (CComboBox*)GetDlgItem(IDC_COMBO_HOUSE_STATUS);
	if (pComboStatus)
	{
		int nIndex = pComboStatus->GetCurSel();
		if (nIndex != CB_ERR)
		{
			pComboStatus->GetLBText(nIndex, m_houseInfo.status);
		}
		else
		{
			AfxMessageBox(_T("请选择房屋状态！"), MB_OK | MB_ICONWARNING);
			pComboStatus->SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

void CHouseEditDlg::OnOK()
{
	if (ValidateInput())
	{
		CDialogEx::OnOK();
	}
} 