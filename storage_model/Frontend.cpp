#include "Frontend.h"

IDrawable::IDrawable(int x0, int y0) : x0_(x0), y0_(y0) {}
IDrawable::~IDrawable() {}

Button::Button(int x0, int y0, int len_x, int len_y, std::wstring& text, sf::Font& font, int letter_size) :
    IDrawable(x0, y0), len_x_(len_x), len_y_(len_y) {
    text_.setFont(font);
    text_.setString(text);
    text_.setPosition(x0 + 10, y0 + len_y / 2 - letter_size / 2);
    text_.setCharacterSize(letter_size);
    text_.setFillColor(sf::Color::Black);
    button_.setFillColor(sf::Color::White);
    button_.setOutlineColor(sf::Color::Black);
    button_.setOutlineThickness(3);
    button_.setPosition(x0_, y0_);
    button_.setSize(sf::Vector2f(len_x_, len_y_));
}
bool Button::Click(int x, int y) {
    return x0_ <= x && x <= x0_ + len_x_ && y0_ <= y && y <= y0_ + len_y_;
}
void Button::draw(sf::RenderWindow& window) {
    window.draw(button_);
    window.draw(text_);
}

CheckBox::CheckBox(int x0, int y0, int len_x, int len_y) :
    IDrawable(x0, y0), len_x_(len_x), len_y_(len_y) {
    box_.setOutlineColor(sf::Color::Black);
    box_.setOutlineThickness(3);
    box_.setPosition(x0_, y0_);
    box_.setSize(sf::Vector2f(len_x_, len_y_));
}
void CheckBox::draw(sf::RenderWindow& window) {
    if (status_)
        box_.setFillColor(sf::Color::Green);
    else
        box_.setFillColor(sf::Color::White);
    window.draw(box_);
}
bool CheckBox::Click(int x, int y) {
    if (x0_ <= x && x <= x0_ + len_x_ && y0_ <= y && y <= y0_ + len_y_) {
        status_ ^= true;
        return true;
    }
    return false;
}
bool CheckBox::GetStatus() {
    return status_;
}

InputLine::InputLine(int x0, int y0, int len_x, int len_y, sf::Font& font, int letter_size) :
    IDrawable(x0, y0), len_x_(len_x), len_y_(len_y) {
    text_sfml_.setFont(font);
    text_sfml_.setPosition(x0 + 10, y0 + len_y / 2 - letter_size / 2);
    text_sfml_.setCharacterSize(letter_size);
    text_sfml_.setFillColor(sf::Color::Black);
    line_.setOutlineColor(sf::Color::Black);
    line_.setOutlineThickness(3);
    line_.setPosition(x0_, y0_);
    line_.setSize(sf::Vector2f(len_x_, len_y_));
}
void InputLine::draw(sf::RenderWindow& window) {
    if (!status_)
        line_.setFillColor(sf::Color::White);
    else
        line_.setFillColor(sf::Color::Green);
    text_sfml_.setString(text_);
    window.draw(line_);
    window.draw(text_sfml_);
}
bool InputLine::Click(int x, int y) {
    if (x0_ <= x && x <= x0_ + len_x_ && y0_ <= y && y <= y0_ + len_y_) {
        status_ ^= true;
        return true;
    }
    return false;
}
void InputLine::TypeSymbol(char c) {
    if (!status_)
        return;
    text_.push_back(c);
}
std::wstring InputLine::GetText() {
    return text_;
}
void InputLine::DeleteSymbol() {
    if (!status_)
        return;
    if (!text_.empty())
        text_.pop_back();
}

void InputLine::SetStatus(bool status) {
    status_ = status;
}

void InputLine::SetTextColor(sf::Color color) {
    text_sfml_.setFillColor(color);
}

TextLine::TextLine(int x0, int y0, int len_x, int len_y, std::wstring text,
    sf::Font& font, int letter_size, sf::Color color_outline, 
    sf::Color color_fill) : IDrawable(x0, y0),
    len_x_(len_x), len_y_(len_y), text_(text) {
    text_sfml_.setFont(font);
    text_sfml_.setString(text);
    text_sfml_.setPosition(x0 + 10, y0 + len_y / 2 - letter_size / 2);
    text_sfml_.setFillColor(sf::Color::Black);
    text_sfml_.setCharacterSize(letter_size);
    line_.setOutlineThickness(3);
    line_.setOutlineColor(color_outline);
    line_.setFillColor(color_fill);
    line_.setPosition(x0, y0);
    line_.setSize(sf::Vector2f(len_x, len_y));
}

void TextLine::draw(sf::RenderWindow& window) {
    window.draw(line_);
    window.draw(text_sfml_);
}

bool TextLine::Click(int x, int y) {
    return x0_ <= x && x <= x0_ + len_x_ && y0_ <= y && y <= y0_ + len_y_;
}