#include "OpenWindow.h"

int space_between = 70;
int len_input = 250;
int len_string = 150;
int height = 30;

bool CheckInputedFields(CheckBox* check_boxes[], InputLine* inputing_field_code[]) {
    for (int i = 0; i < 17; ++i) {
        if (i >= 2) {
            if (!check_boxes[i - 2]->GetStatus() || (check_boxes[i - 2]->GetStatus() &&
                !inputing_field_code[i]->GetText().empty() && 
                std::stoi(inputing_field_code[i]->GetText()) <= GetMaxBoxCount(i - 2) && 
                std::stoi(inputing_field_code[i]->GetText()) >= 0)) {
                continue;
            }
        }
        else {
            if (!inputing_field_code[i]->GetText().empty() && 
                std::stoi(inputing_field_code[i]->GetText()) > 0) {
                if (i == 0 && std::stoi(inputing_field_code[i]->GetText()) <= 30) continue;
                else if (i == 1 && std::stoi(inputing_field_code[i]->GetText()) <= 7) continue;
            }
        }
        return false;
    }
    return true;
}

OpenWindow::~OpenWindow() {}

ModelData* OpenWindow::MainLoop() {
    sf::RenderWindow window(sf::VideoMode(735, 750), "Input model");
    sf::Font font;
    font.loadFromFile("Data/ArialRegular.ttf");

    int space_vert = (800 - height * 20) / 21;
    TextLine* string_number_days = new TextLine(space_between, space_vert + 10, 0, 0, 
        L"Введите кол-во дней:", font, 22);
    InputLine* input_number_days = new InputLine(2 * space_between + len_input, space_vert, 
        len_input, height, font, 20);
    TextLine* string_number_shops = new TextLine(space_between, 2 * space_vert + height, 
        len_string, height, L"Введите кол-во магазинов:", font, 22);
    InputLine* input_number_shops = new InputLine(2 * space_between + len_input, 2 * space_vert + height, 
        len_input, height, font, 20);
    TextLine* name_product[15];
    CheckBox* check_boxes[15];
    InputLine* input_quantity[15];
    for (int i = 0; i < 15; ++i) {
        int y_coord = space_vert * (i + 3) + height * (i + 2);
        name_product[i] = new TextLine(space_between, y_coord, len_string, height, 
            GetProductName(i), font, 22);
        check_boxes[i] = new CheckBox(2 * space_between + len_string, y_coord, height, height);
        input_quantity[i] = new InputLine(3 * space_between + len_string + height, y_coord, 
            len_input, height, font, 20);
    }
    std::wstring start_button_string = L"Запустить";
    Button* start_button = new Button(735 - space_between - 130, 20 * space_vert + 17 * height,
        130, height + 3, start_button_string, font, 22);

    InputLine* inputing_field_code[17];
    inputing_field_code[0] = input_number_days;
    inputing_field_code[1] = input_number_shops;
    for (int i = 0; i < 15; ++i) {
        inputing_field_code[2 + i] = input_quantity[i];
    }

    int current_inputing = -1;
    while (window.isOpen()) {
        window.clear(sf::Color(230, 230, 230));
        string_number_days->draw(window);
        string_number_shops->draw(window);
        input_number_days->draw(window);
        input_number_shops->draw(window);
        for (int i = 0; i < 15; ++i) {
            name_product[i]->draw(window);
            check_boxes[i]->draw(window);
            if (check_boxes[i]->GetStatus()) {
                input_quantity[i]->draw(window);
            }
        }
        start_button->draw(window);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return nullptr;
            }
            if (event.type == sf::Event::MouseButtonPressed && 
                event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x, y = event.mouseButton.y;

                if (start_button->Click(x, y) && 
                    CheckInputedFields(check_boxes, inputing_field_code)) {
                    window.close();
                    break;
                }
                
                for (int i = 0; i < 15; ++i) {
                    check_boxes[i]->Click(x, y);
                }

                bool is_any_input_clicked = false;
                for (int i = 0; i < 17; ++i) {
                    if (i >= 2 && i <= 16) {
                        if (check_boxes[i - 2]->GetStatus() &&
                            inputing_field_code[i]->Click(x, y)) {
                            if (current_inputing > -1) {
                                inputing_field_code[current_inputing]->SetStatus(false);
                                inputing_field_code[current_inputing]->SetTextColor(sf::Color::Black);
                            }
                            if (current_inputing != i) {
                                current_inputing = i;
                                is_any_input_clicked = true;
                                inputing_field_code[i]->SetTextColor(sf::Color::White);
                            }
                        }
                    }
                    else {
                        if (inputing_field_code[i]->Click(x, y)) {
                            if (current_inputing > -1) {
                                inputing_field_code[current_inputing]->SetStatus(false);
                                inputing_field_code[current_inputing]->SetTextColor(sf::Color::Black);
                            }
                            if (current_inputing != i) {
                                current_inputing = i;
                                is_any_input_clicked = true;
                                inputing_field_code[i]->SetTextColor(sf::Color::White);
                            }
                        }
                    }
                }

                if (!is_any_input_clicked) {
                    if (current_inputing != -1) {
                        inputing_field_code[current_inputing]->SetStatus(false);
                        inputing_field_code[current_inputing]->SetTextColor(sf::Color::Black);
                    }
                    current_inputing = -1;
                }
            }
            if (current_inputing > -1 && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) {
                    inputing_field_code[current_inputing]->DeleteSymbol();
                }
                else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    inputing_field_code[current_inputing]->TypeSymbol(event.text.unicode);
                }
            }
        }
        window.display();
    }

    number_days_ = std::stoi(input_number_days->GetText());
    number_shops_ = std::stoi(input_number_shops->GetText());
    for (int i = 0; i < 15; ++i) {
        if (!check_boxes[i]->GetStatus()) continue;
        count_products_[i] = std::stoi(input_quantity[i]->GetText());
    }
    delete string_number_days;
    delete string_number_shops;
    delete inputing_field_code[0];
    delete inputing_field_code[1];
    ModelData* result = new ModelData(number_days_, number_shops_, check_boxes, count_products_);
    for (int i = 0; i < 15; ++i) {
        delete name_product[i];
        delete check_boxes[i];
        delete input_quantity[i];
    }
    return result;
}
