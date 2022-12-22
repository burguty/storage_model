#pragma once
#include "Frontend.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class ModelData {
public:
    ModelData(int number_days, int number_shops, CheckBox* check_boxes[], int count_products[]);
    int GetNumberDays();
    int GetNumberShops();
    int GetNumberProducts();
    bool IsEmpty();
    int GetCountProduct(int type);
    bool IsBeingProductUsed(int product_type);
private:
    int number_days_;
    int number_shops_;
    int number_products_ = 0;
    bool used_products_[17];
    int count_products_[17];
    bool empty_ = true;
};

int GetProductPrice(int type);
int GetCountAtBox(int type);
int GetShelfLife(int type);
int GetPurchasePrice(int type);
int GetMaxBoxCount(int type);
std::wstring GetProductName(int type);
std::wstring IntToString(int number);
