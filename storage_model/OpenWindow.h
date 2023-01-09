#pragma once
#include "ModelData.h"
#include "Frontend.h"

class OpenWindow {
public:
    OpenWindow() = default;
    ~OpenWindow();
    ModelData* MainLoop();
private:
    int number_days_ = 0;
    int number_shops_ = 0;
    int count_products_[15];
};
