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
    void ClearStorage();
    void SendProducts();
    void DrawMovableObject(sf::RenderWindow& window);
    Request* GetNextRequest();
    Storage* storage;
    InfoField* info_field;
    Button* button_stop;
    Button* button_create_requests;
    Button* button_order;
    std::vector<Shop*>shops;
    std::vector<Request*>requests;
    std::vector<Request*>deliveries;
    std::deque<IMovable*>on_the_move;
    int condition = 0;
};