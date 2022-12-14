#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class IDrawable {
public:
    IDrawable(int x0, int y0);
    virtual ~IDrawable();
    virtual void draw(sf::RenderWindow& window) = 0;
protected:
    int x0_, y0_;
};

class Button : public IDrawable {
public:
    Button(int x0, int y0, int len_x, int len_y, std::wstring text, sf::Font& font, int letter_size = 24);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void SetText(std::wstring text);
    void SetPosition(int x, int y);
    void SetSizes(int len_x, int len_y);
private:
    int len_x_, len_y_;
    sf::Text text_;
    sf::RectangleShape button_;
};

class CheckBox : public IDrawable {
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

class InputLine : public IDrawable {
public:
    InputLine(int x0, int y0, int len_x, int len_y, sf::Font& font, int letter_size = 24);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void TypeSymbol(char c);
    void DeleteSymbol();
    void SetStatus(bool status);
    void SetTextColor(sf::Color color);
    void Clear();
    void SetPosition(int x, int y);
    void SetSizes(int len_x, int len_y);
    std::wstring GetText();
    void SetText(std::wstring text);
private:
    int len_x_, len_y_;
    std::wstring text_;
    sf::Text text_sfml_;
    sf::RectangleShape line_;
    bool status_ = false;
};

class IMovable : virtual public IDrawable {
public:
    IMovable();
    virtual ~IMovable() {}
    void StartMoving(int target_x, int target_y, long double time);
    bool Move(long double time_delta);
private:
    void Transfer(int x, int y);
    long double speed_x_, speed_y_, time_sum_;
    int x_start_, y_start_, target_x_, target_y_, time_;
};

class TextLine : public IDrawable {
public:
    TextLine(int x0, int y0, int len_x, int len_y, std::wstring text, 
        sf::Font& font, int letter_size = 24, sf::Color color_outline = sf::Color::Transparent,
        sf::Color color_fill = sf::Color::Transparent);
    virtual void draw(sf::RenderWindow& window);
    bool Click(int x, int y);
    void SetText(std::wstring text);
    void SetColor(sf::Color color);
private:
    int len_x_, len_y_;
    sf::RectangleShape line_;
    sf::Text text_sfml_;
};