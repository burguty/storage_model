#pragma once
#include <iostream>
#include <string>

class ModelData {
public:
    // конструктор
    // деструктор
    // Гетеры параметров
    bool IsBeingProductUsed(int product_type);
private:
    // параметры модели
    // массив используемых товаров
    bool empty_;
};