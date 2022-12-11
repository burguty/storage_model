#include "MainWindow.h"

bool MainWindow::MainLoop(ModelData* data) {
    sf::RenderWindow window(sf::VideoMode(1100, 800), "Storage");
    sf::Font font;
    font.loadFromFile("Data/ArialRegular.ttf");

    IClickable* info = nullptr;
    ProductBatch* batch = new ProductBatch(0, 100, 5, 100, 3, 100, 100);
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
                info = batch->Click(x, y);
            }
            if (event.type == sf::Event::TextEntered) {
                // if (event.text.unicode == 8)
            }
        }
        window.clear(sf::Color(230, 230, 230));
        DrawInterface(window);
        batch->draw(window);
        if (info != nullptr)
            info->DrawInformation(window, 780, 30);
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