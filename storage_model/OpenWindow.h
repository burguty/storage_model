#pragma once
#include "ModelData.h"
#include "Frontend.h"

class OpenWindow {
public:
    OpenWindow() = default;
    ~OpenWindow();
    ModelData* MainLoop();
private:
    std::wstring number_days_sfml_ = L"";
    std::wstring number_shops_sfml_ = L"";
    std::wstring count_products_sfml_[17];
};
