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

std::wstring product_name[17] = { L"Сахар" , L"Соль", L"Каша", L"Хлопья", L"Макароны", L"Чай", 
L"Шоколад", L"Кофе", L"Какао", L"Масло", L"Рыба", L"Творог", L"Молоко", L"Сыр",
L"Вино", L"Курица", L"Говядина"};

std::wstring GetProductName(int type) {
    return product_name[type];
}

int shelf_lifes[17] = {3, 2, 5};
int product_price[17] = {90, 80, 120};
int count_at_box[17] = {20, 20, 10};
int GetCountAtBox(int type) {
    return count_at_box[type];
}
int GetProductPrice(int type) {
    return product_price[type];
}
int GetShelfLife(int type) {
    return shelf_lifes[type];
}
