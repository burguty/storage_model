#include "OpenWindow.h"

int space_between = 5;
int len_string = 100;
int height_string = 30;

OpenWindow::~OpenWindow() {}
ModelData* OpenWindow::MainLoop() {
    sf::RenderWindow window(sf::VideoMode(1100, 800), "Input model");
    sf::Font font;
    font.loadFromFile("Data/ArialRegular.ttf");

    int space_vert = (800 - height_string * 20) / 21;
    TextLine string_number_days(space_between, space_vert, 0, 0, L"Введите кол-во дней:", font);
    InputLine input_number_days(2 * space_between + len_string, space_vert, len_string, height_string, font);
    TextLine string_number_shops(space_between, 2 * space_vert + height_string, len_string, height_string, 
        L"Введите кол-во магазинов:", font);
    InputLine input_number_shops(2 * space_between + len_string, 2 * space_vert + height_string, 
        len_string, height_string, font);
    TextLine* name_product[17];
    CheckBox* check_boxes[17];
    InputLine* input_quantity[17];
    for (int i = 0; i < 17; ++i) {
        int y_coord = space_vert * (i + 3) + height_string * (i + 2);
        name_product[i] = new TextLine(space_between, y_coord, len_string, height_string, 
            GetProductName(i), font, 24, sf::Color::Black);
        check_boxes[i] = new CheckBox(2 * space_between + len_string, y_coord, height_string, height_string);
        input_quantity[i] = new InputLine(3 * space_between + len_string + height_string, y_coord, 
            len_string, height_string, font);
    }
    std::wstring start_button_string = L"Запустить";
    Button start_button(20 * space_vert + 19 * height_string, 1100 - space_between - len_string, 
        len_string, height_string, start_button_string, font);

    int operation = 0;
    while (window.isOpen()) {
        window.clear(sf::Color(230, 230, 230));
        input_number_days.draw(window);
        input_number_shops.draw(window);
        for (int i = 0; i < 17; ++i) {
            name_product[i]->draw(window);
            check_boxes[i]->draw(window);
            input_quantity[i]->draw(window);
        }
        start_button.draw(window);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return nullptr;
            }
            else if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                
            }
        }
        window.display();
    }
}