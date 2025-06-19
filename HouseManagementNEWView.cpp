// HouseManagementNEWView.cpp: CHouseManagementNEWView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HouseManagementNEW.h"
#endif

#include "HouseManagementNEWDoc.h"
#include "HouseManagementNEWView.h"
#include "HouseSearchDlg.h"
#include "CustomerEditDlg.h"
#include "CustomerSearchDlg.h"
#include "HouseDBHelper.h"
#include "CustomerDBHelper.h"
#include "LeaseEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHouseManagementNEWView

IMPLEMENT_DYNCREATE(CHouseManagementNEWView, CView)

BEGIN_MESSAGE_MAP(CHouseManagementNEWView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHouseManagementNEWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HOUSES, &CHouseManagementNEWView::OnNMDblclkHouseList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUSTOMERS, &CHouseManagementNEWView::OnNMDblclkCustomerList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CHouseManagementNEWView::OnTcnSelchangeTabCtrl)
	ON_COMMAND(ID_LEASE_ADD, &CHouseManagementNEWView::OnLeaseAddClicked)
	ON_COMMAND(ID_LEASE_EDIT, &CHouseManagementNEWView::OnLeaseEditClicked)
	ON_COMMAND(ID_LEASE_DELETE, &CHouseManagementNEWView::OnLeaseDeleteClicked)
END_MESSAGE_MAP()

// CHouseManagementNEWView 构造/析构

CHouseManagementNEWView::CHouseManagementNEWView() noexcept
{
	// TODO: 在此处添加构造代码

}

CHouseManagementNEWView::~CHouseManagementNEWView()
{
}

BOOL CHouseManagementNEWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHouseManagementNEWView 绘图

void CHouseManagementNEWView::OnDraw(CDC* /*pDC*/)
{
	CHouseManagementNEWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CHouseManagementNEWView 打印


void CHouseManagementNEWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHouseManagementNEWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHouseManagementNEWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHouseManagementNEWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CHouseManagementNEWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHouseManagementNEWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHouseManagementNEWView 诊断

#ifdef _DEBUG
void CHouseManagementNEWView::AssertValid() const
{
	CView::AssertValid();
}

void CHouseManagementNEWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHouseManagementNEWDoc* CHouseManagementNEWView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHouseManagementNEWDoc)));
	return (CHouseManagementNEWDoc*)m_pDocument;
}
#endif //_DEBUG


// CHouseManagementNEWView 消息处理程序

int CHouseManagementNEWView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建标签控件
	if (!m_tabCtrl.Create(WS_CHILD | WS_VISIBLE | TCS_TABS,
		CRect(0, 0, 0, 0), this, IDC_TAB_MAIN))
	{
		return -1;
	}

	// 添加标签页
	m_tabCtrl.InsertItem(0, _T("房屋管理"));
	m_tabCtrl.InsertItem(1, _T("客户管理"));
	m_tabCtrl.InsertItem(2, _T("租赁管理"));

	// 创建房屋列表控件
	if (!m_listHouses.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
		CRect(0, 0, 0, 0), this, IDC_LIST_HOUSES))
	{
		return -1;
	}

	// 设置房屋列表控件样式
	m_listHouses.SetExtendedStyle(m_listHouses.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 添加房屋列表列
	m_listHouses.InsertColumn(0, _T("房屋编号"), LVCFMT_LEFT, 80);
	m_listHouses.InsertColumn(1, _T("地址"), LVCFMT_LEFT, 200);
	m_listHouses.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 80);
	m_listHouses.InsertColumn(3, _T("面积(㎡)"), LVCFMT_RIGHT, 80);
	m_listHouses.InsertColumn(4, _T("价格(万)"), LVCFMT_RIGHT, 80);
	m_listHouses.InsertColumn(5, _T("业主"), LVCFMT_LEFT, 80);
	m_listHouses.InsertColumn(6, _T("状态"), LVCFMT_LEFT, 60);
	m_listHouses.InsertColumn(7, _T("联系方式"), LVCFMT_LEFT, 120);

	// 创建客户列表控件
	if (!m_listCustomers.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
		CRect(0, 0, 0, 0), this, IDC_LIST_CUSTOMERS))
	{
		return -1;
	}

	// 设置客户列表控件样式
	m_listCustomers.SetExtendedStyle(m_listCustomers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 添加客户列表列
	m_listCustomers.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, 80);
	m_listCustomers.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 100);
	m_listCustomers.InsertColumn(2, _T("身份证号"), LVCFMT_LEFT, 120);
	m_listCustomers.InsertColumn(3, _T("联系电话"), LVCFMT_LEFT, 100);
	m_listCustomers.InsertColumn(4, _T("电子邮箱"), LVCFMT_LEFT, 150);
	m_listCustomers.InsertColumn(5, _T("联系地址"), LVCFMT_LEFT, 200);
	m_listCustomers.InsertColumn(6, _T("职业"), LVCFMT_LEFT, 80);
	m_listCustomers.InsertColumn(7, _T("收入水平"), LVCFMT_LEFT, 80);
	m_listCustomers.InsertColumn(8, _T("购房偏好"), LVCFMT_LEFT, 100);
	m_listCustomers.InsertColumn(9, _T("客户状态"), LVCFMT_LEFT, 80);

	// 在OnCreate中创建租赁合同列表控件
	if (!m_listLeases.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
		CRect(0, 0, 0, 0), this, IDC_LIST_LEASES)) // 使用唯一ID
	{
		return -1;
	}
	m_listLeases.SetExtendedStyle(m_listLeases.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 添加租赁合同列表列
	m_listLeases.InsertColumn(0, _T("合同编号"), LVCFMT_LEFT, 80);
	m_listLeases.InsertColumn(1, _T("房屋编号"), LVCFMT_LEFT, 80);
	m_listLeases.InsertColumn(2, _T("客户编号"), LVCFMT_LEFT, 100);
	m_listLeases.InsertColumn(3, _T("起租日期"), LVCFMT_LEFT, 100);
	m_listLeases.InsertColumn(4, _T("退租日期"), LVCFMT_LEFT, 100);
	m_listLeases.InsertColumn(5, _T("租金"), LVCFMT_RIGHT, 80);
	m_listLeases.InsertColumn(6, _T("押金"), LVCFMT_RIGHT, 80);
	m_listLeases.InsertColumn(7, _T("状态"), LVCFMT_LEFT, 80);
	m_listLeases.InsertColumn(8, _T("备注"), LVCFMT_LEFT, 150);

	// 刷新房屋列表
	RefreshHouseList();
	RefreshCustomerList();
	RefreshLeaseList();

	// 设置初始标签页为房屋管理
	m_tabCtrl.SetCurSel(0);
	
	// 确保初始显示正确的标签页内容
	CRect rect;
	GetClientRect(&rect);
	OnSize(0, rect.Width(), rect.Height());

	// 让租赁合同列表控件置顶，避免被遮挡
	m_listLeases.BringWindowToTop();

	return 0;
}

void CHouseManagementNEWView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_tabCtrl.GetSafeHwnd())
	{
		m_tabCtrl.MoveWindow(0, 0, cx, cy);

		CRect rectTab;
		m_tabCtrl.GetClientRect(&rectTab);
		m_tabCtrl.AdjustRect(FALSE, &rectTab);

		int nCurrentTab = m_tabCtrl.GetCurSel();
		if (nCurrentTab == 0) // 房屋管理
		{
			if (m_listHouses.GetSafeHwnd()) m_listHouses.ShowWindow(SW_SHOW);
			if (m_listCustomers.GetSafeHwnd()) m_listCustomers.ShowWindow(SW_HIDE);
			if (m_listLeases.GetSafeHwnd()) m_listLeases.ShowWindow(SW_HIDE);

			if (m_listHouses.GetSafeHwnd()) m_listHouses.MoveWindow(rectTab);
		}
		else if (nCurrentTab == 1) // 客户管理
		{
			if (m_listHouses.GetSafeHwnd()) m_listHouses.ShowWindow(SW_HIDE);
			if (m_listCustomers.GetSafeHwnd()) m_listCustomers.ShowWindow(SW_SHOW);
			if (m_listLeases.GetSafeHwnd()) m_listLeases.ShowWindow(SW_HIDE);

			if (m_listCustomers.GetSafeHwnd()) m_listCustomers.MoveWindow(rectTab);
		}
		else if (nCurrentTab == 2) // 租赁管理
		{
			if (m_listHouses.GetSafeHwnd()) m_listHouses.ShowWindow(SW_HIDE);
			if (m_listCustomers.GetSafeHwnd()) m_listCustomers.ShowWindow(SW_HIDE);
			if (m_listLeases.GetSafeHwnd()) m_listLeases.ShowWindow(SW_SHOW);

			// 按钮显示
			// m_btnLeaseAdd.ShowWindow(SW_SHOW);
			// m_btnLeaseEdit.ShowWindow(SW_SHOW);
			// m_btnLeaseDelete.ShowWindow(SW_SHOW);

			// 按钮和列表布局
			// 按钮在上方
			// int btnTop = rectTab.top + 10;
			// int btnHeight = 32;
			// m_btnLeaseAdd.MoveWindow(rectTab.left + 10, btnTop, 80, btnHeight);
			// m_btnLeaseEdit.MoveWindow(rectTab.left + 100, btnTop, 80, btnHeight);
			// m_btnLeaseDelete.MoveWindow(rectTab.left + 190, btnTop, 80, btnHeight);

			// 列表在按钮下方
			if (m_listLeases.GetSafeHwnd())
			{
				int listTop = rectTab.top + 10;
				m_listLeases.MoveWindow(rectTab.left, listTop, rectTab.Width(), rectTab.bottom - listTop);
			}
		}
	}
}

void CHouseManagementNEWView::RefreshHouseList()
{
	m_listHouses.DeleteAllItems();
	std::vector<HouseInfo> houses = HouseDBHelper::GetAllHouses();
	for (size_t i = 0; i < houses.size(); i++)
	{
		const HouseInfo& house = houses[i];
		CString strID;
		strID.Format(_T("%d"), house.id);
		int nIndex = m_listHouses.InsertItem(i, strID);
		m_listHouses.SetItemText(nIndex, 1, house.address);
		m_listHouses.SetItemText(nIndex, 2, house.type);
		CString strArea, strPrice;
		strArea.Format(_T("%.2f"), house.area);
		strPrice.Format(_T("%.2f"), house.price);
		m_listHouses.SetItemText(nIndex, 3, strArea);
		m_listHouses.SetItemText(nIndex, 4, strPrice);
		m_listHouses.SetItemText(nIndex, 5, _T("")); // 业主
		m_listHouses.SetItemText(nIndex, 6, house.status);
		m_listHouses.SetItemText(nIndex, 7, _T("")); // 联系方式
	}
}

void CHouseManagementNEWView::AddHouse()
{
	CHouseEditDlg dlg(this);
	dlg.m_bIsEdit = FALSE;
	if (dlg.DoModal() == IDOK)
	{
		HouseInfo house = dlg.GetHouseInfo();
		if (HouseDBHelper::AddHouse(house))
		{
			RefreshHouseList();
			AfxMessageBox(_T("房屋信息添加成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("房屋信息添加失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::EditHouse()
{
	int nSelected = m_listHouses.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要编辑的房屋！"), MB_OK | MB_ICONWARNING);
		return;
	}
	int houseID = _ttoi(m_listHouses.GetItemText(nSelected, 0));
	std::vector<HouseInfo> houses = HouseDBHelper::GetAllHouses();
	auto it = std::find_if(houses.begin(), houses.end(), [houseID](const HouseInfo& h) { return h.id == houseID; });
	if (it == houses.end())
	{
		AfxMessageBox(_T("未找到选中的房屋信息！"), MB_OK | MB_ICONERROR);
		return;
	}
	CHouseEditDlg dlg(this);
	dlg.m_bIsEdit = TRUE;
	dlg.SetHouseInfo(*it);
	if (dlg.DoModal() == IDOK)
	{
		HouseInfo newHouse = dlg.GetHouseInfo();
		newHouse.id = houseID;
		if (HouseDBHelper::UpdateHouse(newHouse))
		{
			RefreshHouseList();
			AfxMessageBox(_T("房屋信息修改成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("房屋信息修改失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::DeleteHouse()
{
	int nSelected = m_listHouses.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要删除的房屋！"), MB_OK | MB_ICONWARNING);
		return;
	}
	int houseID = _ttoi(m_listHouses.GetItemText(nSelected, 0));
	CString strMessage;
	strMessage.Format(_T("确定要删除房屋信息吗？\n\n房屋编号：%d"), houseID);
	if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if (HouseDBHelper::DeleteHouse(houseID))
		{
			RefreshHouseList();
			AfxMessageBox(_T("房屋信息删除成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("房屋信息删除失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::SearchHouse()
{
	HouseSearchDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CString keyword = dlg.m_address + dlg.m_type + dlg.m_owner + dlg.m_status + dlg.m_id;
		std::vector<HouseInfo> results = HouseDBHelper::SearchHouses(keyword);
		m_listHouses.DeleteAllItems();
		for (size_t i = 0; i < results.size(); i++)
		{
			const HouseInfo& house = results[i];
			CString strID;
			strID.Format(_T("%d"), house.id);
			int nIndex = m_listHouses.InsertItem(i, strID);
			m_listHouses.SetItemText(nIndex, 1, house.address);
			m_listHouses.SetItemText(nIndex, 2, house.type);
			CString strArea, strPrice;
			strArea.Format(_T("%.2f"), house.area);
			strPrice.Format(_T("%.2f"), house.price);
			m_listHouses.SetItemText(nIndex, 3, strArea);
			m_listHouses.SetItemText(nIndex, 4, strPrice);
			m_listHouses.SetItemText(nIndex, 5, _T(""));
			m_listHouses.SetItemText(nIndex, 6, house.status);
			m_listHouses.SetItemText(nIndex, 7, _T(""));
		}
	}
}

void CHouseManagementNEWView::OnNMDblclkHouseList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iItem != -1)
	{
		EditHouse();
	}
	
	*pResult = 0;
}

void CHouseManagementNEWView::OnNMDblclkCustomerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iItem != -1)
	{
		EditCustomer();
	}
	
	*pResult = 0;
}

void CHouseManagementNEWView::OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 处理标签页切换
	CRect rect;
	GetClientRect(&rect);
	OnSize(0, rect.Width(), rect.Height()); // 触发OnSize来调整控件显示

	int nCurrentTab = m_tabCtrl.GetCurSel();
	if (nCurrentTab == 2) // 租赁管理
	{
		RefreshLeaseList();
	}
	else if (nCurrentTab == 0)
	{
		RefreshHouseList();
	}
	else if (nCurrentTab == 1)
	{
		RefreshCustomerList();
	}

	if (pResult) *pResult = 0;
}

// 客户管理功能实现

void CHouseManagementNEWView::RefreshCustomerList()
{
	m_listCustomers.DeleteAllItems();
	
	CCustomerDBHelper dbHelper;
	if (!dbHelper.ConnectDatabase())
	{
		AfxMessageBox(_T("数据库连接失败！"), MB_OK | MB_ICONERROR);
		return;
	}
	
	// 确保客户表存在
	if (!dbHelper.CreateCustomerTable())
	{
		AfxMessageBox(_T("创建客户表失败！"), MB_OK | MB_ICONERROR);
		return;
	}
	
	std::vector<CustomerInfo> customers = dbHelper.GetAllCustomers();
	
	// 添加调试信息
	CString strDebug;
	strDebug.Format(_T("获取到 %d 条客户记录"), customers.size());
	TRACE(strDebug);
	
	// 如果没有客户数据，添加一些测试数据
	if (customers.empty())
	{
		CustomerInfo testCustomer1;
		testCustomer1.CustomerID = _T("C001");
		testCustomer1.Name = _T("张三");
		testCustomer1.IDCard = _T("110101199001011234");
		testCustomer1.Phone = _T("13800138001");
		testCustomer1.Email = _T("zhangsan@example.com");
		testCustomer1.Address = _T("北京市朝阳区");
		testCustomer1.Occupation = _T("工程师");
		testCustomer1.Income = _T("月薪15000");
		testCustomer1.Preference = _T("三室两厅");
		testCustomer1.Status = _T("活跃");
		testCustomer1.Remark = _T("测试客户1");
		
		if (dbHelper.AddCustomer(testCustomer1))
		{
			customers.push_back(testCustomer1);
		}
		
		CustomerInfo testCustomer2;
		testCustomer2.CustomerID = _T("C002");
		testCustomer2.Name = _T("李四");
		testCustomer2.IDCard = _T("110101199002022345");
		testCustomer2.Phone = _T("13800138002");
		testCustomer2.Email = _T("lisi@example.com");
		testCustomer2.Address = _T("北京市海淀区");
		testCustomer2.Occupation = _T("教师");
		testCustomer2.Income = _T("月薪12000");
		testCustomer2.Preference = _T("两室一厅");
		testCustomer2.Status = _T("活跃");
		testCustomer2.Remark = _T("测试客户2");
		
		if (dbHelper.AddCustomer(testCustomer2))
		{
			customers.push_back(testCustomer2);
		}
	}
	
	for (size_t i = 0; i < customers.size(); i++)
	{
		const CustomerInfo& customer = customers[i];
		int nIndex = m_listCustomers.InsertItem(i, customer.CustomerID);
		
		m_listCustomers.SetItemText(nIndex, 1, customer.Name);
		m_listCustomers.SetItemText(nIndex, 2, customer.IDCard);
		m_listCustomers.SetItemText(nIndex, 3, customer.Phone);
		m_listCustomers.SetItemText(nIndex, 4, customer.Email);
		m_listCustomers.SetItemText(nIndex, 5, customer.Address);
		m_listCustomers.SetItemText(nIndex, 6, customer.Occupation);
		m_listCustomers.SetItemText(nIndex, 7, customer.Income);
		m_listCustomers.SetItemText(nIndex, 8, customer.Preference);
		m_listCustomers.SetItemText(nIndex, 9, customer.Status);
	}
}

void CHouseManagementNEWView::AddCustomer()
{
	CCustomerEditDlg dlg(this);
	dlg.m_bIsEdit = FALSE;
	
	if (dlg.DoModal() == IDOK)
	{
		CustomerInfo customer = dlg.GetCustomerInfo();
		
		CCustomerDBHelper dbHelper;
		if (!dbHelper.ConnectDatabase())
		{
			AfxMessageBox(_T("数据库连接失败！"), MB_OK | MB_ICONERROR);
			return;
		}
		
		if (dbHelper.AddCustomer(customer))
		{
			RefreshCustomerList();
			AfxMessageBox(_T("客户信息添加成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("客户信息添加失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::EditCustomer()
{
	int nSelected = m_listCustomers.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要编辑的客户！"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strCustomerID = m_listCustomers.GetItemText(nSelected, 0);
	
	CCustomerDBHelper dbHelper;
	if (!dbHelper.ConnectDatabase())
	{
		AfxMessageBox(_T("数据库连接失败！"), MB_OK | MB_ICONERROR);
		return;
	}
	
	CustomerInfo* pCustomer = dbHelper.FindCustomer(strCustomerID);
	if (pCustomer == nullptr)
	{
		AfxMessageBox(_T("未找到选中的客户信息！"), MB_OK | MB_ICONERROR);
		return;
	}

	CCustomerEditDlg dlg(this);
	dlg.m_bIsEdit = TRUE;
	dlg.SetCustomerInfo(*pCustomer);
	
	if (dlg.DoModal() == IDOK)
	{
		CustomerInfo newCustomer = dlg.GetCustomerInfo();
		newCustomer.CustomerID = strCustomerID; // 保持原ID不变
		
		if (dbHelper.UpdateCustomer(newCustomer))
		{
			RefreshCustomerList();
			AfxMessageBox(_T("客户信息修改成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("客户信息修改失败！"), MB_OK | MB_ICONERROR);
		}
	}
	
	delete pCustomer;
}

void CHouseManagementNEWView::DeleteCustomer()
{
	int nSelected = m_listCustomers.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要删除的客户！"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strCustomerID = m_listCustomers.GetItemText(nSelected, 0);
	CString strName = m_listCustomers.GetItemText(nSelected, 1);
	
	CString strMessage;
	strMessage.Format(_T("确定要删除客户信息吗？\n\n客户编号：%s\n姓名：%s"), strCustomerID, strName);
	
	if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CCustomerDBHelper dbHelper;
		if (!dbHelper.ConnectDatabase())
		{
			AfxMessageBox(_T("数据库连接失败！"), MB_OK | MB_ICONERROR);
			return;
		}
		
		if (dbHelper.DeleteCustomer(strCustomerID))
		{
			RefreshCustomerList();
			AfxMessageBox(_T("客户信息删除成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("客户信息删除失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::SearchCustomer()
{
	CCustomerSearchDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		// 显示搜索结果
		std::vector<CustomerInfo> results = dlg.GetSearchResults();
		m_listCustomers.DeleteAllItems();
		
		for (size_t i = 0; i < results.size(); i++)
		{
			const CustomerInfo& customer = results[i];
			int nIndex = m_listCustomers.InsertItem(i, customer.CustomerID);
			
			m_listCustomers.SetItemText(nIndex, 1, customer.Name);
			m_listCustomers.SetItemText(nIndex, 2, customer.IDCard);
			m_listCustomers.SetItemText(nIndex, 3, customer.Phone);
			m_listCustomers.SetItemText(nIndex, 4, customer.Email);
			m_listCustomers.SetItemText(nIndex, 5, customer.Address);
			m_listCustomers.SetItemText(nIndex, 6, customer.Occupation);
			m_listCustomers.SetItemText(nIndex, 7, customer.Income);
			m_listCustomers.SetItemText(nIndex, 8, customer.Preference);
			m_listCustomers.SetItemText(nIndex, 9, customer.Status);
		}
	}
}

void CHouseManagementNEWView::RefreshLeaseList()
{
	m_listLeases.ShowWindow(SW_SHOW);
	m_listLeases.DeleteAllItems();
	std::vector<LeaseInfo> leases = LeaseDBHelper::GetAllLeases();
	for (size_t i = 0; i < leases.size(); i++)
	{
		const LeaseInfo& lease = leases[i];
		CString strLeaseID, strHouseID, strRent, strDeposit;
		strLeaseID.Format(_T("%d"), lease.leaseID);
		strHouseID.Format(_T("%d"), lease.houseID);
		strRent.Format(_T("%.2f"), lease.rent);
		strDeposit.Format(_T("%.2f"), lease.deposit);
		CString strStartDate = lease.startDate.GetStatus() == COleDateTime::valid ? lease.startDate.Format(_T("%Y-%m-%d")) : _T("——");
		CString strEndDate = lease.endDate.GetStatus() == COleDateTime::valid ? lease.endDate.Format(_T("%Y-%m-%d")) : _T("——");
		int nIndex = m_listLeases.InsertItem((int)i, strLeaseID);
		m_listLeases.SetItemText(nIndex, 1, strHouseID);
		m_listLeases.SetItemText(nIndex, 2, lease.customerID);
		m_listLeases.SetItemText(nIndex, 3, strStartDate);
		m_listLeases.SetItemText(nIndex, 4, strEndDate);
		m_listLeases.SetItemText(nIndex, 5, strRent);
		m_listLeases.SetItemText(nIndex, 6, strDeposit);
		m_listLeases.SetItemText(nIndex, 7, lease.status);
		m_listLeases.SetItemText(nIndex, 8, lease.remark);
	}
}

void CHouseManagementNEWView::AddLease()
{
	LeaseEditDlg dlg(this);
	dlg.m_bIsEdit = FALSE;
	if (dlg.DoModal() == IDOK)
	{
		LeaseInfo lease = dlg.m_leaseInfo;
		if (LeaseDBHelper::AddLease(lease))
		{
			RefreshLeaseList();
			AfxMessageBox(_T("租赁合同添加成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("租赁合同添加失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::EditLease()
{
	int nSelected = m_listLeases.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要编辑的合同！"), MB_OK | MB_ICONWARNING);
		return;
	}
	int leaseID = _ttoi(m_listLeases.GetItemText(nSelected, 0));
	LeaseInfo lease = LeaseDBHelper::GetLeaseByID(leaseID);
	LeaseEditDlg dlg(this);
	dlg.m_bIsEdit = TRUE;
	dlg.m_leaseInfo = lease;
	if (dlg.DoModal() == IDOK)
	{
		LeaseInfo newLease = dlg.m_leaseInfo;
		newLease.leaseID = leaseID;
		if (LeaseDBHelper::UpdateLease(newLease))
		{
			RefreshLeaseList();
			AfxMessageBox(_T("租赁合同修改成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("租赁合同修改失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::DeleteLease()
{
	int nSelected = m_listLeases.GetNextItem(-1, LVNI_SELECTED);
	if (nSelected == -1)
	{
		AfxMessageBox(_T("请先选择要删除的合同！"), MB_OK | MB_ICONWARNING);
		return;
	}
	int leaseID = _ttoi(m_listLeases.GetItemText(nSelected, 0));
	CString strMessage;
	strMessage.Format(_T("确定要删除租赁合同吗？\n\n合同编号：%d"), leaseID);
	if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if (LeaseDBHelper::DeleteLease(leaseID))
		{
			RefreshLeaseList();
			AfxMessageBox(_T("租赁合同删除成功！"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("租赁合同删除失败！"), MB_OK | MB_ICONERROR);
		}
	}
}

void CHouseManagementNEWView::OnLeaseAddClicked() { AddLease(); }
void CHouseManagementNEWView::OnLeaseEditClicked() { EditLease(); }
void CHouseManagementNEWView::OnLeaseDeleteClicked() { DeleteLease(); }
