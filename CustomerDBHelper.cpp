#include "pch.h"
#include "CustomerDBHelper.h"
#include <afxdb.h>

// 构造函数
CCustomerDBHelper::CCustomerDBHelper()
{
    m_pDatabase = nullptr;
}

// 析构函数
CCustomerDBHelper::~CCustomerDBHelper()
{
    CloseDatabase();
}

// 连接数据库
BOOL CCustomerDBHelper::ConnectDatabase()
{
    try
    {
        if (m_pDatabase != nullptr)
        {
            m_pDatabase->Close();
            delete m_pDatabase;
        }

        m_pDatabase = new CDatabase();
        CString strConnect = _T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;");
        
        if (!m_pDatabase->OpenEx(strConnect))
        {
            delete m_pDatabase;
            m_pDatabase = nullptr;
            return FALSE;
        }
        
        return TRUE;
    }
    catch (...)
    {
        if (m_pDatabase != nullptr)
        {
            delete m_pDatabase;
            m_pDatabase = nullptr;
        }
        return FALSE;
    }
}

// 关闭数据库连接
void CCustomerDBHelper::CloseDatabase()
{
    if (m_pDatabase != nullptr)
    {
        m_pDatabase->Close();
        delete m_pDatabase;
        m_pDatabase = nullptr;
    }
}

// 创建客户表
BOOL CCustomerDBHelper::CreateCustomerTable()
{
    if (m_pDatabase == nullptr)
        return FALSE;

    try
    {
        CString strSQL = _T("IF NOT EXISTS (SELECT * FROM sysobjects WHERE name='Customers' AND xtype='U') ")
                        _T("CREATE TABLE Customers (")
                        _T("CustomerID NVARCHAR(50) PRIMARY KEY,")
                        _T("Name NVARCHAR(100) NOT NULL,")
                        _T("IDCard NVARCHAR(20),")
                        _T("Phone NVARCHAR(20),")
                        _T("Email NVARCHAR(100),")
                        _T("Address NVARCHAR(200),")
                        _T("Occupation NVARCHAR(50),")
                        _T("Income NVARCHAR(50),")
                        _T("Preference NVARCHAR(200),")
                        _T("Status NVARCHAR(20),")
                        _T("Remark NVARCHAR(500)")
                        _T(")");

        m_pDatabase->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (...)
    {
        return FALSE;
    }
}

// 添加客户
BOOL CCustomerDBHelper::AddCustomer(const CustomerInfo& customer)
{
    if (m_pDatabase == nullptr)
        return FALSE;

    try
    {
        // 检查客户ID是否已存在
        if (FindCustomer(customer.CustomerID) != nullptr)
            return FALSE;

        // 检查身份证号是否已存在
        if (!customer.IDCard.IsEmpty())
        {
            CString strSQL;
            strSQL.Format(_T("SELECT COUNT(*) FROM Customers WHERE IDCard = '%s'"), customer.IDCard);
            
            CRecordset rs(m_pDatabase);
            rs.Open(CRecordset::forwardOnly, strSQL);
            
            if (!rs.IsEOF())
            {
                CDBVariant var;
                rs.GetFieldValue((short)0, var);
                long count = var.m_lVal;
                if (count > 0)
                    return FALSE;
            }
            rs.Close();
        }

        // 插入新客户
        CString strSQL;
        strSQL.Format(_T("INSERT INTO Customers (CustomerID, Name, IDCard, Phone, Email, Address, Occupation, Income, Preference, Status, Remark) ")
                      _T("VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"),
                      customer.CustomerID, customer.Name, customer.IDCard, customer.Phone, customer.Email,
                      customer.Address, customer.Occupation, customer.Income, customer.Preference, customer.Status, customer.Remark);

        m_pDatabase->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (...)
    {
        return FALSE;
    }
}

// 删除客户
BOOL CCustomerDBHelper::DeleteCustomer(const CString& strCustomerID)
{
    if (m_pDatabase == nullptr)
        return FALSE;

    try
    {
        CString strSQL;
        strSQL.Format(_T("DELETE FROM Customers WHERE CustomerID = '%s'"), strCustomerID);
        
        m_pDatabase->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (...)
    {
        return FALSE;
    }
}

// 更新客户信息
BOOL CCustomerDBHelper::UpdateCustomer(const CustomerInfo& customer)
{
    if (m_pDatabase == nullptr)
        return FALSE;

    try
    {
        // 检查身份证号是否与其他客户重复
        if (!customer.IDCard.IsEmpty())
        {
            CString strSQL;
            strSQL.Format(_T("SELECT COUNT(*) FROM Customers WHERE IDCard = '%s' AND CustomerID != '%s'"), 
                         customer.IDCard, customer.CustomerID);
            
            CRecordset rs(m_pDatabase);
            rs.Open(CRecordset::forwardOnly, strSQL);
            
            if (!rs.IsEOF())
            {
                CDBVariant var;
                rs.GetFieldValue((short)0, var);
                long count = var.m_lVal;
                if (count > 0)
                    return FALSE;
            }
            rs.Close();
        }

        // 更新客户信息
        CString strSQL;
        strSQL.Format(_T("UPDATE Customers SET ")
                      _T("Name = '%s', IDCard = '%s', Phone = '%s', Email = '%s', Address = '%s', ")
                      _T("Occupation = '%s', Income = '%s', Preference = '%s', Status = '%s', Remark = '%s' ")
                      _T("WHERE CustomerID = '%s'"),
                      customer.Name, customer.IDCard, customer.Phone, customer.Email, customer.Address,
                      customer.Occupation, customer.Income, customer.Preference, customer.Status, customer.Remark,
                      customer.CustomerID);

        m_pDatabase->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (...)
    {
        return FALSE;
    }
}

// 查找客户
CustomerInfo* CCustomerDBHelper::FindCustomer(const CString& strCustomerID)
{
    if (m_pDatabase == nullptr)
        return nullptr;

    try
    {
        CString strSQL;
        strSQL.Format(_T("SELECT * FROM Customers WHERE CustomerID = '%s'"), strCustomerID);
        
        CRecordset rs(m_pDatabase);
        rs.Open(CRecordset::forwardOnly, strSQL);
        
        if (!rs.IsEOF())
        {
            CustomerInfo* pCustomer = new CustomerInfo();
            
            CString strValue;
            rs.GetFieldValue(_T("CustomerID"), strValue);
            pCustomer->CustomerID = strValue;
            
            rs.GetFieldValue(_T("Name"), strValue);
            pCustomer->Name = strValue;
            
            rs.GetFieldValue(_T("IDCard"), strValue);
            pCustomer->IDCard = strValue;
            
            rs.GetFieldValue(_T("Phone"), strValue);
            pCustomer->Phone = strValue;
            
            rs.GetFieldValue(_T("Email"), strValue);
            pCustomer->Email = strValue;
            
            rs.GetFieldValue(_T("Address"), strValue);
            pCustomer->Address = strValue;
            
            rs.GetFieldValue(_T("Occupation"), strValue);
            pCustomer->Occupation = strValue;
            
            rs.GetFieldValue(_T("Income"), strValue);
            pCustomer->Income = strValue;
            
            rs.GetFieldValue(_T("Preference"), strValue);
            pCustomer->Preference = strValue;
            
            rs.GetFieldValue(_T("Status"), strValue);
            pCustomer->Status = strValue;
            
            rs.GetFieldValue(_T("Remark"), strValue);
            pCustomer->Remark = strValue;
            
            rs.Close();
            return pCustomer;
        }
        
        rs.Close();
        return nullptr;
    }
    catch (...)
    {
        return nullptr;
    }
}

// 搜索客户
std::vector<CustomerInfo> CCustomerDBHelper::SearchCustomers(const CString& strKeyword)
{
    std::vector<CustomerInfo> results;
    
    if (m_pDatabase == nullptr)
        return results;

    try
    {
        CString strPattern;
        strPattern.Format(_T("%%%s%%"), strKeyword);
        
        CString strSQL;
        strSQL.Format(_T("SELECT * FROM Customers WHERE ")
                      _T("CustomerID LIKE '%s' OR Name LIKE '%s' OR Phone LIKE '%s' OR ")
                      _T("Address LIKE '%s' OR IDCard LIKE '%s'"),
                      strPattern, strPattern, strPattern, strPattern, strPattern);
        
        CRecordset rs(m_pDatabase);
        rs.Open(CRecordset::forwardOnly, strSQL);
        
        while (!rs.IsEOF())
        {
            CustomerInfo customer;
            CString strValue;
            
            rs.GetFieldValue(_T("CustomerID"), strValue);
            customer.CustomerID = strValue;
            
            rs.GetFieldValue(_T("Name"), strValue);
            customer.Name = strValue;
            
            rs.GetFieldValue(_T("IDCard"), strValue);
            customer.IDCard = strValue;
            
            rs.GetFieldValue(_T("Phone"), strValue);
            customer.Phone = strValue;
            
            rs.GetFieldValue(_T("Email"), strValue);
            customer.Email = strValue;
            
            rs.GetFieldValue(_T("Address"), strValue);
            customer.Address = strValue;
            
            rs.GetFieldValue(_T("Occupation"), strValue);
            customer.Occupation = strValue;
            
            rs.GetFieldValue(_T("Income"), strValue);
            customer.Income = strValue;
            
            rs.GetFieldValue(_T("Preference"), strValue);
            customer.Preference = strValue;
            
            rs.GetFieldValue(_T("Status"), strValue);
            customer.Status = strValue;
            
            rs.GetFieldValue(_T("Remark"), strValue);
            customer.Remark = strValue;
            
            results.push_back(customer);
            rs.MoveNext();
        }
        
        rs.Close();
    }
    catch (...)
    {
        // 出错时返回空结果
    }
    
    return results;
}

// 获取所有客户
std::vector<CustomerInfo> CCustomerDBHelper::GetAllCustomers()
{
    std::vector<CustomerInfo> results;
    
    if (m_pDatabase == nullptr)
        return results;

    try
    {
        CString strSQL = _T("SELECT * FROM Customers ORDER BY CustomerID");
        
        CRecordset rs(m_pDatabase);
        rs.Open(CRecordset::forwardOnly, strSQL);
        
        while (!rs.IsEOF())
        {
            CustomerInfo customer;
            CString strValue;
            
            rs.GetFieldValue(_T("CustomerID"), strValue);
            customer.CustomerID = strValue;
            
            rs.GetFieldValue(_T("Name"), strValue);
            customer.Name = strValue;
            
            rs.GetFieldValue(_T("IDCard"), strValue);
            customer.IDCard = strValue;
            
            rs.GetFieldValue(_T("Phone"), strValue);
            customer.Phone = strValue;
            
            rs.GetFieldValue(_T("Email"), strValue);
            customer.Email = strValue;
            
            rs.GetFieldValue(_T("Address"), strValue);
            customer.Address = strValue;
            
            rs.GetFieldValue(_T("Occupation"), strValue);
            customer.Occupation = strValue;
            
            rs.GetFieldValue(_T("Income"), strValue);
            customer.Income = strValue;
            
            rs.GetFieldValue(_T("Preference"), strValue);
            customer.Preference = strValue;
            
            rs.GetFieldValue(_T("Status"), strValue);
            customer.Status = strValue;
            
            rs.GetFieldValue(_T("Remark"), strValue);
            customer.Remark = strValue;
            
            results.push_back(customer);
            rs.MoveNext();
        }
        
        rs.Close();
    }
    catch (...)
    {
        // 出错时返回空结果
    }
    
    return results;
} 