#pragma once
#include "Products.h"
#include "ModelData.h"
#include "InfoField.h"
#include "Shop.h"

class MainWindow {
public:
    MainWindow() = default;
    bool MainLoop(ModelData* data);
private:
    void DrawInterface(sf::RenderWindow& window);
    void ClearMemory();
    Request* GetNextRequest();
    std::wstring text_button_stop = L"Остановить модель";
    Storage* storage;
    InfoField* info_field;
    Button* button_stop;
    Button* button_create_requests;
    std::vector<Shop*>shops;
    std::vector<Request*>requests;
    std::vector<Request*>deliveries;
};