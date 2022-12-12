#include <iostream>
#include <vector>
#include <algorithm>
#include "ModelData.h"
#include "Frontend.h"

ModelData::ModelData(int number_days, int number_shops, std::vector<CheckBox*>& check_boxes) :
    number_days_(number_days), number_shops_(number_shops) {
    for (int i = 0; i < check_boxes.size(); ++i) {
        used_products_[i] = check_boxes[i]->GetStatus();
        number_products_ += used_products_[i];
    }
    empty_ = (number_products_ == 0);
}

int ModelData::GetNumberDays() {
    return number_days_;
}

int ModelData::GetNumberShops() {
    return number_shops_;
}

int ModelData::GetNumberProducts() {
    return number_products_;
}

bool ModelData::IsEmpty() {
    return empty_;
}

bool ModelData::IsBeingProductUsed(int product_type) {
    return used_products_[product_type];
}

std::string FromIntToString(int value) {
    if (value == 0) {
        return "0";
    }
    std::string ans;
    bool is_lesser_zero = (value < 0);
    value = abs(value);
    while (value) {
        ans += (value % 10 + '0');
        value /= 10;
    }
    if (is_lesser_zero) ans += '-';
    std::reverse(ans.begin(), ans.end());
    return ans;
}

int FromStringToInt(std::string str) {
    if (str == "0") return 0;
    int ans = 0;
    if (str[0] == '-') {
        for (int i = 1; i < str.size(); ++i) {
            ans *= 10;
            ans += (str[i] - '0');
        }
        return -ans;
    }
    for (char z : str) {
        ans *= 10;
        ans += (z - '0');
    }
    return ans;
}

int shelf_lifes[17];
int GetShelfLife(int type) {
    return shelf_lifes[type];
}
