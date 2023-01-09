#include "MainWindow.h"

bool MainWindow::MainLoop(ModelData* data) {
    std::mt19937 gen(0);
    sf::Clock clock;
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
    int day = 0;
    condition = 0;
    info_field = new InfoField(storage, 1170, 30 + 30 + 15, font_text);
    button_stop = new Button(30, 800, 250, 50, 
        L"Остановить модель", font_text, 25);
    button_order = new Button(30 + 250 + 15, 800, 220, 50, 
        L"Заказать", font_text, 25);
    button_create_requests = new Button(30 + 250 + 15, 800, 220, 50, 
        L"Принять заказы", font_text, 25);
    text_time_of_purchase = new TextLine(30+250+15 + 220 + 15, 800, 100, 50,
        L"", font_text, 25);
    text_day = new TextLine(1170 - 10, 20, 100, 30,
        L"День номер " + IntToString(day + 1), font_text, 30);
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
                if (condition == 3) {
                    if (info_field->Click(x, y)) {
                        deliveries.push_back(requests.back());
                        requests.pop_back();
                        info_field->ChangeMode(GetNextRequest());
                    }
                    continue;
                }
                if (info_field->Click(x, y)) {
                    continue;
                }
                if (condition == 4) {
                    if (button_order->Click(x, y)) {
                        if (storage->IsOrderCountsCorrect()) {
                            info_field->ChangeMode(nullptr);
                            storage->StopPurchasePhase();
                            condition = 5;
                        }
                        continue;
                    }
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
                    bool click = false;
                    for (int i = 0; i < shops.size(); i++) {
                        IClickable* result = shops[i]->Click(x, y);
                        if (result != nullptr) {
                            click = true;
                            info_field->ChangeMode(result);
                            break;
                        }
                    }
                    if (click)
                        continue;
                }
                info_field->ChangeMode(storage->Click(x, y));
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) {
                    info_field->TypeSymbol();
                } else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    info_field->TypeSymbol(event.text.unicode);
                }
            }
        }
        window.clear(sf::Color(230, 230, 230));
        long double time = clock.restart().asSeconds();
        // Анимация развоза товаров
        if (condition == 0) {
            if (!on_the_move.empty()) {
                auto it = on_the_move.begin();
                while (it != on_the_move.end()) {
                    if ((*it)->Move(time)) {
                        delete (*it);
                        it = on_the_move.erase(it);
                    } else
                        ++it;
                }
            } else {
                condition = 1;
                on_the_move = storage->GoToTheNextDayCars();
            }
        }
        // анимация завоза
        if (condition == 1) {
            for (int i = 0; i < on_the_move.size(); i++) {
                if (on_the_move[i]->Move(time) && i == 0) {
                    i--;
                    on_the_move.pop_front();
                }
            }
            if (on_the_move.empty()) {
                if (day == data->GetNumberDays()) {
                    condition = -1;
                    text_day->SetColor(sf::Color::Red);
                    text_day->SetText(L"Моделирование заверешено");
                } else {
                    condition = 2;
                }
            }
        }
        // ожидание заказов и изменение цены
        if (condition == 2) {
            button_create_requests->draw(window);
        }
        // обработка заказов
        if (condition == 3) {
            if (requests.empty()) {
                condition = 4;
                time_of_purchase = gen() % 5 + 1;
                storage->StartPurchasePhase(time_of_purchase);
                text_time_of_purchase->SetText(L"Доставка через " +
                    IntToString(time_of_purchase) +
                    (time_of_purchase == 1 ? L" день" : (time_of_purchase == 5 ? L" дней" : L" дня")));
            }
        }
        // заказ новых товаров
        if (condition == 4) {
            text_time_of_purchase->draw(window);
            button_order->draw(window);
        }
        // +1 день и вывоз просрочки
        if (condition == 5) {
            day += 1;
            text_day->SetText(L"День номер " + IntToString(day + 1));
            storage->GoToTheNextDay();
            ClearStorage();
            condition = 6;
        }
        // Анимация удаления
        if (condition == 6) {
            if (!on_the_move.empty()) {
                auto it = on_the_move.begin();
                while (it != on_the_move.end()) {
                    if ((*it)->Move(time)) {
                        delete (*it);
                        it = on_the_move.erase(it);
                    } else
                        ++it;
                }
            } else {
                condition = 0;
                SendProducts();
            }
        }
        DrawInterface(window);
        storage->draw(window);
        info_field->draw(window);
        text_day->draw(window);
        button_stop->draw(window);
        DrawMovableObject(window);
        for (int i = 0; i < shops.size(); i++) {
            shops[i]->draw(window);
        }
        window.display();
    }
}

void MainWindow::ClearStorage() {
    std::vector<ProductBatch*>trash = storage->Clearing();
    for (ProductBatch* batch : trash) {
        batch->StartMoving(1000, 1000, 2);
        on_the_move.push_back(static_cast<IMovable*>(batch));
    }
}

void MainWindow::SendProducts() {
    for (Request* delivery : deliveries) {
        int product_type = delivery->GetProductType();
        for (ProductBatch* batch : storage->ProductShipments(product_type, delivery->approved_count_)) {
            batch->SetColor(sf::Color(0, 191, 255, 120));
            batch->StartMoving(delivery->GetTargetX(), delivery->GetTargetY(), 2);
            delivery->AddProductBatch(batch);
            on_the_move.push_back(static_cast<IMovable*>(batch));
        }
    }
    while (!deliveries.empty()) {
        delete deliveries.back();
        deliveries.pop_back();
    }
}

void MainWindow::DrawMovableObject(sf::RenderWindow& window) {
    for (IMovable* obj : on_the_move)
        obj->draw(window);
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
    delete button_order;
    delete text_day;
    delete text_time_of_purchase;
    for (IMovable* movable : on_the_move)
        delete movable;
    for (Shop* shop : shops)
        delete shop;
    for (Request* request : requests)
        delete request;
    for (Request* delivery : deliveries)
        delete delivery;
}