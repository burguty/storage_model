#include "OpenWindow.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFMLworks");

    sf::Font font;
    font.loadFromFile("Data/ArialRegular.ttf");
    InputLine inputline(100, 100, 300, 100, font);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed 
                && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                inputline.Click(x, y);
            } 
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8)
                    inputline.DeleteSymbol();
                else
                    inputline.TypeSymbol(event.text.unicode);
            }
        }
        window.clear(sf::Color::White);
        inputline.draw(window);
        window.display();
    }


    }
    return 0;
}