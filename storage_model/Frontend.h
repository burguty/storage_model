#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class WindowObject {
public:
    WindowObject(int x0, int y0);
    virtual ~WindowObject();
    virtual void draw(sf::RenderWindow& window) = 0;
protected:
    int x0_, y0_;
};

class Button : public WindowObject {
public:
    Button(int x0, int y0, int len_x, int len_y, std::wstring& text, sf::Font& font, int letter_size = 24);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
private:
    int len_x_, len_y_;
    sf::Text text_;
    sf::RectangleShape button_;
};

class CheckBox : public WindowObject {
public:
    CheckBox(int x0, int y0, int len_x, int len_y);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    bool GetStatus();
private:
    int len_x_, len_y_;
    bool status_ = false;
    sf::RectangleShape box_;
};

class InputLine :public WindowObject {
public:
    InputLine(int x0, int y0, int len_x, int len_y, sf::Font& font, int letter_size = 24);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void TypeSymbol(char c);
    void DeleteSymbol();
    std::wstring GetText();
private:
    int len_x_, len_y_;
    std::wstring text_ = L"";
    sf::Text text_sfml_;
    sf::RectangleShape line_;
    bool status_ = false;
};

class IDrawable {
public:
    IDrawable(int x0, int y0);
    virtual ~IDrawable() {}
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void Move(int x, int y) = 0;
protected:
    int x0_, y0_;
};