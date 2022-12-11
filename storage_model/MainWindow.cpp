#include "MainWindow.h"

bool MainWindow::MainLoop(ModelData* data) {
    sf::RenderWindow window(sf::VideoMode(1100, 800), "Storage");
    sf::Font font;
    font.loadFromFile("Data/ArialRegular.ttf");

    InfoField* info_field = new InfoField(780, 30);
    StorageRoom* room = new StorageRoom(0, 100, 100, font);
    std::wstring add = L"Добавить  партию", del = L"Удалить";
    Button* add_button = new Button(30, 700, 250, 80, add, font);
    Button* delete_button = new Button(300, 700, 250, 80, del, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return true;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                if (add_button->Click(x, y)) {
                    room->AddDelivery(new ProductBatch(0, 100, 0, 30, 2, 0, 0));
                } else if (delete_button->Click(x, y)) {
                    room->ProductShipments(90);
                } else {
                    info_field->ChangeMode(room->Click(x, y));
                }
            }
            if (event.type == sf::Event::TextEntered) {
                // if (event.text.unicode == 8)
            }
        }
        window.clear(sf::Color(230, 230, 230));
        DrawInterface(window);
        room->draw(window);
        info_field->draw(window);
        add_button->draw(window);
        delete_button->draw(window);
        window.display();
    }
}

void MainWindow::DrawInterface(sf::RenderWindow& window) {
    sf::RectangleShape field;
    field.setSize(sf::Vector2f(700, 650));
    field.setPosition(30, 30);
    field.setOutlineThickness(5);
    field.setOutlineColor(sf::Color::Black);
    field.setFillColor(sf::Color::White);
    window.draw(field);
}