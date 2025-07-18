// HouseEditDlg.h: 房屋编辑对话框类的接口
//

#pragma once
#include "HouseManagementNEWDoc.h"

// CHouseEditDlg 对话框
class CHouseEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHouseEditDlg)

public:
	CHouseEditDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHouseEditDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOUSE_EDIT };
#endif

	// 房屋信息
	HouseInfo m_houseInfo;
	BOOL m_bIsEdit; // TRUE表示编辑模式，FALSE表示添加模式
	// CHouseManagementNEWDoc* m_pDoc; // 不再需要文档对象引用

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	// 设置房屋信息
	void SetHouseInfo(const HouseInfo& houseInfo);
	// 获取房屋信息
	HouseInfo GetHouseInfo() const;
	// 验证输入数据
	BOOL ValidateInput();
	virtual void OnOK();
}; 