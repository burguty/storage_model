#pragma once
#include "ModelData.h"
#include "Frontend.h"

class OpenWindow {
public:
    OpenWindow();
    virtual ~OpenWindow();
    ModelData* MainLoop();
private:
};