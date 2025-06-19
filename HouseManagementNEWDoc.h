// HouseManagementNEWDoc.h: CHouseManagementNEWDoc 类的接口
//

#pragma once
#include <vector>
#include <string>
#include "HouseDBHelper.h"
#include "CustomerDBHelper.h"

// 房屋信息结构体 - 已移至HouseDBHelper.h
// struct HouseInfo
// {
// 	CString strID;           // 房屋编号
// 	CString strAddress;      // 地址
// 	CString strType;         // 房屋类型
// 	double dArea;            // 面积
// 	double dPrice;           // 价格
// 	CString strOwner;        // 业主
// 	CString strStatus;       // 状态（出租/出售/自住）
// 	CString strDescription;  // 描述
// 	CString strContact;      // 联系方式
// 	
// 	HouseInfo()
// 	{
// 		dArea = 0.0;
// 		dPrice = 0.0;
// 	}
// };

// 客户信息结构体 - 已移至CustomerDBHelper.h
// struct CustomerInfo
// {
// 	CString strID;           // 客户编号
// 	CString strName;         // 客户姓名
// 	CString strIDCard;       // 身份证号
// 	CString strPhone;        // 联系电话
// 	CString strEmail;        // 电子邮箱
// 	CString strAddress;      // 联系地址
// 	CString strOccupation;   // 职业
// 	CString strIncome;       // 收入水平
// 	CString strPreference;   // 购房偏好（户型、区域等）
// 	CString strStatus;       // 客户状态（潜在/意向/成交/流失）
// 	CString strRemark;       // 备注
// 	
// 	CustomerInfo()
// 	{
// 		// 构造函数，初始化默认值
// 	}
// };

class CHouseManagementNEWDoc : public CDocument
{
protected: // 仅从序列化创建
	CHouseManagementNEWDoc() noexcept;
	DECLARE_DYNCREATE(CHouseManagementNEWDoc)

// 特性
public:
	// 房屋数据管理
	// std::vector<HouseInfo> m_houses;
	
	// 客户数据管理
	// std::vector<CustomerInfo> m_customers;
	
	// 房屋管理功能
	// BOOL AddHouse(const HouseInfo& house);
	// BOOL DeleteHouse(const CString& strID);
	// BOOL UpdateHouse(const HouseInfo& house);
	// HouseInfo* FindHouse(const CString& strID);
	// std::vector<HouseInfo> SearchHouses(const CString& strKeyword);
	
	// 客户管理功能
	// BOOL AddCustomer(const CustomerInfo& customer);
	// BOOL DeleteCustomer(const CString& strID);
	// BOOL UpdateCustomer(const CustomerInfo& customer);
	// CustomerInfo* FindCustomer(const CString& strID);
	// std::vector<CustomerInfo> SearchCustomers(const CString& strKeyword);
	
	// 数据持久化
	// BOOL SaveToFile(const CString& strFilePath);
	// BOOL LoadFromFile(const CString& strFilePath);
	// BOOL SaveCustomersToFile(const CString& strFilePath);
	// BOOL LoadCustomersFromFile(const CString& strFilePath);

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CHouseManagementNEWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
