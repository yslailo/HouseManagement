#pragma once
#include <vector>
#include <afxdb.h>

struct HouseInfo {
    int id;
    CString address, type, status, description;
    double area, price;
};

class HouseDBHelper {
public:
    static bool AddHouse(const HouseInfo& house);
    static bool UpdateHouse(const HouseInfo& house);
    static bool DeleteHouse(int houseID);
    static std::vector<HouseInfo> GetAllHouses();
    static std::vector<HouseInfo> SearchHouses(const CString& keyword);
    static bool IsHouseIDExist(const CString& houseID);
}; 