#include "ModelData.h"
#include "Frontend.h"

ModelData::ModelData(int number_days, int number_shops, 
    CheckBox* check_boxes[], int count_products[]) : 
    number_days_(number_days), number_shops_(number_shops) {
    for (int i = 0; i < 17; ++i) {
        used_products_[i] = check_boxes[i]->GetStatus();
        number_products_ += used_products_[i];
        count_products_[i] = count_products[i];
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

int ModelData::GetCountProduct(int type) {
    return count_products_[type];
}

std::wstring PRODUCT_NAME[17] = {L"Сахар" , L"Соль", L"Каша", L"Хлопья", L"Макароны", L"Чай",
L"Шоколад", L"Кофе", L"Какао", L"Масло", L"Рыба", L"Творог", L"Молоко", L"Сыр",
L"Вино", L"Курица", L"Говядина"};
int SHELF_LIFE[17] = {3, 2, 5};
int MAX_BOX_COUNT[15] = {15, 15, 15};
int PRODUCT_PRICE[17] = {20, 30, 50, 60, 10, 100, 200, 150, 300, 250, 140, 420, 260, 780, 960, 150, 120};
int COUNT_AT_BOX[17] = {20, 20, 10};
int PURCHASE_PRICE[15] = {5, 5, 15};

std::wstring GetProductName(int type) {
    return PRODUCT_NAME[type];
}

int GetCountAtBox(int type) {
    return COUNT_AT_BOX[type];
}
int GetProductPrice(int type) {
    return PRODUCT_PRICE[type];
}
int GetShelfLife(int type) {
    return SHELF_LIFE[type];
}
int GetPurchasePrice(int type) {
    return PURCHASE_PRICE[type];
}

std::wstring IntToString(int number) {
    if (number == 0)
        return L"0";
    std::wstring result = L"";
    bool flag = false;
    if (number < 0)
        flag = true, number *= -1;
    while (number != 0) {
        result.push_back((number % 10) + '0');
        number /= 10;
    }
    if (flag)
        result.push_back('-');
    std::reverse(result.begin(), result.end());
    return result;
}

int GetMaxBoxCount(int type) {
    return MAX_BOX_COUNT[type];
}