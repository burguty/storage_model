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
    void ClearMemory();
    std::wstring text_button_stop = L"Остановить модель";
    Storage* storage;
    InfoField* info_field;
    Button* button_stop;
};