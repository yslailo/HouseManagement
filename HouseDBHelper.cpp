#include "pch.h"
#include "HouseDBHelper.h"

CString GetConnStr() {
    return _T("Driver={SQL Server};Server=localhost;Database=HouseManagement;Trusted_Connection=yes;");
}

bool HouseDBHelper::AddHouse(const HouseInfo& house) {
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("INSERT INTO House (Address, Type, Area, Price, Status, Description) VALUES ('%s','%s',%f,%f,'%s','%s')"),
        house.address, house.type, house.area, house.price, house.status, house.description);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

bool HouseDBHelper::UpdateHouse(const HouseInfo& house) {
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("UPDATE House SET Address='%s', Type='%s', Area=%f, Price=%f, Status='%s', Description='%s' WHERE HouseID=%d"),
        house.address, house.type, house.area, house.price, house.status, house.description, house.id);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

bool HouseDBHelper::DeleteHouse(int houseID) {
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("DELETE FROM House WHERE HouseID=%d"), houseID);
    try {
        db.ExecuteSQL(sql);
        db.Close();
        return true;
    } catch (...) {
        db.Close();
        return false;
    }
}

std::vector<HouseInfo> HouseDBHelper::GetAllHouses() {
    std::vector<HouseInfo> houses;
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return houses;
    CRecordset rs(&db);
    rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM House"));
    while (!rs.IsEOF()) {
        HouseInfo h;
        CDBVariant var;
        
        rs.GetFieldValue(_T("HouseID"), var);
        h.id = var.m_iVal;
        
        rs.GetFieldValue(_T("Address"), h.address);
        rs.GetFieldValue(_T("Type"), h.type);
        
        rs.GetFieldValue(_T("Area"), var);
        h.area = var.m_dblVal;
        
        rs.GetFieldValue(_T("Price"), var);
        h.price = var.m_dblVal;
        
        rs.GetFieldValue(_T("Status"), h.status);
        rs.GetFieldValue(_T("Description"), h.description);
        
        houses.push_back(h);
        rs.MoveNext();
    }
    rs.Close();
    db.Close();
    return houses;
}

std::vector<HouseInfo> HouseDBHelper::SearchHouses(const CString& keyword) {
    std::vector<HouseInfo> houses;
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return houses;
    CString sql;
    sql.Format(_T("SELECT * FROM House WHERE Address LIKE '%%%s%%' OR Type LIKE '%%%s%%' OR Status LIKE '%%%s%%'"), keyword, keyword, keyword);
    CRecordset rs(&db);
    rs.Open(CRecordset::forwardOnly, sql);
    while (!rs.IsEOF()) {
        HouseInfo h;
        CDBVariant var;
        
        rs.GetFieldValue(_T("HouseID"), var);
        h.id = var.m_iVal;
        
        rs.GetFieldValue(_T("Address"), h.address);
        rs.GetFieldValue(_T("Type"), h.type);
        
        rs.GetFieldValue(_T("Area"), var);
        h.area = var.m_dblVal;
        
        rs.GetFieldValue(_T("Price"), var);
        h.price = var.m_dblVal;
        
        rs.GetFieldValue(_T("Status"), h.status);
        rs.GetFieldValue(_T("Description"), h.description);
        
        houses.push_back(h);
        rs.MoveNext();
    }
    rs.Close();
    db.Close();
    return houses;
}

bool HouseDBHelper::IsHouseIDExist(const CString& houseID) {
    CDatabase db;
    if (!db.OpenEx(GetConnStr(), CDatabase::noOdbcDialog)) return false;
    CString sql;
    sql.Format(_T("SELECT COUNT(*) FROM House WHERE HouseID=%s"), houseID);
    CRecordset rs(&db);
    try {
        rs.Open(CRecordset::forwardOnly, sql);
        if (!rs.IsEOF()) {
            CDBVariant var;
            rs.GetFieldValue((short)0, var);
            long count = var.m_lVal;
            rs.Close();
            db.Close();
            return count > 0;
        }
        rs.Close();
        db.Close();
        return false;
    } catch (...) {
        rs.Close();
        db.Close();
        return false;
    }
} 