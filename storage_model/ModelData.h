#pragma once
#include <iostream>
#include <string>
#include <vector>

class ModelData {
public:
    ModelData(int number_days, int number_shops, std::vector<CheckBox*>& check_boxes);
    int GetNumberDays();
    int GetNumberShops();
    int GetNumberProducts();
    bool IsEmpty();
    bool IsBeingProductUsed(int product_type);
private:
    int number_days_;
    int number_shops_;
    int number_products_ = 0;
    bool used_products_[20];
    bool empty_ = true;
};

std::string FromIntToString(int value);
int FromStringToInt(std::string str);