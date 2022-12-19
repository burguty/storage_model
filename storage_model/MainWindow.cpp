#include "MainWindow.h"

bool MainWindow::MainLoop(ModelData* data) {
    sf::RenderWindow window(sf::VideoMode(1650, 900), "Storage");
    sf::Font font_text, font_storage_room;
    font_text.loadFromFile("Data/ArialRegular.ttf");
    font_storage_room.loadFromFile("Data/CORBEL.ttf");
    storage = new Storage(data,
        30 + (1150 - 30) / 2 - 425,
        780 - 5 - 350 - 10,
        font_storage_room);
    info_field = new InfoField(1170, 30);
    button_stop = new Button(1170, 784, 250, 50, text_button_stop, font_text);

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
                info_field->ChangeMode(storage->Click(x, y));
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) {
                    // delete last 
                } else {
                    // event.text.unicode - символ
                }
            }
        }
        window.clear(sf::Color(230, 230, 230));
        DrawInterface(window);
        storage->draw(window);
        info_field->draw(window, font_text);
        button_stop->draw(window);
        window.display();
    }
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
}