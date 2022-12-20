#include "MainWindow.h"

bool MainWindow::MainLoop(ModelData* data) {
    std::mt19937 gen(0);

    sf::RenderWindow window(sf::VideoMode(1650, 900), "Storage");
    sf::Font font_text, font_storage_room;
    font_text.loadFromFile("Data/ArialRegular.ttf");
    font_storage_room.loadFromFile("Data/CORBEL.ttf");

    shops.resize(data->GetNumberShops());
    shops[0] = new Shop(0, font_text, gen);
    for (int i = 1; i < data->GetNumberShops(); i++) {
        shops[i] = new Shop((i % 2 == 0 ? -1 : 1) * (i + 1) / 2, font_text, gen);
    }
    storage = new Storage(data,
        30 + (1150 - 30) / 2 - 425,
        780 - 5 - 350 - 10,
        font_storage_room);
    info_field = new InfoField(storage, 1170, 30, font_text);
    button_stop = new Button(1170, 784, 250, 50, text_button_stop, font_text);
    button_create_requests = new Button(30, 800, 220, 50, L"Принять заказы", font_text, 25);
    int day = 0;
    int condition = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                ClearMemory();
                return true;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                if (button_stop->Click(x, y)) {
                    window.close();
                    ClearMemory();
                    return false;
                }
                if (info_field->Click(x, y)) {
                    if (condition == 3) {
                        deliveries.push_back(requests.back());
                        storage->ProductShipments(requests.back()->GetProductType(),
                            requests.back()->approved_count_ * GetCountAtBox(requests.back()->GetProductType()));
                        requests.pop_back();
                        info_field->ChangeMode(GetNextRequest());
                    }
                    continue;
                }
                if (condition == 2) {
                    if (button_create_requests->Click(x, y)) {
                        condition = 3;
                        info_field->ChangeMode(nullptr);
                        for (int i = 0; i < shops.size(); i++) {
                            requests.push_back(shops[i]->MakeRequest(storage));
                        }
                        info_field->ChangeMode(GetNextRequest());
                        continue;
                    }
                    info_field->ChangeMode(storage->Click(x, y));
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) {
                    info_field->DeleteSymbol();
                } else {
                    info_field->TypeSymbol(event.text.unicode);
                }
            }
        }
        window.clear(sf::Color(230, 230, 230));
        // развоз
        if (condition == 0) {
            condition = 1;
        }
        // завоз
        if (condition == 1) {
            condition = 2;
        }
        // ожидание заказов и изменение цены
        if (condition == 2) {
            button_create_requests->draw(window);
        }
        // обработка заказов
        if (condition == 3) {
            if (requests.empty())
                condition = 4;
        }
        // заказ новых товаров
        if (condition == 4) {
            condition = 5;
        }
        if (condition == 5) {
            day += 1;
            storage->GoToTheNextDay();
            for (ProductBatch* batch : storage->Clearing())
                delete batch;
            condition = 0;
        }
        DrawInterface(window);
        storage->draw(window);
        info_field->draw(window);
        button_stop->draw(window);
        for (int i = 0; i < shops.size(); i++) {
            shops[i]->draw(window);
        }
        window.display();
    }
}

Request* MainWindow::GetNextRequest() {
    while (!requests.empty() && requests.back() == nullptr)
        requests.pop_back();
    if (requests.empty())
        return nullptr;
    return requests.back();
}

void MainWindow::DrawInterface(sf::RenderWindow& window) {
    sf::RectangleShape field;
    field.setSize(sf::Vector2f(1150 - 30, 780 - 30));
    field.setPosition(30, 30);
    field.setOutlineThickness(5);
    field.setOutlineColor(sf::Color::Black);
    field.setFillColor(sf::Color::White);
    window.draw(field);
}

void MainWindow::ClearMemory() {
    delete storage;
    delete info_field;
    delete button_stop;
    delete button_create_requests;
}