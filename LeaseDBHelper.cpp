#include "pch.h"
#include "LeaseDBHelper.h"
#include "HouseDBHelper.h" // 引入GetConnStr声明

#ifndef DBVT_TIMESTAMP
#define DBVT_TIMESTAMP 7
#endif

// 添加租赁合同
bool LeaseDBHelper::AddLease(const LeaseInfo& lease) {
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("INSERT INTO Leases (HouseID, CustomerID, StartDate, EndDate, Rent, Deposit, Status, Remark) "
                 "VALUES (%d, '%s', '%s', '%s', %f, %f, '%s', '%s')"),
        lease.houseID, lease.customerID,
        lease.startDate.Format(_T("%Y-%m-%d")), lease.endDate.Format(_T("%Y-%m-%d")),
        lease.rent, lease.deposit, lease.status, lease.remark);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

// 更新租赁合同
bool LeaseDBHelper::UpdateLease(const LeaseInfo& lease) {
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("UPDATE Leases SET HouseID=%d, CustomerID='%s', StartDate='%s', EndDate='%s', Rent=%f, Deposit=%f, Status='%s', Remark='%s' WHERE LeaseID=%d"),
        lease.houseID, lease.customerID,
        lease.startDate.Format(_T("%Y-%m-%d")), lease.endDate.Format(_T("%Y-%m-%d")),
        lease.rent, lease.deposit, lease.status, lease.remark, lease.leaseID);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

// 删除租赁合同
bool LeaseDBHelper::DeleteLease(int leaseID) {
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("DELETE FROM Leases WHERE LeaseID=%d"), leaseID);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

// 获取所有租赁合同
std::vector<LeaseInfo> LeaseDBHelper::GetAllLeases() {
    std::vector<LeaseInfo> leases;
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return leases;
    CRecordset rs(&db);
    rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM Leases ORDER BY LeaseID DESC"));
    while (!rs.IsEOF()) {
        LeaseInfo lease;
        CDBVariant var;
        rs.GetFieldValue(_T("LeaseID"), var); lease.leaseID = var.m_iVal;
        rs.GetFieldValue(_T("HouseID"), var); lease.houseID = var.m_iVal;
        rs.GetFieldValue(_T("CustomerID"), lease.customerID);
        // 起租日期
        rs.GetFieldValue(_T("StartDate"), var);
        if (var.m_pdate) {
            TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)var.m_pdate;
            lease.startDate.SetDateTime(pts->year, pts->month, pts->day, pts->hour, pts->minute, pts->second);
        } else {
            lease.startDate.SetStatus(COleDateTime::null);
        }
        // 退租日期
        rs.GetFieldValue(_T("EndDate"), var);
        if (var.m_pdate) {
            TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)var.m_pdate;
            lease.endDate.SetDateTime(pts->year, pts->month, pts->day, pts->hour, pts->minute, pts->second);
        } else {
            lease.endDate.SetStatus(COleDateTime::null);
        }
        rs.GetFieldValue(_T("Rent"), var); lease.rent = var.m_dblVal;
        rs.GetFieldValue(_T("Deposit"), var); lease.deposit = var.m_dblVal;
        rs.GetFieldValue(_T("Status"), lease.status);
        rs.GetFieldValue(_T("Remark"), lease.remark);
        leases.push_back(lease);
        rs.MoveNext();
    }
    rs.Close();
    db.Close();
    return leases;
}

// 根据ID获取租赁合同
LeaseInfo LeaseDBHelper::GetLeaseByID(int leaseID) {
    LeaseInfo lease = {};
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return lease;
    CString sql;
    sql.Format(_T("SELECT * FROM Leases WHERE LeaseID=%d"), leaseID);
    CRecordset rs(&db);
    rs.Open(CRecordset::forwardOnly, sql);
    if (!rs.IsEOF()) {
        CDBVariant var;
        rs.GetFieldValue(_T("LeaseID"), var); lease.leaseID = var.m_iVal;
        rs.GetFieldValue(_T("HouseID"), var); lease.houseID = var.m_iVal;
        rs.GetFieldValue(_T("CustomerID"), lease.customerID);
        // 起租日期
        rs.GetFieldValue(_T("StartDate"), var);
        if (var.m_pdate) {
            TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)var.m_pdate;
            lease.startDate.SetDateTime(pts->year, pts->month, pts->day, pts->hour, pts->minute, pts->second);
        } else {
            lease.startDate.SetStatus(COleDateTime::null);
        }
        // 退租日期
        rs.GetFieldValue(_T("EndDate"), var);
        if (var.m_pdate) {
            TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)var.m_pdate;
            lease.endDate.SetDateTime(pts->year, pts->month, pts->day, pts->hour, pts->minute, pts->second);
        } else {
            lease.endDate.SetStatus(COleDateTime::null);
        }
        rs.GetFieldValue(_T("Rent"), var); lease.rent = var.m_dblVal;
        rs.GetFieldValue(_T("Deposit"), var); lease.deposit = var.m_dblVal;
        rs.GetFieldValue(_T("Status"), lease.status);
        rs.GetFieldValue(_T("Remark"), lease.remark);
    }
    rs.Close();
    db.Close();
    return lease;
}

// 添加租金收取记录
bool LeaseDBHelper::AddLeasePayment(const LeasePaymentInfo& payment) {
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("INSERT INTO LeasePayments (LeaseID, PayDate, Amount, Status, Remark) VALUES (%d, '%s', %f, '%s', '%s')"),
        payment.leaseID, payment.payDate.Format(_T("%Y-%m-%d")), payment.amount, payment.status, payment.remark);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

// 获取某合同下所有租金收取记录
std::vector<LeasePaymentInfo> LeaseDBHelper::GetPaymentsByLeaseID(int leaseID) {
    std::vector<LeasePaymentInfo> payments;
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return payments;
    CString sql;
    sql.Format(_T("SELECT * FROM LeasePayments WHERE LeaseID=%d ORDER BY PayDate DESC"), leaseID);
    CRecordset rs(&db);
    rs.Open(CRecordset::forwardOnly, sql);
    while (!rs.IsEOF()) {
        LeasePaymentInfo pay;
        CDBVariant var;
        rs.GetFieldValue(_T("PaymentID"), var); pay.paymentID = var.m_iVal;
        rs.GetFieldValue(_T("LeaseID"), var); pay.leaseID = var.m_iVal;
        rs.GetFieldValue(_T("PayDate"), var);
        if (var.m_pdate) {
            TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)var.m_pdate;
            pay.payDate.SetDateTime(pts->year, pts->month, pts->day, pts->hour, pts->minute, pts->second);
        } else {
            pay.payDate.SetStatus(COleDateTime::null);
        }
        rs.GetFieldValue(_T("Amount"), var); pay.amount = var.m_dblVal;
        rs.GetFieldValue(_T("Status"), pay.status);
        rs.GetFieldValue(_T("Remark"), pay.remark);
        payments.push_back(pay);
        rs.MoveNext();
    }
    rs.Close();
    db.Close();
    return payments;
}

// 删除租金收取记录
bool LeaseDBHelper::DeleteLeasePayment(int paymentID) {
    CDatabase db;
    if (!db.OpenEx(_T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;"), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("DELETE FROM LeasePayments WHERE PaymentID=%d"), paymentID);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
} 