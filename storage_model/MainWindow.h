#pragma once
#include "Products.h"
#include "ModelData.h"
#include "InfoField.h"

class MainWindow {
public:
    MainWindow() = default;
    bool MainLoop(ModelData* data);
private:
    void DrawInterface(sf::RenderWindow& window);
};