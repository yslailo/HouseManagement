// HouseManagementNEWView.h: CHouseManagementNEWView 类的接口
//

#pragma once
#include "HouseEditDlg.h"
#include "CustomerEditDlg.h"
#include "CustomerSearchDlg.h"

class CHouseManagementNEWView : public CView
{
protected: // 仅从序列化创建
	CHouseManagementNEWView() noexcept;
	DECLARE_DYNCREATE(CHouseManagementNEWView)

// 特性
public:
	CHouseManagementNEWDoc* GetDocument() const;

// 操作
public:
	// 房屋管理操作
	void RefreshHouseList();
	void AddHouse();
	void EditHouse();
	void DeleteHouse();
	void SearchHouse();
	
	// 客户管理操作
	void RefreshCustomerList();
	void AddCustomer();
	void EditCustomer();
	void DeleteCustomer();
	void SearchCustomer();

	// 租赁管理操作
	void RefreshLeaseList();
	void AddLease();
	void EditLease();
	void DeleteLease();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHouseManagementNEWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CListCtrl m_listHouses; // 房屋列表控件
	CListCtrl m_listCustomers; // 客户列表控件
	CListCtrl m_listLeases; // 租赁合同列表控件
	CTabCtrl m_tabCtrl; // 标签控件，用于切换房屋、客户、租赁管理
	CButton m_btnLeaseAdd;
	CButton m_btnLeaseEdit;
	CButton m_btnLeaseDelete;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkHouseList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkCustomerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLeaseAddClicked();
	afx_msg void OnLeaseEditClicked();
	afx_msg void OnLeaseDeleteClicked();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HouseManagementNEWView.cpp 中的调试版本
inline CHouseManagementNEWDoc* CHouseManagementNEWView::GetDocument() const
   { return reinterpret_cast<CHouseManagementNEWDoc*>(m_pDocument); }
#endif

