#pragma once
#include <vector>
#include <afxdb.h>

struct CustomerInfo {
    CString CustomerID;
    CString Name;
    CString IDCard;
    CString Phone;
    CString Email;
    CString Address;
    CString Occupation;
    CString Income;
    CString Preference;
    CString Status;
    CString Remark;
};

class CCustomerDBHelper {
public:
    CCustomerDBHelper();
    ~CCustomerDBHelper();
    
    // 数据库连接管理
    BOOL ConnectDatabase();
    void CloseDatabase();
    BOOL CreateCustomerTable();
    
    // 客户管理功能
    BOOL AddCustomer(const CustomerInfo& customer);
    BOOL UpdateCustomer(const CustomerInfo& customer);
    BOOL DeleteCustomer(const CString& strCustomerID);
    CustomerInfo* FindCustomer(const CString& strCustomerID);
    std::vector<CustomerInfo> SearchCustomers(const CString& strKeyword);
    std::vector<CustomerInfo> GetAllCustomers();

private:
    CDatabase* m_pDatabase;
}; 