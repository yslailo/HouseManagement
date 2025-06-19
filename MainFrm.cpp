// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "HouseManagementNEW.h"
#include "HouseManagementNEWView.h"
#include "HouseManagementNEWDoc.h"
#include <afxdb.h>

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	// 房屋管理菜单消息处理
	ON_COMMAND(ID_HOUSE_ADD, &CMainFrame::OnHouseAdd)
	ON_COMMAND(ID_HOUSE_EDIT, &CMainFrame::OnHouseEdit)
	ON_COMMAND(ID_HOUSE_DELETE, &CMainFrame::OnHouseDelete)
	ON_COMMAND(ID_HOUSE_SEARCH, &CMainFrame::OnHouseSearch)
	ON_COMMAND(ID_HOUSE_IMPORT, &CMainFrame::OnHouseImport)
	ON_COMMAND(ID_HOUSE_EXPORT, &CMainFrame::OnHouseExport)
	// 客户管理菜单消息处理
	ON_COMMAND(ID_CUSTOMER_ADD, &CMainFrame::OnCustomerAdd)
	ON_COMMAND(ID_CUSTOMER_EDIT, &CMainFrame::OnCustomerEdit)
	ON_COMMAND(ID_CUSTOMER_DELETE, &CMainFrame::OnCustomerDelete)
	ON_COMMAND(ID_CUSTOMER_SEARCH, &CMainFrame::OnCustomerSearch)
	ON_COMMAND(ID_CUSTOMER_IMPORT, &CMainFrame::OnCustomerImport)
	ON_COMMAND(ID_CUSTOMER_EXPORT, &CMainFrame::OnCustomerExport)
	ON_COMMAND(ID_TEST_DB_CONNECT, &CMainFrame::OnTestDbConnect)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	// if (!CreateDockingWindows())
	// {
	// 	TRACE0("未能创建停靠窗口\n");
	// 	return -1;
	// }

	// m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	// m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	// DockPane(&m_wndFileView);
	// CDockablePane* pTabbedBar = nullptr;
	// m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	// m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	// DockPane(&m_wndOutput);
	// m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	// DockPane(&m_wndProperties);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	// 直接return TRUE; 不再创建任何辅助窗口。
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	// 直接return; 不再设置任何辅助窗口图标。
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	// m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	// m_wndOutput.UpdateFonts();
}

void CMainFrame::OnHouseAdd()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->AddHouse();
	}
}

void CMainFrame::OnHouseEdit()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->EditHouse();
	}
}

void CMainFrame::OnHouseDelete()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->DeleteHouse();
	}
}

void CMainFrame::OnHouseSearch()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->SearchHouse();
	}
}

void CMainFrame::OnHouseImport()
{
	// 获取当前活动的视图和文档
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	CHouseManagementNEWDoc* pDoc = (CHouseManagementNEWDoc*)GetActiveDocument();
	if (pView && pDoc)
	{
		// 已改为数据库操作，不再支持文件导入
		AfxMessageBox(_T("房屋数据已改为数据库存储，不再支持文件导入！"), MB_OK | MB_ICONINFORMATION);
		// CFileDialog dlg(TRUE, _T("csv"), _T("houses"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		// 	_T("CSV文件 (*.csv)|*.csv|所有文件 (*.*)|*.*||"), this);
		
		// if (dlg.DoModal() == IDOK)
		// {
		// 	if (pDoc->LoadFromFile(dlg.GetPathName()))
		// 	{
		// 		pView->RefreshHouseList();
		// 		AfxMessageBox(_T("数据导入成功！"), MB_OK | MB_ICONINFORMATION);
		// 	}
		// 	else
		// 	{
		// 		AfxMessageBox(_T("数据导入失败！"), MB_OK | MB_ICONERROR);
		// 	}
		// }
	}
}

void CMainFrame::OnHouseExport()
{
	// 获取当前活动的视图和文档
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	CHouseManagementNEWDoc* pDoc = (CHouseManagementNEWDoc*)GetActiveDocument();
	if (pView && pDoc)
	{
		// 已改为数据库操作，不再支持文件导出
		AfxMessageBox(_T("房屋数据已改为数据库存储，不再支持文件导出！"), MB_OK | MB_ICONINFORMATION);
		// CFileDialog dlg(FALSE, _T("csv"), _T("houses"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		// 	_T("CSV文件 (*.csv)|*.csv|所有文件 (*.*)|*.*||"), this);
		
		// if (dlg.DoModal() == IDOK)
		// {
		// 	if (pDoc->SaveToFile(dlg.GetPathName()))
		// 	{
		// 		AfxMessageBox(_T("数据导出成功！"), MB_OK | MB_ICONINFORMATION);
		// 	}
		// 	else
		// 	{
		// 		AfxMessageBox(_T("数据导出失败！"), MB_OK | MB_ICONERROR);
		// 	}
		// }
	}
}

void CMainFrame::OnCustomerAdd()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->AddCustomer();
	}
}

void CMainFrame::OnCustomerEdit()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->EditCustomer();
	}
}

void CMainFrame::OnCustomerDelete()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->DeleteCustomer();
	}
}

void CMainFrame::OnCustomerSearch()
{
	// 获取当前活动的视图
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	if (pView)
	{
		pView->SearchCustomer();
	}
}

void CMainFrame::OnCustomerImport()
{
	// 获取当前活动的视图和文档
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	CHouseManagementNEWDoc* pDoc = (CHouseManagementNEWDoc*)GetActiveDocument();
	if (pView && pDoc)
	{
		// 已改为数据库操作，不再支持文件导入
		AfxMessageBox(_T("客户数据已改为数据库存储，不再支持文件导入！"), MB_OK | MB_ICONINFORMATION);
		// CFileDialog dlg(TRUE, _T("csv"), _T("customers"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		// 	_T("CSV文件 (*.csv)|*.csv|所有文件 (*.*)|*.*||"), this);
		
		// if (dlg.DoModal() == IDOK)
		// {
		// 	if (pDoc->LoadCustomersFromFile(dlg.GetPathName()))
		// 	{
		// 		pView->RefreshCustomerList();
		// 		AfxMessageBox(_T("客户数据导入成功！"), MB_OK | MB_ICONINFORMATION);
		// 	}
		// 	else
		// 	{
		// 		AfxMessageBox(_T("客户数据导入失败！"), MB_OK | MB_ICONERROR);
		// 	}
		// }
	}
}

void CMainFrame::OnCustomerExport()
{
	// 获取当前活动的视图和文档
	CHouseManagementNEWView* pView = (CHouseManagementNEWView*)GetActiveView();
	CHouseManagementNEWDoc* pDoc = (CHouseManagementNEWDoc*)GetActiveDocument();
	if (pView && pDoc)
	{
		// 已改为数据库操作，不再支持文件导出
		AfxMessageBox(_T("客户数据已改为数据库存储，不再支持文件导出！"), MB_OK | MB_ICONINFORMATION);
		// CFileDialog dlg(FALSE, _T("csv"), _T("customers"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		// 	_T("CSV文件 (*.csv)|*.csv|所有文件 (*.*)|*.*||"), this);
		
		// if (dlg.DoModal() == IDOK)
		// {
		// 	if (pDoc->SaveCustomersToFile(dlg.GetPathName()))
		// 	{
		// 		AfxMessageBox(_T("客户数据导出成功！"), MB_OK | MB_ICONINFORMATION);
		// 	}
		// 	else
		// 	{
		// 		AfxMessageBox(_T("客户数据导出失败！"), MB_OK | MB_ICONERROR);
		// 	}
		// }
	}
}

void CMainFrame::OnTestDbConnect()
{
	CDatabase db;
	CString strConn = _T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;");
	if (db.OpenEx(strConn, CDatabase::noOdbcDialog))
	{
		AfxMessageBox(_T("数据库连接成功！"));

		// 测试查询一条数据
		CRecordset rs(&db);
		rs.Open(CRecordset::forwardOnly, _T("SELECT TOP 1 * FROM House"));
		if (!rs.IsEOF())
		{
			CString address;
			rs.GetFieldValue(_T("Address"), address);
			CString msg;
			msg.Format(_T("第一条房屋地址：%s"), address);
			AfxMessageBox(msg);
		}
		else
		{
			AfxMessageBox(_T("房屋表没有数据。"));
		}
		rs.Close();

		db.Close();
	}
	else
	{
		AfxMessageBox(_T("数据库连接失败！"));
	}
}
