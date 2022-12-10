#pragma once
#include "ModelData.h"
#include "Frontend.h"

class InputWindow {
public:
    InputWindow();
    ~InputWindow();
    ModelData* MainLoop();
private:
    int GetNumber(std::string s); // выброс исключения
};