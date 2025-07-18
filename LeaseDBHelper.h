#pragma once
#include <vector>
#include <afxdb.h>

// 租赁合同信息结构体
struct LeaseInfo {
    int leaseID;
    int houseID;
    CString customerID;
    COleDateTime startDate;
    COleDateTime endDate;
    double rent;
    double deposit;
    CString status;
    CString remark;
};

// 租金收取记录结构体
struct LeasePaymentInfo {
    int paymentID;
    int leaseID;
    COleDateTime payDate;
    double amount;
    CString status;
    CString remark;
};

// 租赁管理数据库操作类
class LeaseDBHelper {
public:
    // 租赁合同操作
    static bool AddLease(const LeaseInfo& lease);
    static bool UpdateLease(const LeaseInfo& lease);
    static bool DeleteLease(int leaseID);
    static std::vector<LeaseInfo> GetAllLeases();
    static LeaseInfo GetLeaseByID(int leaseID);

    // 租金收取记录操作
    static bool AddLeasePayment(const LeasePaymentInfo& payment);
    static std::vector<LeasePaymentInfo> GetPaymentsByLeaseID(int leaseID);
    static bool DeleteLeasePayment(int paymentID);
}; 